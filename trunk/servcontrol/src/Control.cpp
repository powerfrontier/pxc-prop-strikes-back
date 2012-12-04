#include <iostream>
#include <math.h>
#include <signal.h>
#include <thread>
#include <mutex>
#include <string.h>

#include <Control.h>
#include <ServerLoad.h>
#include <SetZoneToServerSend.h>
#include <ControlProfile.h>


using namespace std;

	volatile int breakflag; // = 1; //Variable que fa de sincronització pel timeout de l'espera entre rebalancejos
	volatile int timeout; // = 1; //Variable que fa de sincronització pel timeout de l'espera de peticion

double Control::getAverage() {
	//Gets the average load of servers.
	double incr = 0;
	list<Server*>::iterator it;
	for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
		incr += (*it)->load.totalLoad;
	}
	return incr/NSERVERS;
}

double Control::getStDev() {
	//Gets the standar desviation load of servers.
	double avg = getAverage();
	double incr = 0;
	list<Server*>::iterator it;
	for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
	    	incr += pow((*it)->load.totalLoad - avg, 2);
	}
	return sqrt(incr / (NSERVERS - 1));
}

void Control::zoneChange(Server* sourceServer, int changedZonePosition, Server* destinationServer) {
	//TODO: hacer cambioZona
}

void Control::balance() {
	Server* maxLoadServer = servers.back();
	Server* minLoadServer = servers.front();
	double standardDev = getStDev();
	int numIterations = 0;
	double minLoadZone = 1.0;//minLoadZone = 1.0;
	int changedZonePosition; //posicionZonaACambiar;
	double minDev = 1.0; //TODO S'ha d'ajustar bé el valor

	while ( standardDev > minDev && numIterations < servers.size() && maxLoadServer->load.distribution.size() != 1)
	{
		// Escogemos la zona menos cargada de maxLoadServer
		for(int i = 0; i < maxLoadServer->load.distribution.size();++i)
		{
			if(maxLoadServer->load.distribution.at(i).load < minLoadZone){
				minLoadZone = maxLoadServer->load.distribution.at(i).load;
				minLoadZone = i; // Guardamos la zona donde esta la carga minima
			}
		}
		// Faltaria mirar quin es el servidor amb les zones mes properes
		changedZonePosition = minLoadZone; // de moment faig aixo perque funcioni
		zoneChange(maxLoadServer, changedZonePosition, minLoadServer);
		// Actualizamos tabla zona/server
		zoneServer[changedZonePosition] = minLoadServer;
		servers.sort();
		standardDev = getStDev();
		numIterations++;
		// Preparación variables para la siguiente iteracion
		minLoadZone = 1;
		maxLoadServer = servers.back();
		minLoadServer = servers.front();
	}
}

void Control::fillIpServerTable(){
	strcpy(Control::instance().ipServers[0],IP_GAME_1);
	strcpy(Control::instance().ipServers[1],IP_GAME_2);	
}

void Control::initializeServerList() {
	
	//rellenar la lista de servidores con servidores con ip definida en el .h como constante y id secuencial 
	Control::fillIpServerTable();	
	int i;
	for(i = 0; i < NSERVERS; ++i){
		servers.push_back(new Server(i,ipServers[i]));
	}	
}

void Control::initializeConnections() {
	list<Server*>::iterator it;
	for(it=servers.begin();it!=servers.end();it++) {
		(*it)->c = new TCPConnection();
		(*it)->c->connect((*it)->ip, PORT_GAME);
	}
	loginConnection->connect(IP_LOGIN, PORT_LOGIN);
	routerConnection->connect(IP_ROUTER, PORT_ROUTER);
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

void Control::zoneAssignment(){
	int modZonesPerServer = NZONES % NSERVERS;
	int zoneIndex = modZonesPerServer;
	int i;
	SetZoneToServerSend* setZoneToServerSend;
	Server* server = servers.front();
	for(i = 0; i < modZonesPerServer; ++i){
		setZoneToServerSend = new SetZoneToServerSend(i,server->id); // Enviamos id de zona y de servidor para que este lo guarde
		server->c->send(*setZoneToServerSend);
		zoneServer[i] = server;		
	}
	
	list<Server*>::iterator it;
	for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
		setZoneToServerSend = new SetZoneToServerSend(zoneIndex,(*it)->id); // Enviamos id de zona y de servidor para que este lo guarde
		(*it)->c->send(*setZoneToServerSend);
		zoneServer[zoneIndex] = (*it);
		zoneIndex++;
		if ( zoneIndex == NZONES ){
			return;
		}		
	}
}

Control::~Control(){
	list<Server*>::iterator it;
	for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
		delete *it;		
		it = servers.erase(it);
	}
}


void balanceHandle(int signum) {
    breakflag = 1;
}


void loadRequestHandle(int signum){
  timeout = 0;
}

int main() {
	//ControlProfile
	TransferableFactory::instance().setProfile(new ControlProfile());
	TransferableFactory::instance().setProtocol("0.1a");
	
	//Inicialización
	Control::instance().initializeServerList();

	Control::instance().initializeConnections();

	list<Server*>::iterator it;
	timeout = 1;	
	Control::instance().recievedConnectionMask = 0;
	int shift = (sizeof(int) * 8) - NSERVERS;	
	Control::instance().recievedConnectionMask = ~Control::instance().recievedConnectionMask;
	Control::instance().recievedConnectionMask = Control::instance().recievedConnectionMask >> shift; // Ponemos a 1 únicamente NSERVERS
	breakflag = 1; // Ponemos a 1 para entrar en la primera vuelta del bucle
	while(1) {
		if(breakflag) {
			alarm(0);	// Apagamos el timer
			for(it=Control::instance().servers.begin();it!=Control::instance().servers.end();it++) { //Para todos los servidores...
				// Inicializamos la carga total del server
				(*it)->load.totalLoad = 0;
				ServerLoadSend* serverLoadSend = new ServerLoadSend();
				(*it)->c->send(*serverLoadSend); //Enviamos la instruccion de solicitud de carga			 
			}
			signal(SIGALRM, loadRequestHandle);
			alarm(WAITING_RESPONSE_TIME);
			while(Control::instance().recievedConnectionMask && timeout) { 
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
			breakflag = 0; 
			timeout = 1;  
			signal(SIGALRM, balanceHandle);
			alarm(REBALANCING_TIME);
		}
		//para una segunda version implementar un listener que escuche peticiones de emergencia
	}

	return 0;
}
