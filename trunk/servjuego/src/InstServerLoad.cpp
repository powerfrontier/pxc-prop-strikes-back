#include <InstServerLoad.h>

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