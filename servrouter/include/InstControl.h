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

//ID 6
class GetServerIpRvcd : public Datagram<GetServerIpRvcd> {
	int mServerId;
	char mIpServer[16]; //Char[16] con la IP del server
	char mPortServer[5]; //Char[5] con el puerto del server
	
public:
	GetServerIpRvcd();
	~GetServerIpRvcd();
	
	void exec(Connection* c) const throw();
};
	
//ID 7: Initialize (not change) a new server/zone pair, create both if not exist
class SetZoneToServerRcvd : public Datagram<SetZoneToServerRcvd> {
	int mIdServer; //-1 as invalid
	int mIdZone; //-1 as invalid
public:
	SetZoneToServerRcvd();
	~SetZoneToServerRcvd();
	
	void exec(Connection* c) const throw();
};
	
//ID 60: Add a new user/modify it's current token
class AddUserRcvd : public Datagram<AddUserRcvd> {
	int mIdUser;
	int mToken;
	int mZoneId;
	
public:
	AddUserRcvd();
	~AddUserRcvd();
	
	void exec(Connection* c) const throw();
};

//ID 61: Removes a currently active user
class DelUserRcvd : public Datagram<DelUserRcvd> {
	int mUserId;
public:
	DelUserRcvd();
	~DelUserRcvd();
	
	void exec(Connection* c) const throw();
};

#endif
