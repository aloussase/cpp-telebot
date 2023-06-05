#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <iostream>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "get_updates_payload.hpp"
#include "get_updates_response.hpp"

static char *BOT_TOKEN = nullptr;
static std::string API_PREFIX;

auto getUpdates(httplib::SSLClient &client) {
    get_updates_payload payload;
    payload.allowed_updates().push_back("messages");

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
        std::cout << update.message.text << "\n";
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

    getUpdates(client);

    return 0;
}
