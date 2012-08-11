//
//--
//
//	trainTaskMain.cc
//
//	TrainTask main file
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

#include "trainTask.h"
#include "../buffer_server/communicateData.h"
// old command set run routine
#undef	OLD_COMMAND_RUN
#define BlackBoard
#define START_POSITION

extern "C" {
    #include "../include/i486/cs452.h"
    #include "../include/i486/stdlib.h"
    #include "../syscall/sysCall.h"
    // train application definitions
    #include "../train/ironBird.h"
    #include "../pathfind/graphtype.h"
}; // C function interface

#include "../include/util/errorCode.h"
#include "../include/util/type.h"
    //--
    //  Global Objects
    //--
TrainTask	aTrain;
CommunicateData aCommData;
    //--
    //  Global Variables
    //--
unsigned char sensorData[11];
BYTE LastTriggeredSensor = 0;
int Tid_centralControl;
int Tid_blackBoard;
int Tid_BB_distributor;
int Tid_BB_collector;
// serial message write
char strWrite[100];
// serial message read
char strRead[100];
char dataToBlackboard[3];
// command set data from Central control
CommandSet cmdSet[MAX_COMMAND_SET];
CommandSet *pCmdSet = (aTrain.aCommandSet);
// command set btw BlackBoard
CommData Data;

void speedTest( void );

//
// get sensor data from sensor server
//
inline void getSensorData_fromSensorServer( void ) 
{
    char ch;
    // asking sensor srv for sensor data 
    ch = FROM_TRAIN_TASK;
    Send(SensorSrv, &ch, sizeof(char), &sensorData, 10 * sizeof(char));
}

//
// get sensor data from sensor server
//
inline void getCommandSet_fromCentralControl( BYTE _lastSensor ) 
{
    int tid;
    char index;
    char innerIndex;
// for command set
    oneHop stepsToFollow[NUMSEGMENTS];

// getting command set from central control
    tid = Receive(&stepsToFollow, NUMSEGMENTS * sizeof(oneHop));
    Reply(tid, NULL,NULL);
    int i = aTrain.setCommandSet( stepsToFollow );
    sprintf(strWrite, "SET COMMAND AND SIZE IS %d\n", i);
    Write(strWrite, strLen(strWrite), WYSE);
    aTrain.printCommandSet();
}

//
// test function for communicating with the distributor
//
distributorTest() 
{
    CommData	response;
    char	inputChar;
    int		readLength = 0;

    sprintf(strWrite, "[train, distributortest()]\n\r");
    Write(strWrite, strLen(strWrite), WYSE);
    while(1) {
	sprintf(strWrite,"[>train, send to distributor for asking data]\n\r");
	Write(strWrite, strLen(strWrite), WYSE);
	response = aCommData.getData();
	// send distributor asking the message from BB per se
	Send(Tid_BB_distributor, &response, sizeof(response),
	    &response, sizeof(response));
	sprintf(strWrite, "\n[<train [%d] get data from distributor....]\n\r",
	    aTrain.getTrainNumber());
	Write(strWrite, strLen(strWrite), WYSE);
	aCommData.setData( response );
	sprintf(strWrite, "\n[Read processing to ....]\n\r");
	Write(strWrite, strLen(strWrite), WYSE);
	readLength = Read(&strRead, WYSE);
	cprintf("[TT] : read data is [%s] len %d\n", strRead, readLength);
	Delay(10);
    } // while
}

//
// XXX WARNING this function is not workable
// 
int toGoDistance = 0;
int lastArrow = 0;

void displayDistance(int _toGoDistance, int currentDistance) 
{
    int ratioDist;
    int i;

    if( toGoDistance != _toGoDistance) {
	ratioDist = toGoDistance / 100;
	if(ratioDist>0) {
	    for(i =0;i< ratioDist;i++) {
		console_xy(i,10);
		cprintf(" ");
	    }
	    toGoDistance = _toGoDistance;
	    ratioDist = toGoDistance / 100;
	    for(i =0;i< ratioDist;i++) {
		console_xy(i,10);
		cprintf("-");
	    }
	}
    }
    ratioDist = currentDistance / 100;
    if(ratioDist > 0) {
	console_xy(ratioDist, 11);
	cprintf("^");
    }
    if(lastArrow >= 0) {
	console_xy(lastArrow, 11);
	cprintf(" ");
    }
    lastArrow = ratioDist;
}

