#include <iostream>
#include <math.h>
#include <signal.h>

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
		posicionZonaACambiar = zonaCargaMin; // David: de moment faig aixo perque funcioni
		cambioZona(serverMaxCarga, posicionZonaACambiar, serverMinCarga);
		servers.sort(); //David: trec els parametres perque la funcio estandard es aixi
		standardDev = getStDev();
		numIterations++;
		// Preparación variables para la siguiente iteracion
		cargaMinZona = 1;
		serverMaxCarga = servers.back();
		serverMinCarga = servers.front();
	}
}


void solicitarCarga(server* server) {
	int result;
	GetServerLoad* getServerLoad = new GetServerLoad(server);
	server->c->send(getServerLoad);	
	result = server->c->read();	
	
	
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


void handleSolicitarCarga(int sig){
  timeout = 0;
}

volatile int timeout = 1; //Variable que fa de sincronització pel timeout
int rebuts;
std::mutex rebuts_mutex;

void managerConexiones() {
	// El manager de conexions crea threads i els hi asigna una conexió per a fer les comunicacions amb els servidors de joc
	
	rebuts = 0;
	int shift = (sizeof(int) * 8) - NSERVERS; 
	list<server*>::iterator it;
	signal(SIGALRM, handleSolicitarCarga);
	timeout = 1
	rebuts = ~rebuts;
	rebuts = rebuts >> shift // Ponemos a 1 únicamente NSERVERS
	//while(1) {
		alarm(TIMEOUTTHREAD);
		while(rebuts && timeout) {
			for(it=servers.begin();it!=servers.end();it++) { //Per a tots els servidors
				std::thread t(solicitarCarga, *it); //solicitar carga con thread nuevo
			}
						
		}
		if(rebuts) { //s'ha sortit del bucle pel tiemout
			cout << "No responde el server:" << endl; //TODO: hacer que repase la máscara para saber que servers no responden
		}
    
		//Ens parem fins que l'algoritme de balanceig hagi acabat
		//while(semafor_balanceig) {}
	//}
}

volatile int breakflag = 1;

void handle(int sig) {
    breakflag = 1;
}

int main() {

	signal(SIGALRM, handle);
	list<server*>::iterator it;

	inicializarConexiones();

	while(1) {
		if(breakflag) {
			/*int res;
        	for(it=servers.begin();it!=servers.end();it++) {
          	  //solicitar_carga
          	  res = solicitarCarga(*it);
          	  if(res < 0) {
            	server* aux = *it;
            	cout << "No responde el server:" << (*aux).id << endl;
          	  }
        	}*/
			std::thread t(managerConexiones); //crear thread del manager de conexiones
			servers.sort();
			balanceo(); //ejecutar algoritmo balanceo
			//enviar las ordenes de balanceo necesarias a los servidores o hacerlo es cambiarZona?
			semafor_balanceig = 0;
        
			//contar TIME segundos
			breakflag = 0;
			alarm(TIME);
		}
		//para una segunda version implementar un listener que escuche peticiones de emergencia
	}

	return 0;
}
