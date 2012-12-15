#ifndef _TEST_CLASSES_H_
#define _TEST_CLASSES_H_

#include <Datagram.h>
#include <Connection.h>
#include <iostream>

//Orden que se envía
class TestTransferableSent100 : public Datagram<TestTransferableSent100> {
  char username[32]; //Nombre de usuario
  char password[32]; //Contraseña en texto plano, vamos por SSL
public:
	TestTransferableSent100(char* user, char* pwd);
	virtual ~TestTransferableSent100();
	
};

class TestTransferableSent102 : public Datagram<TestTransferableSent102> {
    int clientId; //Id del cliente en nuestro sistema
  int token; //Token de la sesión actual del cliente
public:
	TestTransferableSent102(int idClient,int idToken);
	virtual ~TestTransferableSent102();
	
};


//Orden que se recibe
class TestTransferableRcvd103 : public Datagram<TestTransferableRcvd103> {
 int answerCode; //Código de respuesta de logout. 0=OK, 1=Invalid username. 2=Invalid token. El resto si os motiva hacer alguno.

public:
	TestTransferableRcvd103() : Datagram<TestTransferableRcvd103>("TestTransferableRcvd103"){}
	virtual ~TestTransferableRcvd103();
	
	void exec(Connection* c) const throw();
};

//Orden que se recibe
class TestTransferableRcvd101 : public Datagram<TestTransferableRcvd101> {
  int answerCode; //Código de respuesta al intento de login. 0 = login correcto, 1=Wrong username or password, los demás que os hagan falta.
  char routerIp[16]; //Ip del router al que se conectará seguida de uno o más caracteres NULL \0 para indicar el final de string. Si no es un login correcto se ignora.
  char routerPort[5];
  int clientId; //Id del cliente en nuestro sistema (mapeo directo id-username)
  int token; //Token de sesión si es un login correcto. Si no, se ignora. Login necesita mandar el mismo token con el id del cliente al router, junto a la zona en la que se encuentra inicialmente

public:
   TestTransferableRcvd101() : Datagram<TestTransferableRcvd101>("TestTransferableRcvd101"){}
  virtual ~TestTransferableRcvd101();
	void exec(Connection* c) const throw();
};

class TestProfile : public TransferableProfile {
public:
	TestProfile();
	virtual ~TestProfile();
//	const TransferableProfile::Creators& getCreators(const std::string& protocol) const throw(TransferableVersionException&);
//	const TransferableProfile::CreatorIds& getCreatorIds(const std::string& protocol) const throw(TransferableVersionException&);
};

#endif

