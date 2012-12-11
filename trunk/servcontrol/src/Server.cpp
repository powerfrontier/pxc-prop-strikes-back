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

void Server::printServer() {
	cout << "Id: " << id << endl;
	cout << "IP: " << ip << endl;
	cout << "Connection: " << c << endl;
	cout << "Carga total: " << load.totalLoad << endl;
	cout << load.distribution.size() << endl;
	for(int i=0; i<load.distribution.size(); ++i){
		cout << "zona " << load.distribution.at(i)->zone << " carga: " << load.distribution.at(i)->load << endl; 
	}
}
