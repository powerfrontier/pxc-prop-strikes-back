#include "Connection.h"

Connection::Connection() throw() : mCallback(NULL), mIsOpen(false) {}

Connection::~Connection() throw(ConnectionException){
	mCallback = NULL;
}

void Connection::setCallbackFunction(ConnectionCallback* t) throw(){
	mCallback = t;
}
