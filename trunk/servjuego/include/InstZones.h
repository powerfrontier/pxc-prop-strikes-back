#ifndef _INST_ZONES_H_
#define _INST_ZONES_H_

#include <Datagram.h>

//ID 7
class SetZoneOrderRcvd : public Datagram<SetZoneOrderRcvd> {
	int idZone; //id de la zona a cargar
	int idServer; //id del server receptor, esta id se guarda para que el server sepa 'quien es', pues cuando se pida la carga de las zonas, a control le hará falta saber la id de este

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

//ID 3
class SetZoneToServerRcvd : public Datagram<SetZoneToServerRcvd> {
	int mIdZone; //id de la zona a cargar
	int mIdServer; //id del server receptor, esta id se guarda para que el server sepa 'quien es', pues cuando se pida la carga de las zonas, a control le hará falta saber la id de este	

public:
	SetZoneToServerRcvd();
	~SetZoneToServerRcvd();
	
	void exec(Connection* c) throw();
};

#endif
