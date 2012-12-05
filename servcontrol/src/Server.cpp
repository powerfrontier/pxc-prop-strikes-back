#include <Server.h>
#include <iostream>

using namespace std;

Server::~Server(){
cout << "Destruct Server";
	fflush(stdout);
	c->close();
	delete c;	
cout << "End Destruct Server";
	fflush(stdout);
}



