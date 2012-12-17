#include <iostream>
#include <math.h>
#include <signal.h>
#include <thread>
#include <mutex>
#include <string.h>
#include <unistd.h>

#include <Control.h>
#include <ServerLoad.h>
#include <SetZoneToServer.h>
#include <ControlProfile.h>
#include <RemoveZone.h>
#include <GetZone.h>
#include <RouterChangeZone.h>
#include <IPServer.h>
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
	return incr/Control::instance().numberServers;
}

double Control::getStDev() {
	//Gets the standar desviation load of servers.
	double avg = getAverage();
	double incr = 0;
	list<Server*>::iterator it;
	for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
	    	incr += pow((*it)->load.totalLoad - avg, 2);
	}
	if(Control::instance().numberServers < 2) {
		return sqrt(incr / Control::instance().numberServers);
	}
	else {
		return sqrt(incr / (Control::instance().numberServers - 1));
	}
}

void Control::zoneChange(Server* sourceServer, int changedZonePosition, Server* destinationServer) {
	// Ordenamos quitar la zona de un servidor
	cout << "ZONE CHANGE: " << "origen: " << sourceServer->id << " zona: " << changedZonePosition << " destino: " << destinationServer->id << endl;

	RemoveZoneSend* removeZoneSend = new RemoveZoneSend(changedZonePosition);
	sourceServer->c->send(*removeZoneSend);
	delete(removeZoneSend);  	
	
	GetZoneSend* getZoneSend = new GetZoneSend(changedZonePosition, sourceServer->id);
	destinationServer->c->send(*getZoneSend);
	delete(getZoneSend);   
	
	RouterChangeZoneSend* routerChangeZoneSend = new RouterChangeZoneSend(changedZonePosition, sourceServer->id, destinationServer->id );
	Control::instance().routerConnection->send(*routerChangeZoneSend);
	delete(routerChangeZoneSend);	
}

void Control::balance() {
	Server* maxLoadServer = servers.back();
	Server* minLoadServer = servers.front();
	double standardDev = getStDev();
	int numIterations = 0;
	double minLoadZone = 1.0;
	int posMinLoadZone = -1;
	int changedZonePosition;
	double minDev = 1.0; //TODO S'ha d'ajustar bé el valor
	
	standardDev = 2;//TODO solamente de debug
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
	Control::instance().ipServers = new char*[Control::instance().numberServers];
	Control::instance().portServers = new char*[Control::instance().numberServers];
	for(int i=0; i<Control::instance().numberServers; i++) { // Se necesita memoria dinamica
		ipServers[i] = new char[IPLENGTH];
		portServers[i] = new char[PORTLENGTH];
	}
	Control::instance().ipRouter = (char*) malloc (IPLENGTH);
	Control::instance().portRouter = (char*) malloc (PORTLENGTH);
	Control::instance().ipLogin = (char*) malloc (IPLENGTH);
	Control::instance().portLogin = (char*) malloc (PORTLENGTH);
	int j = 0;
	for (int i = 0; i < Control::instance().resultBD.num_rows(); ++i) {
		if(Control::instance().resultBD[i][2] == TYPE_GAME_SERVER && j < Control::instance().numberServers) {
			strcpy(ipServers[j], Control::instance().resultBD[i][0]);
			//cout << "copia de direccions!!!!" << Control::instance().resultBD[i][0] << endl;
			strcpy(portServers[j], Control::instance().resultBD[i][1]);
			j++;
		}
		else if(Control::instance().resultBD[i][2] == TYPE_ROUTER_SERVER) {
			strcpy(Control::instance().ipRouter, Control::instance().resultBD[i][0]);
			strcpy(Control::instance().portRouter, Control::instance().resultBD[i][1]);
		}
		else if(Control::instance().resultBD[i][2] == TYPE_LOGIN_SERVER) {
			strcpy(Control::instance().ipLogin, Control::instance().resultBD[i][0]);
			strcpy(Control::instance().portLogin, Control::instance().resultBD[i][1]);
		}
        }
}

char* Control::getIpServerById(int id){
	return ipServers[id];
}

char* Control::getPortServerById(int id){
	return portServers[id];
}

/*void Control::initializeServerList() {
	list<Server*>::iterator it;
	Control::fillIpServerTable();
	for(int i = 0; i < Control::instance().numberServers; ++i){
		servers.push_back(new Server(i,ipServers[i], portServers[i]));
	}
}*/

