#include "get_updates.hpp"
#include "config.hpp"
#include "get_updates_payload.hpp"
#include "get_updates_response.hpp"

void get_updates::add_observer(std::shared_ptr<updates_observer> o) noexcept {
  m_observers.push_back(o);
}

void get_updates::start() noexcept {
  get_updates_payload payload;
  payload.timeout() = POLL_TIMEOUT;
  payload.allowed_updates().emplace_back("messages");

  for (;;) {
    payload.offset() = m_last_update_id + 1;

    auto result =
        m_client.Post(API_PREFIX + "/getUpdates",
                      nlohmann::json(payload).dump(), "application/json");

    if (!result) {
      continue;
    }

    nlohmann::json body = nlohmann::json::parse(result.value().body);

    std::vector<nlohmann::json> results = body["result"];

    for (const auto &result : results) {
      int update_id = result["update_id"];

      if (update_id > m_last_update_id) {
        m_last_update_id = update_id;
      }

      try {
        auto update = result.get<get_updates_response>();
        for (auto &observer : m_observers) {
          observer->notify(update);
        }
      } catch (const std::exception &ex) {
      }
    }
  }
}