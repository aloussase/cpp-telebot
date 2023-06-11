cmake_minimum_required(VERSION "3.18")
project(
  telebot
  LANGUAGES CXX
  DESCRIPTION "A telegram bot for my ayudantias")

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third_party/cpp-httplib)
add_subdirectory(third_party/json)

add_executable(
  telebot
  main.cpp
  command.cpp
  get_updates.cpp
  log_updates.cpp
  command_handler.cpp
  get_updates_response.cpp
  get_updates_payload.cpp)

target_include_directories(telebot PRIVATE ${HTTPLIB_INCLUDE_DIR}
                                           ${NLOHMANN_JSON_INCLUDE_INSTALL_DIR})

target_link_libraries(telebot PRIVATE nlohmann_json::nlohmann_json ssl crypto
                                      httplib::httplib)
