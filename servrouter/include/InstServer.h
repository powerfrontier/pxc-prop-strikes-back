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
class SetAddUserRcvd : public Datagram<SetAddUserRcvd> {
	int32_t idUser;
	int32_t idZone;
public:
	SetAddUserRcvd();
	~SetAddUserRcvd();
	
	void exec(Connection* c) throw();
};

//ID 9
class SetDelUserRcvd : public Datagram<SetDelUserRcvd> {
	int32_t mIdUser;
	int32_t mIdZone;
public:
	SetDelUserRcvd();
	~SetDelUserRcvd();
	
	void exec(Connection* c) throw();
};

//ID 155
class EndZoneTransferRcvd : public Datagram<EndZoneTransferRcvd> {
	int32_t mZoneId;
public:
	EndZoneTransferRcvd();
	~EndZoneTransferRcvd();
	
	void exec(Connection* c) throw();
};

#endif

