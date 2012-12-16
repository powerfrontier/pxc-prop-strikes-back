#ifndef _CLIENT_CLOSED_CONNECTION_H_
#define _CLIENT_CLOSED_CONNECTION_H_

#include <Datagram.h>


class ClientClosedConnection : public ConnectionClosedListener {  
public:
  void callOnClose(Connection*) throw(); 
  
};



#endif