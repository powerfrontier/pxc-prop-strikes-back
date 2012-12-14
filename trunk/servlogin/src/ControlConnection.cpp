#include <ControlConnection.h>
#include <Login.h>
#include <iostream>
#include <NewClient.h>

using namespace std;

void ControlConnectionRcvd::exec(Connection* c) const throw(){
  Login::instance().controlConnection = c;
}

ControlConnectionRcvd::~ControlConnectionRcvd(){

	
}

