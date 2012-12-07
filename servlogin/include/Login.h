#ifndef _Login_h
#define _Login_h
 
#include <Singleton.h>
#include <Connection.h>
#include <ConnectionManager.h>

#define CLIENT_PORT "3456"
#define CONTROL_PORT "3457"

class Login : public Singleton<Login> {
  ConnectionManager* manager;

public:
	virtual ~Login();
	void initializeManager();

};

#endif
