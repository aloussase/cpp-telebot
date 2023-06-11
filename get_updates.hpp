#pragma once

#include <httplib.h>
#include <memory>
#include <thread>
#include <vector>

#include "updates_observer.hpp"

class get_updates {
public:
  get_updates(httplib::SSLClient &client) : m_client(client) {}

  void start() noexcept;

  void add_observer(std::shared_ptr<updates_observer> o) noexcept;

private:
  int m_last_update_id = -1;
  std::vector<std::shared_ptr<updates_observer>> m_observers = {};
  httplib::SSLClient &m_client;
};