#include "../conexion/include/Datagram.h"

class GetServerLoad : public Datagram<GetSeverLoad> {
	//atributos de la clase
	server* s;

public:
	GetServerLoad(server* gameServer): Datagram<GetServerLoad>("GetServerLoad"), s(gameServer)  {} 

};
