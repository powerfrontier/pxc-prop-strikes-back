#include <PXCGame.h>
#include <iostream>

GameCreator* Client::sGameCreator = new PXCGameCreator();

PXCGame::PXCGame(PXCZone* zone) : ClientGame(zone) {

}

PXCGame::~PXCGame() {
	
}	

bool PXCGame::init() {
	std::cout << "Zona " << mZone->id() << " iniciada." << std::endl;
	return true;
}

//Dummy game logic: move things to their directions
bool PXCGame::gameStep (double stepTime) throw() {
	PXCZone* z = static_cast<PXCZone*>(mZone);
	auto it = z->mUserData.begin();

	while (it != z->mUserData.end()) {
		it->second->update(stepTime);
		++it;
	}

	return true;
}

void PXCGame::sendState() throw() {
	//TODO: Estado de los usuarios
	//GameServer::instance().sendToClients();
}

ClientGame* PXCGameCreator::create(int zoneId) {
	PXCZone* z = new PXCZone(zoneId);
	PXCGame* g = new PXCGame(z);

	return g;
}


