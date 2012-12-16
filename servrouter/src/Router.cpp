#include <Router.h>
#include <InstServer.h>

Router::ControlListener::ControlListener() { }
Router::ControlListener::~ControlListener() { }
void Router::ControlListener::callbackFunction(Transferable* received, Connection* c) throw() {
	if (received) {
		received->exec(c);
		delete received;
	}
}


Router::ServerListener::ServerListener() { }
Router::ServerListener::~ServerListener() { }
void Router::ServerListener::callbackFunction(Transferable* received, Connection* c) throw() {
	if (received) {
		received->exec(c);
		delete received;
	}
}


Router::ClientListener::ClientListener() { }
Router::ClientListener::~ClientListener() { }
void Router::ClientListener::callbackFunction(Transferable* received, Connection* c) throw() {
	int serverId = -1;
	if (received) {
		serverId = received->targetId();
		Router::instance().sendToServerUsers(serverId, received);
		delete received;
	}
}


void Router::clear() {
	mRouterUsersMutex.lock();
	auto itUser = mUsers.begin();
	while (itUser != mUsers.end()) {
		delete itUser->second;
		++itUser;
	}
	mUsers.clear();
	mRouterUsersMutex.unlock();
	
	mRouterZonesMutex.lock();
	auto itZone = mZones.begin();
	while (itZone != mZones.end()) {
		delete itZone->second;
		++itZone;
	}
	mZones.clear();
	mRouterZonesMutex.unlock();
	
	mRouterServersMutex.lock();
	auto itServer = mServers.begin();
	while (itServer != mServers.end()) {
		delete itServer->second;
		++itServer;
	}
	mServers.clear();
	mRouterServersMutex.unlock();
}

Router::Router()	: CONTROL_LISTENER(new ControlListener())
			, SERVER_LISTENER(new ServerListener())
			, CLIENT_LISTENER(new ClientListener())
			, mRouterUsersMutex()
			, mRouterZonesMutex()
			, mRouterServersMutex()
			, mServers()
			, mZones()
			, mUsers() {
	
}

	
Router::~Router() {
	clear();
	delete CONTROL_LISTENER;
	delete SERVER_LISTENER;
	delete CLIENT_LISTENER;
}

void Router::addServer(int idServer) {
	std::lock_guard<std::mutex> lk(mRouterServersMutex);
	auto it = mServers.find(idServer);
	Server* srv = NULL;
	
	if (it == mServers.end()) {
		srv = new Server(idServer);
		mServers[idServer] = srv;
	}
}

void Router::connectServer(int idServer, Connection* c) {
	std::lock_guard<std::mutex> lk(mRouterServersMutex);
	auto it = mServers.find(idServer);
	AddRouterSend* addRouter = NULL;
	
	if (it != mServers.end()) {
		it->second->connection(c);
		addRouter = new AddRouterSend();
		c->send(*addRouter);
		delete addRouter;
	}
}

void Router::delServer(int idServer) {
	std::lock_guard<std::mutex> lk(mRouterServersMutex);
	auto it = mServers.find(idServer);
	
	if (it != mServers.end()) {
		delete it->second;
		mServers.erase(it);
	}
}



void Router::addZone(int idZone) {
	std::lock_guard<std::mutex> lk(mRouterZonesMutex);
	auto itZone = mZones.find(idZone);
	Zone* zn = NULL;
	
	if (itZone == mZones.end()) {
		zn = new Zone(idZone);
		mZones[idZone] = zn;
	}
}

void Router::delZone(int idZone) {
	std::lock_guard<std::mutex> lk(mRouterZonesMutex);
	auto itZone = mZones.find(idZone);
	
	if (itZone != mZones.end()) {
		auto itServer = mServers.find(itZone->second->server());
		if (itServer != mServers.end()) {
			itServer->second->delZone(idZone);
		}
		delete itZone->second;
		mZones.erase(itZone);
	}	
}

void Router::startZoneToServerTransfer(int idZone, int newServer) {
	std::lock_guard<std::mutex> lk(mRouterZonesMutex);
	std::lock_guard<std::mutex> lk2(mRouterServersMutex);
	auto itZone = mZones.find(idZone);
	auto itServer = mServers.find(newServer);
	
	if (itZone != mZones.end() && itServer != mServers.end()) {
		itZone->second->newServer(newServer);
	}
}

void Router::endZoneToServerTransfer(int idZone) {
	std::lock_guard<std::mutex> lk(mRouterZonesMutex);
	std::lock_guard<std::mutex> lk2(mRouterServersMutex);
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
			itZone->second->endTransfer();
		} else {
			//TODO: START SCREAMING AND RUN IN CIRCLES
		}
	}
}

void Router::setUserToZone(int idUser, int idZone) {
	std::lock_guard<std::mutex> lk(mRouterUsersMutex);
	std::lock_guard<std::mutex> lk2(mRouterZonesMutex);
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
	std::lock_guard<std::mutex> lk(mRouterUsersMutex);
	auto itUser = mUsers.find(idUser);
	
	if (itUser == mUsers.end()) {
		User* usr = new User(idUser, token);
		mUsers[idUser] = usr;
	}
	else {
		itUser->second->token(token);
	}
}

bool Router::validateUser(int idUser, int token) {
	std::lock_guard<std::mutex> lk(mRouterUsersMutex);
	auto itUser = mUsers.find(idUser);
	
	return (itUser != mUsers.end() && itUser->second->validate(token));
}

void Router::connectUser(int idUser, int token, Connection* c) {
	std::lock_guard<std::mutex> lk(mRouterUsersMutex);
	auto itUser = mUsers.find(idUser);
	
	if (itUser != mUsers.end() && itUser->second->validate(token)) {
		itUser->second->connection(c);
	}
}

void Router::delUser(int idUser) {
	std::lock_guard<std::mutex> lk(mRouterUsersMutex);
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
	std::lock_guard<std::mutex> lk(mRouterServersMutex);
	auto itServer = mServers.find(idServer);
	
	if (itServer != mServers.end()) {
		itServer->second->send(t);
	}
}

void Router::sendToServerUsers(int idServer, Transferable* t) {
	std::lock_guard<std::mutex> lk(mRouterServersMutex);
	auto itServer = mServers.find(idServer);
	
	if (itServer != mServers.end()) {
		itServer->second->sendToUsers(t);
	}
}

void Router::sendToZone(int idZone, Transferable* t) {
	std::lock_guard<std::mutex> lk(mRouterZonesMutex);
	auto itZone = mZones.find(idZone);
	
	if (itZone == mZones.end()) {
		itZone->second->sendToUsers(t);
	}
}

void Router::sendToUser(int idUser, Transferable* t) {
	std::lock_guard<std::mutex> lk(mRouterUsersMutex);
	auto itUser = mUsers.find(idUser);
	
	if (itUser != mUsers.end()) {
		itUser->second->send(t);
	}
}
