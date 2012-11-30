#include <string>
#include "Datagram.h"
#include "ConnectionManager"
#include "Connection.h"

int main(){
	ConnectionManager* n = new ConnectionManager();
	n->listen(std::string("8030"));
	while (1){
	}
}


