#include <InstServer.h>
#include <Router.h>

//SENT

//ID 150
AddRouterSend::AddRouterSend() : Datagram<AddRouterSend>("AddRouterSend") { }
AddRouterSend::~AddRouterSend() { }



//RECEIVED

//ID 8
SetAddUserRcvd::SetAddUserRcvd() : Datagram<SetAddUserRcvd>("SetAddUserRcvd") { }
SetAddUserRcvd::~SetAddUserRcvd() { }

void SetAddUserRcvd::exec(Connection* c) throw() {
	//TODO
}

//ID 9
SetDelUserRcvd::SetDelUserRcvd() : Datagram<SetDelUserRcvd>("SetDelUserRcvd") { }
SetDelUserRcvd::~SetDelUserRcvd() { }

void SetDelUserRcvd::exec(Connection* c) throw() {
	//TODO
}

//ID 155
EndZoneTransferRcvd::EndZoneTransferRcvd() : Datagram<EndZoneTransferRcvd>("EndZoneTransferRcvd") { }
EndZoneTransferRcvd::~EndZoneTransferRcvd() { }

void EndZoneTransferRcvd::exec(Connection* c) throw() {
	
}
