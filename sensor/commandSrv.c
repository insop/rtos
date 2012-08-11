/*
//--
//	commandSrv.c
//
//	train commnand out server
//
//	sensor command out and sensor data acqusition
//	speedy command and switch command out
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

#define	SINGLE_SENSOR	0xC1
#define	MULTI_SENSOR	0x85

/*
#define	CommandSrv	4
#define	SensorSrv	8
#define	SensorSched	9
#define TrainTid	7
*/

/* for sensor acqusitino mode */
/* sigle for module by modle */
/* multi for 10 modules in a row */
#undef SINGLE
#define	MULTI

int senModule = 0;
char sensorData[11];
char sensorDataOld[11];
char sharedMemory_sensorData[10];

void sensorOut(void);

main() 
{
    int i;
    int tid;
    int * pData = (int *)&sharedMemory_sensorData;
    char sendData = 's';
    char sensorCnt = 0;
    int delayTime;
    /* receive command */
    char cmd[CMD_SIZE];
    char setTrain=12;
    char speedy = 0;
    char senCnt = 0;
    char inputted =0;
    char sensorSent = 0;
    char dataSent = 0;
    int x;
    char dataIn = 0;
    int send=13;
    int reply=0;
    int tid_sensorServer;

    RegisterAs( "TrainCommandSrv" );
    Delay(10);
    tid_sensorServer = WhoIs( "SensorSrv");
    /*
    cprintf(">>sensor.c:sensor srv %d\n", tid_sensorServer);
    */

/* for magic number check */
    sharedMemory_sensorData[0] =
	sharedMemory_sensorData[9] = SHARED_MAGIC;
    sensorData[10] = '\0';

    Send(tid_sensorServer, &pData, sizeof(int), NULL,NULL); 
    /* Send(SensorSrv, &pData, sizeof(int), NULL,NULL); */
    Delay(5);
/*  console_xy(0,10);
    cprintf("Sensor Data : ");
    console_xy(0,9);
    cprintf("Train Command Srv : ");
*/
    for(i = 0; i<10;i++) {
	sharedMemory_sensorData[i] = sensorDataOld[i] = 0x00;
    }

    while(1) {
	tid = Receive(&cmd,CMD_SIZE * sizeof(char));
	Reply(tid,NULL,NULL);
	if( cmd[CMD_TYPE] == SENSOR_CMD) {
	    if( dataSent == 1) {
		dataSent =0;
	    } 
	    else {
		sensorOut();
		senModule++;
		senModule %= 5; 
	    } 
	}
	/* train speed */
	else if(cmd[CMD_TYPE] = TRAIN_CMD) {
	    Delay(DELAY_TIME_FOR_COMMAND);
	    Put( cmd[TRAIN_SPEED], TRAIN );
	    Put( cmd[TRAIN_NUMBER] , TRAIN );
	    Delay(DELAY_TIME_FOR_COMMAND);
	}
	/* switch command */
	else if(cmd[CMD_TYPE] = SWITCH_CMD) {
	    Delay(DELAY_TIME_FOR_COMMAND-1);
	    Put( cmd[SWITCH_TYPE]   , TRAIN );
	    Put( cmd[SWITCH_NUMBER] , TRAIN );
	    Delay(DELAY_TIME_SWITCH_CLOSE);
	    Put( SWITCH_CLOSE       , TRAIN );
	    Delay(DELAY_TIME_FOR_COMMAND);
	} /* else if */
    } /* while */
    Exit();
}

int fire;

void sensorOut( void ) {
    int i;
#ifdef  SINGLE
    if(senModule == 0)
	fire = UpTime();

	/* getting the sensor data from serial port */
    
    Put(SINGLE_SENSOR + senModule, TRAIN);
    for(i=0;i<2;i++) {
	sensorData[ senModule + i ] = Get( TRAIN );

	/* save sharedMemory_sensorData area */
	/* XXX or make two pair of sensor data */
	if ( sensorData[ senModule + i] !=
		sensorDataOld[senModule + i]) {
	    enterCS();
	    sharedMemory_sensorData[ senModule + i] 
		= sensorData[ senModule + i];
	    exitCS();
	} /* if */

    } /* for */
    if(senModule == 4 ) {
	fire = UpTime() - fire;
	console_xy(27,12);
	cprintf("SenTime: %d", fire);
    }
#endif /* SINGLE */

#ifdef	MULTI
    fire = UpTime();
    Put(MULTI_SENSOR , TRAIN);
    /* getting the sensor data from serial port */
    for(i=0;i<10;i++) {
	sensorData[i] = Get( TRAIN );

	/* save sharedMemory_sensorData area */
	/* XXX or make two pair of sensor data */
	if ( sensorData[i] != sensorDataOld[i]) {
	    enterCS();
	    sharedMemory_sensorData[i] = sensorData[i];
	    exitCS();
	} /* if */
	sensorDataOld[i] = sensorData[i];
    } /* for */
    fire = UpTime() - fire;
    console_xy(0,6);
    cprintf("Sensing Time: %d", fire);
#endif /* MULTI */
}










	    

	/* save only if the data was changed */
/*
	for(i = 0; i<10; i++) {
	    if ( sensorData[i] != sensorDataOld[i]) {
		enterCS();
		sharedMemory_sensorData[i] = sensorData[i];
		exitCS();
	    }
	}
*/

/*
	for(i=0;i<10;i++) {
	    console_xy(20+i,10);
	    cprintf("%x", sharedMemory_sensorData[i]);
	    console_xy(19,10);
	    cprintf(" ");
	}
*/

	/* save the data as old now */
/*
	for(i = 0; i<10;i++)
	    sensorDataOld[i] = sensorData[i];

}
*/
    /* some testing code */

/*
    if(sensorData[3] == 1) {
	sensorCnt++;
	console_xy(15,10);
	cprintf("%d*", sensorCnt);
	Send(TrainTid, &sendData, sizeof(char),NULL,NULL);
    }
    */
