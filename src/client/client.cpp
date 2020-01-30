#include <iostream>
#include <fstream>
#include <string>

#include <zmqpp/zmqpp.hpp>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

using namespace std;
using namespace Poco::JSON;

int main(int argc, char *argv[]) {
	ifstream File;
	
	File.open("resources/config/client.properties");
	if(!File.is_open()) {
		cout << "File doesn't exist/Unable to open." << endl;

		return 0;
	}
	
	string config;

	getline(File,config);

	File.close();

	Poco::JSON::Parser parser;
	Poco::Dynamic::Var parsedJson = parser.parse(config);

	Poco::JSON::Object::Ptr object = parsedJson.extract<Poco::JSON::Object::Ptr>();
	
	string port = object->getValue<string>("port");
	string topic = object->getValue<string>("topic");

	const string endpoint = "tcp://localhost:" + port;

	cout << "Port: " << port << " Topic: " << topic << endl;

	zmqpp::context context;
	zmqpp::socket socket(context, zmqpp::socket_type::subscribe);
	socket.subscribe(topic);

	socket.connect(endpoint);

	while(1) {
		string topic;
		unsigned int order;
		string rcv;

		zmqpp::message message;

		cout << "Receiving ..." << endl;
	
		socket.receive(message);
	
		message >> topic >> order >> rcv;
	
		cout << "Received: " << topic << " " << order << " " << rcv << endl;
	
		Poco::JSON::Parser par;
		Poco::Dynamic::Var parsed = par.parse(rcv);
		Poco::JSON::Object::Ptr obj = parsed.extract<Poco::JSON::Object::Ptr>();

		string fname = obj->getValue<string>("timestamp");
		fname += ".json";

		ofstream file;
		
		file.open(fname);
		if(!file.is_open()) {
			cout << "Failed to create a new file" << endl;
		
			return 0;
		}

		file << rcv; 
		file.close();

		if(order > 0x7FFFFFE)
			break;
	}

	return 1;
}
