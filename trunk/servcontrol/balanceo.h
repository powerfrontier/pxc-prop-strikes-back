#ifndef _balanceo_h
#define _balanceo_h

#include <list>

#include <Connection.h>
#include <SharedStructs.h>

#define MAXDESV 2
#define NSERVERS 2
#define REBALANCING_TIME 5
#define WAITING_RESPONSE_TIME 10
#define ZONES 4
#define IPLOGIN "192.168.0.1"
#define PUERTOLOGIN "3456"
#define IPREDIRECCION "192.168.0.1"
#define PUERTOREDIRECCION "3457"
#define IPJUEGO1 "192.168.0.1"
#define IPJUEGO2 "192.168.0.1"
#define IPJUEGO3 "192.168.0.1"
#define IPJUEGO4 "192.168.0.1"
#define PUERTOJUEGO "3458"

class Control : Singleton {
	double getAverage();
	double getStDev();
	void balanceHandle();
	void loadRequestHandle();
	void writeDownServer();

public:
	Server zoneServer[ZONES];
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
