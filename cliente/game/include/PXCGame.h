#ifndef _PXC_GAME_H_
#define _PXC_GAME_H_

#include <SDL.h>

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

class PXCGame : public ClientGame {
	SDL_Surface* mSurface;
	protected:
	virtual void sendAction();

	void initDrawWindow();
	void draw() const;

	virtual bool gameStep (double stepTime) throw();
	
	public:
	PXCGame(PXCZone* zone);
	virtual ~PXCGame();	

	virtual bool init();
	virtual void setState(int idUser, double pos[3], double dir[3], double speed);
};

class PXCGameCreator : public ZoneCallbackCreator {
public:
	virtual ZoneCallback* create(int zoneId);
};


#endif

