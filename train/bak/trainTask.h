//
//--
//	FEMOS(Fast EMbedded OS) kernel main  
//
//	Train Task class
//
//
// 	Written by INSOP SONG
//
// 	Systems Design
//	
//	Michael Cole
//	Computer Science
//
// 	University of Waterloo, Canada
//
//	June,July 2001
//--
//

#ifndef	_TRAIN_TASK_H_
#define _TRAIN_TASK_H_

extern "C" {
    #include "../include/i486/cs452.h"
    #include "../syscall/sysCall.h"
    // train application definitions
    #include "../train/ironBird.h"
}; // C function interface

#include "../include/util/errorCode.h"
#include "../include/util/type.h"



class TrainTask {

    private:

	BYTE	trainNum;

	BYTE	commandSize;
	BYTE	commandIndex;

	BYTE	speedType; // express or normal 
	BYTE	trainType; // passenger or carge  
	BYTE	trainShowType; // leader or wingman

	Boolean started;
	Boolean finished;
	Boolean normalOperation;
	Boolean emergency;
	Boolean timeOut;
	Boolean deRailed;

	int	currentPosition;
	int	nextPosition;

	int	runningTime;
	int	segmentTime;

	int 	speed;

	BYTE	inputSensorNum;

    public:

	TrainTask();
	TrainTask(int _trainNo);

	void initTrain(BYTE _trainNo);

	inline Boolean  isFinished( void );
	inline Boolean  isStarted( void );
	inline Boolean  isNormalOperation( void );
	inline Boolean  isEmergency( void );
	inline Boolean  isTimeOut( void );
	inline Boolean	isRightPosition( void );
	inline Boolean	isDeRailed( void );

	inline int 	getCommandNext( void );
	inline int 	getSensorNext( void );

	inline void 	setSpeed( void );
	inline void 	setSwitch( void );
	inline void	startTrain( void );
	inline void 	stopTrain( void );

	inline void	increaseSpeed( void );
	inline void	decreaseSpeed( void );

	inline void 	setSegmentTime( int _segTime );
	inline int 	getSegmentTime( void );

	inline void 	getCommandSet( void );
	inline void 	getSensorSet( void );

	inline void 	sendReport( void );

	inline void	reportPosition( void );
	inline void	reportTime( void );
	inline void	reportStatus( void );
};


inline void TrainTask::initTrain(BYTE _trainNum) {
    trainNum = _trainNum;
}



#endif // _TRAIN_TASK_H_
