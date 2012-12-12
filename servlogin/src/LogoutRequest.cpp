#include <LogoutRequest.h>
#include <iostream>
#include <map>
#include <Login.h>
#include <mutex>

void LogoutRequestRcvd::exec(Connection* c) const throw(){
  Login::instance().loginMutex.lock();
  int answerCode; //Código de respuesta de logout. 0=OK, 1=Invalid username. 2=Invalid token. El resto si os motiva hacer alguno.
  std::map<int,int>::iterator it;
  if( Login::instance().userTokenMap.count(clientId) == 0 ){
    answerCode = 1;
  }else{
      it = Login::instance().userTokenMap.find(clientId);
      if( it->second != token ){
	answerCode = 2;
      }else{
	//Desconexión correcta
	Login::instance().userTokenMap.erase(it);
	answerCode = 0;	
	Login::instance().usersConnected--;
      }    
  }
  Login::instance().loginMutex.unlock();
  //Enviar info balanceo
  
  //Enviar info cliente
  LogoutRequestSend* logoutRequestSend = new LogoutRequestSend(answerCode);
  c->sendAnswer(*logoutRequestSend);
  delete logoutRequestSend;
  
  
}

LogoutRequestRcvd::~LogoutRequestRcvd(){
	
}

