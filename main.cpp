#ifndef CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_OPENSSL_SUPPORT
#endif

#include <chrono>
#include <iostream>
#include <memory>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "command_handler.hpp"
#include "config.hpp"
#include "get_updates.hpp"
#include "log_updates.hpp"

static char *BOT_TOKEN = nullptr;
std::string API_PREFIX;

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

  get_updates updates{client};
  auto updatesThread = std::thread([&]() { updates.start(); });

  auto logger = std::make_shared<log_updates>();
  auto handler = std::make_shared<command_handler>(client);

  updates.add_observer(logger);
  updates.add_observer(handler);

  return 0;
}
