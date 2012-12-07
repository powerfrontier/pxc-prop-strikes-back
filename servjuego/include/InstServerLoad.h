#ifndef _INST_SERVER_LOAD_H_
#define _INST_SERVER_LOAD_H_

#include <Datagram.h>

class ServerLoadOrderRcvd : public Datagram<ServerLoadRcvd> {
public:
	void exec(Connection* c) const throw();
};

class ServerLoadAnswerSend : public Datagram<ServerLoadAnswRcvd> {
	int mIdServer;
	int mIdZone;
	double mZoneLoad;
	int mRemainingZones;
	
public:
	ServerLoadAnswerSend(int idServer, int idZone, double zoneLoad, int mRemainingZones);
	~ServerLoadAnswerSend();
};

#endif

