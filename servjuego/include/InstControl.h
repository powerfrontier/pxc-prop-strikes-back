#ifndef _INST_CONTROL_H_
#define _INST_CONTROL_H_

#include <Datagram.h>

//SENT

//ID 0
class ServerLoadAnswerSend : public Datagram<ServerLoadAnswerSend> {
	int mIdServer;
	int mIdZone;
	double mZoneLoad;
	int mRemainingZones;
	
public:
	ServerLoadAnswerSend(int idServer, int idZone, double zoneLoad, int remainingZones);
	~ServerLoadAnswerSend();
};

//ID 1
class GetServerIpSend : public Datagram<GetServerIpSend> {
	int mServerId;
public:
	GetServerIpSend(int serverId);
	~GetServerIpSend();
};


//RECEIVED

//ID 2
class ServerLoadOrderRcvd : public Datagram<ServerLoadOrderRcvd> {
public:
	ServerLoadOrderRcvd();
	~ServerLoadOrderRcvd();
	
	void exec(Connection* c) const throw();
};

//ID 3
class SetZoneOrderRcvd : public Datagram<SetZoneOrderRcvd> {
	int idZone; //id de la zona a cargar
	int idServerDest; //id del server receptor, esta id se guarda para que el server sepa 'quien es', pues cuando se pida la carga de las zonas, a control le hará falta saber la id de este

public:
	SetZoneOrderRcvd();
	~SetZoneOrderRcvd();
	
	void exec(Connection* c) throw();
};

//ID 4
class GetZoneOrderRcvd : public Datagram<GetZoneOrderRcvd> {
	int idZone; //id de la zona a cargar
	int idServerSource; //id del servidor origen que tiene la zona
	
public:
	GetZoneOrderRcvd();
	~GetZoneOrderRcvd();
	
	void exec(Connection* c) throw();
};

//ID 6
class SetServerIpRvcd : public Datagram<SetServerIpRvcd> {
	char mIPServer[16]; //Char[16] con la IP del server
	char mPortServer[5]; //Char[5] con el puerto del server
	
public:
	SetServerIpRvcd();
	~SetServerIpRvcd();
	
	void exec(Connection* c) const throw();
};

//ID 7
class SetZoneToServerRcvd : public Datagram<SetZoneToServerRcvd> {
	int mIdZone; //id de la zona a cargar
	int mIdServer; //id del server receptor, esta id se guarda para que el server sepa 'quien es', pues cuando se pida la carga de las zonas, a control le hará falta saber la id de este	

public:
	SetZoneToServerRcvd();
	~SetZoneToServerRcvd();
	
	void exec(Connection* c) const throw();
};

#endif
