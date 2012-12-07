#ifndef _INST_ZONES_H_
#define _INST_ZONES_H_

class SetZoneOrderRcvd : public Datagram<SetZoneOrderRcvd> {
	- idZone: id de la zona a cargar
	- idServer: id del server receptor, esta id se guarda para que el server sepa 'quien es', pues cuando se pida la carga de las zonas, a control le hará falta saber la id de este
};

class GetZoneOrderRcvd : public Datagram<GetZoneOrderRcvd> {
	- idZone: id de la zona a cargar
	- idServerSource: id del servidor origen que tiene la zona	
};

class 

#endif
