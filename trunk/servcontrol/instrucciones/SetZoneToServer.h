#include <Datagram.h>

class SetZoneToServer : public Datagram<SetZoneToServer> {
	int idZone;
	int idServer;
public:
	SetZoneToServer(int zone, int server): Datagram<SetZoneToServer>("SetZoneToServer"), idZone(zone),idServer(server)  {};
};
