#ifndef _SharedStructs_h
#define _SharedStructs_h

#include <vector>
#include <Connection.h>




class Server {
		public:


	struct zoneLoad {
		      int zone;
		      double load;
	};

	struct serverLoad {
		std::vector<zoneLoad> distribution;
		double totalLoad;
	};  
	int id;
  char* ip;
  Connection* c;
  serverLoad load;
  



  bool operator < (const Server& s) const
  {
    return (load.totalLoad < s.load.totalLoad);
  }

	
	void setLoad(serverLoad sl){
		load = sl;
	}

	serverLoad getLoad(){
		return load;
	}

	
	
  
};

#endif
