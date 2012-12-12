#include <InstZones.h>
#include <GameServer.h>

void SetZoneOrderRcvd::exec(Connection* c) throw() {
	//TODO
}

void GetZoneOrderRcvd::exec(Connection* c) throw() {
	//TODO
}

void SetZoneToServerRcvd::exec(Connection* c) throw() {
	GameServer* gs = GameServer::instancePtr();
	
	if (gs->serverId() == -1) gs->serverId(mIdServer);
	
	if (gs->serverId() == mIdServer) gs->startZone(mIdZone);
}