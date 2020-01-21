#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <zmqpp/zmqpp.hpp>

int main() {
  const std::string endpoint = "tcp://localhost:5555";

  // initialize the 0MQ context
  zmqpp::context context;

  // generate a pull socket
  zmqpp::socket_type type = zmqpp::socket_type::subscribe;
  zmqpp::socket subscriber(context, type);

  subscriber.subscribe("");

  subscriber.connect(endpoint);

  while (1) {
    zmqpp::message message_received;

    subscriber.receive(message_received);

    std::string text;

    message_received >> text;

    std::cout << "[RECEIVED] " << text << std::endl;
  }
}
