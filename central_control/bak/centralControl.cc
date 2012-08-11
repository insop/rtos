//
//--
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


#include "centralControl.h"


CentralControl::CentralControl(void) {
    //cprintf("Central Control created ");
    numTrain = 0;
}

Boolean CentralControl::createTrain( BYTE _trainNum, BYTE _trainTid ) {
    if(numTrain < MaxTrainNum ) {
	trainTidCreated[numTrain] = _trainTid;
	trainNumCreated[numTrain] = _trainNum;
	numTrain++;
	return True;
    }
    else {
	printf("Warning !! No more train can be created \n");
	return False;
    }

}

void CentralControl::printTrain( void ) {
    register i;
    printf("Created train task list....\n");
    printf("----------------------------------------\n");
    for(i = 0; i< numTrain; i++) {
      printf("[%d] TRAIN Num %2d tid: %2d  selected and created\n",
	  i, trainNumCreated[i], trainTidCreated[i]);
    }
    printf("----------------------------------------\n");
}

