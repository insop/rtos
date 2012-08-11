#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"

#define	IdleTaskTID	2
#define DO_STAT


main() {
    int i;
    int tid;

    /* for stat */
    int IdleMem; /* idle task memeory address pData */
    unsigned int maxCnt;
    int memIdle;
    unsigned int idleCnt;
    int zero=0;
    int usage;
    unsigned long sum= 0;

    unsigned int upTime = 0;

    /*int loopCnt = 4;*/
    int loopCnt = 2;
    
    cprintf("Initializing .... ");
    /* get absolute data memory address of TID */
    IdleMem = GetAbsMem(IdleTaskTID);

    tid = Receive(&memIdle,sizeof(int));

    Reply(tid,NULL,NULL);
    /* wait until system reaches the stable state */
    Delay(TICKS_PER_SEC * 2);

    for(i = 0; i < loopCnt ; i++ ) {
	enterCS();
	/* reset the counter */
	absZero(IdleMem + memIdle, sizeof(int));
	exitCS();

	Delay(TICKS_PER_SEC * 1);

	/* get the maximum counter */
	enterCS();
	absRead(IdleMem + memIdle, &maxCnt, sizeof(int));
	exitCS();
	sum += maxCnt;
    } /* for */

    maxCnt = (int) sum / loopCnt;

/* if you don't want stat just undef this */
#ifdef DO_STAT
	console_xy(118,4);
	cprintf("Max:%x",maxCnt);

/* delete the initialzing message */
	console_xy(0,0);
	cprintf("                      ");
	console_xy(90,5);
	cprintf("Uptime:");

    while(1){
	enterCS();
	/* reset the counter */
	absZero(IdleMem + memIdle, sizeof(int));
	exitCS();

	Delay(TICKS_PER_SEC * 1);

	enterCS();
	/* read the number */
	absRead(IdleMem + memIdle, &idleCnt, sizeof(int));
	exitCS();

	if(maxCnt > 100 ) {
	    usage = (int)(100L - 100L * idleCnt / maxCnt);
	    if (usage > 100) {
		usage = 100;
	    } else if (usage < 0) {
		usage =   0;
	    }
	}
	else {
	    usage = 0;
	}

	console_xy(101,4);
	cprintf("%2d %% Idl:%x",usage, idleCnt);

	upTime = UpTime();
	console_xy(116,3);
	cprintf("%d",upTime);
	/*WakeUp(INIT_TID);*/

	/* XXX this is repitittion but ... */
	console_xy(118,4);
	cprintf("Max:%x",maxCnt);
    }
#endif /* DO_STAT */

    Exit();
}
