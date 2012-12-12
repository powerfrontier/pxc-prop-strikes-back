#ifndef _DATAGRAM_H_
#define _DATAGRAM_H_

#include <cstdlib>
#include <string.h>
#include <string>
#include <typeinfo>
#include <exception>

#include <Transferable.h>

class Connection;

//Datagram DOES NOT ALLOW classes with non-static members
template <class T>
class Datagram : public Transferable {
	size_t mSize;
	std::string mType;
protected:
	Datagram(const std::string& type) 	: mSize(sizeof(T))
						, mType(type) { }
public:
	class Creator : public TransferableCreator {
		std::string mType;
		public:
		Creator(const std::string& type) : TransferableCreator(), mType(type) {}

		Transferable* create(void* orig) const throw(TransferableVersionException&) {
			//TODO: Comprobar que es del mismo tipo
			T* ret = new T();
			memcpy(((char*)ret) + sizeof(Datagram<T>), orig, typeSize());

			return ret;
		}

		size_t typeSize() const throw() {
			return sizeof(T)-sizeof(Datagram<T>);
		}

		int type() const throw(TransferableVersionException&) {
			return TransferableFactory::instance().type(mType);
		}
	};

	virtual ~Datagram() { }

	void* transferableObject() const throw() { return (void*) (((char*)this)+sizeof(Datagram<T>)); }

	size_t size() const throw() { return mSize; }

	int type() const throw (TransferableVersionException&) {
		return TransferableFactory::instance().type(mType);
	}

	virtual void exec(Connection* c) const throw() {}
};

#endif

