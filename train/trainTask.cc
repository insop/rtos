//
//--
//
//	FEMOS(Fast EMbedded OS) kernel main  
//
//	Train Task class
//
//
// 	Written by 
//
//	INSOP SONG
//
// 	Systems Design
//	
//	Michael Cole Computer Science
//
// 	University of Waterloo, Canada
//
//	June,July 2001
//--
//


#include "trainTask.h"

const char *strTrainSpeedType[] 	= { "Normal", "Express" };
const char *strTrainType[] 		= { "Passenger", "Cargo", "Zamboni" };
const char *strTrainShowType[] 	= { "Leader", "Wingman", "Solo" };
const char *strSensorType[]  	= { "Not_mine", "Behind_two", "Behind_one", \
"Ahead_two_tail","Ahead_one_tail","Mine","Ahead_two_head","Ahead_one_head" }; 

const int speedTable52[SpeedRange] = 
{   0,		// 0
    0,  	// 1
    17,		// 2
    30,		// 3
    54,		// 4
    70,		// 5 
    103,	// 6
    132,	// 7
    158,	// 8
    197,	// 9
    240,	// 10
    273,	// 11
    332,	// 12
    371,	// 13
    429,	// 14
};

const int speedTable55[SpeedRange] = 
{
    0,		// 0
    0,		// 1
    52,		// 2  
    90,		// 3
    133,	// 4
    160,	// 5
    204,	// 6
    241,	// 7
    268,	// 8
    275,	// 9
    301,	// 10
    311,	// 11
    315,	// 12
    329,	// 13
    360,	// 14
};

//
// XXX this is temporary table

#define some_number	
const int speedTable57[SpeedRange] = 
{   0,		// 0
    0,  	// 1
    17,		// 2
    30,		// 3
    54,		// 4
    70,		// 5 
    103,	// 6
    132,	// 7
    158,	// 8
    197,	// 9
    240,	// 10
    273,	// 11
    332,	// 12
    371,	// 13
    429,	// 14
};

//
// XXX this is temporary table

const int speedTable1[SpeedRange] = 
{   0,		// 0
    0,  	// 1
    17,		// 2
    30,		// 3
    54,		// 4
    70,		// 5 
    103,	// 6
    132,	// 7
    158,	// 8
    197,	// 9
    240,	// 10
    273,	// 11
    332,	// 12
    371,	// 13
    429,	// 14
};

//
// XXX this is temporary table

const int speedTable6[SpeedRange] = 
{   0,		// 0
    0,  	// 1
    17,		// 2
    30,		// 3
    54,		// 4
    70,		// 5 
    103,	// 6
    132,	// 7
    158,	// 8
    197,	// 9
    240,	// 10
    273,	// 11
    332,	// 12
    371,	// 13
    429,	// 14
};

//
// XXX this is temporary table

const int speedTable12[SpeedRange] = 
{   0,		// 0
    0,  	// 1
    17,		// 2
    30,		// 3
    54,		// 4
    70,		// 5 
    103,	// 6
    132,	// 7
    158,	// 8
    197,	// 9
    240,	// 10
    273,	// 11
    332,	// 12
    371,	// 13
    429,	// 14
};
TrainTask::TrainTask(void) {

    speedType      = Normal;
    trainType      = Cargo;
    trainShowType  = Leader;

    started 	   = False;
    finished 	   = False;
    missionStarted = False;

    normalOperation = True;
    emergency 	    = False;
    timeOut 	    = False;
    deRailed	    = False;

}

TrainTask::TrainTask(int _trainNo) {
    trainNum = _trainNo;

}

// 
// set comand sets
//
//	return: size of the commands
//
int  TrainTask::setCommandSet( oneHop * _steps) {

    register BYTE i = 0;

    // reset command index
    indexCommandSet = 0;

// XXX XXX I can still get last sensor

    for(i = 0; i < NUMSEGMENTS; i++) {
	if(_steps[i].distance == NIL) {
	    break;
	}
	else {
	    stepsToFollow[i] = _steps[i];
	} // else
    } // for
    numCommandSet = i;
    return i;
}

