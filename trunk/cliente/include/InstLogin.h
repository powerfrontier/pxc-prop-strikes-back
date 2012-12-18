#ifndef _INST_LOGIN_H_
#define _INST_LOGIN_H_

#include <Datagram.h>
#include <string>

//SENT

//ID 100
class LoginRequestSend : public Datagram<LoginRequestSend> {
  int8_t mUsername[32]; //Nombre de usuario
  int8_t mPassword[32]; //Contraseña en texto plano, vamos por SSL
public:
  LoginRequestSend(const std::string& username, const std::string& password);
  virtual ~LoginRequestSend();
};


//RECEIVED

// 101
class LoginRequestRcvd : public Datagram<LoginRequestRcvd> {
  int32_t	mAnswerCode; //Código de respuesta al intento de login. 0 = login correcto, 1=Wrong username or password, los demás que os hagan falta.
  char		mRouterIp[16]; //Ip del router al que se conectará seguida de uno o más caracteres NULL \0 para indicar el final de string. Si no es un login correcto se ignora.
  char		mRouterPort[5];
  int32_t	mClientId; //Id del cliente en nuestro sistema (mapeo directo id-username)
  int32_t	mToken; //Token de sesión si es un login correcto. Si no, se ignora. Login necesita mandar el mismo token con el id del cliente al router, junto a la zona en la que se encuentra inicialmente
public:
  LoginRequestRcvd();
  ~LoginRequestRcvd();
  
  void exec(Connection*) const throw();
};

#endif

