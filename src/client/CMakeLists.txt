project(Client)

set(CMAKE_CXX_STANDARD 11)

file(GLOB Sources "*.cpp")

link_directories("/usr/lib")
include_directories("/usr/include")

add_executable(Client ${Sources})
target_link_libraries(Client crypto PocoJSON PocoFoundation zmq zmqpp)

install(TARGETS Client DESTINATION "${CHALLANGE_INSTALL_DESTINATION}/client")
install(DIRECTORY resources DESTINATION "${CHALLANGE_INSTALL_DESTINATION}/client")
