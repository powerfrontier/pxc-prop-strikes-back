#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>
#include <set>
#include <queue>
#include <thread>

#include <SDL/SDL.h>

#include <Connection.h>
#include <Singleton.h>


class Zone {
protected:
	int mZoneId;
	std::set<int> mUsers;
	
public:
	Zone(int id);
	virtual ~Zone();
	
	int id() const;
	const std::set<int>& users() const;

	bool hasUser(int idUser) const;
	virtual void addUser(int idUser) = 0;
	virtual void delUser(int idUser) = 0;
};

class ClientGame {
	std::queue<std::pair<Instruction*, Connection*> > mInstructions;

	protected:
	Zone* mZone;
	virtual bool gameStep(double stepTime) throw() = 0;
	virtual void execInstructions() throw();

	public:
	ClientGame(Zone* zone);
	virtual ~ClientGame();	

	virtual bool init() = 0;
	bool step (double stepTime) throw();

	Zone* zone() const;
	virtual void addInstruction (Instruction* ins, Connection* c) throw();
};

class GameCreator {
public:
	virtual ClientGame* create(int zoneId) = 0;
};



class Client : public Singleton<Client> {
public:
	class ClientLoginListener : public ConnectionCallback {
	private:
	friend class Client;
	ClientLoginListener();
	~ClientLoginListener();

	public:
	void callbackFunction(Transferable* received, Connection*) throw();
	};
	
	class ClientRouterListener  : public ConnectionCallback {
	private:
	friend class Client;
	ClientRouterListener();
	~ClientRouterListener();

	public:
	void callbackFunction(Transferable* received, Connection*) throw();
	};

	ClientLoginListener* LOGIN_LISTENER;
	ClientRouterListener* ROUTER_LISTENER;

private:
	static GameCreator* sGameCreator;
protected:
	friend class Singleton<Client>;
	Client();

	ClientGame* mGame;

	int mUserId;
	int mToken;
	std::string mUser;
	std::string mPassword;

	std::string mLoginIp;
	std::string mLoginPort;

	Connection* mLogin;
	Connection* mRouter;
	
	SDL_Surface* mSurface;
	
	std::mutex mGameMutex;
	
	bool mLoggedIn;
	bool mBadLogOn;
	bool mInit;

	bool init();
	void go();
public:
	virtual ~Client();

	void id(int);
	int id() const;
	
	void token(int);
	int token() const;

	void setLoginAddress(const std::string& ip, const std::string& port);
	bool login(const std::string& user, const std::string& password);
	void login(bool right);

	void routerConnection(Connection*);

	void setZone(int idZone);
	
	ClientGame* game();
	
	void addInstruction(Instruction*, Connection*) throw();

	void logout();
	void sendAction(Transferable*);

};

#endif

