#include "SharedStructs.h"
#include <Instruction.h>
#include <mutex>

class SendServerLoad : public Datagram<SendServerLoad> {

	struct server_carga carga;
	server* s;
	int* rebut;
	std::mutex* rebut_mutex;

	public:
	SendServerLoad() : Datagram<SendServerLoad>("SendServerLoad") {}
	void exec(Connection*) const throw();
};

