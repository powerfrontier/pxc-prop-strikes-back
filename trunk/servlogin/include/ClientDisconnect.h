#ifndef _CLIENT_DISCONNECT_H_
#define _CLIENT_DISCONNECT_H_

#include <Datagram.h>
#include <Instruction.h>
#include <Login.h>


class ClientDisconnectSend : public Datagram<ClientDisconnectSend> {
  int clientId; //Id del cliente en nuestro sistema (mapeo directo id-username)
  int token; //Token de sesión si es un login correcto. Si no, se ignora. Login necesita mandar el mismo token con el id del cliente al router, junto a la zona en la que se encuentra inicialmente
public:
  ClientDisconnectSend(int idClient, int idToken): Datagram<ClientDisconnectSend>("ClientDisconnectSend"), clientId(idClient), token(idToken)  {

  };
};



#endif