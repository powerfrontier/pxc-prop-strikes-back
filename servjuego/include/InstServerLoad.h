#ifndef _INST_SERVER_LOAD_H_
#define _INST_SERVER_LOAD_H_

#include <Datagram.h>

//ID 2
class ServerLoadOrderRcvd : public Datagram<ServerLoadOrderRcvd> {
public:
	ServerLoadOrderRcvd();
	~ServerLoadOrderRcvd();
	
	void exec(Connection* c) const throw();
};

//ID 0
class ServerLoadAnswerSend : public Datagram<ServerLoadAnswerSend> {
	int mIdServer;
	int mIdZone;
	double mZoneLoad;
	int mRemainingZones;
	
public:
	ServerLoadAnswerSend(int idServer, int idZone, double zoneLoad, int remainingZones);
	~ServerLoadAnswerSend();
};

//ID 1
class GetServerIpSend : public Datagram<GetServerIpSend> {
	int mServerId;
public:
	GetServerIpSend(int serverId);
	~GetServerIpSend();
};

//ID 6
class SetServerIpRvcd : public Datagram<SetServerIpRvcd> {
	char mIPServer[16]; //Char[16] con la IP del server
	char mPortServer[5]; //Char[5] con el puerto del server
	
public:
	SetServerIpRvcd();
	~SetServerIpRvcd();
	
	void exec(Connection* c) const throw();
};


#endif

