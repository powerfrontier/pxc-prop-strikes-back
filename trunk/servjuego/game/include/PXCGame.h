#ifndef _PXC_GAME_H_
#define _PXC_GAME_H_

#include <Zone.h>
#include <map>

class PXCGame;

class PXCZone : public Zone {
	friend class PXCGame;
	public:
	struct User {
		double mPosition[3];
		double mDirection[3];
		double mDestination[3];
		double mSpeed;

		User(double x, double y, double z, double dx, double dy, double dz);
		void update(double step);
		void setDestination(double x, double y, double z);
		void normalizeDirection();
	};

private:
	std::map<int, User*> mUserData;
public:
	PXCZone(int id);
	virtual ~PXCZone();

	virtual void addUser(int idUser);
	virtual void delUser(int idUser);
};

class PXCGame : public ZoneCallback {
	protected:
	virtual bool gameStep (double stepTime) throw();
	virtual void sendState() throw();
	
	public:
	PXCGame(PXCZone* zone);
	virtual ~PXCGame();	

	virtual bool init();
};

class PXCGameCreator : public ZoneCallbackCreator {
public:
	virtual ZoneCallback* create(int zoneId);
};


#endif

