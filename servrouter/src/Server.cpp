#include <Router.h>

class Router::Server:: {
int mId;
std::map<int, Zone*> mZones;
Connection* mConnection;

public:
Router::Server::Server (int idServer) : mId(idServer) {
	
}

Router::Server::~Server() {
	
}


int Router::Server::id() const {
	return mId;
}


void Router::Server::addZone(Zone* z) {
	if (z) {
		
	}
}

void Router::Server::delZone(int) {
	
}

bool Router::Server::hasZone(int) const {
	
}


void Router::Server::connection(Connection*) {
	
}

Connection* Router::Server::connection() const {
	
}


void Router::Server::send(Transferable*) {
	
}

void Router::Server::sendToUsers(Transferable*) {
	
}

void Router::Server::sendToUsersInZone(int idZone, Transferable* t) {
	
}
