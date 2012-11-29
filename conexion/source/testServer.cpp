#include <string>
#include "Datagram.h"
#include "Connection.h"

int main(){
	Connection* n = new TCPConnection();
	n->connect(std::string("173.194.34.247:80"));
	n->close();
}


