#include "SharedStructs.h"
#include <mutex>

class SendServerLoad :: public Instruction {

	struct server_carga carga;
	server* s;
	int* rebut;
	std::mutex* rebut_mutex;

	public:
	SendServerLoad() : Instruction("SendServerLoad") {}
	void exec();
};