//
// set train number and
// set speed table
//
void TrainTask::setTrainNumber(BYTE _trainNum) {
    register int i;
    trainNum = _trainNum;

    for( i = 0; i < SpeedRange ; i++) {
	if( _trainNum == 52 ) {
	    speedTable[i] = speedTable52[i];
	} // if
	else if( _trainNum == 55 ) {
	    speedTable[i] = speedTable55[i];
	}  // else if
	else if( _trainNum == 57 ) {
	    speedTable[i] = speedTable57[i];
	} // else if 
	else if( _trainNum == 1 ) {
	    speedTable[i] = speedTable1[i];
	} // else if 
	else if( _trainNum == 6 ) {
	    speedTable[i] = speedTable6[i];
	} // else if 
	else if( _trainNum == 12 ) {
	    speedTable[i] = speedTable12[i];
	} // else if 
    } // for
}

//
// check the sensor is start position
//	e.g. terminal sensor(b10,b12,b8,a12)
//
Boolean TrainTask::isStartPosition( BYTE _sensor ) {

    // is this sensor one of the start position
    if( _sensor == SENSOR('b', 10) ||
	_sensor == SENSOR('b', 12) ||
	_sensor == SENSOR('b',  8) ||
	//_sensor == SENSOR('a', 12) ||

	//_sensor == SENSOR('a', 15) ||
	//_sensor == SENSOR('a', 14) ||
	_sensor == SENSOR('a',  2) )  {

	return True;
    }
    else 
	return False;
}

//
// check the sensor is couter part of start position
//	e.g. terminal sensor(a5,a8,a10,c7...)
//
Boolean TrainTask::isCounterStartPosition( BYTE _sensor ) {

    // is this sensor one of the start position
    if( _sensor == SENSOR('a',  5) ||
	_sensor == SENSOR('a',  8) ||
	_sensor == SENSOR('a', 10) ||
	_sensor == SENSOR('c',  7) ||

	_sensor == SENSOR('c', 13) )  {

	return True;
    }
    else 
	return False;
}
// temp sensor size for back up
#define TEMP_SENSOR_SIZE	10
//
// set train at initial position
//
BYTE TrainTask::goStartPosition( void ) {

    register BYTE ch;
    register BYTE lastTriggeredSensor;
    register Boolean finished = False;
    register Boolean directionChanged = False;
    
    // start the train
    startTrain();
    //decreaseSpeed();

    while( !finished ) {

	// asking sensor srv for sensor data 
	ch = FROM_TRAIN_TASK;
	Send(SensorSrv, &ch, sizeof(char), 
		    &sensorData, TEMP_SENSOR_SIZE * sizeof(char));

	//XXX test
	//cprintf("<<%x>>", sensorData[0]);
	// if terminal sensors triggered
	if( isStartPosition( sensorData[0] )) {
	    lastTriggeredSensor = sensorData[0];
	    // stop train
	    stopTrain();

	    //printf(">>>>Backed up at start  %c%d", 
		//SEN_MODULE_CHAR(sensorData[0]),
		//SEN_NUMBER(sensorData[0]));

//#undef NO_CHANGE_DIR
//#ifdef NO_CHANGE_DIR
	    changeDirection();
	    //Delay(4);

	    // set start position of the train 
	    startTrain();

	    do {
		//Delay(1);
		ch = FROM_TRAIN_TASK;
		Send(SensorSrv, &ch, sizeof(char), 
			    &sensorData, TEMP_SENSOR_SIZE * sizeof(char));

	    // XXX XXX XXX
	    //} while( sensorData[0] != 0);
	    } while( sensorData[0] != 0xff);

	    Delay( 3 );
	    stopTrain();
//#endif //NO_CHANGE_DIR

	    // return last triggered data
	    //return lastTriggeredSensor; 
	    return lastTriggeredSensor-1; 
	} // if

	// is counter part of start position
	else if( isCounterStartPosition( sensorData[0] ) &&
		    !directionChanged) {
	    printf(">>>>Backed up counter start at  %c%d", 
		SEN_MODULE_CHAR(sensorData[0]),
		SEN_NUMBER(sensorData[0]));

	    // XXX got some back and forth problem ...

	    // stop train
	    stopTrain();
	    Delay(4);

	    // change the direction
	    changeDirection();
	    Delay(4);

	    // start the train again
	    startTrain();
	    //decreaseSpeed();
	    directionChanged = True;
	} // else if 

	Delay(2);

    } // while
}
// information print of itself
void TrainTask::printTrain( void ) {

    printf("---------------------------------------------------\n");
    printf("Train No: %d  SpeedType: %s  TrainType: %s\n",
	trainNum,strTrainSpeedType[speedType],strTrainType[trainType]);
    printf("---------------------------------------------------\n");

}

