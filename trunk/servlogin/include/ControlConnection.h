#ifndef _CONTROL_CONNECTION_H_
#define _CONTROL_CONNECTION_H_

#include <Datagram.h>
#include <Instruction.h>
#include <Login.h>

class ControlConnectionRcvd : public Datagram<ControlConnectionRcvd> {
public:
  ControlConnectionRcvd() : Datagram<ControlConnectionRcvd>("ControlConnectionRcvd") {}
  virtual ~ControlConnectionRcvd();
  void exec(Connection*) const throw();
};

#endif