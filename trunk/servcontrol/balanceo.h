#ifndef _balanceo_h
#define _balanceo_h

#include <list>
#include <vector>
#include "../instrucciones/SharedStructs.h"

#define MAXDESV 2
#define NSERVERS 10
#define TIME 5
#define ZONES 9


int solicitarCarga(server* server);
//void anadirCarga(Datagram datos, server* s);
void balanceo();
//int accionServidor(server* server, Datagram ordenes);

#endif