//
//---
//
// runTrain()
//
// train running program
//
//--
//
void runTrain(void) 
{
    // index for sensor data
    char index;
    unsigned char checkSen;
    int triggeredTime, oldTime, timeDifference;
    int currentDist = 0;
    // start time
    int startTime;
    // end time
    int endTime;
    // run = end - start
    int runTime, distanceToGo;
    int toGoDist, distDiffPercent, distDiff, result;
    Boolean sensorBingo = False;
    Boolean sensorChecked = False;
    Boolean directionChanged = False;
    Boolean reverseChecked = False;
    // to see if the the sensor range
    Boolean myRange = False;

    strWrite = "Train Task Running ...\n\r";
    Write( strWrite, strLen(strWrite), WYSE);

    while(!aTrain.isFinished()) { // if the task is not finished
	getSensorData_fromSensorServer();
	if( !aTrain.isStarted( ) ) { // if train is not started
	    sprintf(strWrite,"\n\r>>> start train\n\r");
	    Write( strWrite, strLen(strWrite), WYSE);
#ifdef BlackBoard
// need to check two ahead route(sensors) before start
	    checkSen = aTrain.getNextSensor();
	    if( checkSen != EndSensorSignal) {
		Data.sensor = checkSen;
		Data.type = Approval_CheckIn;
		// ask blackboard
		Send(Tid_blackBoard, 
		    &Data,sizeof(Data),&result,sizeof(result));

		if(result !=1) {
		    aTrain.stopTrain();
		    sprintf(strWrite, 
			" >xxxx checkd 0x%x result %d    \n\r",
			checkSen, result);
		    Write( strWrite, strLen(strWrite), WYSE);
		    Delay(100);
		}
		else {
		    sprintf(strWrite, 
			" > checkd 0x%x result %d    \n\r",
			checkSen, result);
		    Write( strWrite, strLen(strWrite), WYSE);
		}
	    }

	    checkSen = aTrain.get2ndNextSensor();
	    if( checkSen != EndSensorSignal) {
		Data.sensor = checkSen;
		Data.type = Approval_CheckIn;
		// ask blackboard
		Send(Tid_blackBoard, 
		    &Data,sizeof(Data),&result,sizeof(result));

		if(result !=1) {
		    aTrain.stopTrain();
		    sprintf(strWrite, 
			" >xxxx checkd 0x%x result %d    \n\r",
			checkSen, result);
		    Write( strWrite, strLen(strWrite), WYSE);
		    Delay(100);
		}
		else {
		    sprintf(strWrite, 
			" > checkd 0x%x result %d    \n\r",
			checkSen, result);
		    Write( strWrite, strLen(strWrite), WYSE);
		}
	    }

	    // XXX XXX 
#endif //BlackBoard

	    // if there is switch command then set the switch
	    if(aTrain.hasSwitchCommand( )) {
		aTrain.setCommandSwitch( );
	    } // if

	    // if the first parity is different from last triggerd 
	    // sensor parity then change direction
	    // set the next switch before leave if it is necessary
	    if(aTrain.hasNextSwitchCommand()){
		aTrain.setNextCommandSwitch();
	    } // if

	    // set the current switch before leave if it is necessary
	    if(aTrain.hasSwitchCommand()) {
		aTrain.setCommandSwitch();
	    } // if

	    sprintf(strWrite, "last sensor %x first one %x and dir %d\n\r",
		LastTriggeredSensor, aTrain.getFirstSensor(),
		aTrain.getDirection());
	    Write( strWrite, strLen(strWrite), WYSE);


	    // if need to change the direction at the beginning
	    if( aTrain.getDirection() == -1 ) {
		sprintf(strWrite, "Parity need to be changed\n\r");
		Write( strWrite, strLen(strWrite), WYSE);
		aTrain.changeDirection();
		// for safety factor delay ??
		Delay(20);
	    } // if
	    // if forward
	    else {
		aTrain.changeDirection(); 	Delay(10);
		aTrain.startTrain(); 		Delay(20);
		aTrain.stopTrain(); 		Delay(10);
		aTrain.changeDirection(); 	Delay(10);
	    }
	    // check start flag, means started the mission
	    aTrain.setStarted();
	    // for checking time difference
	    startTime = endTime = oldTime = UpTime();
	    toGoDist = aTrain.distanceToGoCurrent();
	    // reset distance
	    aTrain.resetDistance( ); 
	    // start train 
	    aTrain.startTrain( );
	}  // if 
	else { // if train is started

// XXX need to check XXX XXX XXX
	    //if( sensorData[0] == 0 ) { // if no sensor comes
	    if( sensorData[0] == 0xff) { // if no sensor comes
		// XXX I need to clear this things....
		if( reverseChecked == True ) {
		    directionChanged = False;
		    reverseChecked = False;
		} //if 
		// no sensors and no matched sensor comes in
		sensorBingo = False;
		// update distance only there is no sensor data comes in
		endTime = UpTime();
		// run time, difference of the time. i.e. delta_t
		runTime = endTime - startTime;
		startTime = endTime;
		currentDist = aTrain.updateDistance( runTime );
		// distance check
		toGoDist = aTrain.distanceToGoCurrent();
		distDiffPercent = 
		    100 * (toGoDist - aTrain.getCurrentDistance())/toGoDist;
		// make sure the sensor is in my range
		if( currentDist > (toGoDist*0.8))  {
		    myRange = True;
		} // if
		// check calculated dist is over the real distance
		if( currentDist > toGoDist )  {
		    distDiffPercent = 
			100 * (toGoDist - aTrain.getCurrentDistance())/toGoDist;
		} // if check calculated dist is over the real distance
		// XXX I need to do this with certain time delay...
		// if I do this too frequent
		if(aTrain.needSlowDown() ) {
		    //&& aTrain.canChangeSpeed(endTime)) {
		    // put break
		    //aTrain.decreaseSpeed();
		    //cprintf("BREAKING>>>>  ");
		    aTrain.setSpeed(SpeedOperationLowest);
		    aTrain.setChangeSpeedTime( endTime );
		} // if
		else if ( aTrain.canChangeSpeed(endTime)){
		    // put accerlaton pedal
		    aTrain.increaseSpeed();
		} // else
	    } // if 
	    else { // XXX if the sensor data is come
		// ignore next sensor trigger
		// after direction change
		if( (directionChanged == True) ){
		    reverseChecked = True;
		    aTrain.resetDistance();
		}
		// XXX need to do some check multiple sensors
		else if( (sensorData[0] == aTrain.getNextSensor() ||
		    sensorData[1] == aTrain.getNextSensor() ||
		    sensorData[2] == aTrain.getNextSensor() || 
		    sensorData[3] == aTrain.getNextSensor() || 
		    aTrain.getNextSensor() == (BYTE)EndSensorSignal) && 
		    //timeToCheckSensor == True &&
		    sensorBingo != True && myRange)
		{
		// XXX I need to report somewhere here with BB XXX XXX 
		    // XXX or I may do this in class XXX
		    // reset my range;
		    myRange = False;
		    // for next mission run
		    LastTriggeredSensor = sensorData[0];
		    // means that waited sensor comes in
		    sensorBingo = True;
#ifdef BlackBoard
//need to check out the sensor
		    Data.sensor = LastTriggeredSensor;
		    Data.type = Approval_CheckOut;
		    // ask blackboard
		    Send(Tid_blackBoard, 
			&Data,sizeof(Data),&result,sizeof(result));
		    sprintf(strWrite, 
			" < checkd OUT 0x%x result %d    \n\r",
			LastTriggeredSensor, result);
		    Write( strWrite, strLen(strWrite), WYSE);

#endif //BlackBoard
		    // XXX for checking time difference
		    // maybe used by GetSmallTime()
		    // to check exact timer in 
		    triggeredTime = UpTime();
		    timeDifference = triggeredTime - oldTime;
		    oldTime = triggeredTime;
		    toGoDist = aTrain.distanceToGoCurrent();
		    distDiffPercent = 
			100 * (toGoDist - aTrain.getCurrentDistance())/toGoDist;
		    // reset the distance estimation
		    aTrain.resetDistance();
		    sprintf(strWrite, " [%c%2d][sensorTime][%3d]  ", 
			    SEN_MODULE_CHAR(sensorData[0]),
			    SEN_NUMBER(sensorData[0]), timeDifference);
		    Write( strWrite, strLen(strWrite), WYSE);
		    sprintf(strWrite, 
			" ToGo %6d Distance %6d Diff %4d Pecent[%2d] \n\r",
			toGoDist, currentDist, toGoDist - currentDist ,
			abs(distDiffPercent));
		    Write( strWrite, strLen(strWrite), WYSE);
		    // if there is more command set
		    if(aTrain.setNextCommandSet()) {
#ifdef BlackBoard
// check in two ahead sensor
			checkSen = aTrain.get2ndNextSensor();
			if(checkSen != EndSensorSignal) {
			    Data.sensor = checkSen;
			    Data.type = Approval_CheckIn;
			    // ask blackboard
			    Send(Tid_blackBoard, 
				&Data,sizeof(Data),&result,sizeof(result));
			    if(result !=1) {
				aTrain.stopTrain();
				sprintf(strWrite, 
				    " >xxxx checkd 0x%x result %d    \n\r",
				    checkSen, result);
				Write( strWrite, strLen(strWrite), WYSE);
				Delay(60);
			    }
			    else {
				sprintf(strWrite, 
				    " > checkd 0x%x result %d    \n\r",
				    checkSen, result);
				Write( strWrite, strLen(strWrite), WYSE);
			    }
			}

			// XXX XXX 
#endif //BlackBoard
			// if direction change is needed
			if(aTrain.isChangeDirection()) {
			    // set directionChaged Flag
			    directionChanged = True;
			    // XXX I need over shoot when 
			    // it need to stop and change direction
			    // XXX I need to change this too more
			    // efficiently
			    // XXX XXX 
			    //if(aTrain.getSpeed() < 4) {
				//Delay(20);
				//Delay(10);
			    //} // if
			    aTrain.stopTrain();
			    if(LastTriggeredSensor == 0x1e || // b15
				LastTriggeredSensor == 0x03 || // a4
				LastTriggeredSensor == 0x44 || // e5
				LastTriggeredSensor == 0x4e  ||  // e14
				LastTriggeredSensor == 0x4a  ||  // e11
				LastTriggeredSensor == 0x2c  ||  // c13 
				LastTriggeredSensor == 0x25  )  // c6 
			    {

				aTrain.startTrain();
				Delay(20);
				aTrain.stopTrain();
				Delay(3);
			    }
			    if(LastTriggeredSensor == 0x26) { // c 07
				aTrain.setSwitchStraight(18);
			    }
			    aTrain.changeDirection();
			    aTrain.startTrain();
			} // inner if
			if(aTrain.hasNextSwitchCommand()){
			    aTrain.setNextCommandSwitch();
			} // if
		    } //if more command sets
		    // if no more command set then finish this session
		    // means finished the mission
		    else {
			aTrain.stopTrain( );
			// set mission end
			aTrain.setFinished();
			// set I need to start over new mission
			aTrain.resetStarted( );
		    } // else if no more command set
		} // inner if the sensor matched 
		// if no  matched sensor until some points
		// I am lost
		else if (aTrain.amIlost()){
			aTrain.stopTrain( );
			LastTriggeredSensor = sensorData[0];
			sprintf(strWrite, 
			    "\n !!!!!!!!!!!!! I am lost at 0x%x>>>>>>>>\n",
			    LastTriggeredSensor);
			Write(strWrite, strLen(strWrite), WYSE);
			// set mission end
			aTrain.setFinished();
			// set I need to start over new mission
			aTrain.resetStarted( );
		} // else if
		// the sensor is not my so I need check the sensors
		else {
		    index = 0;
// need to check XXX XXX XXX
		    //while(sensorData[index] != 0) {
		    while(sensorData[index] != 0xff) {
			index++;
		    } //while
		} // if
	    } // if sensorData checked (matched or not)
	} // train start else 
	// XXX 
	Delay( TRAIN_TASK_TICKS ); // make 100 msec period task 10 Hz
    } // while
} // run train function

