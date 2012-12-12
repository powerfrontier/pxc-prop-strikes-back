#include <Datagram.h>
#include <Control.h>
#include <Instruction.h>
#include <mutex>
#include <list>

class IPServerSend : public Datagram<IPServerSend> {
  char ipSourceServer[IPLENGTH];
  char portSourceServer[PORTLENGTH]; 
public:
	IPServerSend(char ip[IPLENGTH], char port[PORTLENGTH]): Datagram<IPServerSend>("IPServerSend") {
		int i = 0;
		for( i = 0; i < IPLENGTH; ++i){
			ipSourceServer[i] = ip[i];
			if(ip[i] == '\0' ) return;
		}
		i = 0;
		for( i = 0; i < PORTLENGTH; ++i){
			portSourceServer[i] = port[i];
			if(port[i] == '\0' ) return;
		}	
	};
};

class IPServerRcvd : public Datagram<IPServerRcvd> {
	int idServer; 
public:
	IPServerRcvd(): Datagram<IPServerRcvd>("IPServerRcvd")  {};
	virtual ~IPServerRcvd();
	void exec(Connection*) const throw();
};
