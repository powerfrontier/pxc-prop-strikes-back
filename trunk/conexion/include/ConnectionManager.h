#ifndef _CONNECTIONMANAGER_H_
#define _CONNECTIONMANAGER_H_
#include "Connection.h"
#include <thread>
class ConnectionManager{
private:
std::thread *t;
ConnectionCallback *cCallB;
void listenThread(const std::string& port) throw(ConnectionException);
void receive(Connection*) throw(ConnectionException);
public:
ConnectionManager() throw();
~ConnectionManager() throw();
void listen(const std::string&) throw(ConnectionException);
void setCallbackFunction(ConnectionCallback*) throw();
};
#endif
