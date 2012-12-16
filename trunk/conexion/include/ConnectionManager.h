#ifndef _CONNECTIONMANAGER_H_
#define _CONNECTIONMANAGER_H_
#include "Connection.h"
#include <thread>
#include <list>
class ConnectionManager{
private:
struct
std::thread *tListen;
ConnectionCallback *cCallB;
ConnectionClosedListener *myClose;
bool imListening;
std::mutex mOnlineMutex;
std::list<Connection *> conexions;
void listenThread(const std::string& port) throw(ConnectionException);
void listenThreadSecure(const std::string& port, bool) throw(ConnectionException);
void receive(Connection*) throw(ConnectionException);

public:
ConnectionManager() throw();
~ConnectionManager() throw();
void listen(const std::string&) throw(ConnectionException);
void listenSecure(const std::string&, bool) throw(ConnectionException);
void setCallbackFunction(ConnectionCallback*) throw();
void setMyClose(ConnectionClosedListener *func);
bool isListening();
};
#endif
