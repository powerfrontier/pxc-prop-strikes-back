#include <iostream>
#include "balanceo.h"
#include "Connection.h"

using namespace std;

server* zona_servidor[ZONES];
std::list<server*> servers;


int accionServidor(server* server, Datagram ordenes) {
  // Envia las ordenes del rebalanceo al servidor

}

void anadirCarga(Datagram datos, server* s) {
  //Añade la información de carga a la lista de servidores
  s->server_carga = datos.getServer_carga();
  servers.push_back(s);
}

double getAverage() {
  //Obte la mitjana de carrega dels servidors
   for (int i = 0; i < NSERVERS; ++i) {
            incr += servers[i]->server_carga.carga_total;
   }
   return incr/NSERVERS;
}

double getStDev() {
  //Obte la desviacio estandar de la carrega dels servidors
        double avg = getAverage();
        double incr = 0;
        for (int i = 0; i < NSERVERS; ++i) {
            incr += pow(servers[i]->server_carga.carga_total - avg, 2);
        }
        return sqrt(incr / (NSERVERS - 1));
}

void balanceo() {
  //ejecuta el algortimo de balanceo
}

int solicitarCarga(server* server) {
  delete servers;
  std::list<server*> servers;
  connection* c = new TCPConnection();
  if(c.connect(server->ip)) {
    res = c.setReceiveCallback(anadirCarga, new Datagram(), server);
  }
  else
    res = -1
  
  return res;
}

volatile int breakflag = 1;

void handle(int sig) {
    //printf("Hello\n");
    breakflag = 1;
}


int main() {

signal(SIGALRM, handle);

 while(1) {
    if(breakflag) {
        //para todas los servidores
        int res;  
        for(int i=0;i<NSERVERS;i++) {
          //solicitar_carga
          res = solicitarCarga(it.next());
          if(res < 0)
            printf("No responde el server:" %d, it.actual().id);
        }
        servers.sort(servers.begin(), servers.end());
        //ejecutar algoritmo balanceo
        balanceo();
        
        //enviar las ordenes de balanceo necesarias a los servidores
        
        //contar TIME segundos
        breakflag = 0;
        alarm(TIME);
    }
    //para una segunda version implementar un listener que escuche peticiones de emergencia 
?  }

?  return 0;
}
