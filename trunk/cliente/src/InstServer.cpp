#include <InstServer.h>
#include <Client.h>
#include <PXCGame.h>
//SENT

//ID 502
SetDestinationSend::SetDestinationSend(int32_t id, int32_t token, double x, double y, double z) : Datagram<SetDestinationSend>("SetDestinationSend"), mUserId(id), mToken(token), mDestination{x, y, z} { }
SetDestinationSend::~SetDestinationSend() { }

//ID 501
SetZoneSend::SetZoneSend(int32_t id, int32_t token, int32_t zone) : Datagram<SetZoneSend>("SetZoneSend"), mUserId(id), mToken(token), mZone(zone) { }
SetZoneSend::~SetZoneSend() { }


//RECEIVED

//ID 501
SetZoneRcvd::SetZoneRcvd() : Datagram<SetZoneRcvd>("SetZoneRcvd") { }
SetZoneRcvd::~SetZoneRcvd() { }

void SetZoneRcvd::exec(Connection*) const throw() {
	Client::instance().setZone(mZone);
}

//ID 500
SetUserStatusRcvd::SetUserStatusRcvd() : Datagram<SetUserStatusRcvd>("SetUserStatusRcvd") { }
SetUserStatusRcvd::~SetUserStatusRcvd() { }

void SetUserStatusRcvd::exec(Connection*) const throw() {
	PXCGame* game = static_cast<PXCGame*>(Client::instance().game());
	if (game) {
		if (game->zone()->id() == mZone) {
			game->setUserState(mUserId, mPos, mDir, mSpeed);
		}
	}
}
