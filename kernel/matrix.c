#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"

main() {
    int i, j;
    int tid, tid1;

    int send=13;
    int reply=0;
    int *pSend = &send;
    int *pReply = &reply;

    int x;
    int y;
    int dlyTime;
    int MaxY;
    int StartY;
    int letter;
    int xx;
    int mode = 1; /* 1 is down 0 is up */

    tid = MyTid();
    StartY = 25;
    x = ((tid-6)*3)+3;
    letter = (x%10);
    y = StartY;
    dlyTime = (x%20);
    MaxY = 40;
    srand(tid);

    while(1) {
	xx=rand();
	letter = xx % 10;
	mode = xx % 10;
	if( mode != 1) {
	    y = StartY;
	    do {
		xx=rand();
		dlyTime = xx % 10+30;
		console_xy( x, y-1);
		cprintf("-");
		console_xy( x, y++);
		/*cprintf("%d", letter);*/
		cprintf("%d", letter);
		Delay(dlyTime);
	    } while(!(y > MaxY));
	    console_xy( x, MaxY);
	    cprintf("-");

	}
	else {
	    y = MaxY;
	    do {
		xx=rand();
		dlyTime = xx % 10;
		console_xy( x, y+1);
		cprintf("_");
		console_xy( x, y--);
		/*cprintf("%d", letter);*/
		cprintf("%d", letter);
		Delay(dlyTime);
	    } while((y >= StartY));
	    console_xy( x, StartY);
	    cprintf("-");

	    y = StartY;
	}
	    
    }

    Exit();
}
