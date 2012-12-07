#include <Datagram.h>
#include <Server.h>
#include <Instruction.h>
#include <mutex>
#include <list>

class IPServerSend : public Datagram<IPServerSend> {
  //char ipSourceServer[16];
  
public:
	IPServerSend(): Datagram<IPServerSend>("IPServerSend")   {};
};

class IPServerRcvd : public Datagram<IPServerRcvd> {
	int idServer; 
public:
	IPServerRcvd(): Datagram<IPServerRcvd>("IPServerRcvd")  {};
	virtual ~IPServerRcvd();
	void exec(Connection*) const throw();
};
