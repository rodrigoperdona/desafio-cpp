cmake_minimum_required(VERSION 2.8)

project(Client)

set(CMAKE_CXX_STANDARD 11)

file(GLOB Sources "*.cpp")

link_directories("/usr/local/lib")

add_executable(Client ${Sources})
target_link_libraries(Client zmq zmqpp)

install(TARGETS Client DESTINATION "${CHALLANGE_INSTALL_DESTINATION}")
