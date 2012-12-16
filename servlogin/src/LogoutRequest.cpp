#include <LogoutRequest.h>
#include <ClientDisconnect.h>
#include <iostream>
#include <map>
#include <Login.h>
#include <mutex>

void LogoutRequestRcvd::exec(Connection* c) const throw(){
  Login::instance().loginMutex.lock();
  int answerCode; //Código de respuesta de logout. 0=OK, 1=Invalid username. 2=Invalid token. El resto si os motiva hacer alguno.
  std::map<int,int>::iterator it;
  std::cout << "Usuarios connectats prelogout: " << Login::instance().usersConnected << std::endl;
  it = Login::instance().userTokenMap.find(clientId);
  if( it == Login::instance().userTokenMap.end() ){
    answerCode = 1;
  }else{
      it = Login::instance().userTokenMap.find(clientId);
      if( it->second != token ){
	answerCode = 2;
      }else{
	//Desconexión correcta
	std::string username = Login::instance().idToUserMap.find(clientId)->second;
	Login::instance().userToIdMap.erase(Login::instance().userToIdMap.find(username));
	Login::instance().idToUserMap.erase(Login::instance().idToUserMap.find(clientId));
	Login::instance().idToConnectionMap.erase(Login::instance().idToConnectionMap.find(clientId));
	Login::instance().connectionToIdMap.erase(Login::instance().connectionToIdMap.find(c));
	Login::instance().userTokenMap.erase(it);
	answerCode = 0;	
	Login::instance().usersConnected--;
	std::cout << "Salida del else de logout" << std::endl;
	//Enviar info balanceo
	//ClientDisconnectSend* clientDisconnectSend = new ClientDisconnectSend(clientId,token);
	//Login::instance().controlConnection->sendAnswer(*clientDisconnectSend);  
	//delete clientDisconnectSend;
      }    
  }
  Login::instance().loginMutex.unlock();
  
  //Enviar info cliente
  LogoutRequestSend* logoutRequestSend = new LogoutRequestSend(answerCode);
  c->sendAnswer(*logoutRequestSend);
  delete logoutRequestSend;
  std::cout << "Usuarios connectats post logout: " << Login::instance().usersConnected << std::endl;
  
}

LogoutRequestRcvd::~LogoutRequestRcvd(){
	
}

