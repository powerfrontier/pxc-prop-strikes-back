#ifndef _INST_CLIENT_H_
#define _INST_CLIENT_H_

#include <Datagram.h>

//SENT

//ID 201
class ConnectCLientSend : public Datagram<ConnectCLientSend> {
	int32_t mIdClient;
	int32_t mZone;
public:
	ConnectCLientSend();
	~ConnectCLientSend();
	
};

//RECEIVED


//ID 200
class ConnectCLientRcvd : public Datagram<ConnectCLientRcvd> {
	int32_t mIdClient;
	int32_t mToken;
public:
	ConnectCLientRcvd();
	~ConnectCLientRcvd();
	
	void exec(Connection* c) const throw();
};

#endif

