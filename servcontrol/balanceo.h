#ifndef _balanceo_h
#define _balanceo_h

#include <list>

#include "../conexion/include/Connection.h"
#include "../instrucciones/SharedStructs.h"

#define MAXDESV 2
#define NSERVERS 10
#define TIME 5
#define ZONES 9
#define IPLOGIN "192.168.0.1"
#define PUERTOLOGIN 3456
#define IPREDIRECCION "192.168.0.1"
#define PUERTOREDIRECCION 3457
#define IPJUEGO1 "192.168.0.1"
#define IPJUEGO2 "192.168.0.1"
#define IPJUEGO3 "192.168.0.1"
#define IPJUEGO4 "192.168.0.1"
#define PUERTOJUEGO 3458

void inicializarListaServidores();
void inicializarConexiones();
void managerConexiones();
int solicitarCarga(server* server);
void balanceo();
void cambioZona(server* serverMaxCarga, int posicionZonaACambiar, server* serverMinCarga);

#endif
