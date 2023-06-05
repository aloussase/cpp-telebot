#pragma once

#include <string>

enum class Command {
    SendSchedule,
    Noop
};

namespace command {
    Command parse(const std::string &) noexcept;
};