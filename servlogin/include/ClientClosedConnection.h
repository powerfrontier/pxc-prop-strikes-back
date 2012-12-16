#ifndef _CLIENT_DISCONNECT_CONTROL_H_
#define _CLIENT_DISCONNECT_H_

#include <Datagram.h>


class ClientClosedConnection : public ConnectionClosedListener {  
public:
  void CallOnClose(Connection*) throw(); 
  
};



#endif