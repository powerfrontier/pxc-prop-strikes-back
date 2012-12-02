#include <iostream>
#include <math.h>
#include <signal.h>
#include <thread>
#include <mutex>

#include "balanceo.h"
#include <GetServerLoad.h>

using namespace std;


double Control::getAverage() {
	//Gets the average load of servers.
	double incr = 0;
	list<Server>::iterator it;
	for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
		incr += (*it).load.totalLoad;
	}
	return incr/NSERVERS;
}

double Control::getStDev() {
	//Gets the standar desviation load of servers.
	double avg = getAverage();
	double incr = 0;
	list<Server>::iterator it;
	for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
	    	incr += pow((*it).load.totalLoad - avg, 2);
	}
	return sqrt(incr / (NSERVERS - 1));
}

void Control::zoneChange(Server sourceServer, int changedZonePosition, Server destinationServer) {
	//TODO: hacer cambioZona
}

void Control::balance() {
	Server maxLoadServer = servers.back();
	Server minLoadServer = servers.front();
	double standardDev = getStDev();
	int numIterations = 0;
	double minLoadZone = 1.0;//minLoadZone = 1.0;
	int changedZonePosition; //posicionZonaACambiar;
	double minDev = 1.0; //TODO S'ha d'ajustar bé el valor

	while ( standardDev > minDev && numIterations < servers.size() && maxLoadServer.load.distribution.size() != 1)
	{
		// Escogemos la zona menos cargada de maxLoadServer
		for(int i = 0; i < maxLoadServer.load.distribution.size();++i)
		{
			if(maxLoadServer->load.distribution.at(i).load < minLoadZone){
				minLoadZone = maxLoadServer.load.distribution.at(i).carga;
				minLoadZone = i; // Guardamos la zona donde esta la carga minima
			}
		}
		// Faltaria mirar quin es el servidor amb les zones mes properes
		changedZonePosition = minLoadZone; // de moment faig aixo perque funcioni
		zoneChange(maxLoadServer, changedZonePosition, minLoadServer);
		servers.sort();
		standardDev = getStDev();
		numIterations++;
		// Preparación variables para la siguiente iteracion
		minLoadZone = 1;
		maxLoadServer = servers.back();
		minLoadServer = servers.front();
	}
}

void Control::initializeServerList() {
	//TODO: hacer inicializarListaServidores
	//rellenar la lista de servidores con servidores con ip definida en el .h como constante y id secuencial y un reparto de las zonas arbitrario
}

void Control::initializeConnections() {
	list<Server>::iterator it;
	for(it=servers.begin();it!=servers.end();it++) {
		(*it).c = new TCPConnection();
		(*it).c->connect((*it).ip, PORT_GAME);
	}
	loginConnection->connect(IP_LOGIN, PORT_LOGIN);
	routerConnection->connect(IP_REDIRECTION, PORT_ROUTER);
}


void Control::balanceHandle() {
    breakflag = 1;
}


void Control::loadRequestHandle(){
  timeout = 0;
}

void Control::writeDownServer(){
	int i;
	int serverMask = 1;
	int serverNum = 0;
	int serverConnectList = ~recievedConnectionMask;
	for( i = 0; i < NSERVERS; i++){
		serverNum = serverConnectList & serverMask;
		if(!serverNum){
			cout << "El servidor: " << i << " no responde.";
		}
		serverMask *= 2;
	}
}




int main() {
	Control::instance().initializeConnections();
	//std::thread t(consola);
	//signal(SIGALRM, handle);
	list<Server>::iterator it;
	Control::instance().timeout = 1;	
	Control::instance().recievedConnectionMask = 0;
	int shift = (sizeof(int) * 8) - NSERVERS;	
	Control::instance().recievedConnectionMask = ~Control::instance().recievedConnectionMask;
	Control::instance().recievedConnectionMask = Control::instance().recievedConnectionMask >> shift; // Ponemos a 1 únicamente NSERVERS
	Control::instance().breakflag = 1; // Ponemos a 1 para entrar en la primera vuelta del bucle
	while(1) {
		if(Control::instance().breakflag) {
			alarm(0);	// Apagamos el timer
			for(it=Control::instance().servers.begin();it!=Control::instance().servers.end();it++) { //Para todos los servidores...
				GetServerLoad* getServerLoad = new GetServerLoad(*it, &Control::instance().recievedConnectionMask, &Control::instance().recievedMutex);
				(*it).c->send(*getServerLoad); //Enviamos la instruccion de solicitud de carga			 
			}
			signal(SIGALRM, Control::instance().loadRequestHandle);
			alarm(WAITING_RESPONSE_TIME);
			while(Control::instance().recievedConnectionMask && Control::instance().timeout) { 
				//Esperamos sin hacer nada o...								
				//...mirar si se puede poner el proceso a dormir y que se despierte con un signal también
			}
			alarm(0);
			if(Control::instance().recievedConnectionMask) { //s'ha sortit del bucle pel timeout
				Control::instance().writeDownServer();
			}
			Control::instance().servers.sort(); //ordenamos la lista
			Control::instance().balance(); //ejecutar algoritmo balanceo y envia las instrucciones de balanceo a los servidores de juego
			//poner la alarma para el proximo rebalanceo
			Control::instance().breakflag = 0; 
			Control::instance().timeout = 1;  
			signal(SIGALRM, Control::instance().balanceHandle);
			alarm(REBALANCING_TIME);
		}
		//para una segunda version implementar un listener que escuche peticiones de emergencia
	}

	return 0;
}
