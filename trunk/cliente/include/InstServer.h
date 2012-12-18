#ifndef _INST_SERVER_H_
#define _INST_SERVER_H_

#include <Datagram.h>

//SENT

//ID 502
class SetDestinationSend : public Datagram<SetDestinationSend> {
	int32_t mUserId;
	int32_t mToken;
	double mDestination[3];
public:
	SetDestinationSend(int32_t id, int32_t token, double x, double y, double z);
	~SetDestinationSend();
};

//ID 501
class SetZoneSend : public Datagram<SetZoneSend> {
	int32_t mUserId;
	int32_t mToken;
	int32_t mZone;
public:
	SetZoneSend(int32_t user, int32_t token, int32_t zone);
	~SetZoneSend();
};

//RECEIVED

//ID 501
class SetZoneRcvd : public Datagram<SetZoneRcvd> {
	int32_t mUserId;
	int32_t mToken;
	int32_t mZone;
public:
	SetZoneRcvd();
	~SetZoneRcvd();
	
	void exec(Connection*) const throw();
};


//ID 500
class SetUserStatusRcvd : public Datagram<SetUserStatusRcvd> {
	int32_t mUserId;
	int32_t mZone;
	double mPos[3];
	double mDir[3];
	double mSpeed;
public:
	SetUserStatusRcvd();
	~SetUserStatusRcvd();
	
	void exec(Connection*) const throw();
};


#endif
