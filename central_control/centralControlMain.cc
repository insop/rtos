//
//--
//
//	centralControlMain.cc
//
//	central controller
//	Main file
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

#include "../central_control/centralControl.h"

#define	TOKENIZER_ENABLE	

extern "C" {
    #include "../include/i486/cs452.h"
    #include "../include/util/type.h"
    #include "../syscall/sysCall.h"
    #include "../train/ironBird.h"
    #include "../pathfind/graphtype.h"
    #include "../schedular/schedulartype.h"
}; // C function interface 

// 
// forward declaration
//
void getShortPath(int trainNo, char destination);

    //
    //-- 
    // global object 
    //--
    //

CentralControl aCentralCtrl;


    //
    //-- 
    // global variable 
    //--
    //
int	Tid_trainTaskMain;
int	Tid_oneStep;
int	Tid_pathFind;
int	Tid_blackBoard;
int	Tid_tokenizer;
int	Tid_schedular;

// string for Write;
char 	strWrite[100];

CommandSet cmdSet[MAX_COMMAND_SET] ={
/* type  sensor swNo   switchType speed extra*/
{ START , 0x00, 3,  	STRAIGHT, 2, 	0}, /* start */
{ NORMAL, 0x04, 5,  	CURVE,    4, 	0}, /* a4->  5  C 	*/
{ NORMAL, 0x26, 0,  	0	, 6, 	0}, /* c7->  0  0 	*/
{ NORMAL, 0x4a, 9,  	CURVE   , 6, 	0}, /* e11-> 9  C 	*/
{ NORMAL, 0x39, 10 , 	STRAIGHT,  6, 	0}, /* d10-> 10  s 	*/
{ NORMAL, 0x2b, 15 , 	STRAIGHT,  6, 	0}, /* c12-> 15  s 	*/
{ NORMAL, 0x1f, 6 , 	CURVE   ,  4, 	0}, /* b16-> 6  c 	*/
{ NORMAL, 0x24, 5 , 	STRAIGHT,  2, 	0}, /* c5-> 5  s 	*/
{ NORMAL, 0x22, 0 , 	0       , 0, 	0}, /* c3->  0   0 	*/

{ NORMAL, 0x42, 0 , 	0       , 0, 	0}, /* e3->  0  0 	*/
{ END,    0x37, 0 , 	CURVE   , 0, 	0}


};


// XXX test multi train
int multiTrainIndex = 0;

// XXX just for the tokenizer
BYTE selectedTrain[MaxTrainNum] = {0,0,0,0,0,0};


//
// create multiple trains
//
void createTrains( void ) {

    BYTE checkFlag;
    BYTE trainNum;
    BYTE createdNum = 0;

// data from train
    BYTE lastTriggeredSensor;

    int	trainTid;

//
//  Create trains
//



    for(int i=0; i<MaxTrainNum; i++) {
	sprintf(strWrite, "Do you want to create [%d] Train?.. (y/n)\n",
	    TrainNumbers[i]);
	Write(strWrite, strLen(strWrite), WYSE);

	trainNum = Get(WYSE);
	if(trainNum == 'y' || trainNum =='Y') {

	    trainTid = 
		Create("TrainTaskMain", TrainTaskPriority);

	    trainNum = TrainNumbers[i]; 

	    // for tokenizer
	    selectedTrain[createdNum] = trainNum;

	    // send the train their assigned train number
	    Send( trainTid, &trainNum, sizeof(char), 
		&lastTriggeredSensor,sizeof(char));

	    // set up train data
	    aCentralCtrl.createTrain( 
		trainNum, trainTid, lastTriggeredSensor);

	    sprintf(strWrite,
		"tr [%d] TRAIN Num %d created\n",
		aCentralCtrl.getTrainNum());
	    Write(strWrite, strLen(strWrite), WYSE);

	    createdNum++;
	}

    }

    //for(i=0;i<createdNum;i++) {
    // send msg to the selected trains
	//Send(aCentralCtrl.getTrainTid(i), NULL,NULL,NULL,NULL);
    //}

    aCentralCtrl.printTrain();

    sprintf(strWrite, "%d Trains are created\n", createdNum);
    Write(strWrite, strLen(strWrite), WYSE);

}


