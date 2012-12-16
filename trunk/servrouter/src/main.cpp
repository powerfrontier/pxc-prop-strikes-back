#include <Router.h>
#include <RouterProfile.h>
#include <ConnectionManager.h>

#include <iostream>

int main(int argc, char** argv) {
	ConnectionManager* controlCM = NULL;
	ConnectionManager* serverCM = NULL;
	ConnectionManager* clientCM = NULL;
	Router* r = NULL;
	
	std::string controlPort = "";
	std::string serverPort = "";
	std::string clientPort = "";
	
	std::string command = "";
	
	if (argc < 4) {
		std::cerr << "Error: No listening ports entered." << std::endl;
		std::cerr << "Usage:" << std::endl;
		std::cerr << "router controlPort serverPort clientPort" << std::endl;
		return 1;
	}

	controlPort = std::string(argv[1]);
	serverPort = std::string(argv[2]);
	clientPort = std::string(argv[3]);
	
	//Configure connection
	TransferableFactory::instance().setProfile(new RouterProfile());
	TransferableFactory::instance().setProtocol("0.1a");
	
	//Start listening
	controlCM = new ConnectionManager();
	serverCM = new ConnectionManager();
	clientCM = new ConnectionManager();
	
	controlCM->setCallbackFunction(r);
	serverCM->setCallbackFunction(r);
	clientCM->setCallbackFunction(r);
	try {
		controlCM->listenSecure(controlPort, true);
		serverCM->listenSecure(serverPort, true);
		clientCM->listenSecure(clientPort, false);
	}
	catch (ConnectionException& cs) {
		std::cerr << "Error trying to listen connections: " << cs.what() << std::endl;
		
		delete controlCM;
		delete serverCM;
		delete clientCM;
		delete r;
		return 1;
	}
	catch (std::system_error& se) {
		std::cerr << "Error trying to listen connections: " << se.what() << std::endl;
		
		delete controlCM;
		delete serverCM;
		delete clientCM;
		delete r;
		return 1;
	}
	catch (...) {
		std::cerr << "Unexpected error: " << std::endl;
		
		delete controlCM;
		delete serverCM;
		delete clientCM;
		delete r;
		return 1;
	}
	
	while (command != "exit") {
		std::cin >> command;
		//sleep(2);
	}
	
	delete controlCM;
	delete serverCM;
	delete clientCM;
	delete r;
	return 1;
}
