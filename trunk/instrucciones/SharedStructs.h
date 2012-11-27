#ifndef _SharedStructs_h
#define _SharedStructs_h

struct zona_carga {
        int zona;
        double carga;
};

struct server_carga {
        zona_carga distribucion[];
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

#endif
