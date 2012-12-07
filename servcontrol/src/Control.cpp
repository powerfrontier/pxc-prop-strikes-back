#include <iostream>
#include <math.h>
#include <signal.h>
#include <thread>
#include <mutex>
#include <string.h>

#include <Control.h>
#include <ServerLoad.h>
#include <SetZoneToServer.h>
#include <ControlProfile.h>
#include <RemoveZone.h>
#include <GetZone.h>
#include <RouterChangeZone.h>

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
	// Ordenamos quitar la zona de un servidor
	RemoveZoneSend* removeZoneSend = new RemoveZoneSend(changedZonePosition);
	sourceServer->c->send(*removeZoneSend);  	
	GetZoneSend* getZoneSend = new GetZoneSend(changedZonePosition, sourceServer->id);
	destinationServer->c->send(*getZoneSend);  
	//RouterChangeZoneSend* routerChangeZoneSend = new RouterChangeZoneSend(changedZonePosition, sourceServer->id, destinationServer->id );
	
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
	for(int i=0; i<NZONES; i++) { // Se necesita memoria dinamica
		ipServers[i] = (char*) malloc (17);
	}
	strcpy(ipServers[0],IP_GAME_1);
	strcpy(ipServers[1],IP_GAME_2);
}

char* Control::getIpServerById(int id){
	return ipServers[id];
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
	//char* gamePortchar;
	//long double i = 0;
	int i=0;
	//string gamePortStr;
	//int gamePort;
	//cout << "inici Conexions";
	//fflush(stdout);	
	//gamePortchar = (char*) malloc(5);
	//strcpy(gamePortchar,PORT_GAME_1);
	//cout << "preatoi";
	//fflush(stdout);
	//gamePort = atoi(gamePortchar);
	//		cout << "postatoi";
	//fflush(stdout);
	
	for(it=servers.begin();it!=servers.end();it++) {
		(*it)->c = new TCPConnection();
		//cout << (*it)->c << endl;
		//cout << to_string(gamePort + i);
		/*if((*it)->c->connect((*it)->ip, PORT_GAME_1)){
			cout << (gamePort + i) << endl;
		}*/

		if(i == 0){
			if((*it)->c->connect((*it)->ip, PORT_GAME_1)){
				cout << "Servidor " << i << " conectado\n";
			}else{
				cout << "Servidor " << i << " NO conectado\n";
			}
		}else if( i == 1){
			if((*it)->c->connect((*it)->ip, PORT_GAME_2)){
				cout << "Servidor " << i << " conectado\n";
			}else{
				cout << "Servidor " << i << " NO conectado\n";
			}
			//while ((*it)->c->connect((*it)->ip, PORT_GAME_2) == false){
			//	cout << "5 ingenieros, nuff said";
			//}
		}
		++i;
	}
	//cout << "salgo del bucle" << endl;

	//loginConnection = new TCPConnection();
	//if(loginConnection->connect(IP_LOGIN, PORT_LOGIN)){
	//	cout << "Servidor login conectado\n";
				
	//}else{
	//	cout << "Servidor login NO conectado\n";
	//}
	//routerConnection = new TCPConnection();
	//if(routerConnection->connect(IP_ROUTER, PORT_ROUTER)){
	//	cout << "Servidor redireccion conectado\n";
				
	//}else{
	//	cout << "Servidor redireccion NO conectado\n";
	//}
	//cout << "3" << endl;

	//loginConnection->connect(IP_LOGIN, PORT_LOGIN);
	//routerConnection->connect(IP_ROUTER, PORT_ROUTER);
}

void Control::writeDownServer(){
	int i;
	int serverMask = 1;
	int serverNum = 0;
	int serverConnectList = ~recievedConnectionMask;
	for( i = 0; i < NSERVERS; i++){
		serverNum = serverConnectList & serverMask;
		if(!serverNum){
			cout << "El servidor: " << i << " no responde." << endl;
		}
		serverMask *= 2;
	}
}

void Control::zoneAssignment(){
	cout << "asignando zonas" << endl;
	int modZonesPerServer = NZONES % NSERVERS;
	int zoneIndex = modZonesPerServer;
	int i;
	SetZoneToServerSend* setZoneToServerSend;
	Server* server = servers.front();
	for(i = 0; i < modZonesPerServer; ++i){

		fflush(stdout);
		setZoneToServerSend = new SetZoneToServerSend(i,server->id); // Enviamos id de zona y de servidor para que este lo guarde
		
		server->c->send(*setZoneToServerSend);
		zoneServer[i] = server;		
	}
	
	list<Server*>::iterator it;
	for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
		setZoneToServerSend = new SetZoneToServerSend(zoneIndex,(*it)->id); // Enviamos id de zona y de servidor para que este lo guarde
//cout << zoneIndex << " " << (*it)->id << endl;
		(*it)->c->send(*setZoneToServerSend);
//cout << zoneIndex << " " << (*it)->id << endl;
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
cout << "final inicializar servers" << endl;
	Control::instance().initializeConnections();
	Control::instance().zoneAssignment();
cout << "final inicializar conexiones" << endl;
	list<Server*>::iterator it;
	timeout = 1;	
	Control::instance().recievedConnectionMask = 0;
	int shift = (sizeof(int) * 8) - NSERVERS;	
	Control::instance().recievedConnectionMask = ~Control::instance().recievedConnectionMask;
	Control::instance().recievedConnectionMask = Control::instance().recievedConnectionMask >> shift; // Ponemos a 1 únicamente NSERVERS
	breakflag = 1; // Ponemos a 1 para entrar en la primera vuelta del bucle
cout << "final inicializacion" << endl;	
	while(1) { 
		if(breakflag) {
			cout << "rebalanceo" << endl;	
			alarm(0);	// Apagamos el timer
			for(it=Control::instance().servers.begin();it!=Control::instance().servers.end();it++) { //Para todos los servidores...
				// Inicializamos la carga total del server
				(*it)->load.totalLoad = 0;
				ServerLoadSend* serverLoadSend = new ServerLoadSend();
			cout << "antes de enviar instruccion solicitar carga al server" << (*it)->id << endl;
				(*it)->c->send(*serverLoadSend); //Enviamos la instruccion de solicitud de carga
			cout << "despues de enviar instruccion carga al server" << (*it)->id << endl;
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
