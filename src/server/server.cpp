#include <fstream>
#include <iostream>

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <zmqpp/zmqpp.hpp>

bool GetConfiguration(std::string &topic, std::string &port) {
  std::ifstream file;
  std::string configuration;

  file.open("./resources/config/server.properties");
  if (!file.is_open()) return false;
  getline(file, configuration);
  file.close();

  Poco::JSON::Parser parser;
  Poco::Dynamic::Var result = parser.parse(configuration);
  Poco::JSON::Object::Ptr configuration_object =
      result.extract<Poco::JSON::Object::Ptr>();

  topic = configuration_object->getValue<std::string>("topic");
  port = configuration_object->getValue<std::string>("port");

  return true;
}

std::string GenerateMessage() {
  static unsigned int id = 0;

  uint8_t velocity = rand() % 256;

  Poco::Timestamp time_stamp;
  std::string time_stamp_formatted = Poco::DateTimeFormatter::format(
      time_stamp, Poco::DateTimeFormat::ISO8601_FORMAT);

  std::string message_text("{ \"id\" : " + std::to_string(++id) +
                           ", \"velocidade\" : " + std::to_string(velocity) +
                           ", \"timestamp\" : \"" + time_stamp_formatted + "\" }");

  return message_text;
}

int main() {
  std::string topic;
  std::string port;
  std::string sync_port;

  if (!GetConfiguration(topic, port)) {
    std::cout << "Failed to open configuration file." << std::endl;
    return -1;
  }

  const std::string endpoint = "tcp://*:" + port;

  zmqpp::context context;

  zmqpp::socket publisher(context, zmqpp::socket_type::pub);

  publisher.bind(endpoint);

  int32_t order = 0;
  while (1) {
    zmqpp::message message;

    message.add(topic);
    message.add(order);
    message.add(GenerateMessage());

    publisher.send(message);

    std::cout << "[SENT] " << order << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (++order < 0) order = 0;
  }
}

