#include "SharedStructs.h"



Server::~Server(){
	c.close();
	delete c;	
}



