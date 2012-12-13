#include <NewUser.h>
#include <RouterNewUser.h>
#include <GameNewUser.h>
#include <Control.h>

#include <iostream>

void NewUserRcvd::exec(Connection* c) const throw(){
	
	std::cout << "me llega una instruccion NewUser" << std::endl;
}

NewUserRcvd::~NewUserRcvd(){
	
}