void Control::initializeConnectionsAndServerList() {
	Control::fillIpServerTable();
	list<Server*>::iterator it;
	int i=0;
	//Control::instance().numberServersIni = Control::instance().numberServers;	
	int ipsIni = Control::instance().numberServers;
	Connection* auxConn = NULL;
	Server* auxServ = NULL;

	for(int i = 0; i < ipsIni; ++i){
		auxConn = new TCPConnectionSecurity(std::string("control.pem"), std::string("dh1024.pem"));
		if(auxConn->connect(ipServers[i], portServers[i])){
			cout << "Servidor " << i << " conectado" << "Conexion: " << auxConn << endl;
			auxServ = new Server(i, ipServers[i], portServers[i]);
			auxServ->c = auxConn;
			cout << "Conexion: " << auxServ->c << endl;
			servers.push_back(auxServ);
		}else{
			Control::instance().numberServers--;
			cout << "Servidor " << i << " NO conectado" << "Nº Servers juego conectados: " << Control::instance().numberServers << endl;
			delete auxConn;
		}
		
	}

	loginConnection = new TCPConnectionSecurity(std::string("control.pem"), std::string("dh1024.pem"));
	if(loginConnection->connect(Control::instance().ipLogin, Control::instance().portLogin)){
		cout << "Servidor login conectado" << endl;
				
	}else{
		cout << "Servidor login NO conectado" << endl;
	}

	routerConnection = new TCPConnectionSecurity(std::string("control.pem"), std::string("dh1024.pem"));
	if(routerConnection->connect(Control::instance().ipRouter, Control::instance().portRouter)){
		cout << "Servidor redireccion conectado" << endl;
				
	}else{
		cout << "Servidor redireccion NO conectado" << endl;
	}
}

/*void Control::writeDownServer(){
	int i;
	int serverMask = 1;
	int serverNum = 0;
	int serverConnectList = ~recievedConnectionMask;
	for( i = 0; i < Control::instance().numberServersIni; i++){
		serverNum = serverConnectList & serverMask;
		if(!serverNum){
			cout << "El servidor: " << i << " NO responde" << endl;
		}
		serverMask *= 2;
	}
}*/

