#include <InstZones.h>
#include <GameServer.h>

SetZoneOrderRcvd::SetZoneOrderRcvd() : Datagram<SetZoneOrderRcvd>("SetZoneOrderRcvd") { }
SetZoneOrderRcvd::~SetZoneOrderRcvd() { }

void SetZoneOrderRcvd::exec(Connection* c) throw() {
	//TODO
}

GetZoneOrderRcvd::GetZoneOrderRcvd() : Datagram<GetZoneOrderRcvd>("GetZoneOrderRcvd") { }
GetZoneOrderRcvd::~GetZoneOrderRcvd() { }

void GetZoneOrderRcvd::exec(Connection* c) throw() {
	//TODO
}

SetZoneToServerRcvd::SetZoneToServerRcvd() : Datagram<SetZoneToServerRcvd>("SetZoneToServerRcvd") { }
SetZoneToServerRcvd::~SetZoneToServerRcvd() { }

void SetZoneToServerRcvd::exec(Connection* c) throw() {
	GameServer* gs = GameServer::instancePtr();
	
	if (gs->serverId() == -1) gs->serverId(mIdServer);
	
	if (gs->serverId() == mIdServer) gs->startZone(mIdZone);
}