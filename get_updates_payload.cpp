#include "get_updates_payload.hpp"

void to_json(nlohmann::json &json, const get_updates_payload &payload) {
  json = {
      {"timeout", payload.timeout()},
      {"offset", payload.offset()},
      {"allowed_updates", payload.allowed_updates()},
  };
}

void from_json(const nlohmann::json &json, get_updates_payload &payload) {
  json.at("timeout").get_to(payload.timeout());
  json.at("offset").get_to(payload.offset());
  json.at("allowed_updates").get_to(payload.allowed_updates());
}