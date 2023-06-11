#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class get_updates_payload {
public:
  get_updates_payload() = default;

  get_updates_payload(int offset, int timeout = 0)
      : m_offset{offset}, m_timeout{timeout} {}

  [[nodiscard]] int timeout() const noexcept { return m_timeout; }

  [[nodiscard]] int &timeout() noexcept { return m_timeout; }

  [[nodiscard]] int offset() const noexcept { return m_offset; }

  [[nodiscard]] int &offset() noexcept { return m_offset; }

  [[nodiscard]] const std::vector<std::string> &
  allowed_updates() const noexcept {
    return m_allowed_updates;
  }

  [[nodiscard]] std::vector<std::string> &allowed_updates() noexcept {
    return m_allowed_updates;
  }

private:
  std::vector<std::string> m_allowed_updates = {};
  int m_timeout = {};
  int m_offset = {};
};

void to_json(nlohmann::json &json, const get_updates_payload &payload);

void from_json(const nlohmann::json &json, get_updates_payload &payload);
