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


#include "trainTask.h"
#include "../buffer_server/communicateData.h"

// old command set run routine
#undef	OLD_COMMAND_RUN

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

// sensor data
BYTE sensorData[11];

BYTE LastTriggeredSensor = 0;
int Tid_centralControl;
int Tid_blackBoard;
int Tid_BB_distributor;
int Tid_BB_collector;

// serial message write
char strWrite[100];

// serial message read
char strRead[100];

// command set data from Central control
CommandSet cmdSet[MAX_COMMAND_SET];

CommandSet *pCmdSet = (aTrain.aCommandSet);



void speedTest( void );

//
// get sensor data from sensor server
//
inline void getSensorData_fromSensorServer( void ) {

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
    //node shortPathNode[NUMSEGMENTS];
    oneHop stepsToFollow[NUMSEGMENTS];

//
// old one
// getting command set from central control
    //tid = Receive(&cmdSet, MAX_COMMAND_SET * sizeof(CommandSet));
    //Reply(tid, NULL,NULL);

// getting command set from central control
    //tid = Receive(&stepsToFollow, NUMSEGMENTS * sizeof(oneHop));
    //Reply(tid, NULL,NULL);
    Send(Tid_centralControl, &_lastSensor, sizeof(_lastSensor), 
	    &stepsToFollow, NUMSEGMENTS * sizeof(oneHop));

    int i = aTrain.setCommandSet( stepsToFollow );

    sprintf(strWrite, "tr SET COMMAND AND SIZE IS %d\n", i);
    Write(strWrite, strLen(strWrite), WYSE);

    aTrain.printCommandSet();

    //aTrain.printCommandSet(0);
    //aTrain.printCommandSet(1);
#if 0
oneHop * ptrHop;
char ch;
    do{
	printf("press n or q \n");
	ch = Get(WYSE);
	if(ch == 'n') {
	    printf("Next>\n");
	    if(aTrain.hasMoreCommandSet() ) {
		printf("Has more>\n");
		ptrHop = aTrain.getNextCommandSet();
		aTrain.printCommandSet(ptrHop);
	    } // if
	    else {
		printf("No more>\n");
		break;
	    }
	}
    } while(ch!='q');
#endif

		
}


//
//
// test function for communicating with the distributor
//
//
distributorTest() {

    CommData	response;
    char	inputChar;

    int		readLength = 0;

    sprintf(strWrite, "[train, distributortest()]\n");
    Write(strWrite, strLen(strWrite), WYSE);

    while(1) {

	sprintf(strWrite,"[>train, send to distributor for asking data]\n");
	Write(strWrite, strLen(strWrite), WYSE);

	// make packet
	//response.fromType = Consume;
	//response.from = aTrain.getTrainNumber();
	// XXX i should change this type things
	aCommData.initData(Consume, (TaskType)aTrain.getTrainNumber());
	//aCommData.printData();

	response = aCommData.getData();

	// send distributor asking the message from BB per se
	Send(Tid_BB_distributor, &response, sizeof(response),
	    &response, sizeof(response));

	sprintf(strWrite, "\n[<train [%d] get data from distributor....]\n",
	    aTrain.getTrainNumber());
	Write(strWrite, strLen(strWrite), WYSE);

	aCommData.setData( response );

	// print out the responsed message
	//aCommData.printData();

	sprintf(strWrite, "\n[Read processing to ....]\n");
	Write(strWrite, strLen(strWrite), WYSE);

	readLength = Read(&strRead, WYSE);
	cprintf("[TT] : read data is [%s] len %d\n", strRead, readLength);
	//inputChar = Get(WYSE);
	//cprintf("\ninputted char [%c] [0x%x] [%d]\n", 
	    //inputChar, inputChar, inputChar);
	Delay(10);

    } // while


}





//
// XXX WARNING this function is not workable
// 
int toGoDistance = 0;
int lastArrow = 0;

