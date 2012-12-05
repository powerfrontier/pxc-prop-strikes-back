#ifndef _RouterChangeZone_h
#define _RouterChangeZone_h

class RouterChangeZoneSend : public Datagram<RouterChangeZoneSend> {
int idZone;
int idSourceServer;
int idDestServer;
public:
	RouterChangeZoneSend(int zone, int sourceServer, int destServer): Datagram<RouterChangeZoneSend>("RouterChangeZoneSend") ,idZone(zone), idSourceServer(sourceServer), idDestServer(destServer) {};
};

#endif
