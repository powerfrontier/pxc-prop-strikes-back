#include <NewUser.h>
#include <RouterNewUser.h>
#include <GameNewUser.h>
#include <Control.h>

#include <iostream>


void NewUserRcvd::exec(Connection* c) const throw(){
	
	std::cout << "ME LLEGA una instruccion NewUser" << std::endl;
	int zone = Control::instance().getZoneDB(idUser); //aconseguim la zona on estaba l'usuari de BD
	std::cout << "zona guardada!!! " << zone << std::endl;
	Server* gameServer = Control::instance().zoneServer[zone]; //aconseguim el server que está ara en aquesta zona
	std::cout << "que pertenece a: " << gameServer->id << std::endl;
	//std::cout << "ENVIANDO instruccion RouterNewUserSend" << std::endl;
	//RouterNewUserSend* intrRouter = new RouterNewUserSend(idUser, token, zone);
	//Control::instance().routerConnection->sendAnswer(*intrRouter);
	//delete(intrRouter);
	std::cout << "ENVIANDO instruccion GameNewUserSend" << std::endl;
	GameNewUserSend* intrGame = new GameNewUserSend(idUser, zone);
	gameServer->c->sendAnswer(*intrGame);
	delete(intrGame);
}

NewUserRcvd::~NewUserRcvd(){
	
}

