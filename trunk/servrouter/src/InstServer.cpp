#include <InstServer.h>
#include <Router.h>

//SENT

//ID 150
AddRouterSend::AddRouterSend() : Datagram<AddRouterSend>("AddRouterSend") { }
AddRouterSend::~AddRouterSend() { }



//RECEIVED

//ID 8
AddUserRcvd::AddUserRcvd() : Datagram<AddUserRcvd>("AddUserRcvd") { }
AddUserRcvd::~AddUserRdcvd() { }

void AddUserRcvd::exec(Connection* c) throw() {
	//TODO
}

//ID 9
DelUserRcvd::DelUserRcvd() : Datagram<DelUserRcvd>("DelUserRcvd") { }
DelUserRcvd::~DelUserRcvd() { }

void DelUserRcvd::exec(Connection* c) throw() {
	//TODO
}

//ID 155
EndZoneTransferRcvd::EndZoneTransferRcvd() : Datagram<EndZoneTransferRcvd>("EndZoneTransferRcvd") { }
EndZoneTransferRcvd::~EndZoneTransferRcvd() { }

void EndZoneTransferRcvd::exec(Connection* c) throw() {
	
}
