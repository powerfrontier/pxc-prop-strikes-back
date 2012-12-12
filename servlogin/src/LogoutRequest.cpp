#include <LogoutRequest.h>
#include <iostream>

void LogoutRequestRcvd::exec(Connection* c) const throw(){
//  int clientId; //Id del cliente en nuestro sistema
  //int token; //Token de la sesión actual del cliente
  
  int answerCode; //Código de respuesta de logout. 0=OK, 1=Invalid username. 2=Invalid token. El resto si os motiva hacer alguno.
  
  
  
}

LogoutRequestRcvd::~LogoutRequestRcvd(){
	
}

