#include <DisconnectUser.h>
#include <RouterDeleteUser.h>
#include <GameDeleteUser.h>
#include <Control.h>

#include <iostream>

void DisconnectUserRcvd::exec(Connection* c) const throw(){

	std::cout << "LLEGA UN LOGOUT DEL USUARIO: " << idUser << std::endl;
	std::cout << "ENVIANDO instruccion RouterDeleteUserSend" << std::endl;
	RouterDeleteUserSend* intrRouter = new RouterDeleteUserSend(idUser);
	Control::instance().routerConnection->sendAnswer(*intrRouter);
	delete(intrRouter);
	//no nos hace falta ya que el server de juego en donde está el usuario al hacer logout lo sabe router y no control
	//std::cout << "ENVIANDO instruccion GameDeleteUserSend" << std::endl; 
	//GameDeleteUserSend* intrGame = new GameDeleteUserSend(idUser);
	//gameServer->c->sendAnswer(*intrGame);
	//delete(intrGame);
}

DisconnectUserRcvd::~DisconnectUserRcvd(){
	
}
