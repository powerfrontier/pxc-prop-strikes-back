#include <Instruction.h>
#include <Zone.h>

#define TD_DEFAULT_BOUNDARY 0.75
#define TD_DEFAULT_CRITICAL_BOOUNDARY 0.9
#define TD_DEFAULT_DECREASER 0.9

class ZoneCallback {
	public:
	ZoneCallback(Zone*);
	virtual ~ZoneCallback();	

	virtual bool init(Zone* zone) = 0;
	virtual bool step (double stepTime) = 0;
	virtual void dispose () = 0;

	virtual saveZone() const;
	Zone*	zone() const;
	virtual void addInstruction (Instruction* ins) = 0;
};

class ZoneHandler {
	ZoneCallback* 	mGame;

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
	void		sendTDBalanceCall(double delay);
	void		sendTDCriticalCall(double delay);
	bool		run() const;
	void		run(bool);
	void		saveStats(int idZone, double stepTime);
public:
	ZoneHandler (ZoneCallback* game, Zone* zone);
	virtual ~ZoneHandler();
	
	ZoneHandler& zone (Zone*);

	ZoneHandler& minSteps (double steps);
	ZoneHandler& maxSteps (double steps);
	ZoneHandler& timeDilationBoundary (double loadPercentage);
	ZoneHandler& timeDilationCriticalBoundary (double loadPercentage);
	ZoneHandler& timeDilationDecreaser (double decreaser);

	double minSteps() const;
	double maxSteps() const;
	double timeDilation () const;
	double timeDilationBoundary () const;
	double timeDilationDecreaser () const;

	void start ();
	void stop ();
	void doBackup ();

	// Returns a number from 0 to 1 with the current load
	// With 0 equals no load and 1 equals max load
	double getLoad () const;
};
