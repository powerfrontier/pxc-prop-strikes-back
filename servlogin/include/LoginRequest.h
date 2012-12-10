#ifndef _LOGIN_REQUEST_H_
#define _LOGIN_REQUEST_H_

#include <Datagram.h>
#include <Instruction.h>


class LoginRequestSend : public Datagram<LoginRequestSend> {
  int answerCode; //Código de respuesta al intento de login. 0 = login correcto, 1=Wrong username or password, los demás que os hagan falta.
  char routerIp[16]; //Ip del router al que se conectará seguida de uno o más caracteres NULL \0 para indicar el final de string. Si no es un login correcto se ignora.
  int clientId; //Id del cliente en nuestro sistema (mapeo directo id-username)
  int token; //Token de sesión si es un login correcto. Si no, se ignora. Login necesita mandar el mismo token con el id del cliente al router, junto a la zona en la que se encuentra inicialmente
public:
  LoginRequestSend(): Datagram<LoginRequestSend>("LoginRequestSend")  {};
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