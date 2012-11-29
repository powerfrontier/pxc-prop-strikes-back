#ifndef _CONNECTIONMANAGER_H_
#define _CONNECTIONMANAGER_H_
#include "Connection.h"
class ConnectionManager{
private:
BIO* bioStack;
void listen(const std::string& port) throw(ConnectionException);
public:
ConnectionManager() throw();
~ConnectionManager() throw();
void listenPort(const std::string& port) throw(ConnectionException);
void setCallbackFunction(ConnectionCallback*) throw();
};
#endif
