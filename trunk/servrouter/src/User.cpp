#include <Router.h>

Router::User::User(int id, int token) mId(id), mToken(token), mZone(-1), mConnection(NULL) {
	
}

Router::User::~User() {
	delete mConnection;
}


int Router::User::id() const {
	return mId;
}


int Router::User::token() const {
	return mToken;
}

User& Router::User::token(int token) {
	mToken = token;
}

bool Router::User::validate(int token) {
	return mToken = token;
}


void Router::User::zone(int idZone) {
	mZone = idZone;
}

int Router::User::zone() const {
	return mZone;
}


void Router::User::connection(Connection* c) {
	mConnection = c;
}

Connection* Router::User::Connection() const {
	return mConnection;
}


void Router::User::send(Transferable* t) {
	if (mConnection) mConnection->send(t);
}
