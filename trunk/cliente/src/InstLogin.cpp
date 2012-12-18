#include <InstLogin.h>
#include <InstRouter.h>
#include <cstring>
#include <Client.h>


//ID 100
LoginRequestSend::LoginRequestSend(const std::string& username, const std::string& password) : Datagram<LoginRequestSend>("LoginRequestSend") {
	strncpy((char*)mUsername, username.c_str(), 31);  
	strncpy((char*)mPassword, password.c_str(), 31);
	mUsername[31] = 0;
	mPassword[31] = 0;
}

LoginRequestSend::~LoginRequestSend() { }


//RECEIVED

// 101
LoginRequestRcvd::LoginRequestRcvd() : Datagram<LoginRequestRcvd>("LoginRequestRcvd") { }
LoginRequestRcvd::~LoginRequestRcvd() { }
  
void LoginRequestRcvd::exec(Connection*) const throw() {
	Connection* router = NULL;
	ConnectClientSend* conn = NULL;
	
	if (mAnswerCode != 0) {
		Client::instance().correctLogin(false);
		return;
	}
	
	Client::instance().id(mClientId);
	Client::instance().token(mToken);
	
	router = new TCPConnectionSecurity();
	if (!router->connect(mRouterIp, mRouterPort)) {
		delete router;
		Client::instance().isOnLogin(false);
		return;
	}
	Client::instance().routerConnection(router);
	
	conn = new ConnectClientSend(mClientId, mToken);
	router->send(*conn);
	delete conn;
	
}