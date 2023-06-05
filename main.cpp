#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <iostream>

#include <httplib.h>
#include <nlohmann/json.hpp>
#include <blockingconcurrentqueue.h>

#include "get_updates_payload.hpp"
#include "get_updates_response.hpp"
#include "command.hpp"

static char *BOT_TOKEN = nullptr;
static std::string API_PREFIX;

auto getUpdates(httplib::SSLClient &client, moodycamel::BlockingConcurrentQueue<get_updates_response> &queue) {
    auto lastUpdateId = -1;

    for (;;) {
        get_updates_payload payload;

        payload.timeout() = 60;
        payload.offset() = lastUpdateId + 1;
        payload.allowed_updates().emplace_back("messages");

        auto result = client.Post(
                API_PREFIX + "/getUpdates",
                nlohmann::json(payload).dump(),
                "application/json"
        );

        if (!result) {
            std::cerr << "Request for updates failed with error: " << result.error() << "\n";
            return;
        }

        nlohmann::json body = nlohmann::json::parse(result.value().body);
        auto updates = body.at("result").get<std::vector<get_updates_response>>();

        for (const auto &update: updates) {
            if (update.update_id > lastUpdateId) {
                lastUpdateId = update.update_id;
            }
            queue.enqueue(update);
        }
    }
}

[[noreturn]] auto logUpdates(moodycamel::BlockingConcurrentQueue<get_updates_response> &queue) {
    for (;;) {
        get_updates_response update;
        queue.wait_dequeue(update);
        std::cout << update.message.text << "\n";
    }
}

[[noreturn]] auto handleUpdates(moodycamel::BlockingConcurrentQueue<get_updates_response> &queue) {
    for (;;) {
        get_updates_response update;
        queue.wait_dequeue(update);

        switch (command::parse(update.message.text)) {
            case Command::SendSchedule:
                std::cout << "Sending schedule\n";
                break;
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

    client.set_keep_alive(true);

    moodycamel::BlockingConcurrentQueue<get_updates_response> queue;

    std::thread updatesThread([&]() {
        getUpdates(client, queue);
    });

    std::thread handlerThread([&]() {
        handleUpdates(queue);
    });

    std::thread loggingThread([&]() {
        logUpdates(queue);
    });

    updatesThread.join();
    handlerThread.join();
    loggingThread.join();

    return 0;
}
