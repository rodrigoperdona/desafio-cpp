#include <iostream>
#include <fstream>
#include <string>

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <zmqpp/zmqpp.hpp>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/LocalDateTime.h>

using namespace std;

string getMessage() {
	static int id = 0;
	unsigned char speed = rand() % 256;
	
	string timestamp = Poco::DateTimeFormatter::format(Poco::LocalDateTime(), 
			   Poco::DateTimeFormat::ISO8601_FORMAT);

	return ("{\n\"id\": \"" + to_string(id++) +
		"\",\n\"velocidade\": \"" + to_string(speed) +
		"\",\n\"timestamp\": \"" + timestamp + "\"\n}");

}

int main(int argc, char * argv[]) {
	ifstream File;

	File.open("resources/config/server.properties");

	if(!File.is_open()) {
		cout << "File doesn't exist/Unable to open" << endl;	
	
		return 0;
	}

	string config;
	
	getline(File, config);

	File.close();

	Poco::JSON::Parser parser;
	Poco::Dynamic::Var parsedJson = parser.parse(config);

	Poco::JSON::Object::Ptr object = parsedJson.extract<Poco::JSON::Object::Ptr>();

	string port = object->getValue<string>("port");
	string topic = object->getValue<string>("topic");

	const string endpoint = "tcp://*:" + port;

	cout << "Port: " << port << " Topic: " << topic << endl;

	zmqpp::context context;
	zmqpp::socket socket(context, zmqpp::socket_type::pub);
	socket.bind(endpoint);

	zmqpp::message message;

	unsigned int order = 0;

	while(1) {		
		message.add(topic);
		message.add(order);
		message.add(getMessage());

		socket.send(message);

		cout << "SENT " << order << endl;

		this_thread::sleep_for(chrono::milliseconds(1000));

		if(order++ > 0x7FFFFFFF)
			break;

	}


}
