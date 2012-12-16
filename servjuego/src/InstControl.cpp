#include <InstControl.h>
#include <GameServer.h>

//SENT

//ID 0
ServerLoadAnswerSend::ServerLoadAnswerSend	( int idServer, int idZone, double zoneLoad
						, int remainingZones)
						: Datagram<ServerLoadAnswerSend>("ServerLoadAnswerSend")
						, mIdServer(idServer)
						, mIdZone(idZone)
						, mZoneLoad(zoneLoad)
						, mRemainingZones(remainingZones) {
	
}

ServerLoadAnswerSend::~ServerLoadAnswerSend() {
	
}

//ID 1
GetServerIpSend::GetServerIpSend(int serverId) 	: Datagram<GetServerIpSend>("GetServerIpSend")
						, mServerId(serverId) { }
						
GetServerIpSend::~GetServerIpSend() { }

//RECEIVED

//ID 2
ServerLoadOrderRcvd::ServerLoadOrderRcvd() : Datagram<ServerLoadOrderRcvd>("ServerLoadOrderRcvd") { }
ServerLoadOrderRcvd::~ServerLoadOrderRcvd() { }

void ServerLoadOrderRcvd::exec(Connection* c) const throw() {
	GameServer::instance().SendZoneLoads(c);
}

//ID 3
SetZoneOrderRcvd::SetZoneOrderRcvd() : Datagram<SetZoneOrderRcvd>("SetZoneOrderRcvd") { }
SetZoneOrderRcvd::~SetZoneOrderRcvd() { }

void SetZoneOrderRcvd::exec(Connection* c) throw() {
}

//ID 4
GetZoneOrderRcvd::GetZoneOrderRcvd() : Datagram<GetZoneOrderRcvd>("GetZoneOrderRcvd") { }
GetZoneOrderRcvd::~GetZoneOrderRcvd() { }

void GetZoneOrderRcvd::exec(Connection* c) throw() {
	//TODO
}

//ID 6
SetServerIpRvcd::SetServerIpRvcd() : Datagram<SetServerIpRvcd>("SetServerIpRvcd") { }
SetServerIpRvcd::~SetServerIpRvcd() { }

void SetServerIpRvcd::exec(Connection* c) const throw() {
	//TODO
}

//ID 7
SetZoneToServerRcvd::SetZoneToServerRcvd() : Datagram<SetZoneToServerRcvd>("SetZoneToServerRcvd") { }
SetZoneToServerRcvd::~SetZoneToServerRcvd() { }

void SetZoneToServerRcvd::exec(Connection* c) const throw() {
	GameServer* gs = GameServer::instancePtr();
	
	if (gs->serverId() == -1) gs->serverId(mIdServer);
	
	if (gs->serverId() == mIdServer) gs->startZone(mIdZone);
}