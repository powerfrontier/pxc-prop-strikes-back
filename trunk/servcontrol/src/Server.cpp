#include <Server.h>



Server::~Server(){
	c->close();
	delete c;	
}



