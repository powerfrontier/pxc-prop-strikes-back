//Intruccion que mandamos desde control  (en el exec de la NewUser) al router para que sepa que se ha añadido un usuario nuevo con id, token i zona (la zona la consultamos en la bd (poner una funcion que sea de mapeo en plan cutre).
#include <Datagram.h>
#include <Instruction.h>

#include <iostream>

class RouterNewUserSend : public Datagram<RouterNewUserSend> {
public:
	int idUser;
	int token;
	int zonaUser;
	
	RouterNewUserSend(int user, int tok, int zone): Datagram<RouterNewUserSend>("RouterNewUserSend"), idUser(user), token(tok), zonaUser(zone)  {};
};
