/*
//--
//	sensorSrv.h
//
//	sensor server
//	which provide sensor data in 
//	packed form(higher nibble(0: A module, 4: E module,
//		    lower nibble( 0: #1 sensor, f: #16 sensor))
//	this read sensor data from sensor.c using absRead()
//	like shared memory read(with disable interrupt)
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
#include "../train/ironBird.h"

/*#include "../include/util/type.h"*/

/*
#define	CommandSrv	4
#define TrainTid	7
#define SensorTid	6
*/

/* shared memory sensor data */
char sharedMemory_sensorData[10];

/* sensor data for send */
unsigned char sensorSendData[10];


/*
*  interpreteData(pDisplayBuffer)
*
*  process raw sensor data 
*  and put these into display buffer 
*  with time stamp
*
*/

char senModChar[10] = { 'a','a', 'b','b', 'c','c', 'd','d','e','e'};
char bitC[8] = {0x80,0x40,0x20,0x10,0x08,0x40,0x02,0x01};
unsigned char upperIndex[10] =
{0x00,0x00,0x10,0x10,0x20,0x20,0x30,0x30,0x40,0x40};


void interpreteData( void ) 
{
    register char i, bit, bitComp;
    register char senCnt;
     /*Error(ErrorNone,"sensor interpreteData START>");*/
/*
     for(i=0;i<10;i++) {
	  console_xy(0,7+i);
	  cprintf(" "); 
     }
*/
     /* trigger sensor counter */
     senCnt = 0;
     for( i = 0 ; i < 10 ; i++) {
	 if(sharedMemory_sensorData[i] != 0) {
	     bitComp = sharedMemory_sensorData[i];

	     for( bit = 0 ; bit < 8 ; bit++){
		 if( (bitComp & (0x80)) != 0  ) {
		      /*console_xy(0,7+i);*/
		      /*cprintf("* M: %c,    ", senModChar[i]); */
		      /*console_xy(10,7+i);*/
		      if((i%2) == 1) {
			  /*cprintf("SenNo: %d    ", bit+9);*/
			  sensorSendData[senCnt] = 
			    (upperIndex[i]) | (bit+8);
		      }
		      else {
			  /*cprintf("SenNo: %d     ", bit+1);*/
			  sensorSendData[senCnt] = 
			    (upperIndex[i]) | (bit);
		      }
		      senCnt++;
		 }/* if */
		 bitComp<<=1;
	     }/* for */
	 }  /*if */
     } /* for */
     /* if there is no sensor then it is ZERO*/
     /* XXX 
     sensorSendData[senCnt] = 0;
     */
     sensorSendData[senCnt] = 0xff;
     /*cprintf("[0] %x sensor cnt %d ", sensorSendData[0], senCnt);*/
}

main() 
{
    int  SensorMem, tid, memData, i;
    char type;
    SensorMsg senMsg;

    RegisterAs("SensorSrv");
    Delay(10);

    /* get absolute data memory address of TID */
    SensorMem = GetAbsMem(SensorTid);
    /* wait for send from trainCommandSrv(=sensor.c) */
    tid = Receive( &memData, sizeof(int));
    Reply(tid,NULL,NULL);
/* read the shared memory */
    enterCS();
    absRead(SensorMem + memData, 
	&sharedMemory_sensorData, 10 * sizeof(char));
    exitCS();
/* check magic number for shared memory */
    if(sharedMemory_sensorData[0] != SHARED_MAGIC &&
	sharedMemory_sensorData[9] != SHARED_MAGIC) {
	cprintf("WARNING !!! shared memory error \n");
    }

    for(i=0;i<10;i++) {
	sensorSendData[i] = sharedMemory_sensorData[i] = 0; 
    }

    Delay(TICKS_PER_SEC * 2);
    while(1) {
	/* waiting for the request*/
	tid = Receive( &type, sizeof(char) );
	enterCS();
	absRead(SensorMem + memData, 
	    &sharedMemory_sensorData, 10 * sizeof(char));
	exitCS();
	interpreteData();
	/* from train task */
	if( type == FROM_TRAIN_TASK ) {
	    Reply(tid, &sensorSendData, 10 * sizeof(char));
	}
	/* some unknown sender */
	else {
	    Reply(tid,NULL,NULL);
	} /* else */
	/*
	for(i=0;i<10;i++) {
	    console_xy(20+i,10);
	    cprintf("%x", sharedMemory_sensorData[i]);
	}
	*/
	/*Delay(2);*/
    } /* while */
}


    
	 

