#ifndef _LOGOUT_REQUEST_H_
#define _LOGOUT_REQUEST_H_

#include <Datagram.h>
#include <Instruction.h>


class LogoutRequestSend : public Datagram<LogoutRequestSend> {
  int answerCode; //Código de respuesta de logout. 0=OK, 1=Invalid username. 2=Invalid token. El resto si os motiva hacer alguno.
public:
  LogoutRequestSend(): Datagram<LogoutRequestSend>("LogoutRequestSend")  {};
};

class LogoutRequestRcvd : public Datagram<LogoutRequestRcvd> {
  int clientId; //Id del cliente en nuestro sistema
  int token; //Token de la sesión actual del cliente
public:
  LogoutRequestRcvd() : Datagram<LogoutRequestRcvd>("LogoutRequestRcvd") {}
  virtual ~LogoutRequestRcvd();
  void exec(Connection*) const throw();
};

#endif