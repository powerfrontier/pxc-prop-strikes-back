#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <string>
#include "Datagram.h"
#include "Transferable.h"
 
/* OpenSSL headers */
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

class Connection;

struct ConnectionCallback {
ConnectionCallback();
virtual ~ConnectionCallback();


virtual void callbackFunction(Transferable* received, Connection*) throw() = 0;
};



class ConnectionException : public std::exception {
std::string mWhat;
public:
ConnectionException(const std::string& message) throw();
virtual ~ConnectionException() throw();

const char* what() const throw();
};


class Connection {
protected:
ConnectionCallback* mCallback;
bool	mIsOpen;
std::string mPort;
public:
Connection() throw();
virtual ~Connection() throw(ConnectionException);

//Set the objective ip and try to connect to 
virtual const std::string& getPort() = 0;
virtual bool connect(const std::string& ipAddr, const std::string& port) throw(ConnectionException) = 0;
virtual void close() throw() = 0;

virtual bool isLinkOnline() throw() = 0;

virtual void send(Transferable& message) throw (ConnectionException) = 0;
virtual void sendAnswer(Transferable& message) throw (ConnectionException) = 0;
virtual void receive() throw(ConnectionException) = 0;

void setCallbackFunction(ConnectionCallback*) throw();
};

class TCPConnection : public Connection {
private:
BIO *sbio;
std::thread *tListen;
std::mutex mOnlineMutex;
bool online;
void receiveThread();
void receiveTransfThread() throw(ConnectionException);
void setLinkOnline(bool);
virtual void close(bool) throw();
public:
TCPConnection() throw();
TCPConnection(BIO*, std::string port) throw();
virtual ~TCPConnection() throw();
virtual const std::string& getPort();
virtual bool connect(const std::string& ipAddr, const std::string& port) throw(ConnectionException);
virtual void close() throw();
virtual bool isLinkOnline() throw();
virtual void send(Transferable& message) throw (ConnectionException);
virtual void sendAnswer(Transferable& message) throw (ConnectionException);
virtual void receive() throw(ConnectionException);
};

class UDPConnection : public Connection {
public:
UDPConnection() throw();
virtual ~UDPConnection() throw();

virtual bool connect(std::string& ipAddr, const std::string& port) throw(ConnectionException);

virtual void close() throw();

virtual bool isLinkOnline() throw();
virtual const std::string& getPort();
virtual void send(Transferable& message) throw (ConnectionException);
virtual void sendAnswer(Transferable& message) throw (ConnectionException);
virtual void receive() throw(ConnectionException);
};

class TCPConnectionSecurity : public Connection {
private:
int sock;
SSL_CTX *ctx;
BIO *sbio;
SSL *ssl;
std::thread *tListen;
std::mutex mOnlineMutex;
char *pass;

bool online;
void receiveThread();
void receiveTransfThread() throw(ConnectionException);
void setLinkOnline(bool);
virtual void close(bool) throw();
public:
int password_cb(char *buf,int num, int rwflag,void *userdata);
TCPConnectionSecurity() throw();
TCPConnectionSecurity(SSL*, std::string port) throw();
virtual ~TCPConnectionSecurity() throw();
virtual const std::string& getPort();
virtual bool connect(const std::string& ipAddr, const std::string& port) throw(ConnectionException);
virtual void close() throw();
virtual bool isLinkOnline() throw();
virtual void send(Transferable& message) throw (ConnectionException);
virtual void sendAnswer(Transferable& message) throw (ConnectionException);
virtual void receive() throw(ConnectionException);
};

#endif

