#ifndef _INST_SERVER_H_
#define _INST_SERVER_H_

#include <Datagram.h>

//SENT

//ID 8
class AddUserSend : public Datagram<AddUserSend> {
	
};

//ID 9
class DelUserSend : public Datagram<DelUserSend> {
	
};

//ID 150
class AddRouterSend : public Datagram<AddRouterSend> {
public:
	AddRouterSend();
	~AddRouterSend();
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

//ID 155
class EndZoneTransferRcvd : public Datagram<EndZoneTransferRcvd> {
	int mZoneId;
public:
	EndZoneTransferRcvd();
	~EndZoneTransferRcvd();
	
	void exec(Connection* c) throw();
};

#endif

