/*
//--
//	train.c
//
//	TrainTask main file
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
*/

#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"
#include "ironBird.h"

/*
#define	CommandSrv	4
#define SensorSrv	8
*/

/* this is not just sensor it send serial and get data form serial 
#define SensorTid	6
*/

CommandSet sampleCommand[] ={
/* type  sensor swNo   switchType speed extra*/
{ START , 0x00, 3,  	STRAIGHT, 4, 	0}, /* start */
{ NORMAL, 0x04, 5,  	CURVE,    7, 	0}, /* a4->  5  C */
{ NORMAL, 0x26, 0,  	0	, 7, 	0}, /* c7->  0  0 */
{ NORMAL, 0x4a, 9,  	CURVE   , 7, 	0}, /* e11-> 9  C */
{ NORMAL, 0x39, 10 , 	STRAIGHT,  9, 	0}, /* d10-> 10  s */
{ NORMAL, 0x2b, 15 , 	STRAIGHT,  9, 	0}, /* c12-> 15  s */
{ NORMAL, 0x1f, 6 , 	CURVE   ,  9, 	0}, /* b16-> 6  c */
{ NORMAL, 0x24, 5 , 	STRAIGHT,  5, 	0}, /* c5-> 5  s */
{ NORMAL, 0x22, 0 , 	0       , 0, 	0}, /* c3->  0   0 */

{ NORMAL, 0x42, 0 , 	0       , 0, 	0}, /* e3->  0  0 */
{ END,    0x37, 0 , 	CURVE   , 0, 	0}


};


main() {

    int i;
    char sensorData[11];
    char getData = 's';
    char sendData;
    char ch;
    char started = 0;
    int tid;
    int commandIndex = 0;
    char trainNo;

    char cmd[CMD_SIZE];

    Delay(2);

    console_xy(0,10);
    cprintf("Sensor Data : ");


    Put('t', WYSE);
    tid = Receive(&trainNo, sizeof(char));
    Reply(tid, NULL,NULL);

    Put('r', WYSE);
    /*
    trainNo = Get(WYSE);
    Put(trainNo, WYSE);
    */

    if(trainNo =='a') {
	trainNo = 12;
    }
    else if(trainNo =='b') {
	trainNo = 55;
    }
    else if(trainNo =='c') {
	trainNo = 57;
    }


    while(1) {

	/* asking sensor srv for sensor data */
	ch = FROM_TRAIN_TASK;
	Send(SensorSrv, &ch, sizeof(char), &sensorData, 10 * sizeof(char));

	i = 0;
	console_xy(0,20);
	cprintf("                                 "); 
	console_xy(0,20);

	/* display sensor data for testing */
	for(i=0;i<10;i++) {
	    if ( sensorData[i] != 0) {
		cprintf("%d:<%x>", i, (sensorData[i]));
	    }
	} 

	
	/* start the train */
	if(!started) {

	    /* initial switch command set */
	    cmd[CMD_TYPE] = SWITCH_CMD; 
	    cmd[SWITCH_NUMBER] = sampleCommand[commandIndex].switchNo;
	    cmd[SWITCH_TYPE] = sampleCommand[commandIndex].switchType;
	    Send(SensorTid, &cmd, CMD_SIZE * sizeof(char), NULL, NULL);

	    /* initial train command set */
	    cmd[CMD_TYPE] = TRAIN_CMD; 
	    cmd[TRAIN_NUMBER] = trainNo;
	    cmd[TRAIN_SPEED] = sampleCommand[commandIndex].speed;
	    Send(SensorTid, &cmd, CMD_SIZE * sizeof(char), NULL, NULL);

	    started = 1;
	    commandIndex++;
	}

	if(sensorData[0] !=0) {
	    if( sensorData[0] == sampleCommand[commandIndex].sensor ){
		cprintf("Bingo CI %d", commandIndex);
		Put('C',WYSE);

		/* switch command set */

		if(sampleCommand[commandIndex].switchNo != 0 ) {
		    cmd[CMD_TYPE] = SWITCH_CMD; 
		    cmd[SWITCH_NUMBER] = sampleCommand[commandIndex].switchNo;
		    cmd[SWITCH_TYPE] = sampleCommand[commandIndex].switchType;
		    Send(SensorTid, &cmd, CMD_SIZE * sizeof(char), NULL, NULL);
		}

		/* train command set */
		cmd[CMD_TYPE] = TRAIN_CMD; 
		cmd[TRAIN_SPEED] = sampleCommand[commandIndex].speed;
		cmd[TRAIN_NUMBER] = trainNo;
		Send(SensorTid, &cmd, CMD_SIZE * sizeof(char), NULL, NULL);

		commandIndex++;
	    }
	}

	Delay(2);

    } /* while */
}
	    

