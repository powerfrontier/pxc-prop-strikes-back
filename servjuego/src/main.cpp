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
	
	//Start listening
	controlCM = new ConnectionManager();
	routerCM = new ConnectionManager();
	
	controlCM->setCallbackFunction(gs->CONTROL_LISTENER);
	routerCM->setCallbackFunction(gs->ROUTER_LISTENER);
	try {
		controlCM->listenSecure(controlPort, true);
		routerCM->listenSecure(routerPort, true);
	}
	catch (ConnectionException& cs) {
		gs->stopAll();
		delete gs;
		delete controlCM;
		delete routerCM;
		return 1;
	}
	catch (std::system_error& se) {
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
	while(1){

	}	
	/*
	while (command != "exit") {
		std::cout << std::endl << ">";
		std::cin >> command;
	}*/
	gs->stopAll();
	std::cout << "1" << std::endl;
	delete controlCM;
	std::cout << "2" << std::endl;
	delete routerCM;
	std::cout << "3" << std::endl;
	delete gs;
	std::cout << "out" << std::endl;
}
