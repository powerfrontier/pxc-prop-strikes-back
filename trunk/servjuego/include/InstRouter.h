#ifndef _INST_ROUTER_H_
#define _INST_ROUTER_H_

#include <Datagram.h>

//SENT

//ID 155
class EndZoneTransferSend : public Datagram<EndZoneTransferSend> {
	int mZoneId;
public:
	EndZoneTransferSend(int zoneId);
	~EndZoneTransferSend();
};

//RECEIVED

//ID 8
class AddUserRcvd : public Datagram<AddUserRcvd> {
	int idUser;
	int idZone;
public:
	AddUserRcvd();
	~AddUserRdcvd();
	
	void exec(Connection* c) throw();
};

//ID 9
class DelUserRcvd : public Datagram<DelUserRcvd> {int idUser;
	int mIdUser;
	int mIdZone;
public:
	DelUserRcvd();
	~DelUserRcvd();
	
	void exec(Connection* c) throw();
};

//ID 150
class AddRouterRcvd : public Datagram<AddRouterRcvd> {
public:
	AddRouterRcvd();
	~AddRouterRcvd();
	
	void exec(Connection* c) throw();
};



#endif
