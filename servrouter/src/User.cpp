#include <Router.h>

Router::User::User(int id, int token) : mId(id), mToken(token), mZone(-1), mConnection(NULL) {
	
}

Router::User::~User() {
	if (mConnection) {
		delete mConnection;
	}
}


int Router::User::id() const {
	return mId;
}


int Router::User::token() const {
	return mToken;
}

Router::User* Router::User::token(int token) {
	mToken = token;
	return this;
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
	if (mConnection) delete mConnection;
	mConnection = c;
}

Connection* Router::User::connection() const {
	return mConnection;
}


void Router::User::send(Transferable* t) {
	if (mConnection) mConnection->send(*t);
}