void displayDistance(int _toGoDistance, int currentDistance) {

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
void runTrain(void) {

    char firstSensor;
    int triggeredTime, oldTime, timeDifference;
    int currentDist = 0;

    // start time
    int startTime;

    // end time
    int endTime;

    // run = end - start
    int runTime;

    int distanceToGo;

    Boolean sensorBingo = False;
    Boolean sensorChecked = False;
    Boolean directionChanged = False;
    Boolean reverseChecked = False;

    // to see if the the sensor range
    Boolean myRange = False;

    int toGoDist;
    int distDiffPercent;
    int distDiff;

    strWrite = "tr Train Task Running ...\n";
    Write( strWrite, strLen(strWrite), WYSE);

    while(!aTrain.isFinished()) { // if the task is not finished

	getSensorData_fromSensorServer();

	if( !aTrain.isStarted( ) ) { // if train is not started

	    sprintf(strWrite,"\n>>> press key to start train\n");
	    Write( strWrite, strLen(strWrite), WYSE);

	    // XXX
	    //Get(WYSE);
	    // XXX
	    //Get(WYSE);

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

	    sprintf(strWrite, "tr last sensor %x first one %x and dir %d\n",
		LastTriggeredSensor, aTrain.getFirstSensor(),
		aTrain.getDirection());
	    Write( strWrite, strLen(strWrite), WYSE);

	    // if need to change the direction at the beginning
	    if( aTrain.getDirection() == -1 ) {

		sprintf(strWrite, "tr Parity need to be changed\n");
		Write( strWrite, strLen(strWrite), WYSE);

		aTrain.changeDirection();

		// for safety factor delay ??
		Delay(20);

	    } // if

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

	    if( sensorData[0] == 0 ) { // if no sensor comes

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

		    if(abs(distDiffPercent) > 50) {

		    // XXX I need to report somewhere here with BB 
		    // about distance overflow so maybe delayed ...XXX XXX 

			sprintf(strWrite, 
			    "tr >>  Waring!!! too much dist error toGo[%d] "
			    "curDist[%d] Percent[%d] \n",
			    toGoDist, currentDist, distDiffPercent);
			Write(strWrite, strLen(strWrite), WYSE);

		    } // inner if


		} // if check calculated dist is over the real distance
		    

		// XXX I need to do this with certain time delay...
		// if I do this too frequent
		if(aTrain.needSlowDown() &&
		    aTrain.canChangeSpeed(endTime)) {

		    // put break
		    aTrain.decreaseSpeed();
		    aTrain.setChangeSpeedTime( endTime );

		} // if

		else if ( aTrain.canChangeSpeed(endTime)){

		    // put accerlaton pedal
		    aTrain.increaseSpeed();

		} // else

		console_xy(10,30);
		cprintf("ToGo %5d CurDist %10d estimated sensor"
		    " [0x%2x] " "Speed[%02d] " ,toGoDist, 
		    aTrain.getCurrentDistance(), currentDist, 
		    aTrain.getNextSensor(), aTrain.getSpeed());
		//displayDistance(toGoDist, currentDist);
		    
	    } // if 
	    else { // if the sensor data is come

		// ignore next sensor trigger
		// after direction change
		if( (directionChanged == True) ){

		    reverseChecked = True;
		    aTrain.resetDistance();
		}

		// XXX need to do some check multiple sensors

		else if( (sensorData[0] == aTrain.getNextSensor() ||
		aTrain.getNextSensor() == EndSensorSignal) && 
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

		    sprintf(strWrite, "tr [%c%2d][sensorTime][%3d]  ", 
			    SEN_MODULE_CHAR(sensorData[0]),
			    SEN_NUMBER(sensorData[0]), timeDifference);
		    Write( strWrite, strLen(strWrite), WYSE);

		    sprintf(strWrite, 
			" ToGo %6d Distance %6d Diff %4d Pecent[%2d] \n",
			toGoDist, currentDist, toGoDist - currentDist ,
			abs(distDiffPercent));
		    Write( strWrite, strLen(strWrite), WYSE);

		    // if there is more command set
		    if(aTrain.setNextCommandSet()) {

			// if direction change is needed
			if(aTrain.isChangeDirection()) {

			    // set directionChaged Flag
			    directionChanged = True;


			    // XXX I need over shoot when 
			    // it need to stop and change direction
			    // XXX I need to change this too more
			    // efficiently
			    if(aTrain.getSpeed() < 4) {

				Delay(20);
				//Delay(10);
			    } // if

			    aTrain.stopTrain();

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

	    } // if sensorData checked (matched or not)

	} // train start else 

	// XXX 
	Delay( TRAIN_TASK_TICKS ); // make 100 msec period task 10 Hz

    } // while

} // run train function


//
//--
//
//  main
//
//--
//
main() {

    int tid;

    int commandIndex = 0;

    char cmd[CMD_SIZE];
    // first tripped sensor
    char firstSensor;

    int triggeredTime, oldTime, timeDifference;
    int currentDist = 0;
    int ccTid;
    BYTE myNum;
    char registerName[22];
    int timeKeeper;

    int dataFromCC;

// get assigned train number from Central control
    Tid_centralControl = Receive(&myNum, sizeof(myNum));

// register name as TrainTaskMain + myNum
    sprintf(registerName, "tr TrainTaskMain%d",myNum);
    RegisterAs( registerName );

// wait until register is finished....
    Delay(10);
    Reply(Tid_centralControl, NULL,NULL);

    //Tid_centralControl = WhoIs( "CentralControlMain" );

    // XXX BB yet
    //Tid_blackBoard = WhoIs( "BlackBoardMain" );
    //Tid_BB_distributor = WhoIs( "BB_Distributor" );
    //Tid_BB_collector = WhoIs( "BB_Collector" );

// set train number
    aTrain.setTrainNumber( myNum );

// print train information
    aTrain.printTrain();


// XXX // waiting from start queue from CC
    tid = Receive(&dataFromCC,sizeof(dataFromCC));
    Reply(tid,NULL,NULL);

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
	    // XXX
	    //getCommandSet_fromCentralControl();

	    // XXX
	    getCommandSet_fromCentralControl(LastTriggeredSensor);

	    // mission is started
	    aTrain.setMissionStart();

	} // if

	else { // run the train 

	    runTrain();

	// reset all status
	    aTrain.resetStarted();

	    aTrain.resetFinished();

	    aTrain.resetMissionStart();

	    sprintf(strWrite, "\n\n tr Train Mission End>>>>>.....press key\n");
	    Write(strWrite, strLen(strWrite), WYSE);

	    Get(WYSE);
	    // XXX
	    Get(WYSE);

	    // XXX
	    Send(Tid_centralControl, NULL,NULL,NULL,NULL);

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

    //sprintf(strWrite, "INPUT SPEED\n");
    //Write(strWrite, strLen(strWrite), WYSE);
	//ch =Get(WYSE);
	//aTrain.setSpeed(ch -'0'+10);

    //
    // velocity print out test
    //
    //BYTE i;
    //for(i=0;i<15;i++) {
	//printf("[%d] velocity %d\n",i, aTrain.getVelocity(i));
    //}

    int i,innerCount=0;
    char oldSensor = 0xff;
    Boolean first = True;
    //while(1) {
    for(i=2;i<15;i++) {
	sprintf(strWrite, "se speed [%d]------------>>>>>\n\n  ",i );
	Write(strWrite, strLen(strWrite), WYSE);
	aTrain.setSpeed(i);

	while(innerCount < 22) {

	    // get the command set from central control
	    getSensorData_fromSensorServer();

	    //currentDist += (2 * aTrain.getVelocity());
	    //console_xy(10,30);
	    //cprintf("\t\tDistance %d", currentDist);

	    // XXX ? XXX not checked 
	    if( (sensorData[0] != 0 && oldSensor != sensorData[i]) ||
		first) {
		oldSensor = sensorData[i];
		first = False;

		innerCount++;

		// XXX for checking time difference
		// maybe used by GetSmallTime()
		// to check exact timer in 
		triggeredTime = UpTime();
		timeDifference = triggeredTime - oldTime;
		oldTime = triggeredTime;
		console_xy(0,29);
		sprintf(strWrite, "se [%c%d][sensorTime][%d]\n  ", 
			SEN_MODULE_CHAR(sensorData[0]),
			SEN_NUMBER(sensorData[0]), timeDifference
		);
		Write(strWrite, strLen(strWrite), WYSE);
		//printf("Distance %d\n", currentDist);
		currentDist = 0;
	    } // if

	    Delay( 2 );
	} // while innerCount
	innerCount = 0;
	sprintf(strWrite, "se ---------------------------------<<<<<\n\n  " );
	Write(strWrite, strLen(strWrite), WYSE);
	aTrain.stopTrain();
	Delay(40);
    }// outter for i
    //} // while
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



//
//
//
//////////////////////////////////////////////////////////
// 
// runn test for old command set
//
#ifdef	OLD_COMMAND_RUN
// main while loop
    while(1) {

	// if mission is not started 
	if( !aTrain.isMissionStarted()) {

	    // get the command set from central control

	    // XXX
	    //getCommandSet_fromCentralControl();

	    // mission is started
	    aTrain.setMissionStart();

	} // if

	else {

	    strWrite = "Train Task Running ...\n\r";
	    Write( strWrite, strLen(strWrite), WYSE);

	    // if the task is not finished
	    while(!aTrain.isFinished()) {

		// get sensor data from sensor server 
		getSensorData_fromSensorServer();

		
		//tempDist += (177*2);
		console_xy(10,30);
		cprintf("\t\tDistance %d", tempDist);
		
		if( !aTrain.isStarted() ) {
		    // set the switch
		    //aTrain.setSwitch( cmdSet[commandIndex].switchNo,
					    //cmdSet[commandIndex].switchType );
		    // start the train 
		    aTrain.startTrain();
		    //aTrain.setSpeed( 5 );

		    // set started
		    aTrain.setStarted();

		    commandIndex++;

		    // for checking time difference
		    oldTime = UpTime();

		}  // if 

		else if(sensorData[0] !=0) {

		    // XXX for checking time difference
		    // maybe used by GetSmallTime()
		    // to check exact timer in 
		    triggeredTime = UpTime();
		    timeDifference = triggeredTime - oldTime;
		    oldTime = triggeredTime;
		    console_xy(0,10);
		    cprintf(" [%c%d][sensorTime][%d]  ", 
			    SEN_MODULE_CHAR(sensorData[0]),
			    SEN_NUMBER(sensorData[0]), timeDifference
		    );
		    cprintf("Distance %d\n", tempDist);
		    tempDist = 0;

		    if( sensorData[0] == cmdSet[commandIndex].sensor ){
			//cprintf("Bingo CI %d", commandIndex);

			// set new the switch, if it is needed to do
			if(cmdSet[commandIndex].switchNo != 0 ) {
			    aTrain.setSwitch( cmdSet[commandIndex].switchNo,
						    cmdSet[commandIndex].switchType );
			} // inner most if 

			// set new speed
			aTrain.setSpeed( cmdSet[commandIndex].speed );

			commandIndex++;


		    // XXX
		    // you do some testing
		    // to make aTrain.setFinished()

		    
		    } // inner if


		} // else if

		// make 100 msec period task 10 Hz
		Delay( TRAIN_TASK_TICKS );

	    } // inner while

	    // XXX do someting
	    // upon the result of the running
	    //
	    // and resetMissionStarted()
	    //
	    // e.g. ask next mission request
	    // 	or  report the error


	} // else 

    } // outter while 
#endif // OLD_COMMAND_RUN
