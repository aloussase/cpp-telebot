#pragma once

#include "get_updates_response.hpp"
#include "updates_observer.hpp"

class log_updates : public updates_observer {
public:
  void notify(get_updates_response) noexcept override;
};