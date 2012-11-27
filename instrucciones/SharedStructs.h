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

#endif
