/*
//--
//	sensorReporter.c
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
    char strWrite[20];

    int	tid_sensorSrv;

    unsigned char sensorSendData[5], sensor;

    RegisterAs("SensorReporter");
    Delay(10);

    tid_sensorSrv = WhoIs( "SensorSrv");

    while( 1 ) {
	Send(tid_sensorSrv,NULL,NULL, &sensorSendData,5*sizeof(char));
	sensor = sensorSendData[0];
	sprintf(strWrite,"se %c %d\n",SEN_MODULE_CHAR(sensor),
	    SEN_NUMBER(sensor));
	Write(strWrite, strLen(strWrite), WYSE);
	Delay( 5 );
    } /* while */


}
	    

