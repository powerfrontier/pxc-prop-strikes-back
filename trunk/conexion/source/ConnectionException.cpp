#include "Connection.h"
ConnectionException::ConnectionException(const std::string& what) throw() : mWhat(what) { }
ConnectionException::~ConnectionException() throw() { }
const char* ConnectionException::what() const throw() { return mWhat.c_str(); }



