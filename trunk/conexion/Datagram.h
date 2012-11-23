#ifndef DATAGRAM_H
#define DATAGRAM_H

#include <cstdlib>
#include <string.h>
#include <string>
#include <typeinfo>

#include "Transferable.h"


//Datagram DOES NOT ALLOW classes with non-static members
template <class T>
class Datagram : public Transferable {
       size_t mSize;
protected:
       Datagram() : mSize(sizeof(T)) {}
public:
       class Creator : public TransferableCreator {
               public:
               virtual Transferable* create(Transferable& tr) const throw(WrongTransferableException) {
                       if (tr.size() != typeSize()) throw WrongTransferableException("Sizes are different.");
                       T* ret = (T*) NULL;
                       ret = (T*) malloc(tr.size());
                       memcpy(ret, &tr, tr.size());
                       return ret;
               }

               virtual size_t typeSize() const {
                       return sizeof(T);
               }
       };

       Transferable& transferableObject() { return (T&) *this; }

       virtual size_t size() const { return mSize; }
};

#endif
