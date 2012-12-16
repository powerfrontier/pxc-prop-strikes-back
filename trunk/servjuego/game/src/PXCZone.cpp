#include <PXCGame.h>
#include <cmath>

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
	double length = 0.0;
	double maxLength = 0.0;
	
	length = sqrt	( dest[0] - mPosition[0] 
			+ dest[1] - mPosition[1]
			+ dest[2] - mPosition[2] );
	maxLength = sqrt( mDestination[0] - mPosition[0]
			+ mDestination[1] - mPosition[1]
			+ mDestination[2] - mPosition[2] );
	
	//If we've gone further than our destination
	if (length > maxLength) {
		mPosition[0] = mDestination[0];
		mPosition[1] = mDestination[1];
		mPosition[2] = mDestination[2];
	} else {
		mPosition[0] = dest[0];
		mPosition[1] = dest[1];
		mPosition[2] = dest[2];
	}
}

void PXCZone::User::setDestination(double x, double y, double z) {
	//Set destination
	mDestination[0] = x;
	mDestination[1] = y;
	mDestination[2] = z;

	//Set direction
	mDirection[0]=mDestination[0] - mPosition[0];
	mDirection[1]=mDestination[1] - mPosition[1];
	mDirection[2]=mDestination[2] - mPosition[2];
	normalizeDirection();
}

void PXCZone::User::normalizeDirection() {
	double length = 0.0;
	
	length = sqrt	( mDestination[0] * mDestination[0] 
			+ mDestination[1] * mDestination[1] 
			+ mDestination[2] * mDestination[2] );
	
	mDestination[0] /= length;
	mDestination[1] /= length;
	mDestination[2] /= length;
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

