#include <DisconnectUser.h>
#include <RouterNewUser.h>
#include <GameNewUser.h>
#include <Control.h>

#include <iostream>

void DisconnectUserRcvd::exec(Connection* c) const throw(){
	
	std::cout << "me llega una instruccion DisconnectUserRcvd" << std::endl;
}

DisconnectUserRcvd::~DisconnectUserRcvd(){
	
}
