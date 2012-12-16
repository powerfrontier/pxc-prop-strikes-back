#include <Router.h>
#include <RouterProfile.h>
#include <ConnectionManager.h>

#include <iostream>

int main(int argc, char** argv) {
	ConnectionManager* cm = NULL;
	Router* r = NULL;
	
	std::string listeningPort = "";
	std::string command = "";
	
	if (argc < 2) {
		std::cerr << "Error: No listening port set." << std::endl;
		return 1;
	}

	listeningPort = std::string(argv[1]);
	
	//Configure connection
	TransferableFactory::instance().setProfile(new RouterProfile());
	TransferableFactory::instance().setProtocol("0.1a");
	
	
}
