#include <iostream>
#include <math.h>
#include <signal.h>
#include <thread>
#include <mutex>
#include <string.h>
#include <unistd.h>

#include <mysql++.h>

#include <Control.h>
#include <ServerLoad.h>
#include <SetZoneToServer.h>
#include <ControlProfile.h>
#include <RemoveZone.h>
#include <GetZone.h>
#include <RouterChangeZone.h>
#include <LoginOnline.h>

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
	if(NSERVERS < 2) {
		return sqrt(incr / NSERVERS);
	}
	else {
		return sqrt(incr / (NSERVERS - 1));
	}
}

void Control::zoneChange(Server* sourceServer, int changedZonePosition, Server* destinationServer) {
	// Ordenamos quitar la zona de un servidor
	cout << "zone change: " << "origen: " << sourceServer->id << " zona: " << changedZonePosition << " desti: " << destinationServer->id << endl;
	RemoveZoneSend* removeZoneSend = new RemoveZoneSend(changedZonePosition);
	sourceServer->c->send(*removeZoneSend);
	delete(removeZoneSend);  	
	GetZoneSend* getZoneSend = new GetZoneSend(changedZonePosition, sourceServer->id);
	destinationServer->c->send(*getZoneSend);
	delete(getZoneSend);   
	//RouterChangeZoneSend* routerChangeZoneSend = new RouterChangeZoneSend(changedZonePosition, sourceServer->id, destinationServer->id );
	
}

void Control::balance() {
	Server* maxLoadServer = servers.back();
	Server* minLoadServer = servers.front();
	double standardDev = getStDev();
	int numIterations = 0;
	double minLoadZone = 1.0;//minLoadZone = 1.0;
	int posMinLoadZone = -1;
	int changedZonePosition; //posicionZonaACambiar;
	double minDev = 1.0; //TODO S'ha d'ajustar bé el valor
	
	standardDev = 2;//solamente de debug
	/*cout << "Algoritmo balanceo:" << endl;
	cout << "stdDev: " << standardDev << endl;
	cout << "iteracion:" << numIterations << endl;
	cout << "server size: " << servers.size() << endl;
	cout << "maxloadserver id: " << maxLoadServer->id << endl;
	cout << "size:" << maxLoadServer->load.distribution.size() << endl;*/
	while (standardDev > minDev && numIterations < servers.size() && maxLoadServer->load.distribution.size() != 1 && maxLoadServer != minLoadServer)
	{	cout << "entro en while" << endl;
		// Escogemos la zona menos cargada de maxLoadServer
		for(int i = 0; i < maxLoadServer->load.distribution.size();++i)
		{
			if(maxLoadServer->load.distribution.at(i)->load < minLoadZone){
				minLoadZone = maxLoadServer->load.distribution.at(i)->load;
				posMinLoadZone = i; // Guardamos la zona donde esta la carga minima
			}
		}
		// Faltaria mirar quin es el servidor amb les zones mes properes
		changedZonePosition = posMinLoadZone; // de moment faig aixo perque funcioni
		//if(posMinLoadZone != -1) // s'ha selecciona almenys alguna zona i la carrega es > 0
		//{
			zoneChange(maxLoadServer, changedZonePosition, minLoadServer);
		//}
		// Actualizamos tabla zona/server
		zoneServer[changedZonePosition] = minLoadServer;
		servers.sort();
		standardDev = getStDev();
		numIterations++;
		// Preparación variables para la siguiente iteracion
		minLoadZone = 1;
		posMinLoadZone = -1;
		maxLoadServer = servers.back();
		minLoadServer = servers.front();
	}
	cout << "Salidendo Algoritmo balanceo:" << endl;
}

void Control::fillIpServerTable(){
	for(int i=0; i<NZONES; i++) { // Se necesita memoria dinamica
		ipServers[i] = (char*) malloc (17);
	}
	strcpy(ipServers[0],IP_GAME_1);
	strcpy(ipServers[1],IP_GAME_2);
	strcpy(ipServers[2],IP_GAME_3);
}

void Control::fillPortServerTable(){
	for(int i=0; i<NZONES; i++) { // Se necesita memoria dinamica
		portServers[i] = (char*) malloc (PORTLENGTH);
	}
	strcpy(portServers[0],PORT_GAME_1);
	strcpy(portServers[1],PORT_GAME_2);
	strcpy(portServers[2],PORT_GAME_3);
}

char* Control::getIpServerById(int id){
	return ipServers[id];
}

char* Control::getPortServerById(int id){
	return portServers[id];
}

