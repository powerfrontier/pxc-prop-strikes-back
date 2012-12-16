#include <InstServer.h>
#include <Router.h>

//SENT

//ID 150
AddRouterSend::AddRouterSend() : Datagram<AddRouterSend>("AddRouterSend") { }
AddRouterSend::~AddRouterSend() { }



//RECEIVED

//ID 155
EndZoneTransferRcvd::EndZoneTransferRcvd() : Datagram<EndZoneTransferRcvd>("EndZoneTransferRcvd") { }
EndZoneTransferRcvd::~EndZoneTransferRcvd() { }

void EndZoneTransferRcvd::exec(Connection* c) throw() {
	
}
