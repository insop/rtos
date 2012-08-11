#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"
#include "../include/i486/machine/serial.h"


/* make matrix task running up to full tasks(63 tasks) */
#undef  MATRIX_TASKS

#include "../train/ironBird.h"

char *testStr="1234567890abcdefghijklmnopqrstuvwxyz";
#define	STAT_TID	3

/* higest priority + 1 */
#define NAME_SERVER_PRI	2

#define	CREATE_STAT



main() {
    int i;
    int tid;
    int numTask;
    int NumMatrix;
    int runningTime;
    char inputChar;

    console_clear();
    numTask = 3; /* kernel, init, idle already maden from kernel */
    
#ifdef	CREATE_STAT
/* doing statistics job */
/* do not remove this */
/* if you don't want this change sched.c */
/* that file is related with idle.c */
    numTask++;
    tid = Create("Stat", LOWEST_PRI - 1); 		/* tid = 3 */
#endif /* CREATE_STAT */

/* Create Name server */
    numTask++;
    tid = Create("NameServer", NAME_SERVER_PRI ); 	/* tid = 4 */

    Delay(TICKS_PER_SEC * 6);
    console_xy( 45, 0);
    cprintf(
    "[ F E M O S ] Fast EMbedded Operating System.    ");
    console_xy( 0, 2);
    cprintf(
    "-----------------------------------------------------------------"
    "-----------------------------------------------------------------");
    console_xy( 90, 4);
    cprintf( "CPU Usage:     %");

/* !!! do not change the creating order until Name server is done */
	
    numTask++;
    tid = Create("CommandSrv", CommandSrvPriority); 		/* tid = 5 */

    numTask++;
    tid = Create("SensorSrv", SensorSrvPriority); 		/* tid = 6 */

    numTask++;
    tid = Create("SensorSched", SensorSchedPriority); 		/* tid = 7 */

    numTask++;
    tid = Create("CentralControlMain", CentralControlPriority); /* tid = 8 */

    numTask++;
    tid = Create("PathFind", PathFindPriority); 		/* tid = 9 */

    numTask++;
    tid = Create("OneStep", OneStepPriority);			/* tid = 10 */

    numTask++; 
    tid = Create("Schedular", SchedularPriority); 		/* tid = 11 */

    numTask++; 
    tid = Create("Tokenizer", TokenizerPriority); 		/* tid = 12 */

    numTask++; 
    tid = Create("BlackBoardMain", BlackBoardPriority); 	/* tid = 13 */

#if 0

    numTask++; 
    tid = Create("TaskA", 50); 		/* tid = 14 */

    numTask++; 
    tid = Create("TaskB", 50); 		/* tid = 15 */

    numTask++; 
    tid = Create("TaskC", 50); 		/* tid = 14 */

    numTask++; 
    tid = Create("TaskD", 50); 		/* tid = 15 */
#endif /* 0 */

/*
    numTask++; 
    tid = Create("Schedular", BlackBoardPriority);
*/
#ifdef MATRIX_TASKS
    NumMatrix = (MAX_TASK_NUM) - numTask;
    /*NumMatrix = 5;*/
    for(i=0; i< NumMatrix;i++) {
	tid = Create("Matrix", 50+(i%10));
	numTask++;
    }
#endif  /*MATRIX_TASKS*/

    console_xy( 90, 3);
    cprintf( "#Tasks   :  %d s  * Uptime: ", numTask);

    console_xy( 0, 5);
    cprintf(
    "-----------------------------------------------------------------"
    "-----------------------------------------------------------------");

    /*console_xy( 0, 7);
    cprintf("TrainCommand>> ");*/

    Exit();
}
