#ifndef _Control_h
#define _Control_h

#include <list>

#include <Connection.h>
#include <Server.h>
#include <Singleton.h>


#define MAXDESV 2
#define NSERVERS 2
#define REBALANCING_TIME 4
#define WAITING_RESPONSE_TIME 2
#define NZONES 4
#define IP_LOGIN "192.168.1.1"
#define PORT_LOGIN "3456"
#define IP_ROUTER "192.168.1.1"
#define PORT_ROUTER "3457"
#define IP_GAME_1 "0.0.0.0"
#define IP_GAME_2 "0.0.0.0"
#define PORT_GAME "3458"

class Control : public Singleton<Control> {
	double getAverage();
	double getStDev();
	char* ipServers[NSERVERS];
	void fillIpServerTable();	
	//friend class Singleton<Control>;	


public:

	Server* zoneServer[NZONES];
	std::list<Server*> servers;
	std::mutex recievedMutex;
	int recievedConnectionMask;
	Connection* loginConnection;  //= new TCPConnection();
	Connection* routerConnection; // = new TCPConnection();
	virtual ~Control();

	//Control();
	void initializeServerList();
	void initializeConnections();
	void balance();
	void zoneChange(Server* sourceServer, int changedZonePosition, Server* destinationServer);
	void writeDownServer();
	void zoneAssignment();

};

#endif
