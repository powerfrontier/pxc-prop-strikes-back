#include <GameServer.h>

bool GameServer::hasZone(int zoneId) const {
	std::map<int, ZoneHandler*>::const_iterator it = mZones.find(zoneId);

	return it != mZones.end();
}

GameServer::GameServer() : Singleton<GameServer>(), mServerId(-1), mZones(), mClients() {

}


GameServer::~GameServer() {
	stopAll();
}


void GameServer::callbackFunction(Transferable* received, Connection* c) throw() {
	//TODO: CHECK CONNECTION SOURCE

	//if (balanceo) received->exec()
	//else send to game zone
}

void GameServer::serverId(int id) {
	mServerId = mServerId == -1 ? id : mServerId;
}

int GameServer::serverId() const {
	return mServerId;
}

void GameServer::addClient(int clientId, int clientZone, int token) {
	std::lock_guard<std::mutex> lk(mClientsMutex);
	mClients[clientId] = std::pair< int, int >(clientZone, token);
}

void GameServer::delClient(int clientId) {
	std::map<int, std::pair<int, int> >::iterator it;

	std::lock_guard<std::mutex> lk(mClientsMutex);
	it = mClients.find(clientId);
	if (it != mClients.end()) mClients.erase(it);
}

bool GameServer::validateClient(int clientId, int clientZone, int token) {
	std::map<int, std::pair<int, int> >::iterator it;
	
	std::lock_guard<std::mutex> lk(mClientsMutex);
	it= mClients.find(clientId);
	return (it != mClients.end() && (it->second).second == token);
}

void GameServer::startZone(int zoneId) {
	ZoneHandler* zh = NULL;
	std::map<int, ZoneHandler*>::iterator it;
	
	std::lock_guard<std::mutex> lk(mZonesMutex);
	it = mZones.find(zoneId);
	if (it != mZones.end()) {	
		zh = new ZoneHandler(zoneId);
		mZones.insert(std::pair<int, ZoneHandler*>(zoneId, zh));
		zh->start();
	}
}

void GameServer::stopZone(int zoneId, bool force) {
	std::map<int, ZoneHandler*>::iterator it;

	std::lock_guard<std::mutex> lk(mZonesMutex);
	it = mZones.find(zoneId);
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

void GameServer::stopAll() {
	std::map<int, ZoneHandler*>::iterator it;
	
	std::lock_guard<std::mutex> lk(mZonesMutex);
	for (it = mZones.begin(); it != mZones.end(); ++it) {
		if (it->second) {
			it->second->stop();
			delete it->second;
		}
	}
	mZones.clear();
}

void GameServer::markForDetach(int zoneId, int idServer) {
	std::map<int, ZoneHandler*>::iterator it;

	std::lock_guard<std::mutex> lk(mZonesMutex);
	it = mZones.find(zoneId);
	if (it != mZones.end()) {
		it->second->maskAsDetachable(idServer);
	}
}

void GameServer::receiveZone(int zoneId, Connection* oldServer) {
	std::map<int, ZoneHandler*>::iterator it;

	std::lock_guard<std::mutex> lk(mZonesMutex);
	it = mZones.find(zoneId);
	//TODO: Implement
}

void GameServer::detachZone(int zoneId, Connection* newServer) {
	std::map<int, ZoneHandler*>::iterator it;

	std::lock_guard<std::mutex> lk(mZonesMutex);
	it = mZones.find(zoneId);
	//TODO: Implement
}

