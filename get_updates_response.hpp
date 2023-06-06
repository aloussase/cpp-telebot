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