void Control::initializeServerList() {
	list<Server*>::iterator it;
	//rellenar la lista de servidores con servidores con ip definida en el .h como constante y id secuencial 
	Control::fillIpServerTable();
	Control::fillPortServerTable();	
	int i;
	for(i = 0; i < NSERVERS; ++i){
	//	cout << "!!!!3" << endl;
		servers.push_back(new Server(i,ipServers[i]));
	}
	//for(it=servers.begin();it!=servers.end();it++)
	//	cout << (*it)->id << endl;	
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
//cout << "!!!!" << (*it)->id << endl;
		(*it)->c = new TCPConnection();
//cout << "!!!!2" << endl;
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
		}else if( i == 2){
			if((*it)->c->connect((*it)->ip, PORT_GAME_3)){
				cout << "Servidor " << i << " conectado\n";
			}else{
				cout << "Servidor " << i << " NO conectado\n";
			}
		}
		++i;
	}
	//cout << "salgo del bucle" << endl;

	loginConnection = new TCPConnection();
	if(loginConnection->connect(IP_LOGIN, PORT_LOGIN)){
		cout << "Servidor login conectado\n";
				
	}else{
		cout << "Servidor login NO conectado\n";
	}
	//routerConnection = new TCPConnection();
	/*if(routerConnection->connect(IP_ROUTER, PORT_ROUTER)){
		cout << "Servidor redireccion conectado\n";
				
	}else{
		cout << "Servidor redireccion NO conectado\n";
	}*/
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
	int zonesRestants = NZONES;
	int modZonesPerServer = NZONES % NSERVERS;
	int zoneIndex = modZonesPerServer;
	int i;
	ZoneLoad* zl;
	SetZoneToServerSend* setZoneToServerSend;
	Server* server = servers.front();
	for(i = 0; i < modZonesPerServer; ++i){

		//fflush(stdout);
		setZoneToServerSend = new SetZoneToServerSend(i,server->id); // Enviamos id de zona y de servidor para que este lo guarde
		
		server->c->send(*setZoneToServerSend);
		delete(setZoneToServerSend);
		zoneServer[i] = server;
		zl = new ZoneLoad(i, 0);
		server->load.distribution.push_back(zl);
		server->load.totalLoad = 0;
		zonesRestants--;	
	}
	
	list<Server*>::iterator it;
	while(zonesRestants > 0) {
		for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
			setZoneToServerSend = new SetZoneToServerSend(zoneIndex,(*it)->id); // Enviamos id de zona y de servidor para que este lo guarde
			//cout << zoneIndex << " " << (*it)->id << endl;
			(*it)->c->send(*setZoneToServerSend);
			//cout << zoneIndex << " " << (*it)->id << endl;
			zoneServer[zoneIndex] = (*it);

			// inicializar atributos de carga i zona del servidor
			zl = new ZoneLoad(zoneIndex, 0);
			(*it)->load.distribution.push_back(zl);
			(*it)->load.totalLoad = 0;
		
			zoneIndex++;
			if ( zoneIndex == NZONES ){
				return;
			}
			zonesRestants--;		
		}
	}
		
}

void Control::eliminarServidor(const int idServer) { //Elimina el servidor de la lista de servidores
	list<Server*>::iterator it;
	for(it=servers.begin();it!=servers.end();it++) {
		if((*it)->id == idServer) {
			servers.erase(it);
			return;
		}
	}
	return;
}

int Control::getZoneDB(int idUsuari) {

	//cout << 	
	return (rand() % 3 + 1); //valor entre 0-4
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

bool compareServersLoad(Server* first, Server* second) {
	//std::cout << first->load.totalLoad << " " << second->load.totalLoad << std::endl;
	return (first->load.totalLoad < second->load.totalLoad);
}


int main() {
	//Conexio a BD
	try
	{
		mysqlpp::Connection* conn = new mysqlpp::Connection("BDpxc03", "mysqlfib.fib.upc.edu", "pxc03", "nJoW03Hi", 3306);
		cout << "Conexió a BD realitzada? " << conn->connected() << endl;
	}
	catch (mysqlpp::ConnectionFailed& e)
	{
		cerr << "ERROR a conexio a BD! Exception: " << e.what() << endl;
	}	
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
	unsigned int shift = (sizeof(int) * 8) - NSERVERS;	
	Control::instance().recievedConnectionMask = ~Control::instance().recievedConnectionMask;
	
	Control::instance().recievedConnectionMask = Control::instance().recievedConnectionMask >> shift; // Ponemos a 1 únicamente NSERVERS

	cout << "enviando conexion a login" << endl;
	LoginOnlineSend* instr = new LoginOnlineSend();
	Control::instance().loginConnection->send(*instr);
	delete(instr);
	
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
				//if((*it)->c->isLinkOnline()) {				
					(*it)->c->send(*serverLoadSend); //Enviamos la instruccion de solicitud de carga
					delete(serverLoadSend);
				//}
				//else {
					//cout << "ERROR: servidor: " << (*it)->id << "CAIDO" << endl;
					//Control::instance().eliminarServidor((*it)->id);
				//}
			cout << "despues de enviar instruccion carga al server" << (*it)->id << endl;
			}
			signal(SIGALRM, loadRequestHandle);
			alarm(WAITING_RESPONSE_TIME);
			while(Control::instance().recievedConnectionMask && timeout) { 
				//Esperamos sin hacer nada o con sleep()
				sleep(50);
			}
			alarm(0);
			if(Control::instance().recievedConnectionMask) { //s'ha sortit del bucle pel timeout
				Control::instance().writeDownServer();
			}
			
			/*for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
				cout << "id!!! " << (*it)->id << endl;
				cout << "load!" << (*it)->load.totalLoad << endl;
			}*/
			Control::instance().servers.sort(compareServersLoad); //ordenamos la lista
			/*for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
				cout << "id!!! " << (*it)->id << endl;
				cout << "load!" << (*it)->load.totalLoad << endl;
			}*/
			Control::instance().balance(); //ejecutar algoritmo balanceo y envia las instrucciones de balanceo a los servidores de juego
			//poner la alarma para el proximo rebalanceo
			breakflag = 0; 
			timeout = 1;  
			signal(SIGALRM, balanceHandle);
			alarm(REBALANCING_TIME);
		}
		sleep(50);
		//para una segunda version implementar un listener que escuche peticiones de emergencia		
	}

	return 0;
}
