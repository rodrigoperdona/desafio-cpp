project(Server)

set(CMAKE_CXX_STANDARD 11)

file(GLOB Sources "*.cpp")

link_directories("/usr/lib")
include_directories("/usr/include")

add_executable(Server ${Sources})
target_link_libraries(Server crypto PocoJSON PocoFoundation zmq zmqpp)

install(TARGETS Server DESTINATION "${CHALLANGE_INSTALL_DESTINATION}/server")
install(DIRECTORY resources DESTINATION "${CHALLANGE_INSTALL_DESTINATION}/server")
