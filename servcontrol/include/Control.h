#ifndef _Control_h
#define _Control_h

#include <list>

#include <Connection.h>
#include <Server.h>
#include <Singleton.h>
#include <mysql++.h>

#define IPLENGTH 16
#define PORTLENGTH 5
#define MAXDESV 2
#define DEF_GAME_SERVERS 3
#define MAX_GAME_SERVERS 50
#define REBALANCING_TIME 4
#define WAITING_RESPONSE_TIME 2
#define NZONES 4
#define TYPE_GAME_SERVER "game"
#define TYPE_ROUTER_SERVER "router"
#define TYPE_LOGIN_SERVER "login"


class Control : public Singleton<Control> {
	double getAverage();
	double getStDev();
	char* ipServers[MAX_GAME_SERVERS];
	char* portServers[MAX_GAME_SERVERS];
	void fillIpServerTable();
	bool compareServersLoad(Server* first, Server* second);	
	//friend class Singleton<Control>;	

public:
	int numberServers;
	char* ipRouter;
	char* portRouter;
	char* ipLogin;
	char* portLogin;
	mysqlpp::Connection* cbd;
	mysqlpp::StoreQueryResult resultBD;
	Server* zoneServer[NZONES];
	std::list<Server*> servers;
	std::mutex recievedMutex;
	unsigned int recievedConnectionMask;
	Connection* loginConnection;
	Connection* routerConnection;
	virtual ~Control();

	int getZoneDB(int idUsuari);
	void eliminarServidor(const int idServer);
	char* getIpServerById(int id);
	char* getPortServerById(int id);
	void initializeServerList();
	void initializeConnections();
	void balance();
	void zoneChange(Server* sourceServer, int changedZonePosition, Server* destinationServer);
	void writeDownServer();
	void zoneAssignment();

};

#endif
