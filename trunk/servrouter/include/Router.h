#ifndef _ROUTER_H_
#define _ROUTER_H_

#include <Singleton>

class Router : public Singleton<Router>, public ConnectionCallback {
	protected:
	friend class Singleton<Router>;
	Router();

	//TODO: map user-token?

	//TODO: map zone-server

	public:
	virtual ~Router();

	virtual void callbackFunction(Transferable* received, Connection* c) throw();

	void sendTransferable(Transferable* t, int idZone);
};

#endif
