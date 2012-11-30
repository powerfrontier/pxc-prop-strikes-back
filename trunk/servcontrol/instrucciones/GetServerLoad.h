#include <Datagram.h>

class GetServerLoad : public Datagram<GetServerLoad> {
	//atributos de la clase
	server* s;
	int* rebut;
	std::mutex* rebut_mutex;

public:
	GetServerLoad(server* gameServer, int* r, std::mutex* rm): Datagram<GetServerLoad>("GetServerLoad"), s(gameServer), rebut(r), rebut_mutex(rm)  {} 

};
