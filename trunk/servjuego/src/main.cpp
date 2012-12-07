#include <GameServer.h>
#include <GameServerProfile.h>
#include <ConnectionManager.h>
#include <Zone.h>

#include <string>
#include <iostream>

int main(int argc, char** argv) {
	ConnectionManager* cm = NULL;
	GameServer* gs = NULL;
	std::string listeningPort = "";
	std::string command = "";
	
	if (argc < 2) {
		std::cerr << "Error: No listening port set." << std::endl;
		return 1;
	}

	//Configure connection
	TransferableFactory::instance().setProfile(new GameServerProfile());
	TransferableFactory::instance().setProtocol("0.1a");
	
	//Create GameServer object
	gs = GameServer::instancePtr();
	
	//Start listening
	cm = new ConnectionManager();
	cm->setCallbackFunction(gs);
	try {
	cm->listen(std::string(argv[1]));
	}
	catch (ConnectionException& cs) {
		std::cerr << "Error trying to listen connections: " << cs.what() << std::endl;
		
		gs->stopAll();
		delete gs;
		delete cm;
		return 1;
	}
	catch (std::system_error& se) {
		std::cerr << "Error trying to listen connections: " << se.what() << std::endl;
		
		gs->stopAll();
		delete gs;
		delete cm;
		return 1;
	}
	
	while (command != "exit") {
		std::cin >> command;
	}
	
	gs->stopAll();
	delete gs;
	delete cm;
}
