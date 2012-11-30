#ifndef _balanceo_h
#define _balanceo_h

#include <list>

#include <Connection.h>
#include <SharedStructs.h>

#define MAXDESV 2
#define NSERVERS 2
#define REBALANCING_TIME 5
#define WAITING_RESPONSE_TIME 10
#define ZONES 4
#define IPLOGIN "192.168.0.1"
#define PUERTOLOGIN "3456"
#define IPREDIRECCION "192.168.0.1"
#define PUERTOREDIRECCION "3457"
#define IPJUEGO1 "192.168.0.1"
#define IPJUEGO2 "192.168.0.1"
#define IPJUEGO3 "192.168.0.1"
#define IPJUEGO4 "192.168.0.1"
#define PUERTOJUEGO "3458"


void inicializarListaServidores();
void inicializarConexiones();
void balanceo();
void cambioZona(server* serverMaxCarga, int posicionZonaACambiar, server* serverMinCarga);

#endif
