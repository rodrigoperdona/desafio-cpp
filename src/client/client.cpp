#include <fstream>
#include <iostream>

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <zmqpp/zmqpp.hpp>

bool GetConfiguration(std::string &port, std::string &topic) {
  std::ifstream file;
  std::string configuration;

  file.open("./resources/config/client.properties");
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

int main() {
  std::string port;
  std::string topic;

  if (!GetConfiguration(port,topic)) {
    std::cout << "Failed to open configuration file." << std::endl;
    return -1;
  }

  const std::string endpoint("tcp://localhost:" + port);

  zmqpp::context context;

  zmqpp::socket subscriber(context, zmqpp::socket_type::subscribe);

  subscriber.subscribe(topic);
  subscriber.connect(endpoint);

  while (1) {
    zmqpp::message message_received;

    subscriber.receive(message_received);

    std::string topic;
    int32_t order;
    std::string information;

    message_received >> topic;
    message_received >> order;
    message_received >> information;

    std::cout << "[RECEIVED] " << topic << " " << order << " " << information
              << std::endl;

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(information);
    Poco::JSON::Object::Ptr configuration_object =
        result.extract<Poco::JSON::Object::Ptr>();

    std::string time_stamp =
        configuration_object->getValue<std::string>("timestamp");

    std::ofstream file;

    std::string file_name(time_stamp + ".json");
    file.open(file_name);
    if (file.is_open()) {
      file << information;
      file.close();
    }
  }
}
