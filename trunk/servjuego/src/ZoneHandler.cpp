#include <ZoneHandler.h>
#include <ctime>
#include <unistd.h>

ZoneHandler::ZoneHandler(Zone& zone, ZoneCallback& game): mZone(zone)
							, mGame(game)
							, mMaxSteps(0.0)
							, mMinSteps(0.0)
							, mMinStepTime(0.0)
							, mMaxStepTime(0.0)
							, mTDBoundary(TD_DEFAULT_BOUNDARY)
							, mTDCriticalBoundary(TD_DEFAULT_CRITICAL_BOUNDARY)
							, mTDDelay(0.0)
							, mTDDecreaser(TD_DEFAULT_DECREASER)
							, mAvgStepTime(0.0)
							, mLoad(0.0)
							, mRun(false) {
	
}

ZoneHandler::~ZoneHandler() {
	run(false);
	if (mGame) {
		mGame->dispose();
		delete mGame;
	}
}

void		ZoneHandler::sendTDBalanceCall(double delay) {
	//TODO: IMPLEMENT
}

void		ZoneHandler::sendTDCriticalCall(double delay) {
	//TODO: IMPLEMENT
}

bool		ZoneHandler::run() const {
	//TODO: MUTEX
	return mRun;
}
void		ZoneHandler::run(bool r) {
	//TODO: MUTEX
	mRun=run;
}

void		ZoneHandler::saveStats(int idZone, double stepTime) {
	//TODO: IMPLEMENT
}

ZoneHandler& ZoneHandler::zone (Zone* z) {
	//TODO: IMPPLEMENT
	
	return *this;
}

ZoneHandler& ZoneHandler::minSteps (double steps) {
	//TODO: MUTEX
	mMinSteps = steps;
	return *this;
}
ZoneHandler& ZoneHandler::maxSteps (double steps) {
	//TODO: MUTEX
	mMaxSteps = steps;
	return *this;
}
ZoneHandler& ZoneHandler::timeDilationBoundary (double loadPercentage) {
	//TODO: MUTEX	
	mTDBoundary = loadPercentage;
	return *this;
}
ZoneHandler& ZoneHandler::timeDilationCriticalBoundary (double loadPercentage) {
	//TODO: MUTEX
	mTDCriticalBoundary = loadPercentage;
	return *this;
}
ZoneHandler& ZoneHandler::timeDilationDecreaser (double decreaser) {
	//TODO: MUTEX
	mTDDecreaser = decreaser;
	return *this;
}

double ZoneHandler::minSteps() const {
	//TODO: MUTEX
	return mMinSteps;
}
double ZoneHandler::maxSteps() const {
	//TODO: MUTEX
	return mMaxSteps;
}
double ZoneHandler::timeDilation () const {
	//TODO: MUTEX
	return mTDDelay;
}
double ZoneHandler::timeDilationBoundary () const {
	//TODO: MUTEX
	return mTDBoundary;
}
double ZoneHandler::timeDilationDecreaser () const {
	//TODO: Mutex
	return mTDDecreaser;
}

void ZoneHandler::start() {
	time_t startTime, endTime;
	double stepTime = 0.0;

	int stepsPerSec = 0;
	double timeTillSec = 0.0;
	double avgStepTime = 0.0;

	//Init the zone callback (a.k.a. game)
	if (!mGame || !mZone || !mGame->init(mZone)) return;
	run(true);
	
	//Start main loop
	time(&startTime);
	while(run() && mGame->step(stepTime * mTDDelay)) {
		time(&endTime);
		stepTime = difftime(end, start);

		++stepsPerSec;
		timeTillSec += stepTime;

		//In case we are running faster than allowed, nop a little
		if (stepTime < mMinStepTime) {
			usleep((unsigned int)(mInStepTime - stepTime)*1000);
		}

		//Calcculate load every second
		if (timeTillSec >= 1.0) {
			//Calculate load
			avgStepTime = timeTillSec / stepsPerSec;
			//Load is compared with step time max and min boundaries
			mLoad = (avgStepTime - mMinStepTime)/(mMaxStepTime - mMinStepTime);

			mLoad = mLoad > 1.0 ? 1.0 : mLoad; //Load can't be negative
			mLoad = mLoad < 0.0 ? 0.0 : mLoad; //Load can't be more than 100%

			//Check load boundaries
			if (mLoad > mTDBoundary) {
			//Start Time Dilation
				//Increase time dilation by decreasing the time multiplier
				mTDDelay *= mTDDecreaser;
				
				//Send stress call to balance server
				if (mTDDelay > mTDCriticalBoundary) sendTDBalanceCall(mTDDelay);
				else sendTDCriticalCall();
			} 
			
			//If we're on time dilation, but the load is below boundary, stop time dilation
			else if (mTDDelay < 1.0 && mLoad < mTDBoundary) mTDDelay = 1.0;

			//Save statistics
			saveStats(mZone->id(), avgStepTime);

			//Reset counters
			timeTillSec = 0.0;
			stepsPerSec = 0;
		}

		time(&startTime);
	}

	mGame->dispose();
}

void ZoneHandler::stop() {
	run(false);
}

void ZoneHandler::doBackup() {

}

