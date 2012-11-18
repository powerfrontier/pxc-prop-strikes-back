#include <iostream>
#include "balanceo.h"

#define TIME 5
#define ZONES 9

using namespace std;

static char* zona_servidor[ZONES];
std::list<server*> servers;

volatile int breakflag = 1;

void handle(int sig) {
    //printf("Hello\n");
    breakflag = !breakflag;
}

int main() {

signal(SIGALRM, handle);
 
 while(1) {
		//contar x segundos
    alarm(TIME);
    while(breakflag) {sleep(1);}
    
		//para todas los servidores
    for(int i=0;i<NSERVERS;i++) {
      connection * c = new TCPConnection();
      if(c.connect()
    }
			//solicitar_carga()
    //ejecutar algoritmo balanceo
		//enviar las ordenes de balanceo necesarias a los servidores
    
    
    //para una segunda version implementar un listener que escuche peticiones de emergencia 
	}

	return 0;
}
