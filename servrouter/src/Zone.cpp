#include <Router.h>


Router::Zone::Zone(int id) : mId(id), mServer(-1), mNewServer(-1), mUsers(), mInTransfer(false) {
	
}

int Router::Zone::server() const {
	return mServer;
}

void Router::Zone::server(int server) {
	mServer = server;
}

int Router::Zone::id() const {
	return mId;
}

void Router::Zone::endTransfer() {
	if (mInTransfer) {
		mServer = mNewServer;
		mNewServer = -1;
		mInTransfer = false;
	}
}

bool Router::Zone::inTransfer() const {
	return mInTransfer;
}


int Router::Zone::newServer() const {
	return mNewServer;
}

void Router::Zone::newServer(int newServer) {
	mNewServer = newServer;
	mInTransfer = true;
}


void Router::Zone::addUser(User* usr) {
	if (usr) {
		auto itUser = mUsers.find(usr->id());
		if (itUser == mUsers.end()) {
			mUsers[usr->id()] = usr;
		}
	}
}

void Router::Zone::delUser(int idUser) {
	auto itUser = mUsers.find(idUser);
	if (itUser != mUsers.end()) {
		mUsers.erase(itUser);
	}
}

bool Router::Zone::hasUser(int idUser) {
	auto itUser = mUsers.find(idUser);
	return itUser != mUsers.end();
}


void Router::Zone::sendToUsers(Transferable* t) {
	auto itUser = mUsers.begin();
	
	while (itUser != mUsers.end()) {
		itUser->second->send(t);
		++itUser;
	}
}
