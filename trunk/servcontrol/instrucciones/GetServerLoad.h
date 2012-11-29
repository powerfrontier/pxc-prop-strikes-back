#include "../conexion/include/Datagram.h"

class GetServerLoad : public Datagram<GetSeverLoad> {

	//atributos de la clase

public:
	GetServerLoad: Datagram<GetServerLoad>("GetServerLoad") {} //Poner parametro para saber que servidor le envia la petición y luego lo ponga en la respuesta el servidor de juego

};
