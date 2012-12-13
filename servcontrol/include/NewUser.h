//Instruccion que recibe control de login al conectarse un usuario: recibimos un int id i un int token

#include <Datagram.h>
#include <Server.h>
#include <Instruction.h>
#include <list>


class NewUserRcvd : public Datagram<NewUserRcvd> {
	int idUser;
	int token;
	
	public:
	NewUserRcvd() : Datagram<NewUserRcvd>("NewUserRcvd") {}
	virtual ~NewUserRcvd();
	void exec(Connection*) const throw();
};
