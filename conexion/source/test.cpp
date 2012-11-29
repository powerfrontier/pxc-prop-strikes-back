#include <string>
#include "Datagram.h"
#include "Connection.h"

int main(){
	Connection* n = new TCPConnection();
	n->connect(std::string("127.0.0.1"),std::string("80"));
	n->close();
}


