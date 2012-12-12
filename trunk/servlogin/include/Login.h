#ifndef _LOGIN_H_
#define _LOGIN_H_
 
#include <Singleton.h>
#include <Connection.h>
#include <ConnectionManager.h>

#include <map>

#define CLIENT_PORT "3456"
#define CONTROL_PORT "3457"
#define ROUTER_IP "127.0.0.1"
#define ROUTER_PORT "3500"
#define IP_LENGTH 16
#define PORT_LENGTH 5

class Login : public Singleton<Login> {

	Connection* controlConnection;
	ConnectionManager* manager;
	unsigned int nextFreeToken;
	std::map<int,int> userTokenMap;
	std::mutex loginMutex;
	unsigned int usersConnected;
	friend class LoginRequestRcvd;
	friend class LogoutRequestRcvd;
public:

	virtual ~Login();
	void initializeManager();
	void initializeLogin();
	bool validate(const char* user,const char* pwd);
	Connection* getControlConnection();
	
};

#endif
