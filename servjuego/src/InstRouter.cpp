#include <InstRouter.h>
#include <GameServer.h>

//ID 8
AddUserRcvd::AddUserRcvd() : Datagram<AddUserRcvd>("AddUserRcvd") { }
AddUserRcvd::~AddUserRdcvd() { }

void AddUserRcvd::exec(Connection* c) throw() {
	GameServer::instance().addClient(mIdUser, mIdZone);
}

//ID 9
DelUserRcvd::DelUserRcvd() : Datagram<DelUserRcvd>("DelUserRcvd") { }
DelUserRcvd::~DelUserRcvd() { }

void DelUserRcvd::exec(Connection* c) throw() {
	GameServer::instance().delClient(mIdUser);
}

//ID 150
AddRouterRcvd::AddRouterRcvd() : Datagram<AddRouterRcvd>("AddRouterRcvd") { }
AddRouterRcvd::~AddRouterRcvd() { }

void AddRouterRcvd::exec(Connection* c) throw() {
	GameServer::instance().addRouter(c);
}

//ID 155
EndZoneTransferSend::EndZoneTransferSend(int zoneId) : Datagram<EndZoneTransferSend>("EndZoneTransferSend"), mZoneId(zoneId) { }
EndZoneTransferSend::~EndZoneTransferSend() { }

