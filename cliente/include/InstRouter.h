#ifndef _INST_ROUTER_H_
#define _INST_ROUTER_H_

#include <Datagram.h>

//SENT

//ID 200
class ConnectClientSend : public Datagram<ConnectClientSend> {
	int32_t mIdClient;
	int32_t mToken;
public:
	ConnectClientSend(int32_t idClient, int32_t token);
	~ConnectClientSend();
};

//RECEIVED

//ID 201

class ConnectCLientReceived : public Datagram<ConnectCLientReceived> {
	int32_t mAccessCode;
	int32_t mZone;
public:
	ConnectCLientReceived();
	~ConnectCLientReceived ();
	
	void exec(Connection*) const throw();
};

#endif

