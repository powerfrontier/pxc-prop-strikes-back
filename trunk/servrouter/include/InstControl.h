#ifndef _INST_CONTROL_H_
#define _INST_CONTROL_H_

#include <Datagram.h>

//SENT

//ID 1
class GetServerIpSend : public Datagram<GetServerIpSend> {
	int mServerId;
public:
	GetServerIpSend(int serverId);
	~GetServerIpSend();
};



//RECEIVED


//ID 5
class ChangeZoneRcvd : public Datagram<ChangeZoneRcvd> {
	int32_t mIdZone;
	int32_t mIdSourceServer;
	int32_t mIdDestServer;
public:
	ChangeZoneRcvd();
	~ChangeZoneRcvd();
	
	void exec(Connection* c) const throw();
};

//ID 6
class GetServerIpRvcd : public Datagram<GetServerIpRvcd> {
	int32_t mServerId;
	char mIpServer[16]; //Char[16] con la IP del server
	char mPortServer[5]; //Char[5] con el puerto del server
	
public:
	GetServerIpRvcd();
	~GetServerIpRvcd();
	
	void exec(Connection* c) const throw();
};
	
//ID 7: Initialize (not change) a new server/zone pair, create both if not exist
class SetZoneToServerRcvd : public Datagram<SetZoneToServerRcvd> {
	int32_t mIdServer; //-1 as invalid
	int32_t mIdZone; //-1 as invalid
public:
	SetZoneToServerRcvd();
	~SetZoneToServerRcvd();
	
	void exec(Connection* c) const throw();
};
	
//ID 60: Add a new user/modify it's current token
class AddUserRcvd : public Datagram<AddUserRcvd> {
	int32_t mIdUser;
	int32_t mToken;
	int32_t mZoneId;
	
public:
	AddUserRcvd();
	~AddUserRcvd();
	
	void exec(Connection* c) const throw();
};

//ID 61: Removes a currently active user
class DelUserRcvd : public Datagram<DelUserRcvd> {
	int32_t mUserId;
public:
	DelUserRcvd();
	~DelUserRcvd();
	
	void exec(Connection* c) const throw();
};


class ConnectUserRcvd : public Datagram<ConnectUserRcvd> {
	int32_t mUserId;
	int32_t mToken;
public:
	ConnectUserRcvd();
	~ConnectUserRcvd();
	
	void exec(Connection* c) const throw();
};

class ConnectUserSend : public Datagram<ConnectUserSend> {
	int32_t mUserId;
	int32_t mToken;
public:
	ConnectUserSend(int32_t id, int32_t t);
	~ConnectUserSend();

};

#endif
