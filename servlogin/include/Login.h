#ifndef _LOGIN_H_
#define _LOGIN_H_
 
#include <Singleton.h>
#include <Connection.h>
#include <ConnectionManager.h>
#include <ClientClosedConnection.h>
#include <mysql.h>
#include <map>

#define CLIENT_PORT "3450"
#define CONTROL_PORT "3456"
#define ROUTER_IP "127.0.0.1"
#define ROUTER_PORT "3500"
#define IP_LENGTH 16
#define PORT_LENGTH 5

#define DBHOSTNAME "mysqlfib.fib.upc.es"
#define DBUSER   "pxc03"
#define DBPASSWORD "nJoW03Hi"
#define DBNAME       "BDpxc03"

class Login : public Singleton<Login> {
	MYSQL *mysqlConnection;
	Connection* controlConnection;
	ConnectionManager* managerClient;
	ConnectionManager* managerControl;
	ClientClosedConnection* clientClosedConnection;
	bool controlConnected;
	unsigned int nextFreeToken;
	std::map<int,int> userTokenMap;
	std::map<int,Connection*> idToConnectionMap;
	std::map<Connection*,int> connectionToIdMap;
	std::map<std::string,int> userToIdMap;
	std::map<int,std::string> idToUserMap;
	std::mutex loginMutex;
	unsigned int usersConnected;
	friend class LoginRequestRcvd;
	friend class LogoutRequestRcvd;
	friend class ControlConnectionRcvd;
	friend class ClientClosedConnection;
public:

	virtual ~Login();
	void initializeManager();
	void initializeLogin();
	bool validate(std::string user,std::string pwd);
	Connection* getControlConnection();
	
};

#endif
