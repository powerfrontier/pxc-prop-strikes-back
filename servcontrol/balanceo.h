#ifndef _balanceo_h
#define _balanceo_h

#include <list>

#define MINLOAD 0.10
#define NSERVERS 10

struct zona_usuario {
	int zona;
	int usuarios;
};

struct server {
	zona_usuario distribucion[];
	int carga;
	static char* ip;
};

std::list<server*> servers;

int solicitar_carga(server* server);
int clasificar_server(server* server);
int accion_servidor(server* server);
int rebalanceo();

#endif
