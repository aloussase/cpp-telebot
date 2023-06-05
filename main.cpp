#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <iostream>
#include <stdlib.h>
#include <vector>

#include <httplib.h>
#include <nlohmann/json.hpp>

static char *BOT_TOKEN = nullptr;

auto main() -> int {
  BOT_TOKEN = getenv("BOT_TOKEN");

  if (!BOT_TOKEN) {
    std::cerr << "Failed to retrieve BOT_TOKEN\n";
    exit(EXIT_FAILURE);
  }

  httplib::SSLClient client("api.telegram.org");
  auto apiPrefix = "/bot" + std::string(BOT_TOKEN);

  nlohmann::json payload = {
      {"offset", 0},
      {"timeout", 0},
      {"allowed_updates", {"messages"}},
  };

  auto result = client.Get(apiPrefix + "/getMe");
  if (result) {
    std::cout << result->body << "\n";
  } else {
    std::cerr << "Request for updates failed with error: " << result.error()
              << "\n";
  }

  return 0;
}