//
//  get track ID ( A or B)
//
void getTrackId( void ) {

    BYTE checkFlag;
    BYTE trackId;
    checkFlag = False;
    //console_xy(0,7);

    sprintf(strWrite, "\n\r TRACK ID (a or b)>>>>>\n\r");
    Write(strWrite, strLen(strWrite), WYSE);

    do{

	trackId = Get(WYSE);
	if(trackId == 'a' || trackId =='b' ||
	   trackId == 'A' || trackId =='B') { 
	    //console_xy(0,6);
	    //cprintf("                              ");
	    //console_xy(0,7);

	    sprintf(strWrite, " TRACK %c is selected\n\r", trackId);
	    Write(strWrite, strLen(strWrite), WYSE);

	    checkFlag = True;
	    aCentralCtrl.setTrackId( trackId );
	}
	else {
	    //console_xy(0,6);

	    sprintf(strWrite, " Invalid TrackId A or B please \n\r");
	    Write(strWrite, strLen(strWrite), WYSE);

	    //console_xy(0,7);

	    sprintf(strWrite, " TRACK ID (a or b)>>>>>\n\r" );
	    Write(strWrite, strLen(strWrite), WYSE);

	    checkFlag = False;
	}
    } while(!checkFlag);

    // send trackId
    //Send(PathFindTid, &trackId, sizeof(char), NULL,NULL);
    Send(Tid_pathFind, &trackId, sizeof(char), NULL,NULL);
    //Send(OneStepTid, &trackId, sizeof(char), NULL,NULL);
    Send(Tid_oneStep, &trackId, sizeof(char), NULL,NULL);
}

//
// get random node
//
BYTE getRandomNode( BYTE _lastSensor) {
// random destination

    int seedValue;
    char mod, num, sensor;


// XXX set seed value for random function
    seedValue = UpTime();
    srand(seedValue);

    
    do {
	//srand(MyTid());

	mod = rand();
	mod %=5;
	mod = abs(mod);
	num = rand();
	num %=16;
	num++;
	num = abs(num);

	sensor = SENSOR( (mod+'a'), num); 

	//sprintf(strWrite, 
	    //">>>>Random NODE [%c:%d] sen 0x%x \n\r",mod+'a', num,
	    //lastSensor);
	//Write(strWrite, strLen(strWrite), WYSE);



// do until get the value different from the last triggered one
    } while(sensor == _lastSensor);

	sprintf(strWrite, 
	    "\n\n\ntr >>>>Random NODE [%c:%d] sen 0x%x \n\n\n",mod+'a', num,
	    sensor);
	Write(strWrite, strLen(strWrite), WYSE);

	//Get(WYSE);


    return sensor;
}

//
// random run
//
void randomRun() {

    int i, tid;
    int randomRun;

    char randomSensor;
    char lastSensor;
    char trainNo;

    for(randomRun=0; randomRun < 4; randomRun++) {

	// make random mission for all selected trains
	for(i=0; i< aCentralCtrl.getTrainNum(); i++) {

	    trainNo = aCentralCtrl.getTrainNoFromIndex(i);

	    // get last sensor 
	    lastSensor = aCentralCtrl.getLastSensorFromNo(trainNo);

	    // get random destination
	    randomSensor = getRandomNode(lastSensor);

	    // get short path
	    getShortPath(trainNo, randomSensor);

    
	}// for inner

	// need to get finished signal from train
	for(i=0; i< aCentralCtrl.getTrainNum();i++) {
	    
	    trainNo = aCentralCtrl.getTrainNoFromIndex(i);

	    tid = Receive(&lastSensor, CMD2CC_SIZE*sizeof(char));

	    // just check the sender is Train Task
	    if(aCentralCtrl.isTrainTid(tid)){

		trainNo = aCentralCtrl.getTrainNumFromTid(tid);
		//lastSensor = dataFromUI[0];

		aCentralCtrl.setLastSensorFromNo(trainNo, lastSensor);

		sprintf(strWrite, ">>>Train[%d] finishes its mission with"
		    "sen[0x%x] \n", trainNo, lastSensor);
		Write(strWrite, strLen(strWrite), WYSE);
	    }

	    else // if msg is not from tokenizer
	    {
		sprintf(strWrite, 
		    "Warning!!! I(CC) am expecting the msg from TT\n");
		Write(strWrite, strLen(strWrite), WYSE);

	    } // else main

	    // reply to the sender, train
	    Reply(tid, NULL,NULL);


	} // for


	sprintf(strWrite, "\n\rOne Random mission finished\n");
	Write(strWrite, strLen(strWrite), WYSE);

    }// for outter

}

