#include "command.hpp"

namespace command {
    Command parse(const std::string &s) noexcept {
        if (!s.starts_with('/')) {
            return Command::Noop;
        }

        if (s == "/horario") {
            return Command::SendSchedule;
        }

        return Command::Noop;
    }
}
