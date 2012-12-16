#include <Router.h>

Router::Server::Server (int idServer) : mId(idServer), mZones(), mConnection(NULL) {
	
}

Router::Server::~Server() {
	if (mConnection) {
		delete mConnection;
	}
}


int Router::Server::id() const {
	return mId;
}


void Router::Server::addZone(Zone* z) {
	int idZone = -1;
	if (z) {
		idZone = z->id();
		mZones[idZone] = z;
	}
}

void Router::Server::delZone(int idZone) {
	auto itZone = mZones.find(idZone);
	if (itZone != mZones.end()) {
		mZones.erase(itZone);
	}
}

bool Router::Server::hasZone(int idZone) const {
	auto itZone = mZones.find(idZone);
	return itZone != mZones.end();
}


void Router::Server::connection(Connection* c) {
	if (mConnection) delete mConnection;
	mConnection = c;
}

Connection* Router::Server::connection() const {
	return mConnection;
}


void Router::Server::send(Transferable* t) {
	if (mConnection) mConnection->send(*t);
}

void Router::Server::sendToUsers(Transferable* t) {
	auto itZone = mZones.begin();
	
	while (itZone != mZones.end()) {
		itZone->second->sendToUsers(t);
		++itZone;
	}
}

void Router::Server::sendToUsersInZone(int idZone, Transferable* t) {
	auto itZone = mZones.find(idZone);
	
	if (itZone != mZones.end()) {
		itZone->second->sendToUsers(t);
	}
}