void getShortPath(int trainNo, char destination) {
//void getShortPath( void ) {

    char instruction[3];
    char index;
    char innerIndex;
    BYTE ch;
    BYTE i;
    BYTE mod;
    BYTE num;
    BYTE lastSensor;
    int	 trainTid;
    node shortPathNode[NUMSEGMENTS];
    oneHop stepsToFollow[NUMSEGMENTS];
    speedNode speedStep[NUMSEGMENTS];

    int seedValue;


// get the train tid
    trainTid = aCentralCtrl.getTrainTIDfromNo(trainNo);

    // instruction for shortpath 
    instruction[0] = 's'; 

    //console_xy(0,15);
    sprintf(strWrite, "\n\rShort path...for TID %d\n\r", trainTid);
    Write(strWrite, strLen(strWrite), WYSE);
#undef NO_START_POS

#ifdef	NO_START_POS

    sprintf(strWrite, ">>Enter Start sensor module(a,b,c,d,e)\n\r");
    Write(strWrite, strLen(strWrite), WYSE);

    mod = Get(WYSE);
    
    sprintf(strWrie, ">>Enter Start sensor number"
	"(1,.a(10),b(11),c(12),d(13),e(14),f(15))\n\r");
    Write(strWrite, strLen(strWrite), WYSE);

    num = Get(WYSE);
    // if a - f
    if(num >= 'a' ) {
	num = 10 + ( num -'a');
	instruction[1] = SENSOR( mod, num); 

	sprintf(strWrite, "Start NODE %x \n\r",SENSOR( mod, num ));
	Write(strWrite, strLen(strWrite), WYSE);

    } // if
    // if 1 - 9
    else  {
	num = ( num -'0');
	instruction[1] = SENSOR( mod, num); 

	sprintf(strWrie, "Start NODE %x \n\r",SENSOR( mod, num ));
	Write(strWrite, strLen(strWrite), WYSE);
    } // if

#else
    instruction[1] = lastSensor;
    sprintf(strWrite, "Start NODE from train %x \n\r",lastSensor ); 
    Write(strWrite, strLen(strWrite), WYSE);
    
#endif 	// NO_START_POS

#ifndef TOKENIZER_ENABLE
    sprintf(strWrite, ">>Enter End sensor module(a,b,c,d,e)\n\r");
    Write(strWrite, strLen(strWrite), WYSE);

    mod = Get(WYSE);

    sprintf(strWrite, ">>Enter End sensor number"
	"(1,.a(10),b(11),c(12),d(13),e(14),f(15))\n\r");
    Write(strWrite, strLen(strWrite), WYSE);

    num = Get(WYSE);

    // if a - f
    if(num >= 'a' ) {

	num = 10 + ( num -'a');
	instruction[2] = SENSOR( mod, num); 

	sprintf(strWrite, "End NODE %x \n\r",SENSOR( mod, num ));
	Write(strWrite, strLen(strWrite), WYSE);
    } // if
    // if 1 - 9
    else  {
	num = ( num -'0');
	instruction[2] = SENSOR( mod, num); 

	sprintf(strWrite, "End NODE %x \n\r",SENSOR( mod, num ));
	Write(strWrite, strLen(strWrite), WYSE);

    } // else 
#else	// TOKENIZER_ENABLE

    instruction[1] = aCentralCtrl.getLastSensorFromNo(trainNo);
    instruction[2] = destination;


#endif 	// TOKENIZER_ENABLE


    Send(Tid_pathFind, &instruction, 3 * sizeof(char),
	&shortPathNode, NUMSEGMENTS * sizeof(node));

    // send fast path to onestep
    Send(Tid_oneStep, &shortPathNode, NUMSEGMENTS * sizeof(node),
	&stepsToFollow, NUMSEGMENTS * sizeof(oneHop));

    sprintf(strWrite, "\n\rGot result from helpers\n\r");
    Write(strWrite, strLen(strWrite), WYSE);
#define SCHEDULAR
#ifdef SCHEDULAR
    for (i = 0; i < NUMSEGMENTS; i++) {
      if (stepsToFollow[i].distance == NIL) {
        stepsToFollow[i].direction = trainNo;
        break;
      }
    }

	// send one step info to Schedular 
	Send(Tid_schedular, &stepsToFollow, NUMSEGMENTS * sizeof(oneHop),
	    &speedStep, NUMSEGMENTS * sizeof(speedNode));

  if (speedStep[0].speed < 0) {	// Path is used, try again
    for (i = 0; i < NUMSEGMENTS; i++) {
      if (speedStep[i].sensorNum == NIL) {
        break;
      }
      // Generate new short path that might not cause collision
      shortPathNode[i].module = speedStep[i].module;
      shortPathNode[i].sensorNum = speedStep[i].sensorNum;
    }
    if (shortPathNode[0].sensorNum != NIL) {
      Send(Tid_oneStep, &shortPathNode, NUMSEGMENTS * sizeof(node),
          &stepsToFollow, NUMSEGMENTS * sizeof(oneHop));
      for (i = 0; i < NUMSEGMENTS; i++) {
        if (stepsToFollow[i].distance == NIL) {
          stepsToFollow[i].direction = trainNo;
          break;
        }
      }
      Send(Tid_schedular, &stepsToFollow, NUMSEGMENTS *
          sizeof(oneHop), &speedStep, NUMSEGMENTS * sizeof(speedNode));
    }
  }

#endif
    // XXX send command to train task
    Send(trainTid, &stepsToFollow, NUMSEGMENTS * sizeof(oneHop), NULL,NULL);
	
#undef DISPLAY_DATA
#ifdef DISPLAY_DATA
    index = 0;

    sprintf(strWrite, "\nCommand Sets ....\n\r");
    Write(strWrite, strLen(strWrite), WYSE);

    sprintf(strWrite, 
    "--------------------------------------------------------\n");
    Write(strWrite, strLen(strWrite), WYSE);

    while( stepsToFollow[index].distance != NIL && index < 10){

	sprintf(strWrite, "Dir:%2d Dist: %2d Senor[%c:%2d] >> ",
	    stepsToFollow[index].direction,
	    stepsToFollow[index].distance,
	    stepsToFollow[index].thisNode.module + 'a', 
	    stepsToFollow[index].thisNode.sensorNum );
	Write(strWrite, strLen(strWrite), WYSE);

	innerIndex = 0;
	while(
	  stepsToFollow[index].instlist[innerIndex].sequence != -1) {
	    sprintf(strWrite, " [Switch %2d TYPE %d] ", 
		stepsToFollow[index].instlist[innerIndex].sequence,
		stepsToFollow[index].instlist[innerIndex].setting);
	    Write(strWrite, strLen(strWrite), WYSE);
	    innerIndex++;
	} // inner while

	sprintf(strWrite, "\n");
	Write(strWrite, strLen(strWrite), WYSE);

	index++;
    } // while

    sprintf(strWrite, 
    "--------------------------------------------------------\n");
    Write(strWrite, strLen(strWrite), WYSE);

#endif // DISPLAY_DATA


    
    //cprintf("one step is generated. press q to quit or continiue \n");
    //ch = Get(WYSE);
    //if( ch=='q' || ch=='Q') {
	//quit = True;
    //}
//
    //for(i = 0;i<15; i++) {
	//console_xy(0,15 + i);
	//cprintf(
	//"								 ");
    //}
	
    //} // while

}
	
