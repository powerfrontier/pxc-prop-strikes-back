#include <GameServer.h>
#include <GameServerProfile.h>
#include <ConnectionManager.h>
#include <Zone.h>

#include <string>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv) {
	ConnectionManager* controlCM = NULL;
	ConnectionManager* routerCM = NULL;
	
	GameServer* gs = NULL;
	
	std::string controlPort = "";
	std::string routerPort = "";
	std::string command = "";
	
	if (argc < 3) {
		std::cerr << "Error: No listening ports entered." << std::endl;
		return 1;
	}

	controlPort = std::string(argv[1]);
	routerPort = std::string(argv[2]);
	
	//Configure connection
	TransferableFactory::instance().setProfile(new GameServerProfile());
	TransferableFactory::instance().setProtocol("0.1a");
	
	//Create GameServer object
	gs = GameServer::instancePtr();
	gs->controlPort(controlPort);
	//Start listening
	controlCM = new ConnectionManager();
	routerCM = new ConnectionManager();
	
	controlCM->setCallbackFunction(gs);
	try {
		controlCM->listen(controlPort);
		routerCM->listen(routerPort);
	}
	catch (ConnectionException& cs) {
		std::cerr << "Error trying to listen connections: " << cs.what() << std::endl;
		
		gs->stopAll();
		delete gs;
		delete controlCM;
		delete routerCM;
		return 1;
	}
	catch (std::system_error& se) {
		std::cerr << "Error trying to listen connections: " << se.what() << std::endl;
		
		gs->stopAll();
		delete gs;
		delete controlCM;
		delete routerCM;
		return 1;
	}
	catch (...) {
		std::cerr << "Unexpected error: " << std::endl;
		
		gs->stopAll();
		delete gs;
		delete controlCM;
		delete routerCM;
		return 1;
	}
	
	while (command != "exit") {
		std::cin >> command;
		
	}
	
	gs->stopAll();
	delete controlCM;
	delete routerCM;
	delete gs;
}
