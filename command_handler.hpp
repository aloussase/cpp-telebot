#pragma once

#include <httplib.h>

#include "get_updates_response.hpp"
#include "updates_observer.hpp"

class command_handler : public updates_observer {
public:
  command_handler(httplib::SSLClient &client) : m_client{client} {}

  void notify(get_updates_response) noexcept override;

private:
  httplib::SSLClient &m_client;
};