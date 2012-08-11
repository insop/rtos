//
//--
//
//	centralControl.cc
//
//	central controller
//	class
//
//	which manages train tasks 
//	in mission and plan level
//
//	Train control project
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
//--
//


#include "centralControl.h"


CentralControl::CentralControl(void) {
    //cprintf("Central Control created ");
    numTrain = 0;
}

BYTE CentralControl::getTrainNum( void ) {
    return numTrain;
}

BYTE CentralControl::getTrainNoFromIndex(BYTE _index)  {
    return trainNumCreated[_index];
}

Boolean CentralControl::createTrain( BYTE _trainNum, 
				BYTE _trainTid, BYTE _lastSensor) {
    if(numTrain < MaxTrainNum ) {
	trainTidCreated[numTrain] = _trainTid;
	trainNumCreated[numTrain] = _trainNum;
	lastTriggeredSensor[numTrain] = _lastSensor;
	numTrain++;
	return True;
    } // if
    else {
	printf("Warning !! No more train can be created \n");
	return False;
    } // else

}
BYTE CentralControl::getTrainNumFromTid( int _trainTid ) {

    BYTE trainNo = 0;

    for(int i = 0; i< MaxTrainNum; i++) {

	if(_trainTid == trainTidCreated[i]) {

	    trainNo = trainNumCreated[i]; 
	    break;
	} // if
    } // for

    if(i == MaxTrainNum) {
	cprintf("Warning!!! getTrainNum error\n");
    } // if 

    return trainNo;
}

int CentralControl::getTrainTIDfromNo( BYTE _trainNum ) {

    int trainTid = 0xff;

    for(int i = 0; i< MaxTrainNum; i++) {
	if(_trainNum == trainNumCreated[i]) {

	    trainTid = trainTidCreated[i]; 
	    break;
	} // if
    } // for

    if(i == MaxTrainNum) {
	cprintf("Warning!!! getTrainTid error\n");
    } // if

    return trainTid;
}


BYTE CentralControl::getLastSensorFromNo( BYTE _trainNum ) {

    BYTE sensor = 0xff;

    for(int i = 0; i< MaxTrainNum; i++) {

	if(_trainNum == trainNumCreated[i]) {

	    sensor = lastTriggeredSensor[i]; 
	    break;

	} // if

    } // for

    if(i == MaxTrainNum) {
	cprintf("Warning!!! getLastTriggered error t[%d]\n", _trainNum);
    } // if
    return sensor;
}


Boolean CentralControl::isTrainTid( int _tid) {

    for(int i = 0;i<MaxTrainNum;i++) {

	if(_tid == trainTidCreated[i]) {

	    return True;

	} // if

    } // for



    return False;
}

void CentralControl::setLastSensorFromNo( BYTE _trainNum, BYTE lastSensor ) {


    for(int i = 0; i< MaxTrainNum; i++) {

	if(_trainNum == trainNumCreated[i]) {

	    lastTriggeredSensor[i] = lastSensor;

	    break;

	} // if

    } // for

    if(i == MaxTrainNum) {

	cprintf("Warning!!! setLastSensor t[%d]\n", _trainNum);

    } // if
}



void CentralControl::printTrain( void ) {

    register i;

    printf("Created train task list....\n");
    printf("----------------------------------------\n");

    for(i = 0; i< numTrain; i++) {
      printf("[%d] TRAIN Num %2d tid: %2d  selected and created\n",
	  i, trainNumCreated[i], trainTidCreated[i]);
    } // if

    printf("----------------------------------------\n");
}

