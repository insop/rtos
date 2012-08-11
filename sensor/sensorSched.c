/*
//--
//	sensorSched.c
//
//	scheduler for sensor data acqusition
//	now, this is setted 2 ticks(10Hz, 100 msec)
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
#include "../train/ironBird.h"

/*
#define	CommandSrv	4
#define	SensorSrv	8
#define	SensorTid	6
#define TrainTid	7
*/

main() {
    char sendData='s';

    int	tid_trainCommandSrv;

    RegisterAs("SensorSched");
    Delay(10);

    tid_trainCommandSrv = WhoIs( "TrainCommandSrv");
    /*
    cprintf(">>sensorSched.c:train command srv %d\n", tid_trainCommandSrv);
    */

/* need add name handling jobs here */

    while( 1 ) {
	/* make the sensor job 1 tick(50 msec) period */
	Send(tid_trainCommandSrv,&sendData,sizeof(char),NULL,NULL);
	/*
	Send(SensorTid,&sendData,sizeof(char),NULL,NULL);
	*/
	Delay( SENSOR_SCHEDULING_TICKS );
    } /* while */


}
	    

