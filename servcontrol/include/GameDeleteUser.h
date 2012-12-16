//Instruccio que enviamos (en el exec de la NewUser) desde control con el id de usuario ¿solo? al servidor de juego que toque para informale que tiene un nuevo usuario y que vaya a bd a cargar sus datos.
#include <Datagram.h>
#include <Instruction.h>

#include <iostream>

class GameDeleteUserSend : public Datagram<GameDeleteUserSend> {
public:
	int idUser;
	
	GameDeleteUserSend(int user): Datagram<GameDeleteUserSend>("GameDeleteUserSend"), idUser(user) {};
};
