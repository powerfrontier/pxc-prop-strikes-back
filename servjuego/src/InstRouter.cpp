#include <InstRouter.h>
#include <GameServer.h>

//ID 150
AddRouterRcvd::AddRouterRcvd() : Datagram<AddRouterRcvd>("AddRouterRcvd") { }
AddRouterRcvd::~AddRouterRcvd() { }

void AddRouterRcvd::exec(Connection* c) throw() {
	GameServer::instance().addRouter(c);
}

//ID 155
EndZoneTransferSend::EndZoneTransferSend(int zoneId) : Datagram<EndZoneTransferSend>("EndZoneTransferSend"), mZoneId(zoneId) { }
EndZoneTransferSend::~EndZoneTransferSend() { }

