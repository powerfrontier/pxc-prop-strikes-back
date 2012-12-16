#ifndef _INST_ROUTER_H_
#define _INST_ROUTER_H_

#include <Datagram.h>

//SENT

//ID 
class EndZoneTransferSend : public Datagram<EndZoneTransferSend> {
	int mZoneId;
public:
	EndZoneTransferSend(int zoneId);
	~EndZoneTransferSend();
};

//RECEIVED

//ID 150
class AddRouterRcvd : public Datagram<AddRouterRcvd> {
public:
	AddRouterRcvd();
	~AddRouterRcvd();
	
	void exec(Connection* c) throw();
};

#endif
