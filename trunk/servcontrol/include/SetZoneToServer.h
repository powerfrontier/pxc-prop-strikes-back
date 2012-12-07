#include <Datagram.h>

class SetZoneToServerSend : public Datagram<SetZoneToServerSend> {
	int idZone;
	int idServer;
public:
	SetZoneToServerSend(int zone, int server): Datagram<SetZoneToServerSend>("SetZoneToServerSend"), idZone(zone),idServer(server)  {};
};