int getDataFrom_Tokenizer(void) {

    int tid;
    int trainNo, destination;
    char dataFromUI[CMD2CC_SIZE];
    BYTE lastSensor;

    while(1) {

	tid = Receive(&dataFromUI, CMD2CC_SIZE*sizeof(char));

	if(tid == Tid_tokenizer) {

	    // TRAIN COMMAND
	    if( dataFromUI[0] == TRN_CMD) {


		trainNo = dataFromUI[1];
		// now we are not using source
		// we use last triggered sensor
		// XXX source = dataFromUI[2];
		destination = dataFromUI[3];

		sprintf(strWrite, 
		    "TRAIN command from TOK train[%d], des[%x]\n",
		    trainNo, destination);
		Write(strWrite, strLen(strWrite), WYSE);

		getShortPath( trainNo, destination);
	    }
	    // halt command
	    else if( dataFromUI[0] == HALT_CMD) {

		sprintf(strWrite, "train HALT from TOK\n");
		Write(strWrite, strLen(strWrite), WYSE);

	    }
	    // mission command
	    else if( dataFromUI[0] == MISS_CMD) {

		sprintf(strWrite, "train MISS from TOK\n");
		Write(strWrite, strLen(strWrite), WYSE);

	    }
	    // random command
	    else if( dataFromUI[0] == RAND_CMD) {

		sprintf(strWrite, "train RAND from TOK\n");
		Write(strWrite, strLen(strWrite), WYSE);
		randomRun();

	    }
	    // init command
	    else if( dataFromUI[0] == INIT_CMD) {
		// some switch init calls...

		sprintf(strWrite, "train INIT from TOK\n");
		Write(strWrite, strLen(strWrite), WYSE);

	    } // else if 
	    else {
		cprintf("Warning !!! wrong data from tokenizer\n");
	    } // else

	} // if main

	// if from Train Task 
	else if(aCentralCtrl.isTrainTid(tid)){

	    
	    trainNo = aCentralCtrl.getTrainNumFromTid(tid);
	    lastSensor = dataFromUI[0];

	    aCentralCtrl.setLastSensorFromNo(trainNo, lastSensor);

	    sprintf(strWrite, ">>>Train[%d] finishes its mission with"
		"sen[0x%x] \n", trainNo, lastSensor);
	    Write(strWrite, strLen(strWrite), WYSE);
	}

	else // if msg is not from tokenizer
	{
	    sprintf(strWrite, 
		"Warning!!! I(CC) am expecting the msg from tokenizer\n");
	    Write(strWrite, strLen(strWrite), WYSE);
	} // else main

    // reply to the tokenizer or Train...
	Reply(tid, NULL,NULL);

    } // while

}

	


