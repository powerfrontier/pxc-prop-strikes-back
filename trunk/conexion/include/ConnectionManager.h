#ifndef _CONNECTIONMANAGER_H_
#define _CONNECTIONMANAGER_H_
#include "Connection.h"
class ConnectionManager{
private:

ConnectionCallback *cCallB;
void listenThread(const std::string& port) throw(ConnectionException);
void ConnectionManager::receive(Connection con) throw(ConnectionException);
public:
ConnectionManager() throw();
~ConnectionManager() throw();
void listen(const std::string& port) throw(ConnectionException);
void setCallbackFunction(ConnectionCallback*) throw();
};
#endif
