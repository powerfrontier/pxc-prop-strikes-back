#include "SendServerLoad.h"
#include <balanceo.h>



void SendServerLoad::exec(Connection* c) const throw(){
	//Inserir la carrega a la carrega del server que toca i actualitzar la mascara on toca també
	std::list<Server>::iterator it;
	for(it=Control::instance().servers.begin();it!=Control::instance().servers.end();it++) {
		if ((*it).id == s.id) {
			(*it).load = s.load;
			Control::instance().recievedMutex.lock();
			Control::instance().recievedConnectionMask = Control::instance().recievedConnectionMask || (1 << s.id);
			Control::instance().recievedMutex.unlock();
			return;	
		}		
	}
}



