//
//--
//	centralControl.h
//
//	central controller
//	class
//
//	which manages train tasks 
//	in mission and plan level
//
//	Train control project
//      using FEMOS(Fast EMbedded OS) kernel 
//--
//
//      Written by INSOP SONG
//
//      Systems Design
//      
//      Michael Cole
//      Computer Science
//  
//      University of Waterloo, Canada
//  
//      June,July 2001
//--
//

extern "C" {
    #include "../include/i486/cs452.h"
    #include "../syscall/sysCall.h"
    #include "../include/util/type.h"
    #include "../train/ironBird.h"
}; // C function interface 


#ifndef	_CENTRAL_CONTROL_H_
#define _CENTRAL_CONTROL_H_

const	BYTE	MaxTrainNum = 6;
const 	BYTE	TrainNumbers[6] = { 1, 6, 12, 52, 55, 57};

class	CentralControl {
    private:
	BYTE trackId;
	BYTE numTrain;
	BYTE trainNumCreated[MaxTrainNum];
	BYTE trainTidCreated[MaxTrainNum];


    public:
	CentralControl();

	inline void setTrackId( BYTE _trkId );
	inline BYTE getTrackId( void );
	inline BYTE getMaxTrainNum( void );
	inline BYTE getTrainNum( void );
	    Boolean createTrain( BYTE _trainNum, BYTE _trainTid);
	inline int getTrainTid( BYTE _trainIndex );

		void printTrain( void );

};


inline void CentralControl::setTrackId( BYTE _trkId ) {
    trackId = _trkId;
}

inline BYTE CentralControl::getTrackId( void ) {
    return trackId;
}

inline BYTE CentralControl::getMaxTrainNum( void ) {
    return MaxTrainNum;
}

inline BYTE CentralControl::getTrainNum( void ) {
    return numTrain;
}

inline int CentralControl::getTrainTid( BYTE _trainIndex ) {
    return trainTidCreated[_trainIndex];
}

#endif // _CENTRAL_CONTROL_H_
