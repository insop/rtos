//
//--
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

#include "../central_control/centralControl.h"

extern "C" {
    #include "../include/i486/cs452.h"
    #include "../include/util/type.h"
    #include "../syscall/sysCall.h"
    #include "../train/ironBird.h"
    #include "../pathfind/graphtype.h"
    #include "../schedular/schedulartype.h"
}; // C function interface 

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
int	Tid_schedular;

// string for Write;
char 	strWrite[100];

// string for Read;
char 	strRead[50];

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

// checked sensor for their first run.
BYTE	checkedSensor[MAX_TRAIN_NUM];


// XXX test multi train
int multiTrainIndex = 0;

//
// create all trains 
//
void createTrains( void ) {

    BYTE checkFlag;
    BYTE trainNum;
    BYTE createdNum = 0;

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

	    //trainNum = (BYTE)train52;
	    trainNum = TrainNumbers[i]; 
	    Send( trainTid, &trainNum, sizeof(char), NULL,NULL);
	    aCentralCtrl.createTrain( trainNum, trainTid );

	    sprintf(strWrite,
		"tr [%d] TRAIN Num %d created\n",
		aCentralCtrl.getTrainNum());
	    Write(strWrite, strLen(strWrite), WYSE);

	    createdNum++;
	}

    }

    for(i=0;i<createdNum;i++) {
	Send(aCentralCtrl.getTrainTid(i), NULL,NULL,NULL,NULL);
    }

    aCentralCtrl.printTrain();

    sprintf(strWrite, "%d Trains are created\n", createdNum);
    Write(strWrite, strLen(strWrite), WYSE);

}


//
//
// send message to the train that have been created 
// make them ready and check their last tripped sensors
//
//
void onYourMark( void ) {

    int i; 
    int dataToTrain;

    for(i = 0; i< aCentralCtrl.getTrainNum(); i++) {

	dataToTrain = CC_TT_FIRST_DATA;
	Send(aCentralCtrl.getTrainTid( i ), 
	checkedSensor,sizeof(checkedSensor)
	,&checkedSensor[i],sizeof(BYTE));

    }

}




//
//  get track ID ( A or B)
//
void getTrackId( void ) {

    BYTE checkFlag;
    BYTE trackId;
    checkFlag = False;
    //console_xy(0,7);

    sprintf(strWrite, "\n TRACK ID (a or b)>>>>>\n");
    Write(strWrite, strLen(strWrite), WYSE);

    do{

	trackId = Get(WYSE);
	// XXX
	if(trackId == '\n')
	    trackId = Get(WYSE);

	if(trackId == 'a' || trackId =='b' ||
	   trackId == 'A' || trackId =='B') { 
	    //console_xy(0,6);
	    //cprintf("                              ");
	    //console_xy(0,7);

	    sprintf(strWrite, " TRACK %c is selected\n", trackId);
	    Write(strWrite, strLen(strWrite), WYSE);

	    checkFlag = True;
	    aCentralCtrl.setTrackId( trackId );
	}
	else {
	    //console_xy(0,6);

	    sprintf(strWrite, " Invalid TrackId A or B please \n");
	    Write(strWrite, strLen(strWrite), WYSE);

	    //console_xy(0,7);

	    sprintf(strWrite, " TRACK ID (a or b)>>>>>\n" );
	    Write(strWrite, strLen(strWrite), WYSE);

	    checkFlag = False;
	}
    } while(!checkFlag);

    // send trackId

    //Send(PathFindTid, &trackId, sizeof(char), NULL,NULL);
    Send(Tid_pathFind, &trackId, sizeof(char), NULL,NULL);

    //Send(OneStepTid, &trackId, sizeof(char), NULL,NULL);
    Send(Tid_oneStep, &trackId, sizeof(char), NULL,NULL);

    // send track ID for Schedular
    Send(Tid_schedular, &trackId, sizeof(char), NULL,NULL);
}

