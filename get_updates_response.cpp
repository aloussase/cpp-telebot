#include "get_updates_response.hpp"

void from_json(const nlohmann::json &json, chat &chat) {
  json.at("id").get_to(chat.id);
}

void from_json(const nlohmann::json &json, message &message) {
  json.at("message_id").get_to(message.message_id);
  json.at("text").get_to(message.text);
  json.at("chat").get_to(message.chat);
}

void from_json(const nlohmann::json &json, get_updates_response &response) {
  json.at("update_id").get_to(response.update_id);
  json.at("message").get_to(response.message);
}