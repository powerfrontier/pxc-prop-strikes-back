#include <PXCGame.h>

PXCGame::PXCGame(PXCZone* zone) : ZoneCallback(zone) {

}

PXCGame::~PXCGame() {

}	

bool PXCGame::init() {
	return true;
}

//Dummy game logic: move things to their directions
bool PXCGame::gameStep (double stepTime) throw() {
	auto it = mZone->mUserData.begin();

	while (it != mUserData.end()) {
		it->second->update(stepTime);
		++it;
	}

	return true;
}

void PXCGame::sendState() throw() {
	//TODO: Send status to all connected clients
}

ZoneCallback* PXCGameCreator::create(int zoneId) {
	PXCZone* z = new PXCZone(zoneId);
	PXCGame* g = new PXCGame(z);

	return g;
}


