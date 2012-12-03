#ifndef _ZONE_H_
#define _ZONE_H_

#include <set>
#include <thread>

#include <Instruction.h>

#define TD_DEFAULT_BOUNDARY 0.75
#define TD_DEFAULT_CRITICAL_BOUNDARY 0.9
#define TD_DEFAULT_DECREASER 0.9


class Zone {
	int mZoneId;
	std::set<int> mUsers;
	
public:
	Zone(int id);
	virtual ~Zone();
	
	int id() const;
	const std::set<int>& users() const;
	bool hasUser(int idUser) const;
	
	virtual void* dbData() const = 0;
};

class ZoneCallback {
	public:
	ZoneCallback(int idZone);
	virtual ~ZoneCallback();	

	virtual bool init() = 0;
	virtual bool step (double stepTime) throw() = 0;
	virtual void dispose () = 0;

	const Zone* zone() const;
	virtual void zone(Zone*) = 0;
	virtual void addInstruction (Instruction* ins) = 0;
};

class ZoneCallbackCreator {
public:
	virtual ZoneCallback* create(int zoneId) = 0;
};

class ZoneHandler {
	static ZoneCallbackCreator* sGameCreator;
	
	ZoneCallback* 	mGame;

	std::thread*	mRunThread;
	std::mutex	mGameMutex;
	std::mutex	mParamMutex;
	
	
	//Time control boundaries
	double		mMaxSteps;	//Max steps by second
	double		mMinSteps;	//Min steps by second

	double		mMinStepTime;	//Set by mMaxSteps
	double		mMaxStepTime;	//Set by mMinSteps

	double		mTDBoundary;	//Load percentage for TimeDilation to start. Accepted threshold: 0.6 - 1.0
	double		mTDCriticalBoundary; //Load percentage for critical rebalance to be called. 

	//Time dilation percentage (0 - 1)
	double		mTDDelay;	//Delay multiplier
	double		mTDDecreaser;	//Decreaser to multiply the delay with

	//Statistics
	double 		mAvgStepTime;
	double		mLoad;

	bool		mRun;

protected:
	void		sendStressCall() throw();
	bool		run() throw();
	void		run(bool) throw();
	void		startThread() throw();
	void		saveStats(double avgStepTime) throw();
public:
	static void setGameCreator(ZoneCallbackCreator*);
	
	
	ZoneHandler (int zoneId) throw();
	virtual ~ZoneHandler() throw();
	
	const Zone* zone() throw();
	
	ZoneHandler& minSteps (double steps) throw();
	ZoneHandler& maxSteps (double steps) throw();
	ZoneHandler& timeDilationBoundary (double loadPercentage) throw();
	ZoneHandler& timeDilationCriticalBoundary (double loadPercentage) throw();
	ZoneHandler& timeDilationDecreaser (double decreaser) throw();

	double minSteps() throw();
	double maxSteps() throw();
	double timeDilation () throw();
	double timeDilationBoundary () throw();
	double timeDilationDecreaser () throw();

	void start () throw();
	void stop () throw();
	void doBackup () throw();
	void addInstruction(Instruction*, bool gameInstruction) throw();
	
	// Returns a number from 0 to 1 with the current load
	// With 0 equals no load and 1 equals max load
	double getLoad () throw();
	
	
};

#endif
