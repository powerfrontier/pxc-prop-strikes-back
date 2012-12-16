#ifndef _INST_SERVER_H_
#define _INST_SERVER_H_

#include <Datagram.h>

//SENT

class AddRouterSend : public Datagram<AddRouterSend> {
	int mRouterId;
public:
	AddRouterSend();
	~AddRouterSend();
};


//RECEIVED

#endif

