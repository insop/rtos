//
//--
//
//	blackboard class
//
//	store global data and do some
//	arbitray decision for the client task(train)
//	
//      using FEMOS(Fast EMbedded OS) kernel 
//
//--
//
//      Written by 
//
//	INSOP SONG
//
//      Systems Design
//      
//      Michael Cole Computer Science
//  
//      University of Waterloo, Canada
//  
//      June,July 2001
//
//--
//


#ifndef	_BLACKBOARD_H_
#define _BLACKBOARD_H_

extern "C" {
    #include "../syscall/sysCall.h"
    #include "../train/ironBird.h"
    #include "../include/i486/cs452.h"
}; // C function interface

#include "../buffer_server/communicateData.h"

// A, B, C, D, E
const BYTE SensorModuleNumber	= 5;

// 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
const BYTE SensorNumber		= 16;

class	BlackBoard {

    private:
	// XXX
	// I need sensor arrary
	// with train number
	// with track condition... derail, construction, normal
	// array for train log, using UpTime() report.
	// train time and position. status
	// i.e.
	// sensor[], train[][], train_time[][], train_pos[][]
	// if the index is end just round up...


    // communication data object it has packing and printing 
    // 	and other utilities
	CommunicateData	aCD;


	checkIn[SensorModuleNumber][SensorNumber];

	BYTE	senModuleIndex;
	BYTE	senNumberIndex;
	// time[SensorModuleNumber][SensorNumber];
	// distance[SensorModuleNumber][SensorNumber];



    public:
	BlackBoard();
	// registerTrain with number or priority...
	//
	// registerTrain(train number, train priority, condition...)
	//
	// nodeReadyCheck()

	// inline Boolean checkInNode(BYTE trainNo, BYTE sensorNode);
	// checkOutNode(BYTE trainNo, BYTE sensorNode);
	// approveCheckIn();
	//	reply with this as approve the checke in request
	// disapproveCheckIn();
	//	reply with this as disapproval the checke in request

	// commandSlowDown();
	// commandSpeedUp();
	// commandStopNwait();
	// commandBackUp();

	// updatePostion( train number);
	// 	inside method based on checked in request

	// void printNode();
	// inline void reportNode();
	// ?

	// inline void setNodeDerail();
	// inline void setNodeConstruction();
	// inline void resetNodeDerail();
	// inline void resetNodeConstruction();

	// setSwitchCurve(number);
	// ? why I made these???
	// setSwitchStright(number);

	// inline Boolean isHeadToHead();
	// inline Boolean isHeadToTail();
	
	// void recordLog( train number );
	// void printLog( train number );

	// inline void reportSensorToCentralControl();
	// inline void reportTrainToCentrailCOntrol( train number );


	inline void initBlackBoard();
	void print();


};

#endif	//_BLACKBOARD_H_
