#include <PXCGame.h>
#include <iostream>

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
	SDL_Event ev;
	while( SDL_PollEvent( &ev ) ){
		switch( ev.type ){
			case SDL_MOUSEBUTTONDOWN:
				//TODO: Mouse
			break;
			case SDL_KEYDOWN:
				//TODO: Keyboard
			break;
			default:
			break;
		}
	}
}

bool PXCGame::init() {
	return initDrawWindow();
}

void PXCGame::setUserState(int idUser, double pos[3], double dir[3], double speed) {
	
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


