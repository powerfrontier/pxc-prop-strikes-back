#ifndef _INST_SERVER_H_
#define _INST_SERVER_H_

#include <Datagram.h>

//SENT

//ID 150
class AddRouterSend : public Datagram<AddRouterSend> {
public:
	AddRouterSend();
	~AddRouterSend();
};


//RECEIVED

//ID 155
class EndZoneTransferRcvd : public Datagram<EndZoneTransferRcvd> {
	int mZoneId;
public:
	EndZoneTransferRcvd();
	~EndZoneTransferRcvd();
	
	void exec(Connection* c) throw();
};

#endif

