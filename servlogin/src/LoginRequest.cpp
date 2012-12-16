#include <LoginRequest.h>
#include <ClientDisconnect.h>
#include <ControlConnection.h>
#include <NewClient.h>
#include <Login.h>
#include <iostream>
#include <string.h>
#include <mutex>
#include <map>

using namespace std;

void LoginRequestRcvd::exec(Connection* c) const throw(){
  const char* userPointer;
  const char* pwdPointer;
  std::map<Connection*,int>::iterator it;
  /*Parámetros a pasar*/
  int answerCode; //Código de respuesta al intento de login. 0 = login correcto, 1=Wrong username or password, los demás que os hagan falta.
  char routerIp[16]; //Ip del router al que se conectará seguida de uno o más caracteres NULL \0 para indicar el final de string. Si no es un login correcto se ignora.
  char routerPort[5]; // Puerto del router
  int clientId,oldClientId; //Id del cliente en nuestro sistema (mapeo directo id-username)
  int token,oldToken; //Token de sesión si es un login correcto. Si no, se ignora. Login necesita mandar el mismo token con el id del cliente al router, junto a la zona en la que se encuentra inicialmente
  ///////////////////////
  Login::instance().loginMutex.lock();
  
  userPointer = username;
  pwdPointer = password;
  if(Login::instance().validate(userPointer,pwdPointer) ){
	  answerCode = 0;
	  strcpy(routerIp, ROUTER_IP);
	  strcpy(routerPort,ROUTER_PORT);
	   clientId = Login::instance().nextFreeToken;
	  token = Login::instance().nextFreeToken;
	  Login::instance().userTokenMap.insert(pair<int,int>(clientId,token));
	  it = Login::instance().userConnectionMap.find(c);
	  if(it == Login::instance().userConnectionMap.end()){
	    //Insertamos en el mapa de usuario/conexión
	    Login::instance().userConnectionMap.insert(pair<Connection*,int>(c,clientId));	    
	  }else{
	    //Hacemos logout del antiguo usuario
	    // Recogemos los datos antiguos
	    Login::instance().usersConnected--;
	    oldClientId = it->second;
	    oldToken = Login::instance().userTokenMap.find(oldClientId)->second;
	    ClientDisconnectSend* clientDisconnectSend = new ClientDisconnectSend(oldClientId,oldToken);
	    Login::instance().controlConnection->sendAnswer(*clientDisconnectSend); 	    	    
	    delete clientDisconnectSend;
	  }
	  Login::instance().nextFreeToken++;
	  Login::instance().usersConnected++;
  }else{
	  answerCode = 1;
  }  
  //Enviar info a balanceo  
  NewClientSend* newClientSend = new NewClientSend(clientId,token);
  Login::instance().controlConnection->sendAnswer(*newClientSend);  
   //Enviar info a cliente
  LoginRequestSend* loginRequestSend = new LoginRequestSend(answerCode, routerIp,routerPort, clientId,token);
  cout << "port de client: " << c->getPort() << endl;
  c->sendAnswer(*loginRequestSend);  
  delete newClientSend;
  delete loginRequestSend;
  Login::instance().loginMutex.unlock();
  cout << "Salimos de exec de loginrequest" << endl;
}

LoginRequestRcvd::~LoginRequestRcvd(){

	
}

