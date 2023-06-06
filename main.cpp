#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <iostream>
#include <chrono>

#include <httplib.h>
#include <nlohmann/json.hpp>
#include <blockingconcurrentqueue.h>

#include "get_updates_payload.hpp"
#include "get_updates_response.hpp"
#include "command.hpp"

#define POLL_TIMEOUT 30

static char *BOT_TOKEN = nullptr;
static std::string API_PREFIX;

auto getUpdates(httplib::SSLClient &client,
                moodycamel::BlockingConcurrentQueue<get_updates_response> &command_queue,
                moodycamel::BlockingConcurrentQueue<get_updates_response> &logging_queue) {
    auto lastUpdateId = -1;

    get_updates_payload payload;
    payload.timeout() = POLL_TIMEOUT;
    payload.allowed_updates().emplace_back("messages");

    for (;;) {
        payload.offset() = lastUpdateId + 1;

        auto result = client.Post(
                API_PREFIX + "/getUpdates",
                nlohmann::json(payload).dump(),
                "application/json"
        );

        if (!result) {
            continue;
        }

        nlohmann::json body = nlohmann::json::parse(result.value().body);

        std::vector<nlohmann::json> results = body["result"];

        for (const auto &result: results) {
            int update_id = result["update_id"];

            if (update_id > lastUpdateId) {
                lastUpdateId = update_id;
            }

            try {
                auto update = result.get<get_updates_response>();
                command_queue.enqueue(update);
                logging_queue.enqueue(update);
            } catch (const std::exception &ex) {}
        }
    }
}

[[noreturn]] auto logUpdates(moodycamel::BlockingConcurrentQueue<get_updates_response> &queue) {
    get_updates_response update;

    for (;;) {
        queue.wait_dequeue(update);
        std::cout << "text = " << update.message.text
                  << ", chat = " << update.message.chat.id
                  << ", id = " << update.update_id
                  << "\n";
    }
}

[[noreturn]] auto handleUpdates(
        httplib::SSLClient &client,
        moodycamel::BlockingConcurrentQueue<get_updates_response> &queue
) {
    get_updates_response update;

    for (;;) {
        queue.wait_dequeue(update);

        switch (command::parse(update.message.text)) {
            case Command::SendSchedule: {
                httplib::Params params = {
                        {"chat_id", std::to_string(update.message.chat.id)},
                        {"text",    "Miércoles de 7H00 a 8H30"},
                };

                auto result = client.Post(API_PREFIX + "/sendMessage", params);

                if (!result) {
                    std::cerr << "There was an error sending the schedule: " << result.error() << "\n";
                } else {
                    std::cout << "Successfully sent schedule\n";
                }

                break;
            }
            case Command::Noop:
                break;
        }
    }
}

auto main() -> int {
    BOT_TOKEN = getenv("BOT_TOKEN");

    if (!BOT_TOKEN) {
        std::cerr << "Failed to retrieve BOT_TOKEN\n";
        exit(EXIT_FAILURE);
    }

    API_PREFIX = "/bot" + std::string(BOT_TOKEN);

    httplib::SSLClient client("api.telegram.org");

    client.set_connection_timeout(std::chrono::seconds(POLL_TIMEOUT * 2));
    client.set_keep_alive(true);

    moodycamel::BlockingConcurrentQueue<get_updates_response> commands_queue;
    moodycamel::BlockingConcurrentQueue<get_updates_response> logging_queue;

    std::thread updatesThread([&]() {
        getUpdates(client, commands_queue, logging_queue);
    });

    std::thread handlerThread([&]() {
        handleUpdates(client, commands_queue);
    });

    std::thread loggingThread([&]() {
        logUpdates(logging_queue);
    });

    updatesThread.join();
    handlerThread.join();
    loggingThread.join();

    return 0;
}
