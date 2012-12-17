#include <Client.h>

ClientLoginListener::ClientLoginListener() : ConnectionCallback() { }
ClientLoginListener::~ClientLoginListener() { }

void ClientLoginListener::callbackFunction(Transferable* received, Connection*) throw() {

}

ClientRouterListener::ClientRouterListener() : ConnectionCallback() { }
ClientRouterListener::~ClientRouterListener() { }

void ClientRouterListener::callbackFunction(Transferable* received, Connection*) throw() {

}



Client::Client() 	: CONTROL_LISTENER(new ClientControlListener())
			, ROUTER_LISTENER(new ClientRouterListener())
			, mGame(NULL)
			, mUserId(-1)
			, mUser()
			, mPassword()
			, mLoginIp()
			, mLoginPort()
			, mRouterIp()
			, mRouterPort()
			, mLogin(NULL)
			, mRouter(NULL)
			, mSurface(NULL)
			, mLoggedIn(false)
			, mBadLogOn(false)
			, mInit(false) {

}


Client::~Client() {
	if (CONTROL_LISTENER) delete CONTROL_LISTENER;
	if (ROUTER_LISTENER) delete ROUTER_LISTENER;
	if (mGame) delete mGame;
	if (mLogin) delete mLogin;
	if (mRouter) delete mRouter);
	
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

void Client::setLoginAddress(const std::string& ip, const std::string& port) {
	mLoginIp = ip;
	mLoginPort = port;
}

bool Client::login(const std::string& user, const std::string& password) {
	//TODO: Conectar
	//TODO: InstLogin
	while(!mLoggedIn || !mBadLogOn) { 
		//TODO: TimeOut
	}
	if (!mBadLogOn && init()) {
		go();
		return true;
	} else {
		std::cerr << "Unable to init game" << std::endl;
		return false;
	}
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
	while(mLoggedIn && mInit) {
		mGame->step();
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
	//TODO
	ClientGame* newGame = 
}

Zone* Client::zone() {
	return mGame ? mGame->zone() : NULL;
}

void Client::addInstruction(Instruction* inst, Connection* c) throw() {
	//TODO: if (mGame) mGame->addInstruction(inst, c);
}

void Client::sendAction(Transferable* tr) {
	if (mRouter) mRouter->send(*tr);
}
