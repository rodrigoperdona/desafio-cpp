project(Server)

set(CMAKE_CXX_STANDARD 11)

file(GLOB Sources "*.cpp")

link_directories("/usr/local/lib")

add_executable(Server ${Sources})
target_link_libraries(Server zmq zmqpp)

install(TARGETS Server DESTINATION "${CHALLANGE_INSTALL_DESTINATION}")
