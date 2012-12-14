#include <NewUser.h>
#include <RouterNewUser.h>
#include <GameNewUser.h>
#include <Control.h>

#include <iostream>


void NewUserRcvd::exec(Connection* c) const throw(){
	
	std::cout << "me llega una instruccion NewUser" << std::endl;
	int zone = Control::instance().getZoneDB(idUser); //aconseguim la zona on estaba l'usuari de BD
	Server* gameServer = Control::instance().zoneServer[zone]; //aconseguim el server que está ara en aquesta zona
	
	RouterNewUserSend* intrRouter = new RouterNewUserSend(idUser, token, zone);
	Control::instance().routerConnection->sendAnswer(*intrRouter);
	delete(intrRouter);
	GameNewUserSend* intrGame = new GameNewUserSend(idUser, zone);
	gameServer->c->sendAnswer(*intrGame);
	delete(intrGame);
}

NewUserRcvd::~NewUserRcvd(){
	
}

