#include <ControlConnection.h>
#include <Login.h>
#include <iostream>
#include <ClientClosedConnection.h>

using namespace std;

void ClientClosedConnection::callOnClose(Connection* c) throw(){
	int clientId; 
	string username;
	Login::instance().loginMutex.lock();
	cout << "Cerramos el preclose con usuarios conectados " << Login::instance().usersConnected << endl;
	if(Login::instance().connectionToIdMap.find(c) != Login::instance().connectionToIdMap.end()){
	  clientId = Login::instance().connectionToIdMap.find(c)->second;
	  username = Login::instance().idToUserMap.find(clientId)->second;
	  Login::instance().userTokenMap.erase(Login::instance().userTokenMap.find(clientId));
	  Login::instance().connectionToIdMap.erase(Login::instance().connectionToIdMap.find(c));
	  Login::instance().idToConnectionMap.erase(Login::instance().idToConnectionMap.find(clientId));
	  Login::instance().userToIdMap.erase(Login::instance().userToIdMap.find(username));
	  Login::instance().idToUserMap.erase(Login::instance().idToUserMap.find(clientId));
	  Login::instance().usersConnected--;
	}
	cout << "Cerramos el close con usuarios conectados " << Login::instance().usersConnected << endl;
	Login::instance().loginMutex.unlock();
	
}

