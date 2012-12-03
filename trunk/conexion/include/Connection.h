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


/* Connection interface
 * Usage:
 *
 * Connection* myConnection; //Create with prefered subclass
 * try {
 * if (myConnection->connect("127.0.0.1")) {
 * myConnection->send(myDatagramToSend);
 *	myDatagramToReceive = myConnection->receive(); //Blocking
 * myConnection->setCallbackFunction(myCallback); //All new receiving datagrams will be sent to myCallback
 *	doOtherThings();
}
 * }
 * catch (ConnectionException ce) {
 * cout << ce.what();
 * }
 * myConnection->close();
 * delete myConnection;
 */
class Connection {
protected:
ConnectionCallback* mCallback;
bool	mIsOpen;
std::string mIp;
public:
Connection() throw();
virtual ~Connection() throw(ConnectionException);

//Set the objective ip and try to connect to 
virtual bool connect(const std::string& ipAddr, const std::string& port) throw(ConnectionException) = 0;
virtual void close() throw() = 0;

virtual bool isLinkOnline() throw() = 0;

virtual void send(Transferable& message) throw (ConnectionException) = 0;

virtual void receive() throw(ConnectionException) = 0;

void setCallbackFunction(ConnectionCallback*) throw();
};

class TCPConnection : public Connection {
private:
BIO *sbio;
std::thread *t;
void receiveThread();
void receiveTransfThread() throw(ConnectionException);
public:
TCPConnection() throw();
TCPConnection(BIO*) throw();
virtual ~TCPConnection() throw();
virtual bool connect(const std::string& ipAddr, const std::string& port) throw(ConnectionException);
virtual void close() throw();
virtual bool isLinkOnline() throw();
virtual void send(Transferable& message) throw (ConnectionException);
virtual void receive() throw(ConnectionException);
};

class UDPConnection : public Connection {
public:
UDPConnection() throw();
virtual ~UDPConnection() throw();

virtual bool connect(std::string& ipAddr, const std::string& port) throw(ConnectionException);

virtual void close() throw();

virtual bool isLinkOnline() throw();

virtual void send(Transferable& message) throw (ConnectionException);
virtual void receive() throw(ConnectionException);
};

#endif

