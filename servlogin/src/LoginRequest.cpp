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
  string user;
  string pwd;
  std::map<string,int>::iterator it;
  /*Parámetros a pasar*/
  int answerCode; //Código de respuesta al intento de login. 0 = login correcto, 1=Wrong username or password, los demás que os hagan falta.
  char routerIp[16]; //Ip del router al que se conectará seguida de uno o más caracteres NULL \0 para indicar el final de string. Si no es un login correcto se ignora.
  char routerPort[5]; // Puerto del router
  int clientId,oldClientId; //Id del cliente en nuestro sistema (mapeo directo id-username)
  int token,oldToken; //Token de sesión si es un login correcto. Si no, se ignora. Login necesita mandar el mismo token con el id del cliente al router, junto a la zona en la que se encuentra inicialmente
  ///////////////////////
  
  Login::instance().loginMutex.lock();  
  user = username;
  pwd = password;
  if(Login::instance().validate(user,pwd) ){
	  answerCode = 0;
	  strcpy(routerIp, ROUTER_IP);
	  strcpy(routerPort,ROUTER_PORT);
	   clientId = Login::instance().nextFreeToken;
	  token = Login::instance().nextFreeToken;
	  Login::instance().userTokenMap.insert(pair<int,int>(clientId,token));
	  it = Login::instance().userToIdMap.find(user);
	  if(it != Login::instance().userToIdMap.end()){
	    cout << "Usuario antiguo" << endl;	    
	    Connection* oldConnection;
	    oldClientId = it->second;
	    oldToken = Login::instance().userTokenMap.find(oldClientId)->second;
	    // Borrar conexión anterior
	    Login::instance().userToIdMap.erase(Login::instance().userToIdMap.find(user));
	    Login::instance().idToUserMap.erase(Login::instance().idToUserMap.find(oldClientId));
	    oldConnection = Login::instance().idToConnectionMap.find(oldClientId)->second;
	    Login::instance().connectionToIdMap.erase(Login::instance().connectionToIdMap.find(oldConnection));
	    Login::instance().idToConnectionMap.erase(Login::instance().idToConnectionMap.find(oldClientId));	    
	    Login::instance().userTokenMap.erase(Login::instance().userTokenMap.find(oldClientId));
	    //Hacemos logout del antiguo usuario
	    // Recogemos los datos antiguos
	    Login::instance().usersConnected--;
	    delete oldConnection;
	    
	    ClientDisconnectSend* clientDisconnectSend = new ClientDisconnectSend(oldClientId,oldToken);
	    if(Login::instance().controlConnected){
		Login::instance().controlConnection->sendAnswer(*clientDisconnectSend); 	    	    
	     }
	    delete clientDisconnectSend;
	  }
	  //Insertamos en el mapa de usuario/conexión
	  Login::instance().connectionToIdMap.insert(pair<Connection*,int>(c,clientId));
	  Login::instance().idToConnectionMap.insert(pair<int,Connection*>(clientId,c));
	  Login::instance().userToIdMap.insert(pair<string,int>(user,clientId));	    
	  Login::instance().idToUserMap.insert(pair<int,string>(clientId,user));
	  Login::instance().nextFreeToken++;
	  Login::instance().usersConnected++;
	  cout << "Usuario '" << user << "' logueado correctamente." << endl;
  }else{
	  answerCode = 1;
  }  
  //Enviar info a balanceo  
  if(Login::instance().controlConnected){
    NewClientSend* newClientSend = new NewClientSend(clientId,token);
    Login::instance().controlConnection->sendAnswer(*newClientSend);  
    delete newClientSend;
  }else{
     cout << "Server control no conectado" << endl;
  }
   //Enviar info a cliente
  LoginRequestSend* loginRequestSend = new LoginRequestSend(answerCode, routerIp,routerPort, clientId,token);
  c->sendAnswer(*loginRequestSend);  
  
  delete loginRequestSend;
  Login::instance().loginMutex.unlock();
}

LoginRequestRcvd::~LoginRequestRcvd(){

	
}

