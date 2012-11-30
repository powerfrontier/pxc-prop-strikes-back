#include <iostream>
#include <math.h>
#include <signal.h>
#include <thread>
#include <mutex>

#include "balanceo.h"
#include <GetServerLoad.h>

using namespace std;


double Control::getAverage() {
	//Obte la mitjana de carrega dels servidors
	double incr = 0;
	list<server*>::iterator it;
	for (it=servers.begin(); it!=servers.end(); it++) {
		incr += (*it)->carga.cargaTotal;
	}
	return incr/NSERVERS;
}

double Control::getStDev() {
	//Obte la desviacio estandar de la carrega dels servidors
	double avg = getAverage();
	double incr = 0;
	list<server*>::iterator it;
	for (it=servers.begin(); it!=servers.end(); it++) {
	    	incr += pow((*it)->carga.cargaTotal - avg, 2);
	}
	return sqrt(incr / (NSERVERS - 1));
}

void Control::zoneChange(Server sourceServer, int changedZonePosition, Server destinationServer) {
	//TODO: hacer cambioZona
}

void Control::balance() {
	server* serverMaxCarga = servers.back();
	server* serverMinCarga = servers.front();
	double standardDev = getStDev();
	int numIterations = 0;
	double cargaMinZona = 1.0;
	int zonaCargaMin = 0;
	int posicionZonaACambiar;
	double minDev = 1.0; //TODO S'ha d'ajustar bé el valor

	while ( standardDev > minDev && numIterations < servers.size() && serverMaxCarga->carga.distribucion.size() != 1)
	{
		// Escogemos la zona menos cargada de serverMaxCarga
		for(int i = 0; i < serverMaxCarga->carga.distribucion.size();++i)
		{
			if(serverMaxCarga->carga.distribucion.at(i).carga < cargaMinZona){
				cargaMinZona = serverMaxCarga->carga.distribucion.at(i).carga;
				zonaCargaMin = i; // Guardamos la zona donde esta la carga minima
			}
		}
		// Faltaria mirar quin es el servidor amb les zones mes properes
		posicionZonaACambiar = zonaCargaMin; // de moment faig aixo perque funcioni
		cambioZona(serverMaxCarga, posicionZonaACambiar, serverMinCarga);
		servers.sort();
		standardDev = getStDev();
		numIterations++;
		// Preparación variables para la siguiente iteracion
		cargaMinZona = 1;
		serverMaxCarga = servers.back();
		serverMinCarga = servers.front();
	}
}

void Control::initializeServerList() {
	//TODO: hacer inicializarListaServidores
	//rellenar la lista de servidores con servidores con ip definida en el .h como constante y id secuencial y un reparto de las zonas arbitrario
}

void Control::initializeConnections() {
	list<server*>::iterator it;
	for(it=servers.begin();it!=servers.end();it++) {
		(*it)->c = new TCPConnection();
		(*it)->c->connect((*it)->ip, PUERTOJUEGO);
	}
	conexionLogin->connect(IPLOGIN, PUERTOLOGIN);
	conexionRedireccion->connect(IPREDIRECCION, PUERTOREDIRECCION);
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
	int serverConnectList = ~rebuts;
	for( i = 0; i < NSERVERS; i++){
		serverNum = serverConnectList & serverMask;
		if(!serverNum){
			cout << "El servidor: " << i << " no responde.";
		}
		serverMask *= 2;
	}
}




int main() {
	inicializarConexiones();
	//std::thread t(consola);
	//signal(SIGALRM, handle);
	list<server*>::iterator it;
	timeout = 1;	
	rebuts = 0;
	int shift = (sizeof(int) * 8) - NSERVERS;	
	rebuts = ~rebuts;
	rebuts = rebuts >> shift; // Ponemos a 1 únicamente NSERVERS
	breakflag = 1; // Ponemos a 1 para entrar en la primera vuelta del bucle
	while(1) {
		if(breakflag) {
			alarm(0);	// Apagamos el timer
			for(it=servers.begin();it!=servers.end();it++) { //Para todos los servidores...
				GetServerLoad* getServerLoad = new GetServerLoad(*it, &rebuts, &rebuts_mutex);
				(*it)->c->send(*getServerLoad); //Enviamos la instruccion de solicitud de carga			 
			}
			signal(SIGALRM, handleSolicitarCarga);
			alarm(WAITING_RESPONSE_TIME);
			while(rebuts && timeout) { 
				//Esperamos sin hacer nada o...								
				//...mirar si se puede poner el proceso a dormir y que se despierte con un signal también
			}
			alarm(0);
			if(rebuts) { //s'ha sortit del bucle pel timeout
				writeDownServer();
			}
			servers.sort(); //ordenamos la lista
			balanceo(); //ejecutar algoritmo balanceo y envia las instrucciones de balanceo a los servidores de juego
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
