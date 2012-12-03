#include "SharedStructs.h"
#include <Instruction.h>
#include <mutex>
#include <list>

class SendServerLoad : public Datagram<SendServerLoad> {
	
	int idServer;
	int idZone;
	double zoneLoad;
	int remainingZones;
	public:
	SendServerLoad() : Datagram<SendServerLoad>("SendServerLoad") {}
	virtual ~SendServerLoad();
	void exec(Connection*) const throw();
};

