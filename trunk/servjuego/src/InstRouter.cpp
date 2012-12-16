#include <InstRouter.h>
#include <GameServer.h>

AddRouterRcvd::AddRouterRcvd() : Datagram<AddRouterRcvd>("AddRouterRcvd") { }
AddRouterRcvd::~AddRouterRcvd() { }

void AddRouterRcvd::exec(Connection* c) throw() {
	GameServer::instance().addRouter(c);
}
