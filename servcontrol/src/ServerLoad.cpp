#include <ServerLoad.h>
#include <Control.h>


void ServerLoadRcvd::exec(Connection* c) const throw(){
	//Inserir la carrega a la carrega del server que toca i actualitzar la mascara on toca també
	std::list<Server*>::iterator it;
	int i;
	for(it=Control::instance().servers.begin();it!=Control::instance().servers.end();it++) {
		if ((*it)->id == idServer) {
			for(i = 0; i < (*it)->load.distribution.size(); ++i){
				if((*it)->load.distribution.at(i).zone == idZone){
					(*it)->load.distribution.at(i).load = zoneLoad;	
				}
			}
			if(remainingZones == 0){
				Control::instance().recievedMutex.lock();
				Control::instance().recievedConnectionMask = Control::instance().recievedConnectionMask || (1 << idServer);
				Control::instance().recievedMutex.unlock();
				return;	
			}
			
		}		
	}
}

ServerLoadRcvd::~ServerLoadRcvd(){
	
}

