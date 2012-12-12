#include <PXCGame.h>

PXCZone::User::User(double x, double y, double z, double dx, double dy, double dz) 	: mPosition{x, y, z}
											, mDirection{dx, dy, dz}
											, mDestination{0.0, 0.0, 0.0}
											, mSpeed(0) {
	normalizeDirection();
}

void PXCZone::User::update(double step) {
	double dest[3] = { 	mPosition[0]+mDirection[0]*mSpeed*step, 
				mPosition[1]+mDirection[1]*mSpeed*step, 
				mPosition[2]+mDirection[2]*mSpeed*step };

	//TODO: Check if we've arrived at destination
}

void PXCZone::User::setDestination(double x, double y, double z) {
	mDestination[0] = x;
	mDestination[1] = y;
	mDestination[2] = z;

	//TODO: Set direction
	normalizeDirection();
}

void PXCZone::User::normalizeDirection() {
	//TODO
}

PXCZone::PXCZone(int id) : Zone(id) {

}

PXCZone::~PXCZone() {
	auto it = mUserData.begin();

	while (it != mUserData.end()) {
		delete it->second;
		++it;
	}
}

void PXCZone::addUser(int idUser) {
	User* u = NULL;
	if (!hasUser(idUser)) {
		mUsers.insert(idUser);
		//TODO: Retrieve data
		u = new User(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
		mUserData[idUser] = u;
	}
}

void PXCZone::delUser(int idUser) {
	if (hasUser(idUser)) {
		auto it = mUserData.find(idUser);
		//TODO: Save data

		mUsers.erase(idUser);
		delete it->second;
		mUserData.erase(it);
	}
}

