#ifndef _balanceo_h
#define _balanceo_h

#include <list>

#define MAXDESV 2
#define NSERVERS 10
#define TIME 5
#define ZONES 9

struct zona_carga {
	int zona;
	double carga;
};

struct server_carga {
	zona_carga distribucion[];
	double cargaTotal;
};

struct server {
  static int id;
  static char* ip;
  server_carga carga;
};

int solicitarCarga(server* server);
void anadirCarga(Datagram datos);
void balanceo();
int accionServidor(server* server, Datagram ordenes);

#endif
