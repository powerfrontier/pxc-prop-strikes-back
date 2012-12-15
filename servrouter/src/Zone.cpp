#include <Router.h>

class Router::Zone:: {
		int mId;
		int mServer;
		int mNewServer;
		std::map<int, User*> mUsers;
		
	public:
Router::Zone::Zone(int id, int server) : mId(id), mServer(server), mNewServer(-1), mUsers(), mTransfer(false) {
	
}

int Router::Zone::server() const {
	return mServer;
}

void Router::Zone::server(int server) {
	mServer = server;
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
			mUsers[idUser] = usr;
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
		++itUsers;
	}
}
