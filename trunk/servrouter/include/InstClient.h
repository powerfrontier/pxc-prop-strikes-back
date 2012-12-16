#ifndef _INST_CLIENT_H_
#define _INST_CLIENT_H_

#include <Datagram.h>

class ClientConnectsRcvd : public Datagram<ClientConnectsRcvd> {
	int mIdClient;
	int mToken;
public:
	ClientConnectsRcvd();
	~ClientConnectsRcvd();
	
	void exec(Connection* c) const throw();
};

#endif

