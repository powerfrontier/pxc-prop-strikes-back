#include <Datagram.h>
#include <Server.h>
#include <Instruction.h>
#include <mutex>
#include <list>

class DisconnectUserRcvd : public Datagram<DisconnectUserRcvd> {
	int idUser;
	
	public:
	DisconnectUserRcvd() : Datagram<DisconnectUserRcvd>("DisconnectUserRcvd") {}
	virtual ~DisconnectUserRcvd();
	void exec(Connection*) const throw();
};
