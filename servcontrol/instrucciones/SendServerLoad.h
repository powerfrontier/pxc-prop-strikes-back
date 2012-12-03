#include "SharedStructs.h"
#include <Instruction.h>
#include <mutex>
#include <list>

class SendServerLoad : public Datagram<SendServerLoad> {

	Server s;
	public:
	SendServerLoad() : Datagram<SendServerLoad>("SendServerLoad") {}
	void exec(Connection*) const throw();
};

