#include <ServerLoad.h>
#include <Control.h>
#include <IPServer.h>

void IPServerRcvd::exec(Connection* c) const throw(){
	char ipSend[16];
	bool final = false;
	int i = 0;
	char* ipChar = Control::instance().getIpServerById(idServer);
	while(!final && i < 16)
	{
		ipSend[i] = ipChar[i];		
		if( ipChar[i] == '\0' ){
			final = true;
		}
		i++;
	}

	std::list<Server*>::iterator it;

	for(it=Control::instance().servers.begin();it!=Control::instance().servers.end();it++) {		
		if ((*it)->id == idServer) {
			IPServerSend* ipServerSend = new IPServerSend(ipSend);
			(*it)->c->send(*ipServerSend);		
		}		
	}	
	
}

IPServerRcvd::~IPServerRcvd(){
	
}

