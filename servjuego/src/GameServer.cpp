#include <GameServer.h>

bool GameServer::hasZone(int zoneId) const {
	std::map<int, ZoneHandler*>::iterator it = mZones.find(zoneId);

	return it != mZones.end();
}

GameServer::GameServer() : Singleton<GameServer>(), mZones(), mClients() {

}


GameServer::~GameServer() {
	std::map<int, ZoneHandler*>::iterator it = mZones.begin();

	while (it != mZones.end()) stopZone(it->first);

	mZones.clear();
}


void GameServer::callbackFunction(Transferable* received, Connection*) throw() {
	//TODO: CHECK CONNECTION SOURCE

	//if (balanceo) received->exec()
	//else send to game zone
}

void GameServer::addClient(int clientId, int token) {
	mClients[clientId] = token;
}

void GameServer::delClient(int clientId) {
	std::map<int, int>::iterator it = mClients.find(clientId);

	if (it != mClients.end()) mClients.erase(it);
}

bool GameServer::validateClient(int clientId, int token) {
	std::map<int, int>::iterator it = mClients.find(clientId);
	
	return (it != mClients.end() && it->second == token);
}

void GameServer::startZone(int zoneId) {
	ZoneHandler* zh = NULL;
	std::map<int, ZoneHandler*>::iterator it = mZones.find(zoneId);

	if (it != mZones.end()) {	
		zh = new ZoneHandler(zoneId);
		mZones.insert(std::pair<int, ZoneHandler*>(zoneId, zh));
	}
}

void GameServer::stopZone(int zoneId, bool force) {
	std::map<int, ZoneHandler*>::iterator it = mZones.find(zoneId);

	//If zone has already been marked for detach or it's forced
	if (it != mZones.end() && (force || it->second->detachable() != -1)) {
		delete it->second;
		mZones.erase(it);
	}
	else if (it != mZones.end() && !force) {
		//TODO: Send message "This zone has not been marke for detach)
	}
	else {
		//TODO: "Delete what?"
	}
}

void GameServer::markForDetach(int zoneId, int idServer) {
	std::map<int, ZoneHandler*>::iterator it = mZones.find(zoneId);

	if (it != mZones.end()) {
		it->second->maskAsDetachable(idServer);
	}
}

void GameServer::receiveZone(int zoneId, Connection* oldServer) {
	std::map<int, ZoneHandler*>::iterator it = mZones.find(zoneId);

	//TODO: Implement
}

void GameServer::detachZone(int zoneId, Connection* newServer) {
	std::map<int, ZoneHandler*>::iterator it = mZones.find(zoneId);

	//TODO: Implement
}

