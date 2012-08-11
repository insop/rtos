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


#include "../include/util/type.h"

/* different system  clock */
#define	SYSTEM_CLOCK_20
#undef  SYSTEM_CLOCK_100

/* 
    Caution use small letter not capital
    macro for changing packed sensor form 
    module A 10 will be 0x09
    module B 1  will be 0x10
    BUG>>> number can not be 16
    EX> SENSOR('a', 10);


    !!! known bugs, SENSOR('a',0) gives 00
    which is interpreted as no sensor...
    .. So, checke this
*/
#define SENSOR(mod, no) (((mod - 'a')<<4) | (no-1))

/* get sensor module in character form(a,b,..) from packed sensor data */
#define SEN_MODULE_CHAR(sen) (((sen & 0xf0)>>4)+'a')
/* get sensor module in number form(1(=a), 2...) from packed sensor data */
#define SEN_MODULE_NUM(sen) (((sen & 0xf0)>>4)+1)
/* get sensor number form(1..16) from packed sensor data */
#define SEN_NUMBER(sen) ((sen & 0x0f)+1)

typedef struct {
    char type;

} SensorMsg;

#define MAX_COMMAND_SET		50

/* XXX this is obsolete data type */
typedef struct {
    char type;
    unsigned sensor;
    char switchNo;
    char switchType;
    char speed;
    char extra;
} CommandSet;
/*typedef struct DataCommnicate;*/

/* train enumerator type */
enum TaskType { train01 = 1, train06 = 6, train12 = 12, train52 = 52, 
    train55 = 55, train57 = 57, trainLast, centralControl, blackBoard, oneStep, pathFind};
typedef enum TaskType TaskType;

enum TrainIndex { train01Index, train06Index, train12Index, train52Index , 
    train55Index , train57Index, trainLastIndex };
typedef enum TrainIndex TrainIndex;

/* communication data type */
enum CommDataType {
    Initial, 
/* as KQML style */
    Ask_if,
    Ready,
    Deny,
    Uninsert,
/* ............. */
    Approval_CheckIn, 
    Approval_CheckOut,
    Ack_Report,

    Command_Slow_Down,
    Command_Speed_Up,
    Command_Stop,

    Requeset_CheckIn, 
    Requeset_CheckOut, 

    Report_Normal, 
    Report_Emergency, 
    Report_InMyWay_Ahead1,
    Report_InMyWay_Ahead2,
    Report_InMyWay_Behind1,
    Report_InMyWay_Behind2,

    Report_Mision_Complished,
    Report_Mision_Uncomplished,

    Route_toAll,
    Route_toLeader,
    Route_toWindman,
    Route_toSolo,

    Route_toAhead,
    Route_toBehind,

    Finished 
};

typedef enum CommDataType CommDataType;

enum BufferType { Consume, Produce };
typedef enum BufferType BufferType;

/* commnucation data struct */
struct CommData{

/* tell the buffer server that this message is whether produce or
// consume */
/* XXX maybe I don't need this data type anymore */
/*    BufferType		fromType;*/

    CommDataType	type;

/* from train number or other layer tasks */
    TaskType 		sender;
/* to train number or specified tasks if any */
    TaskType  		receiver;

/* current node sensor, already tripped sensor */
    BYTE 		sensor;
/* next sensor, which will be tripped right ahead*/
    BYTE 		nextSensor;
/* 2nd next sensor, which will be tripped after next sensor */
    BYTE 		next2ndSensor;
/* XXX maybe this is not be using, instead distance maybe used, total time based on last tripped sensor and speed information */ 
    int	 		time;
/* position information based on position estimation */ 
    int	 		position;
    int	 		extra1;
    int  		extra2;
    int  		extra3;
};

typedef struct CommData CommData;


#define	ACK			0xf1f1
#define	NIL			(-1)


/* command in general size of char type */
#define CMD_SIZE		3

#define CMD_TYPE		0

/* instructions from tokenizer */
#define HALT_CMD    	0
#define MISS_CMD    	1
#define RAND_CMD    	2
#define INIT_CMD    	3
#define TRN_CMD   	4 	/* train command from tokenizer */
#define CMD2CC_SIZE 	4




#define TRAIN_NUMBER		1
#define TRAIN_SPEED		2

#define SWITCH_NUMBER 		1
#define SWITCH_TYPE		2

#define	TRAIN_CMD		't'
#define SWITCH_CMD		'w'
#define SENSOR_CMD		's'

#define START			0x23
#define NORMAL			0x24
#define END			0x25

/* switch command type */
#define STRAIGHT		0x21
#define CURVE			0x22
#define SWITCH_CLOSE		0x20

#define FROM_TRAIN_TASK		0x12


/* system 20Hz, */
#ifdef	SYSTEM_CLOCK_20
/* delay for between commands */
    #define DELAY_TIME_FOR_COMMAND		3
    #define DELAY_TIME_SWITCH_CLOSE		2
    #define SENSOR_SCHEDULING_TICKS		2
    #define TRAIN_TASK_TICKS			2
    
    #define	PERIOD_200_M_SEC		4
    #define	PERIOD_150_M_SEC		3
    #define	PERIOD_100_M_SEC		2
    #define	PERIOD_50_M_SEC			1

#endif //SYSTEM_CLOCK_20

#ifdef	SYSTEM_CLOCK_100
/* maybe for 100 Hz oeration */
/* delay for between commands */
    #define DELAY_TIME_FOR_COMMAND		15
    #define DELAY_TIME_SWITCH_CLOSE		15
    #define SENSOR_SCHEDULING_TICKS		10
    #define TRAIN_TASK_TICKS			10
    
    #define	PERIOD_200_M_SEC		20
    #define	PERIOD_150_M_SEC		20
    #define	PERIOD_100_M_SEC		10
    #define	PERIOD_50_M_SEC			5
#endif //SYSTEM_CLOCK_10

/* emergency stop command and resume */
#define EMERGENCY_STOP		97
#define EMERGENCY_RESUME	96 


/* some TID definitions */
#define SensorTid		5
#define SensorSrv		6
#define SensorSched     	7
#define CentralControlTid	8
#define PathFindTid		9
#define TrainTid        	10
#define OneStepTid        	11

/* priority definition */

/* hardware level(highest priority) is from 11 - 20 */
#define SensorSchedPriority	14
#define CommandSrvPriority	15	
#define SensorSrvPriority 	20	

/* middle layer, train task(middle priority)  level is from 21 - 30 */
#define TrainTaskPriority  	25		

/* middle layer advisor tasks  have low priority */
#define BlackBoardPriority	35		
#define OneStepPriority		37		
#define PathFindPriority	38		
#define SchedularPriority	39		

/* upper layer controller task(low priority) level is from 31 - 40 */
#define CentralControlPriority	36		

/* buffer tasks, distributor and collector 
    is between train and central controller 	 */
#define BB_DistributorPriority	31		
#define BB_CollectorPriority	32		

#define CC_DistributorPriority	31		
#define CC_CollectorPriority	32		


#define TokenizerPriority	41		
#define	SensorReporterPriority	42

/* not working now */
#define InputPriority		14	
#define CommandPriority	 	15	


#endif /* _IRON_BIRD_H_ */
