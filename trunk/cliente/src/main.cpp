#include <Client.h>
#include <iostream>
#include <ClientProfile.h>


int main(int argc, char** argv) {
	Client* c = NULL;
	std::string loginIp = "";
	std::string loginPort = "";
	std::string username = "";
	std::string password = "";

	if (argc < 5) {
		std::cerr << "Usage: client iplogin portlogin user pass" << std::endl;
		return 1;
	}

	loginIp = argv[1];
	loginPort = argv[2];
	username = argv[3];
	password = argv[4];

	//Configure connection
	TransferableFactory::instance().setProfile(new ClientProfile());
	TransferableFactory::instance().setProtocol("0.1a");
	c = Client::instancePtr();
	c->setLoginAddress(loginIp, loginPort);
	c->login(username, password);

	delete c;
}
