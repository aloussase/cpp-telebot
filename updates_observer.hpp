#pragma once

#include "get_updates_response.hpp"

class updates_observer {
public:
  virtual void notify(get_updates_response) noexcept = 0;
};