#ifndef _LOGIN_REQUEST_H_
#define _LOGIN_REQUEST_H_

#include <Datagram.h>
#include <Instruction.h>
#include <Login.h>


class LoginRequestSend : public Datagram<LoginRequestSend> {
  int answerCode; //Código de respuesta al intento de login. 0 = login correcto, 1=Wrong username or password, los demás que os hagan falta.
  char routerIp[16]; //Ip del router al que se conectará seguida de uno o más caracteres NULL \0 para indicar el final de string. Si no es un login correcto se ignora.
  char routerPort[5];
  int clientId; //Id del cliente en nuestro sistema (mapeo directo id-username)
  int token; //Token de sesión si es un login correcto. Si no, se ignora. Login necesita mandar el mismo token con el id del cliente al router, junto a la zona en la que se encuentra inicialmente
public:
  LoginRequestSend(int answer, char* ipRouter, char* portRouter, int idClient, int idToken): Datagram<LoginRequestSend>("LoginRequestSend"), answerCode(answer), clientId(idClient), token(idToken)  {
    		int i = 0;
		for( i = 0; i < IP_LENGTH; ++i){
			routerIp[i] = ipRouter[i];
			if(ipRouter[i] == '\0' );
		}
		i = 0;
		for( i = 0; i < PORT_LENGTH; ++i){
			routerPort[i] = portRouter[i];
			if(routerPort[i] == '\0' ) return;
		}    
  };
};



class LoginRequestRcvd : public Datagram<LoginRequestRcvd> {
  char username[32]; //Nombre de usuario
  char password[32]; //Contraseña en texto plano, vamos por SSL
public:
  LoginRequestRcvd() : Datagram<LoginRequestRcvd>("LoginRequestRcvd") {}
  virtual ~LoginRequestRcvd();
  void exec(Connection*) const throw();
};

#endif