void getShortPath( void ) {

    char instruction[3];
    char index;
    char innerIndex;
    BYTE ch;
    //BYTE i;
    BYTE mod;
    BYTE num;
    BYTE lastSensor;
    int	 trainTid;
    node shortPathNode[NUMSEGMENTS];
    oneHop stepsToFollow[NUMSEGMENTS];

    // speed node of Schedular
    speedNode speedStep[NUMSEGMENTS];

    // b9 to e8
    char a[2]={0x18, 0x47};
    // b7 to e5
    char b[2]={0x16, 0x44};

    // get last sensor data from TT 
    trainTid = Receive(&lastSensor, sizeof(lastSensor));


// test;;
    lastSensor = a[0];

    //Boolean quit = False;
    //while( !quit ) {
	// for shortpath
	instruction[0] = 's'; 

	//console_xy(0,15);
	sprintf(strWrite, "\nShort path...\n");
	Write(strWrite, strLen(strWrite), WYSE);
	instruction[1] = lastSensor;
	sprintf(strWrite, "Start NODE from train 0x%x \n",lastSensor ); 
	Write(strWrite, strLen(strWrite), WYSE);
	

	//sprintf(strWrite, ">>Enter End sensor module(a,b,c,d,e)\n");
	//Write(strWrite, strLen(strWrite), WYSE);


	//mod = Get(WYSE);

	//sprintf(strWrite, "GOT sensor module %c\n", mod);
	//Write(strWrite, strLen(strWrite), WYSE);
	// XXX
	//if(mod == '\n')
	 //   mod = Get(WYSE);

	//sprintf(strWrite, ">>Enter End sensor number"
	    //"(1,.a(10),b(11),c(12),d(13),e(14),f(15))\n");
	//Write(strWrite, strLen(strWrite), WYSE);

	
	//num = Get(WYSE);

	//sprintf(strWrite, "GOT sensor number %d\n", num);
	//Write(strWrite, strLen(strWrite), WYSE);

	// if a - f
	//if(num >= 'a' ) {

	    //num = 10 + ( num -'a');
	    //instruction[2] = SENSOR( mod, num); 

	    //sprintf(strWrite, "End NODE %x \n",SENSOR( mod, num ));
	    //Write(strWrite, strLen(strWrite), WYSE);
	//} // if
	// if 1 - 9
	//else  {
	    //num = ( num -'0');
	    //instruction[2] = SENSOR( mod, num); 
	instruction[2] = a[1]; 

	    sprintf(strWrite, "End NODE %x \n",SENSOR( mod, num ));
	    Write(strWrite, strLen(strWrite), WYSE);

	//} // else 
	
	// start node c15 
	//instruction[1] = 0x2f; 
	// end node   d12 
	//instruction[2] = 0x3b; 

	// start node b9
	//instruction[1] = 0x18; 
	// end node   a16
	//instruction[2] = 0x0f; 

	sprintf(strWrite, "before path find \n");
	Write(strWrite, strLen(strWrite), WYSE);


	Send(Tid_pathFind, &instruction, 3 * sizeof(char),
	    &shortPathNode, NUMSEGMENTS * sizeof(node));

	sprintf(strWrite, "path find \n");
	Write(strWrite, strLen(strWrite), WYSE);

	// send fast path to onestep
	Send(Tid_oneStep, &shortPathNode, NUMSEGMENTS * sizeof(node),
	    &stepsToFollow, NUMSEGMENTS * sizeof(oneHop));

	sprintf(strWrite, "one step find \n");
	Write(strWrite, strLen(strWrite), WYSE);

	for (int i = 0; i < NUMSEGMENTS; i++) {
	    if (stepsToFollow[i].distance == NIL) {
		stepsToFollow[i].direction =
		aCentralCtrl.getTrainTid(0);
		break;
	    }
	}

	// send one step info to Schedular 
	Send(Tid_schedular, &stepsToFollow, NUMSEGMENTS * sizeof(oneHop),
	    &speedStep, NUMSEGMENTS * sizeof(speedNode));

	if (speedStep[0].speed < 0) {	// Path is used, try again
	    for (int i = 0; i < NUMSEGMENTS; i++) {
		if (speedStep[i].sensorNum == NIL) {
		    break;
		}
		shortPathNode[i].module = speedStep[i].module;
		shortPathNode[i].sensorNum = speedStep[i].sensorNum;
	    }
	    if (shortPathNode[0].sensorNum != NIL) {
		Send(Tid_oneStep, &shortPathNode, NUMSEGMENTS * sizeof(node),
		    &stepsToFollow, NUMSEGMENTS * sizeof(oneHop));
		Send(Tid_schedular, &stepsToFollow, NUMSEGMENTS *
		    sizeof(oneHop), &speedStep, NUMSEGMENTS *
		    sizeof(speedNode));
	    }
	}

	//Send(aCentralCtrl.getTrainTid(0), &speedStep[0].speed, sizeof(int), NULL, NULL);

	sprintf(strWrite, "schedular find \n");
	Write(strWrite, strLen(strWrite), WYSE);


	sprintf(strWrite, "\nGot all result from helpers\n");
	Write(strWrite, strLen(strWrite), WYSE);

	// XXX send command to train task
	//Send(aCentralCtrl.getTrainTid( 0 ), &stepsToFollow, 
	    //NUMSEGMENTS * sizeof(oneHop), NULL,NULL);

	// XXX XXX finally, return path to train
	Reply(trainTid, &stepsToFollow, 
	    NUMSEGMENTS * sizeof(oneHop));
	// XXX XXX
		







    

//----------------------------------------------------------------------

    // get last sensor data from TT 
    trainTid = Receive(&lastSensor, sizeof(lastSensor));


// test;;
    lastSensor = b[0];

    //Boolean quit = False;
    //while( !quit ) {
	// for shortpath
	instruction[0] = 's'; 

	//console_xy(0,15);
	sprintf(strWrite, "\nShort path...\n");
	Write(strWrite, strLen(strWrite), WYSE);
	instruction[1] = lastSensor;
	sprintf(strWrite, "Start NODE from train 0x%x \n",lastSensor ); 
	Write(strWrite, strLen(strWrite), WYSE);
	

	//sprintf(strWrite, ">>Enter End sensor module(a,b,c,d,e)\n");
	//Write(strWrite, strLen(strWrite), WYSE);


	//mod = Get(WYSE);

	//sprintf(strWrite, "GOT sensor module %c\n", mod);
	//Write(strWrite, strLen(strWrite), WYSE);
	// XXX
	//if(mod == '\n')
	 //   mod = Get(WYSE);

	//sprintf(strWrite, ">>Enter End sensor number"
	    //"(1,.a(10),b(11),c(12),d(13),e(14),f(15))\n");
	//Write(strWrite, strLen(strWrite), WYSE);

	
	//num = Get(WYSE);

	//sprintf(strWrite, "GOT sensor number %d\n", num);
	//Write(strWrite, strLen(strWrite), WYSE);

	// if a - f
	//if(num >= 'a' ) {

	    //num = 10 + ( num -'a');
	    //instruction[2] = SENSOR( mod, num); 

	    //sprintf(strWrite, "End NODE %x \n",SENSOR( mod, num ));
	    //Write(strWrite, strLen(strWrite), WYSE);
	//} // if
	// if 1 - 9
	//else  {
	    //num = ( num -'0');
	    //instruction[2] = SENSOR( mod, num); 
	instruction[2] = b[1]; 

	    sprintf(strWrite, "End NODE %x \n",SENSOR( mod, num ));
	    Write(strWrite, strLen(strWrite), WYSE);

	//} // else 
	
	// start node c15 
	//instruction[1] = 0x2f; 
	// end node   d12 
	//instruction[2] = 0x3b; 

	// start node b9
	//instruction[1] = 0x18; 
	// end node   a16
	//instruction[2] = 0x0f; 

	sprintf(strWrite, "before path find \n");
	Write(strWrite, strLen(strWrite), WYSE);


	Send(Tid_pathFind, &instruction, 3 * sizeof(char),
	    &shortPathNode, NUMSEGMENTS * sizeof(node));

	sprintf(strWrite, "path find \n");
	Write(strWrite, strLen(strWrite), WYSE);

	// send fast path to onestep
	Send(Tid_oneStep, &shortPathNode, NUMSEGMENTS * sizeof(node),
	    &stepsToFollow, NUMSEGMENTS * sizeof(oneHop));

	sprintf(strWrite, "one step find \n");
	Write(strWrite, strLen(strWrite), WYSE);

	for (i = 0; i < NUMSEGMENTS; i++) {
	    if (stepsToFollow[i].distance == NIL) {
		stepsToFollow[i].direction =
		aCentralCtrl.getTrainTid(0);
		break;
	    }
	}

	// send one step info to Schedular 
	Send(Tid_schedular, &stepsToFollow, NUMSEGMENTS * sizeof(oneHop),
	    &speedStep, NUMSEGMENTS * sizeof(speedNode));

	if (speedStep[0].speed < 0) {	// Path is used, try again
	    for (int i = 0; i < NUMSEGMENTS; i++) {
		if (speedStep[i].sensorNum == NIL) {
		    break;
		}
		shortPathNode[i].module = speedStep[i].module;
		shortPathNode[i].sensorNum = speedStep[i].sensorNum;
	    }
	    if (shortPathNode[0].sensorNum != NIL) {
		Send(Tid_oneStep, &shortPathNode, NUMSEGMENTS * sizeof(node),
		    &stepsToFollow, NUMSEGMENTS * sizeof(oneHop));
		Send(Tid_schedular, &stepsToFollow, NUMSEGMENTS *
		    sizeof(oneHop), &speedStep, NUMSEGMENTS *
		    sizeof(speedNode));
	    }
	}

	//Send(aCentralCtrl.getTrainTid(0), &speedStep[0].speed, sizeof(int), NULL, NULL);

	sprintf(strWrite, "schedular find \n");
	Write(strWrite, strLen(strWrite), WYSE);


	sprintf(strWrite, "\nGot all result from helpers\n");
	Write(strWrite, strLen(strWrite), WYSE);

	// XXX send command to train task
	//Send(aCentralCtrl.getTrainTid( 0 ), &stepsToFollow, 
	    //NUMSEGMENTS * sizeof(oneHop), NULL,NULL);

	// XXX XXX finally, return path to train
	Reply(trainTid, &stepsToFollow, 
	    NUMSEGMENTS * sizeof(oneHop));
	// XXX XXX
	
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
    Tid_schedular 	= WhoIs( "Schedular");

    // get track Id
    getTrackId();

    // create all trains and wait.
    createTrains();

    // make the created train ready check last tripped sensor
    //onYourMark();


    while( 1 ) {
	sprintf(strWrite, "\nCentral ctrl Main >>>\n");
	Write(strWrite, strLen(strWrite), WYSE);

	getShortPath();
	// XXX temp wait of finishing sign
	tid = Receive(NULL,NULL);
	Reply(tid,NULL,NULL);

	//console_xy(0,33);

    }

    Exit();


}