//
// -- 
//
// train run with fsm, test
//
// -- 
//
void runTrainFSM(void)
{
    // index for sensor data
    char index;
    unsigned char checkSen;
    int triggeredTime, oldTime, timeDifference;
    int currentDist = 0;
    int startTime, endTime;
    // run = end - start
    int runTime, distanceToGo;
    int toGoDist, distDiffPercent, distDiff, result;
    Boolean sensorBingo = False;
    Boolean sensorChecked = False;
    Boolean directionChanged = False;
    Boolean reverseChecked = False;
    // to see if the the sensor range
    Boolean myRange = False;

    strWrite = "Train Task Running ...\n\r";
    Write( strWrite, strLen(strWrite), WYSE);

    //while() {
	//if() {
	//}
	//else if() {
	//}
	//else if() {
	//}
	//else {
	//}
    //} // while

    while(!aTrain.isFinished()) { // if the task is not finished
	getSensorData_fromSensorServer();
	if( !aTrain.isStarted( ) ) { // if train is not started
	    sprintf(strWrite,"\n\r>>> start train\n\r");
	    Write( strWrite, strLen(strWrite), WYSE);
#ifdef BlackBoard
// need to check two ahead route(sensors) before start
	    checkSen = aTrain.getNextSensor();
	    if( checkSen != EndSensorSignal) {
		Data.sensor = checkSen;
		Data.type = Approval_CheckIn;
		// ask blackboard
		Send(Tid_blackBoard, 
		    &Data,sizeof(Data),&result,sizeof(result));

		if(result !=1) {
		    aTrain.stopTrain();
		    sprintf(strWrite, 
			" >xxxx checkd 0x%x result %d    \n\r",
			checkSen, result);
		    Write( strWrite, strLen(strWrite), WYSE);
		    Delay(100);
		}
		else {
		    sprintf(strWrite, 
			" > checkd 0x%x result %d    \n\r",
			checkSen, result);
		    Write( strWrite, strLen(strWrite), WYSE);
		}
	    }

	    checkSen = aTrain.get2ndNextSensor();
	    if( checkSen != EndSensorSignal) {
		Data.sensor = checkSen;
		Data.type = Approval_CheckIn;
		// ask blackboard
		Send(Tid_blackBoard, 
		    &Data,sizeof(Data),&result,sizeof(result));

		if(result !=1) {
		    aTrain.stopTrain();
		    sprintf(strWrite, 
			" >xxxx checkd 0x%x result %d    \n\r",
			checkSen, result);
		    Write( strWrite, strLen(strWrite), WYSE);
		    Delay(100);
		}
		else {
		    sprintf(strWrite, 
			" > checkd 0x%x result %d    \n\r",
			checkSen, result);
		    Write( strWrite, strLen(strWrite), WYSE);
		}
	    }

	    // XXX XXX 
#endif //BlackBoard

	    // if there is switch command then set the switch
	    if(aTrain.hasSwitchCommand( )) {
		aTrain.setCommandSwitch( );
	    } // if

	    // if the first parity is different from last triggerd 
	    // sensor parity then change direction
	    // set the next switch before leave if it is necessary
	    if(aTrain.hasNextSwitchCommand()){
		aTrain.setNextCommandSwitch();
	    } // if

	    // set the current switch before leave if it is necessary
	    if(aTrain.hasSwitchCommand()) {
		aTrain.setCommandSwitch();
	    } // if

	    sprintf(strWrite, "last sensor %x first one %x and dir %d\n\r",
		LastTriggeredSensor, aTrain.getFirstSensor(),
		aTrain.getDirection());
	    Write( strWrite, strLen(strWrite), WYSE);


	    // if need to change the direction at the beginning
	    if( aTrain.getDirection() == -1 ) {
		sprintf(strWrite, "Parity need to be changed\n\r");
		Write( strWrite, strLen(strWrite), WYSE);
		aTrain.changeDirection();
		// for safety factor delay ??
		Delay(20);
	    } // if
	    // if forward
	    else {
		aTrain.changeDirection(); 	Delay(10);
		aTrain.startTrain(); 		Delay(20);
		aTrain.stopTrain(); 		Delay(10);
		aTrain.changeDirection(); 	Delay(10);
	    }
	    // check start flag, means started the mission
	    aTrain.setStarted();
	    // for checking time difference
	    startTime = endTime = oldTime = UpTime();
	    toGoDist = aTrain.distanceToGoCurrent();
	    // reset distance
	    aTrain.resetDistance( ); 
	    // start train 
	    aTrain.startTrain( );
	}  // if 
	else { // if train is started

// XXX need to check XXX XXX XXX
	    //if( sensorData[0] == 0 ) { // if no sensor comes
	    if( sensorData[0] == 0xff) { // if no sensor comes
		// XXX I need to clear this things....
		if( reverseChecked == True ) {
		    directionChanged = False;
		    reverseChecked = False;
		} //if 
		// no sensors and no matched sensor comes in
		sensorBingo = False;
		// update distance only there is no sensor data comes in
		endTime = UpTime();
		// run time, difference of the time. i.e. delta_t
		runTime = endTime - startTime;
		startTime = endTime;
		currentDist = aTrain.updateDistance( runTime );
		// distance check
		toGoDist = aTrain.distanceToGoCurrent();
		distDiffPercent = 
		    100 * (toGoDist - aTrain.getCurrentDistance())/toGoDist;
		// make sure the sensor is in my range
		if( currentDist > (toGoDist*0.8))  {
		    myRange = True;
		} // if
		// check calculated dist is over the real distance
		if( currentDist > toGoDist )  {
		    distDiffPercent = 
			100 * (toGoDist - aTrain.getCurrentDistance())/toGoDist;
		} // if check calculated dist is over the real distance
		// XXX I need to do this with certain time delay...
		// if I do this too frequent
		if(aTrain.needSlowDown() ) {
		    //&& aTrain.canChangeSpeed(endTime)) {
		    // put break
		    //aTrain.decreaseSpeed();
		    //cprintf("BREAKING>>>>  ");
		    aTrain.setSpeed(SpeedOperationLowest);
		    aTrain.setChangeSpeedTime( endTime );
		} // if
		else if ( aTrain.canChangeSpeed(endTime)){
		    // put accerlaton pedal
		    aTrain.increaseSpeed();
		} // else
	    } // if 
	    else { // XXX if the sensor data is come
		// ignore next sensor trigger
		// after direction change
		if( (directionChanged == True) ){
		    reverseChecked = True;
		    aTrain.resetDistance();
		}
		// XXX need to do some check multiple sensors
		else if( (sensorData[0] == aTrain.getNextSensor() ||
		    sensorData[1] == aTrain.getNextSensor() ||
		    sensorData[2] == aTrain.getNextSensor() || 
		    sensorData[3] == aTrain.getNextSensor() || 
		    aTrain.getNextSensor() == (BYTE)EndSensorSignal) && 
		    //timeToCheckSensor == True &&
		    sensorBingo != True && myRange)
		{
		// XXX I need to report somewhere here with BB XXX XXX 
		    // XXX or I may do this in class XXX
		    // reset my range;
		    myRange = False;
		    // for next mission run
		    LastTriggeredSensor = sensorData[0];
		    // means that waited sensor comes in
		    sensorBingo = True;
#ifdef BlackBoard
//need to check out the sensor
		    Data.sensor = LastTriggeredSensor;
		    Data.type = Approval_CheckOut;
		    // ask blackboard
		    Send(Tid_blackBoard, 
			&Data,sizeof(Data),&result,sizeof(result));
		    sprintf(strWrite, 
			" < checkd OUT 0x%x result %d    \n\r",
			LastTriggeredSensor, result);
		    Write( strWrite, strLen(strWrite), WYSE);

#endif //BlackBoard
		    // XXX for checking time difference
		    // maybe used by GetSmallTime()
		    // to check exact timer in 
		    triggeredTime = UpTime();
		    timeDifference = triggeredTime - oldTime;
		    oldTime = triggeredTime;
		    toGoDist = aTrain.distanceToGoCurrent();
		    distDiffPercent = 
			100 * (toGoDist - aTrain.getCurrentDistance())/toGoDist;
		    // reset the distance estimation
		    aTrain.resetDistance();
		    sprintf(strWrite, " [%c%2d][sensorTime][%3d]  ", 
			    SEN_MODULE_CHAR(sensorData[0]),
			    SEN_NUMBER(sensorData[0]), timeDifference);
		    Write( strWrite, strLen(strWrite), WYSE);
		    sprintf(strWrite, 
			" ToGo %6d Distance %6d Diff %4d Pecent[%2d] \n\r",
			toGoDist, currentDist, toGoDist - currentDist ,
			abs(distDiffPercent));
		    Write( strWrite, strLen(strWrite), WYSE);
		    // if there is more command set
		    if(aTrain.setNextCommandSet()) {
#ifdef BlackBoard
// check in two ahead sensor
			checkSen = aTrain.get2ndNextSensor();
			if(checkSen != EndSensorSignal) {
			    Data.sensor = checkSen;
			    Data.type = Approval_CheckIn;
			    // ask blackboard
			    Send(Tid_blackBoard, 
				&Data,sizeof(Data),&result,sizeof(result));
			    if(result !=1) {
				aTrain.stopTrain();
				sprintf(strWrite, 
				    " >xxxx checkd 0x%x result %d    \n\r",
				    checkSen, result);
				Write( strWrite, strLen(strWrite), WYSE);
				Delay(60);
			    }
			    else {
				sprintf(strWrite, 
				    " > checkd 0x%x result %d    \n\r",
				    checkSen, result);
				Write( strWrite, strLen(strWrite), WYSE);
			    }
			}

			// XXX XXX 
#endif //BlackBoard
			// if direction change is needed
			if(aTrain.isChangeDirection()) {
			    // set directionChaged Flag
			    directionChanged = True;
			    // XXX I need over shoot when 
			    // it need to stop and change direction
			    // XXX I need to change this too more
			    // efficiently
			    // XXX XXX 
			    //if(aTrain.getSpeed() < 4) {
				//Delay(20);
				//Delay(10);
			    //} // if
			    aTrain.stopTrain();
			    if(LastTriggeredSensor == 0x1e || // b15
				LastTriggeredSensor == 0x03 || // a4
				LastTriggeredSensor == 0x44 || // e5
				LastTriggeredSensor == 0x4e  ||  // e14
				LastTriggeredSensor == 0x4a  ||  // e11
				LastTriggeredSensor == 0x2c  ||  // c13 
				LastTriggeredSensor == 0x25  )  // c6 
			    {

				aTrain.startTrain();
				Delay(20);
				aTrain.stopTrain();
				Delay(3);
			    }
			    if(LastTriggeredSensor == 0x26) { // c 07
				aTrain.setSwitchStraight(18);
			    }
			    aTrain.changeDirection();
			    aTrain.startTrain();
			} // inner if
			if(aTrain.hasNextSwitchCommand()){
			    aTrain.setNextCommandSwitch();
			} // if
		    } //if more command sets
		    // if no more command set then finish this session
		    // means finished the mission
		    else {
			aTrain.stopTrain( );
			// set mission end
			aTrain.setFinished();
			// set I need to start over new mission
			aTrain.resetStarted( );
		    } // else if no more command set
		} // inner if the sensor matched 
		// if no  matched sensor until some points
		// I am lost
		else if (aTrain.amIlost()){
			aTrain.stopTrain( );
			LastTriggeredSensor = sensorData[0];
			sprintf(strWrite, 
			    "\n !!!!!!!!!!!!! I am lost at 0x%x>>>>>>>>\n",
			    LastTriggeredSensor);
			Write(strWrite, strLen(strWrite), WYSE);
			// set mission end
			aTrain.setFinished();
			// set I need to start over new mission
			aTrain.resetStarted( );
		} // else if
		// the sensor is not my so I need check the sensors
		else {
		    index = 0;
// need to check XXX XXX XXX
		    //while(sensorData[index] != 0) {
		    while(sensorData[index] != 0xff) {
			index++;
		    } //while
		} // if
	    } // if sensorData checked (matched or not)
	} // train start else 
	// XXX 
	Delay( TRAIN_TASK_TICKS ); // make 100 msec period task 10 Hz
    } // while
}





