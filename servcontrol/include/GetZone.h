#ifndef _GetZone_h
#define _GetZone_h

class GetZoneSend : public Datagram<GetZoneSend> {
int idZone;
int idServerSource;
public:
	GetZoneSend(int zone, int serverSource): Datagram<GetZoneSend>("GetZoneSend") ,idZone(zone), idServerSource(serverSource) {};
};

#endif
