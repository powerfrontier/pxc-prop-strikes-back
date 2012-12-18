#include <Client.h>
#include <iostream>
#include <ctime>

#include <InstLogin.h>
#include <InstRouter.h>

Zone::Zone(int id) : mZoneId(id), mUsers() {
	
}

Zone::~Zone() {
	
}

int Zone::id() const { return mZoneId; }
const std::set<int>& Zone::users() const { return mUsers; }

bool Zone::hasUser(int idUser) const {
	auto it = mUsers.find(idUser);
	
	return it != mUsers.end();
}


void ClientGame::execInstructions() throw() {
	std::pair<Instruction*, Connection*> ins;
	while(!mInstructions.empty()) {
		//Exec every pending instruction and delete ir afterwards
		ins = mInstructions.front();
		ins.first->exec(ins.second);
		delete ins.first;

		mInstructions.pop();
	}
}

ClientGame::ClientGame(Zone* zone) : mInstructions(), mZone(zone) { }

ClientGame::~ClientGame() {
	if (mZone) delete mZone;
}

bool ClientGame::step (double stepTime) throw() {
	bool valid = true;
	valid = gameStep(stepTime);
	if (valid) execInstructions();

	return valid;
}

Zone* ClientGame::zone() const {
	return mZone;
}

void ClientGame::addInstruction (Instruction* ins, Connection* c) throw() {
	if (ins) mInstructions.push(std::pair<Instruction*, Connection*>(ins, c));
}





Client::ClientLoginListener::ClientLoginListener() : ConnectionCallback() { }
Client::ClientLoginListener::~ClientLoginListener() { }

void Client::ClientLoginListener::callbackFunction(Transferable* received, Connection*) throw() {

}

Client::ClientRouterListener::ClientRouterListener() : ConnectionCallback() { }
Client::ClientRouterListener::~ClientRouterListener() { }

void Client::ClientRouterListener::callbackFunction(Transferable* received, Connection*) throw() {

}





Client::Client() 	: Singleton<Client>()
			, LOGIN_LISTENER(new Client::ClientLoginListener())
			, ROUTER_LISTENER(new Client::ClientRouterListener())
			, mGame(NULL)
			, mUserId(-1)
			, mToken(-1)
			, mUser()
			, mPassword()
			, mLoginIp()
			, mLoginPort()
			, mLogin(NULL)
			, mRouter(NULL)
			, mSurface(NULL)
			, mGameMutex()
			, mIsOnLogin(false)
			, mLoggedIn(false)
			, mBadLogOn(false)
			, mInit(false) {

}


Client::~Client() {
	if (LOGIN_LISTENER) delete LOGIN_LISTENER;
	if (ROUTER_LISTENER) delete ROUTER_LISTENER;
	if (mGame) delete mGame;
	if (mLogin) delete mLogin;
	if (mRouter) delete mRouter;
	
	if (mInit) {
		SDL_Quit();
	}
}

void Client::id(int id) {
	mUserId = id;
}

int Client::id() const {
	return mUserId;
}

void Client::token(int token) {
	mToken = token;
}

int Client::token() const {
	return mToken;
}

void Client::setLoginAddress(const std::string& ip, const std::string& port) {
	mLoginIp = ip;
	mLoginPort = port;
	mLogin = new TCPConnectionSecurity();
	
	if (!mLogin->connect(mLoginIp, mLoginPort)) {
		delete mLogin;
		mLogin = NULL;
	}
}

bool Client::login(const std::string& user, const std::string& password) {
	time_t startTime, endTime;
	double connectTime = 0.0;
	
	if (!mLogin) return false;
	LoginRequestSend* login = new LoginRequestSend(user, password);
	
	isOnLogin(true);
	
	mLogin->send(*login);
	delete login;
	
	
	time(&startTime);
	while(isOnLogin()) { 
		//Wait for ConnectClient to work
		time(&endTime);
		connectTime = difftime(endTime, startTime);
		
		if (connectTime > 10.0) {
			std::cerr << "Timeout" << std::endl;
			isOnLogin(false);
			
			return false;
		}
	}if (mBadLogOn) {
		std::cerr << "Wrong username/password!" << std::endl;
		return false;
	}
	else if (init()) {
		mUser = user;
		mPassword = password;
		
		mLoggedIn = true;
		
		go();
		return true;
	} else {
		std::cerr << "Unable to init game" << std::endl;
		return false;
	}
}

void Client::correctLogin(bool right) {
	mBadLogOn = !right;
	isOnLogin(false);
}

void Client::isOnLogin(bool b) {
	mIsOnLogin = b;
}

bool Client::isOnLogin() const {
	return mIsOnLogin;
}

bool Client::isLoggedIn() const {
	return mLoggedIn;
}

bool Client::init() {
	if (mGame) {
		if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
			return false;
		}
		mSurface = SDL_SetVideoMode( 600, 600, 32, SDL_HWSURFACE );
		if (!mSurface) {
			std::cerr << SDL_GetError() << std::endl;
			return false; 
		}
		
		mInit = true;
		return mGame->init();
	}
	return false;
}

void Client::go() {
	time_t startTime, endTime;
	double stepTime = 0.0;
	
	time(&startTime);
	while(mLoggedIn && mInit) {
		mGameMutex.lock();
		mGame->step(stepTime);
		mGameMutex.unlock();
		
		time(&endTime);
		stepTime = difftime(endTime, startTime);
		
		time(&startTime);
	}
}


void Client::routerConnection(Connection* c) {
	if (mRouter) {
		mRouter->close();
		delete mRouter;
	}
	mRouter = c;
}

void Client::setZone(int idZone) {
	ClientGame* newGame = NULL;
	ClientGame* oldGame = mGame;
	
	if (!mGame) return;
	
	newGame = sGameCreator->create(idZone);
	newGame->init();
	mGameMutex.lock();
	mGame = newGame;
	mGameMutex.unlock();
	delete oldGame;
	
}

ClientGame* Client::game() {
	return mGame;
}

void Client::addInstruction(Instruction* inst, Connection* c) throw() {
	if (mGame) mGame->addInstruction(inst, c);
	else {
		inst->exec(c);
		delete inst;
	}
}

void Client::sendAction(Transferable* tr) {
	if (mRouter) mRouter->send(*tr);
}
