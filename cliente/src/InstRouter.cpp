#include <InstRouter.h>
#include <Client.h>

//ID 200
ConnectClientSend::ConnectClientSend(int32_t idClient, int32_t token) : Datagram<ConnectClientSend>("ConnectClientSend"), mIdClient(idClient), mToken(token) { }
ConnectClientSend::~ConnectClientSend() { }

//RECEIVED

//ID 201
ConnectCLientReceived::ConnectCLientReceived() : Datagram<ConnectCLientReceived>("ConnectCLientReceived") { }
ConnectCLientReceived::~ConnectCLientReceived () { }

void ConnectCLientReceived::exec(Connection* ) const throw() {
	if ((Client::instance().id() == mIdClient) && Client::instance().isOnLogin()) {
		Client::instance().setZone(mZone);
		Client::instance().isOnLogin(false);
	}
}