//
// printf the command set with index
//
// XXX XXX XXX
#undef NOT_INCLUDE_THIS
#ifdef NOT_INCLUDE_THIS
void  TrainTask::printCommandSet( oneHop * _oneHop ) {

    register char innerIndex;
	
    if( _oneHop->distance != NIL ) {

	printf(" dir:%2d dist: %2d sen[%c:%d] >> ",
	    _oneHop->direction,
	    _oneHop->distance,
	    _oneHop->thisNode.module + 'a', 
	    _oneHop->thisNode.sensorNum 
	); // cprintf

	innerIndex = 0;

	while(
	  _oneHop->instlist[innerIndex].sequence != -1) {
	    printf(" [SW %d TYPE %d] ", 
		_oneHop->instlist[innerIndex].sequence,
		_oneHop->instlist[innerIndex].setting);
	    innerIndex++;
	} // inner while

	printf("\n");
    } // if
    else 
	printf("\nTRAIN TASK CLASS:: invalid Index[%d]\n", indexCommandSet);

    
}
#endif // NOT_INCLUDE_THIS
//
// printf the command set with index
//
// XXX XXX XXX
void  TrainTask::printCommandSet( BYTE _indexCommand ) {

    register char innerIndex;
	
    if( _indexCommand < numCommandSet) {
	printf("\nTRAIN TASK CLASS:: command[%d]\n", indexCommandSet);

	printf("[%2d]: dir:%2d dist: %2d sen[%c:%d] >> ",
	    _indexCommand, stepsToFollow[_indexCommand].direction,
	    stepsToFollow[_indexCommand].distance,
	    stepsToFollow[_indexCommand].thisNode.module + 'a', 
	    stepsToFollow[_indexCommand].thisNode.sensorNum 
	); // cprintf

	innerIndex = 0;

	while(
	  stepsToFollow[_indexCommand].instlist[innerIndex].sequence != -1) {
	    printf(" [SW %d TYPE %d] ", 
		stepsToFollow[_indexCommand].instlist[innerIndex].sequence,
		stepsToFollow[_indexCommand].instlist[innerIndex].setting);
	    innerIndex++;
	} // inner while

	printf("\n");
    } // if
    else 
	printf("\nTRAIN TASK CLASS:: invalid Index[%d]\n", indexCommandSet);

    
}

//
// printf the whole command sets
//
void  TrainTask::printCommandSet( void ) {

    register char index;
    register char innerIndex;
	
    printf("\nTRAIN TASK CLASS:: command set from CC\n");

    for(index = 0; index < numCommandSet; index++) {

	printf("[%2d]: dir:%2d dist: %2d sen[%c:%d] >> ",
	    index, stepsToFollow[index].direction,
	    stepsToFollow[index].distance,
	    stepsToFollow[index].thisNode.module + 'a', 
	    stepsToFollow[index].thisNode.sensorNum 
	);

	innerIndex = 0;

	while(((stepsToFollow[index].instlist[innerIndex].sequence) != NIL) 
	    && (innerIndex < NUMSWITCH) )
	{
	    printf(" [SW %x TYPE %x] ", 
		stepsToFollow[index].instlist[innerIndex].sequence,
		stepsToFollow[index].instlist[innerIndex].setting);
	    innerIndex++;
	} // inner while

	printf("\n");
    } // for
    
}

