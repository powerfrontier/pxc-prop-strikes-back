#include <Router.h>

void Router::clear() {
	auto itUser = mUsers.begin();
	auto itZone = mZones.begin();
	auto itServers = mServers.begin();
	
	while (itUser != mUsers.end()) {
		delete itUser->second;
		++itUser;
	}
	mUsers.clear();
	
	while (itZone != mZones.end()) {
		delete itZone->second;
		++itZone;
	}
	mZones.clear();
	
	while (itServer != mServers.end()) {
		delete itServer->second;
		++itServer;
	}
	mServers.clear();
}

Router::Router() : mServers(), mZones(), mUsers() {
	
}

	
Router::~Router() {
	Clear();
}


void Router::callbackFunction(Transferable* received, Connection* c) throw() {
	//TODO: Check if client/server/control
}

void Router::addServer(int idServer) {
	auto it = mServers.find(idServer);
	Server* srv = NULL;
	
	if (it == mServers.end()) {
		srv = new Server(idServer);
		mServers[idServer] = srv;
	}
}

void connectServer(int idServer, Connection* c) {
	auto it = mServers.find(idServer);
	
	if (it != mServers.end()) {
		it->second->connection(c);
	}
}

void Router::delServer(int idServer) {
	auto it = mServers.find(idServer);
	
	if (it != mServers.end()) {
		delete it->second;
		mServers.erase(it);
	}
}




void Router::addZone(int idZone) {
	auto itZone = mZones.find(idZone);
	Zone* zn = NULL;
	
	if (itZone == mZoness.end()) {
		zn = new Zone(idZone);
		itZone[idZone] = zn;
	}
}

void Router::delZone(int idZone) {
	auto itZone = mZones.find(idZone);
	
	if (itZone != mZones.end()) {
		auto itServer = mServers.find(mZones->second->server());
		if (itServer != mServers.end()) {
			itServer->delZone(idZone);
		}
		delete itZone->second;
		mZones.erase(itZone);
	}	
}

void Router::startZoneToServerTransfer(int idZone, int newServer) {
	auto itZone = mZones.find(idZone);
	auto itServer = mZones.find(idServer);
	
	if (itZone != mZones.end() && itServer != mServers.end()) {
		itZone->second->inTransfer(true);
		itZone->second->newServer(idServer);
	}
}

void Router::endZoneToServerTransfer(int idZone) {
	auto itZone = mZones.find(idZone);
	int oldServer = -1;
	int newServer = -1;
	
	if ( itZone != mZones.end() && itZone->second->inTransfer() ) {
		oldServer = itZone->second->server();
		newServer = itZone->second->newServer();
		
		auto itOldServer = mServers.find(oldServer);
		auto itNewServer = mServers.find(newServer);
		
		if (itOldServer != mServers.end() && itNewServer != mServers.end()) {
			itOldServer->second->delZone(idZone);
			itNewServer->second->addZone(itZone->second);
			itZone->second->inTransfer(false);
		} else {
			//TODO: START SCREAMING AND RUN IN CIRCLES
		}
	}
}

void Router::setUserToZone(int idUser, int idZone) {
	auto itUser = mUsers.find(idUser);
	auto itNewZone = mZones.find(idZone);
	int oldZone;
	
	if (itUser != mUsers.end() && itNewZone != mZones.end() ) {
		oldZone = itUser->second->zone();
		auto itOldZone = mZones.find(oldZone);
		
		itNewZone->second->addUser(itUser->second);
		itOldZone->second->delUser(idUser);
		itUser->second->zone(idZone);
	}
}



void Router::addUser(int idUser, int token) {
	auto itUser = mUsers.find(idUser);
	
	if (itUser == mUsers.end()) {
		User* usr = new User(idUser, token);
		mUsers[idUser] = usr;
	}
	else {
		itUser->second->token(token);
	}
}

void Router::validateUser(int idUser, int token) {
	auto itUser = mUsers.find(idUser);
	
	return (itUser != mUsers.end() && itUser->second->validate(token));
}

void Router::connectUser(int idUser, int token, Connection* c) {
	auto itUser = mUsers.find(idUser);
	
	if (itUser != mUsers.end() && itUser->second->validate(token)) {
		itUser->second->connection(c);
	}
}

void Router::delUser(int idUser) {
	auto itUser = mUsers.find(idUser);
	
	if (itUser != mUsers.end()) {
		auto itZone = mZones.find(itUser->second->zone());
		if (itZone != mZones.end()) {
			itZone->second->delUser(idUser);
		}
		delete itUser->second;
	}
}


void Router::sendToServer(int idServer, Transferable* t) {
	auto itServer = mServers.find(idServer);
	
	if (itServer != mServers.end()) {
		itServer->second->send(t);
	}
}

void Router::sendToServerUsers(int idServer, Transferable* t) {
	auto itServer = mServers.find(idServer);
	
	if (itServer != mServers.end()) {
		itServer->second->sendToUsers(t);
	}
}

void Router::sendToZone(int idZone, Transferable* t) {
	auto itZone = mZones.find(idZone);
	
	if (itZone = mZones.end()) {
		itZone->second->sendToUsers(t);
	}
}

void Router::sendToUser(int idUser, Transferable* t) {
	auto itUser = mUsers.find(idUser);
	
	if (itUser != mUsers.end()) {
		itUser->second->send(t);
	}
}
