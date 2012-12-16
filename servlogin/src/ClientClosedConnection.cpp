#include <ControlConnection.h>
#include <Login.h>
#include <iostream>
#include <ClientClosedConnection.h>

using namespace std;

void ClientClosedConnection::CallOnClose(Connection* c) throw(){
	int clientId; 
	string username;
	Login::instance().loginMutex.lock();
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
	Login::instance().loginMutex.unlock();
}

