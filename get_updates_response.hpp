#pragma once

#include <string>

#include <nlohmann/json.hpp>

struct chat {
  chat() = default;

  int id = {};
};

struct message {
  message() = default;

  int message_id = {};
  std::string text = {};
  struct chat chat;
};

struct get_updates_response {
  get_updates_response() = default;

  int update_id = {};
  struct message message = {};
};

void from_json(const nlohmann::json &json, chat &chat);

void from_json(const nlohmann::json &json, message &message);

void from_json(const nlohmann::json &json, get_updates_response &response);
