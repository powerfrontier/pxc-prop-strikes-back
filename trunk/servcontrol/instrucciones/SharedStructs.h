#ifndef _SharedStructs_h
#define _SharedStructs_h

#include <vector>
#include <Connection.h>


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
  Connection* c;
  server_carga carga;
  
  bool operator < (const server& s) const
  {
    return (carga.cargaTotal < s.carga.cargaTotal);
  }
  
};

#endif
