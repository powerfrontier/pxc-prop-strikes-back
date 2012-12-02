#include <Datagram.h>

class GetServerLoad : public Datagram<GetServerLoad> {
	//atributos de la clase
	//Server s;
	//int* rebut;
	//std::mutex* rebut_mutex;
	int idServer;
	

public:
	//GetServerLoad(Server gameServer, int* r, std::mutex* rm): Datagram<GetServerLoad>("GetServerLoad"), s(gameServer), rebut(r), rebut_mutex(rm)  {} 
	GetServerLoad(int id): Datagram<GetServerLoad>("GetServerLoad"), idServer(id)  {};
};
