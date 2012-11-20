#include <string>
#include <Datagram.h>


struct ConnectionCallback {
ConnectionCallback();
virtual ~ConnectionCallback();

virtual void callbackFunction(Datagram* received) throw() = 0;
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
ConnectionCallback* mCallback;
bool	mIsOpen;
std::string mIp;
protected:

virtual bool openConnection() throw (ConnectionException) = 0;
public:
Connection() throw();
virtual ~Connection() throw(ConnectionException);

//Set the objective ip and try to connect to 
bool connect(std::string& ipAddr) throw(ConnectionException);
virtual void close() throw() = 0;

virtual bool isLinkOnline() throw() = 0;

virtual void send(const Datagram& message) throw (ConnectionException) = 0;

virtual Datagram* receive() throw(ConnectionException) = 0;

void setCallbackFunction(ConnectionCallback*) throw();
};

class TCPConnection : public Connection {
public:
TCPConnection() throw();
virtual ~TCPConnection() throw();

virtual bool connect(std::string& ipAddr) throw(ConnectionException);

virtual void close() throw();

virtual bool isLinkOnline() throw();

virtual void send(const Datagram& message) throw (ConnectionException);
virtual Datagram* receive() throw(ConnectionException);
};

class UDPConnection : public Connection {
public:
UDPConnection() throw();
virtual ~UDPConnection() throw();

virtual bool connect(std::string& ipAddr) throw(ConnectionException);

virtual void close() throw();

virtual bool isLinkOnline() throw();

virtual void send(const Datagram& message) throw (ConnectionException);
virtual Datagram* receive() throw(ConnectionException);
};
