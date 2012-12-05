#ifndef _CONNECTIONMANAGER_H_
#define _CONNECTIONMANAGER_H_
#include "Connection.h"
#include <thread>
#include <list>
class ConnectionManager{
private:
struct
std::thread *tListen;
std::list<std::thread *> tListenN;
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
