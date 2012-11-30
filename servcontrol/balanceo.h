#ifndef _balanceo_h
#define _balanceo_h

#include <list>

#include <Connection.h>
#include <SharedStructs.h>

#define MAXDESV 2
#define NSERVERS 2
#define REBALANCING_TIME 5
#define WAITING_RESPONSE_TIME 10
#define NZONES 4
#define IP_LOGIN "192.168.0.1"
#define PORT_LOGIN "3456"
#define IP_ROUTER "192.168.0.1"
#define PORT_ROUTER "3457"
#define IP_GAME_1 "192.168.0.1"
#define IP_GAME_2 "192.168.0.1"
#define IP_GAME_3 "192.168.0.1"
#define IP_GAME_4 "192.168.0.1"
#define PORT_GAME "3458"

class Control : Singleton {
	double getAverage();
	double getStDev();
	void balanceHandle();
	void loadRequestHandle();
	void writeDownServer();

public:
	Server zoneServer[NZONES];
	std::list<Server> servers;
	std::mutex recievedMutex;
	int recievedConnectionMask;
	Connection* loginConnection;  //= new TCPConnection();
	Connection* routerConnection; // = new TCPConnection();
	volatile int breakflag; // = 1; //Variable que fa de sincronització pel timeout de l'espera entre rebalancejos
	volatile int timeout; // = 1; //Variable que fa de sincronització pel timeout de l'espera de peticion

	Control();
	void initializeServerList();
	void initializeConnections();
	void balance();
	void zoneChange(Server sourceServer, int changedZonePosition, Server destinationServer);


}

#endif
