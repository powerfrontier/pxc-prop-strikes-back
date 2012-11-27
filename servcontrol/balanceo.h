#ifndef _balanceo_h
#define _balanceo_h

#include <list>
#include <vector>

#define MAXDESV 2
#define NSERVERS 10
#define TIME 5
#define ZONES 9
//
struct zona_carga {
	int zona;
	double carga;
};

struct server_carga {
	std::vector<zona_carga> distribucion;
	double cargaTotal;
};

struct server {
  int id;
  char* ip;
  server_carga carga;
  
  bool operator < (const server& s) const
  {
    return (carga.cargaTotal < s.carga.cargaTotal);
  }
  
};

int solicitarCarga(server* server);
//void anadirCarga(Datagram datos, server* s);
void balanceo();
//int accionServidor(server* server, Datagram ordenes);

#endif