//
//--
//  main
//--
//
main() 
{
    int tid;
    int commandIndex = 0;
    char cmd[CMD_SIZE];
    // first tripped sensor
    unsigned char firstSensor;
    int triggeredTime, oldTime, timeDifference;
    int currentDist = 0;
    int ccTid;
    BYTE myNum;
    char registerName[22];
    int timeKeeper;

// get assigned train number from Central control
    Tid_centralControl = Receive(&myNum, sizeof(myNum));
// register name as TrainTaskMain + myNum
    sprintf(registerName, "TrainTaskMain%d",myNum);
    RegisterAs( registerName );
//  set data for communcation with BB
    Data.sender = myNum;
// wait until register is finished....
    Delay(5);
// set train number
    aTrain.setTrainNumber( myNum );
// set train at start position
#ifdef START_POSITION
    LastTriggeredSensor = aTrain.goStartPosition();
#else
    Reply(Tid_centralControl, &LastTriggeredSensor,sizeof(char));
#endif // START_POSITION

    // XXX BB yet
    Tid_blackBoard = WhoIs( "BlackBoardMain" );
    //Tid_BB_distributor = WhoIs( "BB_Distributor" );
    //Tid_BB_collector = WhoIs( "BB_Collector" );
// reset all status
    aTrain.resetStarted();
    aTrain.resetFinished();
    aTrain.resetMissionStart();
// main while loop
    int startTime;
    int endTime;
    Boolean runFlag = True;;
    while(1) {
	// if mission is not started 
	if( !aTrain.isMissionStarted()) {
	    // get the command set from central control
	    getCommandSet_fromCentralControl(LastTriggeredSensor);
	    Delay(50);
	    sprintf(strWrite, 
		"\n <<<<<<>>>>  Train[%d] Mission Start <<>>>>>\n",
		aTrain.getTrainNumber());
	    Write(strWrite, strLen(strWrite), WYSE);
	    // mission is started
	    aTrain.setMissionStart();
	} // if
	else { // run the train 
	    runTrain();
	// reset all status
	    aTrain.resetStarted();
	    aTrain.resetFinished();
	    aTrain.resetMissionStart();
	    // send msg to Central controller
	    Send(Tid_centralControl, &LastTriggeredSensor,
		sizeof(char), NULL,NULL);
	    sprintf(strWrite, "\n\r <<<>>>>  Train Mission End>>>>>.....\n\r");
	    Write(strWrite, strLen(strWrite), WYSE);
	} // else
	Delay( TRAIN_TASK_TICKS );
    } // while
} // MAIN

