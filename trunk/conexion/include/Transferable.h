#ifndef _TRANSFERABLE_H_
#define _TRANSFERABLE_H_

#define TRANSFERABLE_LAST_VERSION "0.1a"

#include <map>
#include <string>
#include <typeinfo>
#include <exception>
#include <Singleton.h>

/* Exceptions */

class WrongTransferableException : public std::exception {
	std::string mWhat;
	public:
	WrongTransferableException(const std::string& what) throw();
	virtual ~WrongTransferableException() throw();
	const char* what() const throw();
};

class TransferableVersionException : public std::exception {
	std::string mWhat;
	public:
	TransferableVersionException(const std::string& what) throw();
	virtual ~TransferableVersionException() throw();
	const char* what() const throw();
};

/* Basic transferable interface */

class Transferable {
	public:

	//Returns a raw pointer to the data to be sent
	virtual void* transferableObject() const throw() = 0;

	//Returns the object's size to be sent
	virtual size_t size() const throw() = 0;

	//Returns the sending type id
	virtual int type() const throw(TransferableVersionException&) = 0;
	
	//Reimplementable method to check the transferred Instruction consistency
	virtual bool validate() const throw();
};

/* Transferable Factory creator */

class TransferableCreator {
	public:
	virtual Transferable* create(Transferable& orig) const throw(TransferableVersionException&) = 0;
	virtual size_t typeSize() const throw() = 0;
	virtual int type() const throw(TransferableVersionException&) = 0;
};

/* Factory */

class TransferableFactory : public Singleton<TransferableFactory> {
	std::string mProtocolVersion;
	std::map<int, TransferableCreator*> mCreators;
	std::map<std::string, int> mSendingType;

	friend class Singleton<TransferableFactory>;
	TransferableFactory();

	protected:
	void clear() throw();
	void addCreator(TransferableCreator& creator) throw(TransferableVersionException&);
	virtual void setLatestVersion()  throw(TransferableVersionException&);

	public:
	static const std::string NO_PROTOCOL;

	virtual ~TransferableFactory() throw();

	//Get and set for the protocol used in the communication
	const std::string& protocolVersion() const throw();
	//Any new version of the protocol (new instructions, etc) must be handled within this method
	//Calling this method will set the Factory in the given version.
	//An invalid version will throw a TransferableVersionException
	//
	// - Note: This method should read from a protocol file and add Creators
	// 	via another factory, virtual reimplementable method, etc... Kept simple for the sake of practice
	void setProtocol(const std::string& version) throw(TransferableVersionException&);

	//Method called by the sending side to send the instruction ID
	int type(const std::string& type) const throw(TransferableVersionException&);

	//Method called by the receiving side to create an executable instruction
	TransferableCreator* creator(int creatorId) const throw(TransferableVersionException&);
	
};

#endif

