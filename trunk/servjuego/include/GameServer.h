#ifndef _GAME_SERVER_H_
#define _GAME_SERVER_H_

#include <set>

#include <Zone.h>
#include <Singleton.h>

class GameServer : public Singleton<GameServer>, public ConnectionCallback {
	struct ZoneHandlerComparer {
		bool operator()(const ZoneHandler*& lhs, const ZoneHandler*& rhs) {
			return lhs->zone()->id() < rhs->zone()->id();
		}
	};
	
	std::set<ZoneHandler*, ZoneHandlerComparer> mZones;

protected:
	bool hasZone(int zoneId) const;

	friend class Singleton<GameServer>;
	GameServer();
	
public:
	virtual ~GameServer();
	
	virtual void callbackFunction(Transferable* received, Connection*) throw();
	
	//Start managing a zone from DB
	void startZone(int zoneId);
	
	//Totally stop managing a zone, destroying all its related objects
	void stopZone(int zoneId);
	
	//Receiving order from balance to set a zone as detachable for given server
	void markForDetach(int zoneId, int idServer);
	
	//Receiving order from balance to connect with given server to retrieve the zone
	void receiveZone(int zoneId, Connection* oldServer);
	
	//Start cloning a zone to a new server, sending its current Zone data to the new server
	//And sending zone's events to the new server untill all data is copied.
	void detachZone(int zoneId, Connection* newServer);

	
	
	
};



#endif