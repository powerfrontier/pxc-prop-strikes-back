#include <iostream>
#include <math.h>
#include <signal.h>
#include <thread.h>
#include <mutex.h>

#include "balanceo.h"
#include "/instrucciones/GetServerLoad.h"

using namespace std;

server* zona_servidor[ZONES];
std::list<server*> servers;
Connection* conexionLogin = new TCPConnection(IPLOGIN, PUERTOLOGIN);
Connection* conexionRedireccion = new TCPConnection(IPREDIRECCION, PUERTOREDIRECCION);


double getAverage() {
	//Obte la mitjana de carrega dels servidors
	double incr = 0;
	list<server*>::iterator it;
	for (it=servers.begin(); it!=servers.end(); it++) {
		incr += (*it)->carga.cargaTotal;
	}
	return incr/NSERVERS;
}

double getStDev() {
	//Obte la desviacio estandar de la carrega dels servidors
	double avg = getAverage();
	double incr = 0;
	list<server*>::iterator it;
	for (it=servers.begin(); it!=servers.end(); it++) {
	    	incr += pow((*it)->carga.cargaTotal - avg, 2);
	}
	return sqrt(incr / (NSERVERS - 1));
}

void cambioZona(server* serverMaxCarga, int posicionZonaACambiar, server* serverMinCarga) {
	//TODO: hacer cambioZona
}

void balanceo() {
	server* serverMaxCarga = servers.back();
	server* serverMinCarga = servers.front();
	double standardDev = getStDev();
	int numIterations = 0;
	double cargaMinZona = 1.0;
	int zonaCargaMin = 0;
	int posicionZonaACambiar;
	double minDev = 1.0; // S'ha d'ajustar bé el valor

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

void inicializarListaServidores() {
	//TODO: hacer inicializarListaServidores
	//rellenar la lista de servidores con servidores con ip definida en el .h como constante y id secuencial y un reparto de las zonas arbitrario
}

void inicializarConexiones() {
	list<server*>::iterator it;
	for(it=servers.begin();it!=servers.end();it++) {
		(*it)->c = new TCPConection();
		(*it)->c.connect((*it)->ip, PUERTOJUEGO);
	}
	conexionLogin.connect(IPLOGIN, PUERTOLOGIN);
	conexionRedireccion.connect(IPREDIRECCION, PUERTOREDIRECCION);
}

volatile int breakflag = 1; //Variable que fa de sincronització pel timeout de l'espera entre rebalancejos
void handle(int sig) {
    breakflag = 1;
}

volatile int timeout = 1; //Variable que fa de sincronització pel timeout de l'espera de peticion
void handleSolicitarCarga(int sig){
  timeout = 0;
}

int timeout;
int rebuts;
std::mutex rebuts_mutex;

int main() {

	inicializarConexiones();
	//std::thread t(consola);
	signal(SIGALRM, handle);
	signal(SIGALRM, handleSolicitarCarga); //TODO: comprovar si el signal despierta a las 2 funciones handler
	list<server*>::iterator it;
	timeout = 1;	
	rebuts = 0;
	int shift = (sizeof(int) * 8) - NSERVERS;	
	rebuts = ~rebuts;
	rebuts = rebuts >> shift // Ponemos a 1 únicamente NSERVERS

	while(1) {
		if(breakflag) {
			for(it=servers.begin();it!=servers.end();it++) { //Para todos los servidores...
				GetServerLoad* getServerLoad = new GetServerLoad(*it, &rebuts, &rebuts_mutex);
				(*it)->c->send(getServerLoad); //Enviamos la instruccion de solicitud de carga			 
			}
			alarm(WAITING_RESPONSE_TIME);
			while(rebuts && timeout) { 
				//Esperamos sin hacer nada o...								
				//...mirar si se puede poner el proceso a dormir y que se despierte con un signal también
			}
			if(rebuts) { //s'ha sortit del bucle pel tiemout
				cout << "No responde el server:" << endl; //TODO: hacer que repase la máscara para saber que servers no responden
			}
			servers.sort(); //ordenamos la lista
			balanceo(); //ejecutar algoritmo balanceo y envia las instrucciones de balanceo a los servidores de juego
			//poner la alarma para el proximo rebalanceo
			breakflag = 0;
			timeout = 1;
			alarm(REBALANCING_TIME);
		}
		//para una segunda version implementar un listener que escuche peticiones de emergencia
	}

	return 0;
}
