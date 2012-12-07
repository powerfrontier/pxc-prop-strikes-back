#ifndef _ROUTER_H_
#define _ROUTER_H_

#include <map>
#include <Connection.h>

class Router {
	std::map<int, std::pair<int, Connection*> mUserServer;
	
public:
	Router();
	virtual ~Router();
	
	
	
};



#endif
