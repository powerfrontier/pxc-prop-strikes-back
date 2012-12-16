#include <GameServer.h>
#include <InstServerLoad.h>


GameServer::ControlListener::ControlListener() : ConnectionCallback() { }
GameServer::ControlListener::~ControlListener() { }
void GameServer::ControlListener::callbackFunction(Transferable* received, Connection* c) throw() {
	if (received) {
		received->exec(c);
		delete received;
	}
}

GameServer::RouterListener::RouterListener() : ConnectionCallback() { }
GameServer::RouterListener::~RouterListener() { }
void GameServer::RouterListener::callbackFunction(Transferable* received, Connection* c) throw() {
	if(received) {
		int idZone = received->targetId();
		GameServer::instance().queueInstruction(idZone, received, c);
	}
}


bool GameServer::hasZone(int zoneId) const {
	std::map<int, ZoneHandler*>::const_iterator it = mZones.find(zoneId);

	return it != mZones.end();
}

GameServer::GameServer() 	: Singleton<GameServer>()
				, CONTROL_LISTENER(new ControlListener())
				, ROUTER_LISTENER(new RouterListener())
				, mServerId(-1)
				, mZones()
				, mClients()
				, mRouters()
				, mZonesMutex()
				, mClientsMutex()
				, mRoutersMutex() {

}


GameServer::~GameServer() {
	stopAll();
	delete CONTROL_LISTENER;
	delete ROUTER_LISTENER;
}

void GameServer::serverId(int id) {
	mServerId = mServerId == -1 ? id : mServerId;
}

int GameServer::serverId() const {
	return mServerId;
}

void GameServer::addClient(int clientId, int clientZone) {
	std::lock_guard<std::mutex> lk(mClientsMutex);
	mClients[clientId] = clientZone;
}

void GameServer::delClient(int clientId) {
	std::map<int, int>::iterator clIt;
	std::map<int, ZoneHandler*>::iterator zIt;
	
	std::lock_guard<std::mutex> lk(mClientsMutex);
	clIt = mClients.find(clientId);
	if (clIt != mClients.end()) {
		//Erase client info from its zone
		zIt = mZones.find(clIt->second);
		if (zIt != mZones.end()) {
			zIt->second->zone()->delUser(clientId);
		}
		mClients.erase(clIt);
	}
}

void GameServer::startZone(int zoneId) {
	ZoneHandler* zh = NULL;
	std::map<int, ZoneHandler*>::iterator it;
	
	std::lock_guard<std::mutex> lk(mZonesMutex);
	it = mZones.find(zoneId);
	if (it == mZones.end()) {	
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
	std::lock_guard<std::mutex> lk(mZonesMutex);
	std::map<int, ZoneHandler*>::iterator it;
	
	for (it = mZones.begin(); it != mZones.end(); ++it) {
		if (it->second) {
			it->second->stop();
			delete it->second;
		}
	}
	mZones.clear();
}

void GameServer::markForDetach(int zoneId, int idServer) {
	std::lock_guard<std::mutex> lk(mZonesMutex);
	std::map<int, ZoneHandler*>::iterator it;

	it = mZones.find(zoneId);
	if (it != mZones.end()) {
		it->second->maskAsDetachable(idServer);
	}
}

void GameServer::receiveZone(int zoneId, Connection* oldServer) {
	std::lock_guard<std::mutex> lk(mZonesMutex);
	std::map<int, ZoneHandler*>::iterator it;

	it = mZones.find(zoneId);
	//TODO: Implement
}

void GameServer::detachZone(int zoneId, Connection* newServer) {
	std::lock_guard<std::mutex> lk(mZonesMutex);
	std::map<int, ZoneHandler*>::iterator it;

	it = mZones.find(zoneId);
	//TODO: Implement
}
void GameServer::queueInstruction(int idZone, Instruction* ins, Connection* c) {
	if (ins) {
		std::lock_guard<std::mutex> lk(mZonesMutex);
		std::map<int, ZoneHandler*>::iterator it;
		
		it = mZones.find(idZone);
		if (it != mZones.end()) {
			it->second->addInstruction(ins, c);
		}
	}
}	
	
void GameServer::addRouter(int routerId, Connection* c) {
	if (c) {
		std::lock_guard<std::mutex> lk(mRoutersMutex);
		mRouters[routerId] = c;
	}
}
	
void GameServer::SendZoneLoads(Connection* c) {
	std::lock_guard<std::mutex> lk(mZonesMutex);
	auto it = mZones.begin();
	ServerLoadAnswerSend* answer = NULL;
	int idZone = -1;
	double load = 0.0;
	int remaining = 0;
	
	remaining = mZones.size();
	
	while (it != mZones.end()) {
		idZone = it->first;
		load = it->second->getLoad();
		answer = new ServerLoadAnswerSend(serverId(), idZone, load, --remaining);
		c->sendAnswer(*answer);
		delete answer;
		
		++it;
	}
}

void GameServer::sendToClients(Transferable* t) {
	std::lock_guard<std::mutex> lk(mRoutersMutex);
	auto it = mRouters.begin();
	while(it != mRouters.end()) {
		it->second->send(*t);
		++it;
	}
}
