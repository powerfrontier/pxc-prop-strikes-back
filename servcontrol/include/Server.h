#ifndef _Server_h
#define _Server_h

#include <vector>
#include <Connection.h>
#include <ZoneLoad.h>
#include <iostream>

class Server {
	public:
	
	struct serverLoad {
		std::vector<ZoneLoad*> distribution;
		double totalLoad;
	};  
	int id;
	char* ip;
	Connection* c;
	serverLoad load;
	bool operator < (const Server & s) const
	{
		//std::cout << load.totalLoad << " " << s.load.totalLoad << std::endl;
		return (load.totalLoad < s.load.totalLoad);
	}


	Server(int idServer, char* ipServer): id(idServer), ip(ipServer) {} 	
	Server(){}
	virtual ~Server();
	void printServer();
};


#endif
