#ifndef _TRANSFERABLE_H_
#define _TRANSFERABLE_H_

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

	virtual Transferable* transferableObject() const throw() = 0;
	virtual size_t size() const throw() = 0;
	virtual int sendingType() const throw(TransferableVersionException&) = 0;
	virtual int receivingType() const throw(TransferableVersionException&) = 0;
	virtual bool validate() const throw();
};

/* Transferable Factory creator */

class TransferableCreator {
	public:
	virtual Transferable* create(Transferable& orig) const throw(WrongTransferableException&) = 0;
	virtual size_t typeSize() const throw() = 0;
};

/* Factory */

class TransferableFactory : public Singleton<TransferableFactory> {
	std::string mProtocolVersion;
	std::map<int, TransferableCreator*> mCreators;
	std::map<int, std::string> mIdType;
	std::map<std::string, int> mSendingType;
	std::map<std::string, int> mReceivingType;

	friend class Singleton<TransferableFactory>;
	TransferableFactory();

	protected:
	void clear();
	void addCreator(int transferableType, TransferableCreator& creator);
	virtual void setLatestsVersion();

	public:
	static const std::string NO_PROTOCOL;

	virtual ~TransferableFactory();

	const std::string& protocolVersion() const;
	virtual void setProtocol(const std::string& version) throw(TransferableVersionException&);

	int getSendingType(const std::string& type) const throw(TransferableVersionException&);
	int getReceivingType(const std::string& type) const throw(TransferableVersionException&);

	TransferableCreator* creator(int creatorId) const throw(TransferableVersionException&);
	
};

#endif

