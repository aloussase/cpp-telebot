#include <httplib.h>

#include "command.hpp"
#include "command_handler.hpp"
#include "config.hpp"

void command_handler::notify(get_updates_response update) noexcept {
  switch (command::parse(update.message.text)) {
  case Command::SendSchedule: {
    httplib::Params params = {
        {"chat_id", std::to_string(update.message.chat.id)},
        {"text", "Miércoles de 7H00 a 8H30"},
    };

    auto result = m_client.Post(API_PREFIX + "/sendMessage", params);

    if (!result) {
      std::cerr << "There was an error sending the schedule: " << result.error()
                << "\n";
    } else {
      std::cout << "Successfully sent schedule\n";
    }

    break;
  }
  case Command::Noop:
    break;
  }
}