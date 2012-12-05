#ifndef _RemoveZone_h
#define _RemoveZone_h

class RemoveZoneSend : public Datagram<RemoveZoneSend> {
int idZone;
public:
	RemoveZoneSend(int id): Datagram<RemoveZoneSend>("RemoveZoneSend") ,idZone(id) {};
};

#endif
