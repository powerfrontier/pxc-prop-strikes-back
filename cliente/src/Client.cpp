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
	mLogin->send(*login);
	delete login;
	
	
	time(&startTime);
	while(!mLoggedIn || !mBadLogOn) { 
		//Wait for ConnectClient to work
		time(&endTime);
		connectTime = difftime(endTime, startTime);
		
		if (connectTime > 10.0) {
			std::cerr << "Timeout" << std::endl;
			mLogin->close();
			delete mLogin;
			return;
		}
	}
	if (!mBadLogOn && init()) {
		mUser = user;
		mPassword = password;
		
		go();
		return true;
	} else {
		std::cerr << "Unable to init game" << std::endl;
		return false;
	}
}

void Client::login(bool right) {
	mLoggedIn = right;
	mBadLogOn = !right;
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
		mGame->step(stepTime);
		time(&endTime);
		stepTime = difftime(endTime, startTime);
		
		time(&startTime);
	}
}

void Client::logout() {
	mLoggedIn = false;
}

void Client::routerConnection(Connection* c) {
	if (mRouter) {
		mRouter->close();
// 		delete mRouter;
	}
	mRouter = c;
}

void Client::setZone(int idZone) {
	ClientGame* newGame = NULL;
	if (!mGame) return;
	
	newGame = sGameCreator->create(idZone);
	//TODO
	//ClientGame* newGame = 
}

ClientGame* Client::game() {
	return mGame;
}

void Client::addInstruction(Instruction* inst, Connection* c) throw() {
	//TODO: if (mGame) mGame->addInstruction(inst, c);
}

void Client::sendAction(Transferable* tr) {
	if (mRouter) mRouter->send(*tr);
}
