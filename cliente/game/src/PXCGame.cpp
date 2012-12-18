#include <PXCGame.h>
#include <iostream>
#include <InstServer.h>

GameCreator* Client::sGameCreator = new PXCGameCreator();

PXCGame::PXCGame(PXCZone* zone) : ClientGame(zone), mSurface(NULL) {

}

PXCGame::~PXCGame() {
	
}
	
bool PXCGame::initDrawWindow() {
	mSurface = SDL_GetVideoSurface();
	if (!mSurface) return false;
	SDL_WM_SetCaption( "PXC: The Game", NULL ); 
	return true;
}

void PXCGame::logic(double stepTime) {
	PXCZone* z = static_cast<PXCZone*>(mZone);
	auto it = z->mUserData.begin();

	while (it != z->mUserData.end()) {
		it->second->update(stepTime);
		++it;
	}
}

void PXCGame::draw() const {
	PXCZone* z = static_cast<PXCZone*>(mZone);
	PXCZone::User* usr = NULL;
	SDL_Rect cleanRect;
	SDL_Rect user;
	
	if (!mSurface) return;
	
	auto it = z->mUserData.begin();
	
	cleanRect.x = 0;
	cleanRect.y = 0;
	cleanRect.w = 600;
	cleanRect.h = 600;
	
	user.w = 10;
	user.h = 10;
	
	SDL_FillRect(mSurface, &cleanRect, 0);
	
	while (it != z->mUserData.end()) {
		usr = it->second;
		
		user.x = usr->mPosition[0]-5;
		user.y = usr->mPosition[1]-5;
		
		SDL_FillRect(mSurface, &user, -1);
		
		++it;
	}
}

void PXCGame::input() {
	int id = Client::instance().id();
	int token = Client::instance().token();
	SDL_Event ev;
	int32_t x = -1;
	int32_t y = -1;
	SetDestinationSend* sendDest = NULL;
	SetZoneSend* sendZone = NULL;
	
	while( SDL_PollEvent( &ev ) ){
		switch( ev.type ){
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&x, &y);
				sendDest = new SetDestinationSend(id, token, x, y, 0.0);
				Client::instance().sendAction(sendDest);
				delete sendDest;
			break;
			case SDL_KEYDOWN:
				switch (ev.keysim) { //ESTOY VAGO
					case SDLK_0:
						x = 0;
						break;
					case SDLK_1:
						x = 1;
						break;
					case SDLK_2:
						x = 2;
						break;
					case SDLK_3:
						x = 3;
						break;
					case SDLK_4:
						x = 4;
						break;
					case SDLK_5:
						x = 5;
						break;
					case SDLK_6:
						x = 6;
						break;
					case SDLK_7:
						x = 7;
						break;
					case SDLK_8:
						x = 8;
						break;
					case SDLK_9:
						x = 9;
						break;
					break;
				}
				if (x != -1) {
					sendZone = new SetZoneSend(id, token, x);
					Client::instance().sendAction(sendZone);
					delete sendZone;
				}
			break;
			default:
			break;
		}
	}
}

bool PXCGame::init() {
	return initDrawWindow();
}

void PXCGame::setUserState(int idUser, const double pos[3], const double dir[3], double speed) {
	PXCZone* z = static_cast<PXCZone*>(mZone);
	auto it = z->mUserData.find(idUser);
	
	if (it != mUserData.end()) {
		it->second->mPosition[0] = pos[0];
		it->second->mPosition[1] = pos[1];
		it->second->mPosition[2] = pos[2];
		it->second->mDirection[0] = dir[0];
		it->second->mDirection[1] = dir[1];
		it->second->mDirection[2] = dir[2];
		it->second->mSpeed = speed;
		
	} else {
		PXCZone::User* usr = new PXCZone::User(pos[0], pos[1], pos[2], dir[0], dir[0], dir[0]);
		usr->mSpeed = speed;
	}
}

//Dummy game logic: move things to their directions
bool PXCGame::gameStep (double stepTime) throw() {
	logic(stepTime);
	draw();
	input();
	
	return true;
}


ClientGame* PXCGameCreator::create(int zoneId) {
	PXCZone* z = new PXCZone(zoneId);
	PXCGame* g = new PXCGame(z);

	return g;
}


