#include <Transferable.h>

WrongTransferableException::WrongTransferableException(const std::string& what) throw() : mWhat(what) { }
WrongTransferableException::~WrongTransferableException() throw() { }
const char* WrongTransferableException::what() const throw() { return mWhat.c_str(); }

TransferableVersionException::TransferableVersionException(const std::string& what) throw() : mWhat(what) { }
TransferableVersionException::~TransferableVersionException() throw() { }
const char* TransferableVersionException::what() const throw() { return mWhat.c_str(); }

bool Transferable::validate() const throw() { return true; }
Transferable::~Transferable() { }

