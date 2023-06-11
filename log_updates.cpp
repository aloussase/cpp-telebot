#include <iostream>

#include "log_updates.hpp"

void log_updates::notify(get_updates_response update) noexcept {
  std::cout << "text = " << update.message.text
            << ", chat = " << update.message.chat.id
            << ", id = " << update.update_id << "\n";
}