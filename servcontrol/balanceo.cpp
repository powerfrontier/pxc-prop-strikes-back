#include <iostream>
#include "balanceo.h"
#include <math.h>
#include <signal.h>
//#include "../conexion/Connection.h"

using namespace std;

server* zona_servidor[ZONES];
std::list<server*> servers;


//int accionServidor(server* server, Datagram ordenes) {
  // Envia las ordenes del rebalanceo al servidor

//}

/*void anadirCarga(Datagram datos, server* s) {
  //Añade la información de carga a la lista de servidores
  s->server_carga = datos.getServer_carga();
}*/

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

swapZona(int serverMaxCarga, int posicionZonaACambiar, int serverMinCarga) {

}

void balanceo() {
  //ejecuta el algortimo de balanceo
  server* serverMaxCarga = servers.begin();
  server* serverMinCarga = servers.end();
  double standardDev = getStDev();
  int numIterations = 0;
  double cargaMinZona = 1.0; 
  int zonaCargaMin = 0;
  int posicionZonaACambiar; //David: variable nova
  double minDev = 1.0; // S'ha d'ajustar bé el valor
  while ( standarDev > minDev && numIterations < servers.size() )
  {
     // Escogemos la zona menos cargada de serverMaxCarga
     for(int i = 0; i < serverMaxCarga->carga.distribucion.size();++i)
     {
        if(serverMaxCarga->carga.distribucion[i].carga < cargaMinZona){
          cargaMinZona = serverMaxCarga->carga.distribucion[i].carga 
          zonaCargaMin = i; // Guardamos la zona donde esta la carga minima
        }
      }
     // Faltaria mirar quin es el servidor amb les zones mes properes
     posicionZonaACambiar = zonaCargaMin; // David: de moment faig aixo perque funcioni
     cambioZona(serverMaxCarga, posicionZonaACambiar, serverMinCarga);
     servers.sort(); //David: trec els parametres perque la funcio estandard es aixi
     standarDev = getStDev();
     numIterations++;
     // David: no faltaria cargaMinZona = 1; per la propera volta del bucle 
     cargaMinZona = 1;
   }
}

int solicitarCarga(server* server) {
  /*connection* c = new TCPConnection();
  if(c.connect(server->ip)) {
    res = c.setReceiveCallback(anadirCarga, new Datagram(), server);
  }
  else
    res = -1
  
  return res;*/
    return 0;
}

volatile int breakflag = 1;

void handle(int sig) {
    //printf("Hello\n");
    breakflag = 1;
}


int main() {

signal(SIGALRM, handle);
list<server*>::iterator it;

 while(1) {
    if(breakflag) {
        //para todas los servidores
        int res;  
        for(it=servers.begin();it!=servers.end();it++) {
          //solicitar_carga
          res = solicitarCarga(*it);
          if(res < 0) {
            server* aux = *it;
            cout << "No responde el server:" << aux->id << endl;
          }
        }
        servers.sort();
        //ejecutar algoritmo balanceo
        balanceo();
        
        //enviar las ordenes de balanceo necesarias a los servidores
        
        //contar TIME segundos
        breakflag = 0;
        alarm(TIME);
    }
    //para una segunda version implementar un listener que escuche peticiones de emergencia 
 }


return 0;
}
