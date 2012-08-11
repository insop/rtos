#include "../../syscall/sysCall.h"
#include "../../include/i486/cs452.h"

/*#include "../include/util/type.h"*/

#define	CommandSrv	4
#define TrainTid	7
#define SensorTid	6

char sharedMemory_sensorData[10];

void interpreteData( void );

main() {

    int  SensorMem;
    int  tid;
    int  memData;
    int  i;

    /* get absolute data memory address of TID */
    SensorMem = GetAbsMem(SensorTid);

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

    Delay(TICKS_PER_SEC * 2);

    while(1) {

	enterCS();
	absRead(SensorMem + memData, 
	    &sharedMemory_sensorData, 10 * sizeof(char));
	exitCS();

	interpreteData();


	for(i=0;i<10;i++) {
	    console_xy(20+i,10);
	    cprintf("%x", sharedMemory_sensorData[i]);
	    console_xy(19,10);
	    cprintf(" ");
	}


	Delay(2);
    }
}


    
/*
*  interpreteData(pDisplayBuffer)
*
*  process raw sensor data 
*  and put these into display buffer 
*  with time stamp
*
*/
void interpreteData( void ) {

     static char senRow, senCol;
     static char sensor0, sensor1;

     /*Error(ErrorNone,"sensor interpreteData START>");*/

     for(senRow=0;senRow<5;senRow++) {
	sensor0=sharedMemory_sensorData[senRow*2];
	sensor1=sharedMemory_sensorData[senRow*2+1];
	for(senCol=0;senCol<8;senCol++) {
	   if(sensor0 & (0x80)) 
	   {
	      console_xy(19,10);
	      cprintf("Mod: %d", senRow);
	      console_xy(27,10);
	      cprintf("SenNo: %d", senCol);
	      /* put corresponding sensor data with 
		 time into display buffer */

	      /*
	      putDisplayBuffer(senCol, senRow, _pDisBuffer);

	      Error2(ErrorNone,
		"sensorPutData module %d, num %d",
		senRow, senCol);
	      */
	   }
	   sensor0 <<=1;

	   if(sensor1 & (0x80)) 
	   {
	      console_xy(19,10);
	      cprintf("Mod: %d", senRow);
	      console_xy(27,10);
	      cprintf("SenNo: %d", senCol+7);
	      /* put corresponding sensor data with 
		 time into display buffer */
	      /*
	      putDisplayBuffer(senCol+8, senRow, _pDisBuffer);

	      Error2(ErrorNone,
		"sensorPutData module %d, num %d",
		senRow, senCol+8);
	      */
	   }
	   sensor1 <<=1;
	}

     } /* for */


}

