#include <ServerLoad.h>
#include <Control.h>
#include <IPServer.h>

void IPServerRcvd::exec(Connection* c) const throw(){
	char ipSend[IPLENGTH];
	char portSend[PORTLENGTH];
	bool final = false;
	int i = 0;
	char* ipChar = Control::instance().getIpServerById(idServer);
	while(!final && i < IPLENGTH)
	{
		ipSend[i] = ipChar[i];		
		if( ipChar[i] == '\0' ){
			final = true;
		}
		i++;
	}
	final = false;
	i = 0;
	char* portChar = Control::instance().getPortServerById(idServer);
	while(!final && i < PORTLENGTH)
	{
		portSend[i] = portChar[i];		
		if( portChar[i] == '\0' ){
			final = true;
		}
		i++;
	}

	std::list<Server*>::iterator it;

	for(it=Control::instance().servers.begin();it!=Control::instance().servers.end();it++) {		
		if ((*it)->id == idServer) {
			IPServerSend* ipServerSend = new IPServerSend(ipSend, portSend);
			(*it)->c->sendAnswer(*ipServerSend);
			delete ipServerSend;		
		}		
	}	
	
}

IPServerRcvd::~IPServerRcvd(){
	
}

