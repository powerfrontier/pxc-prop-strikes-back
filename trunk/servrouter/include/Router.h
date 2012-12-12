#ifndef _ROUTER_H_
#define _ROUTER_H_

#include <Singleton>

class Router : public Singleton<Router>, public ConnectionCallback {
	protected:
	friend class Singleton<Router>;
	Router();

	//TODO: map zone-server/connection/users
	//TODO: map user-zone/token

	public:
	virtual ~Router();

	virtual void callbackFunction(Transferable* received, Connection* c) throw();

	void sendTransferable(Transferable* t, int idZone);

	void addServer(int idServer, const std::string ip);
	void setZoneToServer(int idZone, int idServer);
	void setUserToZone(int idUser, int idZone);
	void delUser(int idUser);

};

#endif