//
//---
//
//  Central Control Main function
//
//---
//

main() {

    char trainNo;
    int tid;
    BYTE trackId;
    Boolean checkFlag;

    char sendStr[100];

    char test = 18;


    RegisterAs( "CentralControlMain" );
    Delay(10);


    //tid_trainTaskMain 	= WhoIs( "TrainTaskMain");
    Tid_pathFind 	= WhoIs( "PathFind");
    Tid_oneStep 	= WhoIs( "OneStep");
    Tid_blackBoard 	= WhoIs( "BlackBoardMain");
    Tid_tokenizer 	= WhoIs("Tokenizer");
    Tid_schedular 	= WhoIs("Schedular");


    getTrackId();

    //createTrain();

    // multiple train
    createTrains();



// XXX send signal to tokenizer 
    Send(Tid_tokenizer, &selectedTrain,MaxTrainNum * sizeof(BYTE),NULL,NULL);

// XXX send blackboard 
    //Send(Tid_blackBoard, NULL,NULL,NULL,NULL);


    while( 1 ) {

	sprintf(strWrite, "\n\rCentral ctrl Main >>>\n\r");
	Write(strWrite, strLen(strWrite), WYSE);

// get data from tokenizer
	getDataFrom_Tokenizer();

	// XXX temp wait of finishing sign
	//tid = Receive(NULL,NULL);
	//Reply(tid,NULL,NULL);

	//console_xy(0,33);

    }

    Exit();


}
