/*
//--
//	ironBird.h
//
//	All the definitions for 
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

#ifndef _IRON_BIRD_H_
#define _IRON_BIRD_H_



typedef struct {
    char type;

} SensorMsg;

typedef struct {
    char type;
    unsigned sensor;
    char switchNo;
    char switchType;
    char speed;
    char extra;
} CommandSet;

/* command in general size of char type */
#define CMD_SIZE	3

#define CMD_TYPE	0

#define TRAIN_NUMBER	1
#define TRAIN_SPEED	2

#define SWITCH_NUMBER 	1
#define SWITCH_TYPE	2

#define	TRAIN_CMD	't'
#define SWITCH_CMD	'w'
#define SENSOR_CMD	's'

#define START		0x23
#define NORMAL		0x24
#define END		0x25

/* switch command type */
#define STRAIGHT	0x21
#define CURVE		0x22
#define SWITCH_CLOSE	0x20

#define FROM_TRAIN_TASK	0x12

/* delay for between commands */
#define DELAY_TIME_FOR_COMMAND		3
#define DELAY_TIME_SWITCH_CLOSE		2

/* emergency stop command and resume */
#define EMERGENCY_STOP		97
#define EMERGENCY_RESUME	96 


/* some TID definitions */
#define SensorSrv		8
#define SensorSched     	9
#define TrainTid        	7
#define CentralControlTid	10
/* this is not just sensor it send serial and get data form serial */
#define SensorTid		6
/* not working now */
#define	CommandSrv		4

/* priority definition */
#define SensorPriority		15	
#define SensorSchedPriority	14
#define SensorSrvPriority 	20	
#define TrainTaskPriority  	25		
#define CentralControlPriority	35		
/* not working now */
#define InputPriority		14	
#define CommandPriority	 	15	


#endif /* _IRON_BIRD_H_ */
