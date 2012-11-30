#include <string>
#include "Datagram.h"
#include "ConnectionManager.h"
#include "Connection.h"
#include <iostream>

int main(int argc, char** argv){
	ConnectionManager* n = new ConnectionManager();
	n->listen(std::string(argv[1]));
	while (1){
	}
}