void Control::zoneAssignment(){
	int zonesRestants = NZONES;
	int modZonesPerServer = NZONES % Control::instance().numberServers;
	int zoneIndex = modZonesPerServer;
	int i;
	ZoneLoad* zl;
	SetZoneToServerSend* setZoneToServerSend;
	IPServerSend* ipServerSend;
	Server* server = servers.front();

	for(i = 0; i < modZonesPerServer; ++i){
		if(server->c != NULL && server->c->isLinkOnline()) {
			setZoneToServerSend = new SetZoneToServerSend(i,server->id); // Enviamos id de zona y de servidor a juego para que este lo guarde	
			server->c->send(*setZoneToServerSend);
			cout << "server: " << server->id << " zona: " << i << endl; //debug
		//Control::instance().routerConnection->send(*setZoneToServerSend); //reutilizamos la instr para avisar a router tambien
		//ipServerSend = new IPServerSend(server->id, server->ip, server->port);
		//Control::instance().routerConnection->send(*ipServerSend); //le enviamos la ip y puerto a router
			delete(setZoneToServerSend);
		//delete(ipServerSend);
			zoneServer[i] = server;		
			zl = new ZoneLoad(i, 0); //Creamos las distribuciones de zonas y carga para el objeto Server
			server->load.distribution.push_back(zl);
			server->load.totalLoad = 0;
			zonesRestants--;
		}	
	}
	
	list<Server*>::iterator it;
	while(zonesRestants > 0) {
		for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
			if((*it)->c != NULL && (*it)->c->isLinkOnline()) {
				setZoneToServerSend = new SetZoneToServerSend(zoneIndex,(*it)->id); 
				(*it)->c->send(*setZoneToServerSend);
				cout << "server: " << (*it)->id << " zona: " << zoneIndex << endl; //debug
				//Control::instance().routerConnection->send(*setZoneToServerSend); 
				//ipServerSend = new IPServerSend(server->id, server->ip, server->port);
				//Control::instance().routerConnection->send(*ipServerSend);
				delete(setZoneToServerSend);
				//delete(ipServerSend); 
				zoneServer[zoneIndex] = (*it);
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
		
}

void Control::eliminarServidor(const int idServer) {
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
	//return (rand() % 3 + 1); //valor entre 0-4
	//std::string aux (idUsuari);cout << aux << endl;
	std::string consulta = std::string("select IDZONE from PLAYERS where IDUSER='") + std::to_string(idUsuari) + std::string("'");
	cout << consulta << endl;
	mysqlpp::Query query = Control::instance().cbd->query(consulta);
        if (mysqlpp::StoreQueryResult res = query.store()) {
		return res[0][0];
	}
	return -1;
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
	return (first->load.totalLoad < second->load.totalLoad);
}


int main(int argc, char** argv) {
	//Cogemos el numero de servidores como el argumento de entrada
	if(argc >= 2) {
		int aux = atoi(argv[1]);
		if(aux < 1) {
			cout << "El numero de servidores de juego tiene que ser mayor que 0" << endl;
			return -1;
		}
		else if(aux <= MAX_GAME_SERVERS) {
			Control::instance().numberServers = aux;
		}
		else {
			Control::instance().numberServers = MAX_GAME_SERVERS;
			cout << "Se ha especificado un numero de servers de juego mayor que el limite. Se aplica el valor maximo" << endl; 
		}
	}
	else {
		Control::instance().numberServers = DEF_GAME_SERVERS; //Sino ponemos un numero por defecto
		cout << "No se ha especificado el numero de servers de juego. Se aplica el valor por defecto" << endl; 
	}
	cout << "Nº Servers de juego: " << Control::instance().numberServers << endl;
	//Conexio a BD
	try
	{
		cout << "Conectando a BD..."; fflush(stdout);
		mysqlpp::Connection* conn = new mysqlpp::Connection("BDpxc03", "mysqlfib.fib.upc.edu", "pxc03", "nJoW03Hi", 3306);
		if(conn->connected())
			cout << "OK" << endl;
		Control::instance().cbd = conn;
	}
	catch (mysqlpp::ConnectionFailed& e)
	{
		cerr << "ERROR en conexion a BD! Exception: " << e.what() << endl;
	}
	if(Control::instance().cbd->connected()) { 
		mysqlpp::Query query = Control::instance().cbd->query("select * from ADDRESSES");
		Control::instance().resultBD = query.store();
		int aux = 0;
		for (int i = 0; i < Control::instance().resultBD.num_rows(); ++i) {
			if(Control::instance().resultBD[i][2] == TYPE_GAME_SERVER) {
				aux++;	
			}
		}
		if( aux < Control::instance().numberServers) {
			Control::instance().numberServers = aux;
			cout << "El numero de servidores de juego en la BD es menor que el que se demanda. Corrigiendo numero de servers de juego a el de la BD: " << aux << endl; 
		}
			
	}	
	//ControlProfile
	TransferableFactory::instance().setProfile(new ControlProfile());
	TransferableFactory::instance().setProtocol("0.1a");
	//Inicialización
	cout << "Inicializando..." << endl;
	//Control::instance().initializeServerList();
	Control::instance().initializeConnectionsAndServerList();
	cout << "Inicializando...OK" << endl;
	cout << "Asignando zonas a servidores de juego...";
	Control::instance().zoneAssignment();
	cout << "OK" << endl;

	list<Server*>::iterator it;
	timeout = 1;	
	Control::instance().recievedConnectionMask = 0;
	unsigned int shift = (sizeof(int) * 8) - Control::instance().numberServers;	
	Control::instance().recievedConnectionMask = ~Control::instance().recievedConnectionMask;
	Control::instance().recievedConnectionMask = Control::instance().recievedConnectionMask >> shift; // Ponemos a 1 únicamente el numero de servidores de juego
	
	//Enviando señal de vida a login
	LoginOnlineSend* instr = new LoginOnlineSend();
	Control::instance().loginConnection->send(*instr);
	delete(instr);
	
	breakflag = 1; // Ponemos a 1 para entrar en la primera vuelta del bucle
	
	while(1) { 
		if(breakflag) {
			cout << "REBALANCEO" << endl;	
			alarm(0);// Apagamos el timer
			for(it=Control::instance().servers.begin();it!=Control::instance().servers.end();it++) {
				// Inicializamos la carga total del server
				if((*it)->c != NULL && (*it)->c->isLinkOnline()) {				
					(*it)->load.totalLoad = 0;
					ServerLoadSend* serverLoadSend = new ServerLoadSend();
					//cout << "antes de enviar instruccion solicitar carga al server" << (*it)->id << endl;				
					(*it)->c->send(*serverLoadSend); //Enviamos la instruccion de solicitud de carga
					delete(serverLoadSend);
				}
				//else {
					//cout << "ERROR: servidor: " << (*it)->id << "CAIDO" << endl;
					//Control::instance().eliminarServidor((*it)->id);
				//}
				//cout << "despues de enviar instruccion carga al server" << (*it)->id << endl;
			}
			signal(SIGALRM, loadRequestHandle);
			alarm(WAITING_RESPONSE_TIME);
			while(Control::instance().recievedConnectionMask && timeout) { 
				sleep(50);//Esperamos sin hacer nada ni consumir cpu
			}
			alarm(0);
			//if(Control::instance().recievedConnectionMask) { //s'ha sortit del bucle pel timeout
			//	Control::instance().writeDownServer();
			//}
			/*for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
				cout << "id!!! " << (*it)->id << endl;
				cout << "load!" << (*it)->load.totalLoad << endl;
			}*/
			Control::instance().servers.sort(compareServersLoad); //ordenamos la lista
			/*for (it=Control::instance().servers.begin(); it!=Control::instance().servers.end(); it++) {
				cout << "id!!! " << (*it)->id << endl;
				cout << "load!" << (*it)->load.totalLoad << endl;
			}*/
			Control::instance().balance();//Ejecuta algorismo y envia las ordenes pertinentes
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
