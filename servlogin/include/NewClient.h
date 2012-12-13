#ifndef _NEW_CLIENT_H_
#define _NEW_CLIENT_H_

#include <Datagram.h>
#include <Instruction.h>
#include <Login.h>


class NewClientSend : public Datagram<NewClientSend> {
  int clientId; //Id del cliente en nuestro sistema (mapeo directo id-username)
  int token; //Token de sesión si es un login correcto. Si no, se ignora. Login necesita mandar el mismo token con el id del cliente al router, junto a la zona en la que se encuentra inicialmente
public:
  NewClientSend(int idClient, int idToken): Datagram<NewClientSend>("NewClientSend"), clientId(idClient), token(idToken)  {

  };
};



#endif