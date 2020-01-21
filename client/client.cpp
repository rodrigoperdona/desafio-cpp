#include <iostream>
#include <string>

#include <zmqpp/zmqpp.hpp>

int main() {
  const std::string endpoint = "tcp://*:5555";

  zmqpp::context context;

  zmqpp::socket_type type = zmqpp::socket_type::pub;
  zmqpp::socket publisher(context, type);

  publisher.bind(endpoint);

  int number_sent = 0;
  while (1) {
    zmqpp::message message;

    message << std::to_string(number_sent++);

    publisher.send(message);

    std::cout << "[SENT] " << number_sent << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}
