#ifndef _ROUTER_H_
#define _ROUTER_H_

#include <Connection.h>
#include <Singleton.h>
#include <thread>

class Router : public Singleton<Router> {
	
	public:
	class ControlListener : public ConnectionCallback {
		friend class Router;
		ControlListener();
		~ControlListener();
		public:
		void callbackFunction(Transferable* received, Connection* c) throw();
	};
	
	class ServerListener : public ConnectionCallback {
		friend class Router;
		ServerListener();
		~ServerListener();
		public:
		void callbackFunction(Transferable* received, Connection* c) throw();
	};
	
	class ClientListener : public ConnectionCallback {
		friend class Router;
		ClientListener();
		~ClientListener();
		public:
		void callbackFunction(Transferable* received, Connection* c) throw();
	};
	
	ControlListener CONTROL_LISTENER;
	ServerListener SERVER_LISTENER;
	ClientListener CLIENT_LISTENER;
	
	protected:
	class User {
		std::mutex mUserMutex;
		int mId;
		int mToken;
		
		int mZone;
		Connection* mConnection;
		
	public:
		User(int id, int token);
		virtual ~User();
		
		int id() const;
		
		int token() const;
		User* token(int);
		bool validate(int);
		
		void zone(int);
		int zone() const;
		
		void connection(Connection* c);
		Connection* connection() const;
		
		void send(Transferable*);
	};
	
	class Zone {
		std::mutex mZoneMutex;
		int mId;
		int mServer;
		int mNewServer;
		std::map<int, User*> mUsers;
		bool mInTransfer;
		
		public:
		Zone(int id);
		
		int id() const;
		
		int server() const;
		void server(int);
		
		bool inTransfer() const;
		void endTransfer();

		int newServer() const;
		void newServer(int);
		
		void addUser(User*);
		void delUser(int);
		bool hasUser(int);
		
		void sendToUsers(Transferable*);
	};
	
	class Server {
		std::mutex mServerMutex;
		int mId;
		std::map<int, Zone*> mZones;
		Connection* mConnection;
		
		public:
		Server (int idServer);
		virtual ~Server();
		
		int id() const;
		
		void addZone(Zone*);
		void delZone(int);
		bool hasZone(int) const;
		
		void connection(Connection*);
		Connection* connection() const;
		
		void send(Transferable*);
		void sendToUsers(Transferable*);
		void sendToUsersInZone(int idZone, Transferable* t);
	};
	
	friend class Singleton<Router>;
	Router();

	std::mutex mRouterMutex;
	std::map<int, Server*> 	mServers;
	std::map<int, Zone*> 	mZones;
	std::map<int, User*> 	mUsers;

	void clear();
	
	public:
	virtual ~Router();

	void addServer(int idServer);
	void connectServer(int idServer, Connection*);
	void delServer(int idServer);
	
	void addZone(int idZone);
	void delZone(int idZone);
	
	void startZoneToServerTransfer(int idZone, int newServer);
	void endZoneToServerTransfer(int idZone);
	
	
	void addUser(int idUser, int token);
	void delUser(int idUser);
	bool validateUser(int idUser, int token);
	
	void connectUser(int idUser, int token, Connection*);
	void setUserToZone(int idUser, int idZone);
	
	
	void sendToServer(int idServer, Transferable*);
	void sendToServerUsers(int idServer, Transferable*);
	void sendToZone(int idZone, Transferable*);
	void sendToUser(int idUser, Transferable*);
};

#endif
