#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>

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
	virtual void sendState() throw() = 0;

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

	ClientLoginListener* CONTROL_LISTENER;
	ClientRouterListener* ROUTER_LISTENER;

private:
	static GameCreator* sGameCreator;
protected:
	friend class Singleton<Client>;
	Client();

	ClientGame* mGame;

	int mUserId;
	std::string mUser;
	std::string mPassword;

	std::string mLoginIp;
	std::string mLoginPort;

	std::string mRouterIp;
	std::string mRouterPort;

	Connection* mLogin;
	Connection* mRouter;
	bool mLoggedIn;


public:
	virtual ~Client();

	void id(int);
	int id() const;

	void setLoginAddress(const std::string& ip, const std::string& port);
	bool login(const std::string& user, const std::string& password);

	void routerConnection(Connection*);

	void setZone(int idZone);
	Zone* zone();

	void addInstruction(Instruction*, Connection*) throw();

	void logout();
	void sendAction(Transferable*);

};

#endif

