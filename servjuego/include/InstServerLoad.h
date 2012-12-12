#ifndef _INST_SERVER_LOAD_H_
#define _INST_SERVER_LOAD_H_

#include <Datagram.h>

//ID 2
class ServerLoadOrderRcvd : public Datagram<ServerLoadRcvd> {
public:
	void exec(Connection* c) const throw();
};

//ID 0
class ServerLoadAnswerSend : public Datagram<ServerLoadAnswRcvd> {
	int mIdServer;
	int mIdZone;
	double mZoneLoad;
	int mRemainingZones;
	
public:
	ServerLoadAnswerSend(int idServer, int idZone, double zoneLoad, int mRemainingZones);
	~ServerLoadAnswerSend();
};

//ID 1
class GetServerIpSend : public Datagram<GetServerIpSend> {
	int mServerId;
};

//ID 6
class SetServerIpRvcd : public Datagram<SetServerIpRcvd> {
	char mIPServer[16]; //Char[16] con la IP del server
	char mPortServer[5]; //Char[5] con el puerto del server
	
	void exec(Connection* c) const throw();
};


#endif

