#ifndef _SharedStructs_h
#define _SharedStructs_h

#include <vector>
#include <Connection.h>


struct zoneLoad {
        int zone;
        double load;
};

struct serverLoad {
	std::vector<zoneLoad> distribution;
        double totalLoad;
};

class Server {
  int id;
  char* ip;
  Connection* c;
  serverLoad load;
  
  bool operator < (const Server& s) const
  {
    return (load.totalLoad < s.load.totalLoad);
  }
  
};

#endif