//
//---
// run the train with given speed
//	and display the estimatied speed
//---
//
void speedTest( void ) {
    // XXX test
    int triggeredTime, oldTime, timeDifference;
    int currentDist = 0;
	char ch;
    oldTime = UpTime();
    char  strWrite[50];

    sprintf(strWrite, "INPUT SPEED\n\r");
    Write(strWrite, strLen(strWrite), WYSE);
	ch =Get(WYSE);
	aTrain.setSpeed(ch -'0'+10);

    //
    // velocity print out test
    //
    //BYTE i;
    //for(i=0;i<15;i++) {
	//printf("[%d] velocity %d\n",i, aTrain.getVelocity(i));
    //}

    while(1) {
	// get the command set from central control
	getSensorData_fromSensorServer();

	//currentDist += (2 * aTrain.getVelocity());
	//console_xy(10,30);
	//cprintf("\t\tDistance %d", currentDist);
	if(sensorData[0] !=0) {

	    // XXX for checking time difference
	    // maybe used by GetSmallTime()
	    // to check exact timer in 
	    triggeredTime = UpTime();
	    timeDifference = triggeredTime - oldTime;
	    oldTime = triggeredTime;
	    console_xy(0,29);
	    sprintf(strWrite, " [%c%d][sensorTime][%d]\n\r  ", 
		    SEN_MODULE_CHAR(sensorData[0]),
		    SEN_NUMBER(sensorData[0]), timeDifference
	    );
	    Write(strWrite, strLen(strWrite), WYSE);
	    //printf("Distance %d\n", currentDist);
	    currentDist = 0;
	} // if

	Delay( 2 );
    } // while
}

//
// old one
// get train number from central control using Receive
//
void getTrainNumber_fromCentralControl( void ) {

    BYTE tid;
    BYTE trainNo;
    char sendMsg[20];

    // getting message from central control
    // XXX I should think again (like send from here 
    // instead of Receive from CC 
    tid = Receive(&trainNo, sizeof(char));
    Reply(tid, NULL,NULL);

    if(trainNo =='a' || trainNo == 'A') {
	aTrain.setTrainNumber( 12 );
	sendMsg = ">>Train A Selected\n\r";
	Write( sendMsg, strLen(sendMsg), WYSE);
    }
    else if(trainNo =='b' || trainNo == 'B') {
	aTrain.setTrainNumber( 55 );
	sendMsg = ">>Train B Selected\n\r";
	Write( sendMsg, strLen(sendMsg), WYSE);
    }
    else if(trainNo =='c' || trainNo == 'C') {
	aTrain.setTrainNumber( 57 );
	sendMsg = ">>Train C Selected\n\r";
	Write( sendMsg, strLen(sendMsg), WYSE);
    }
}

