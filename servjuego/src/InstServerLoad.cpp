#include <InstServerLoad.h>

ServerLoadOrderRcvd::ServerLoadOrderRcvd() : Datagram<ServerLoadOrderRcvd>("ServerLoadOrderRcvd") { }
ServerLoadOrderRcvd::~ServerLoadOrderRcvd() { }

void ServerLoadOrderRcvd::exec(Connection* c) const throw() {
	//TODO
}

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

GetServerIpSend::GetServerIpSend(int serverId) 	: Datagram<GetServerIpSend>("GetServerIpSend")
						, mServerId(serverId) { }
						
GetServerIpSend::~GetServerIpSend() { }

SetServerIpRvcd::SetServerIpRvcd() : Datagram<SetServerIpRvcd>("SetServerIpRvcd") { }
SetServerIpRvcd::~SetServerIpRvcd() { }

void SetServerIpRvcd::exec(Connection* c) const throw() {
	//TODO
}