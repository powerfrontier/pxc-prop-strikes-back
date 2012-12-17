#include <Client.h>

ClientLoginListener::ClientLoginListener() : ConnectionCallback() { }
ClientLoginListener::~ClientLoginListener() { }

void ClientLoginListener::callbackFunction(Transferable* received, Connection*) throw() {

}

ClientRouterListener::ClientRouterListener() : ConnectionCallback() { }
ClientRouterListener::~ClientRouterListener() { }

void ClientRouterListener::callbackFunction(Transferable* received, Connection*) throw() {

}



Client::Client() 	: CONTROL_LISTENER(new ClientControlListener())
			, ROUTER_LISTENER(new ClientRouterListener())
			, mGame(NULL)
			, mUserId(-1)
			, mUser()
			, mPassword()
			, mLoginIp()
			, mLoginPort()
			, mRouterIp()
			, mRouterPort()
			, mLogin(NULL)
			, mRouter(NULL)
			, mLoggedIn(false) {

}


Client::~Client() {
	if (CONTROL_LISTENER) delete CONTROL_LISTENER;
	if (ROUTER_LISTENER) delete ROUTER_LISTENER;
	if (mGame) delete mGame;
	if (mLogin) delete mLogin;
	if (mRouter) delete mRouter);
}

void Client::id(int);
int Client::id() const;


void Client::setLoginAddress(const std::string& ip, const std::string& port);
bool Client::login(const std::string& user, const std::string& password);

void Client::routerConnection(Connection*);

void Client::setZone(int idZone);
Zone* Client::zone();

void Client::addInstruction(Instruction*, Connection*) throw();

void Client::logout();
void Client::sendAction(Transferable*);
