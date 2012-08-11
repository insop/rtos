#undef DEBUG
#include "../syscall/sysCall.h"
#define CLOCKTICK 2 
#define NUMPERROW 20
#define NUMROWS 5
#define DelayWorks
#define NULL 0


main() {
    int tid1, tid2;
    char strWrite[50];


    sprintf(strWrite,"I am TaskC[%d], priority[%d] \n", MyTid(), MyPriority());
    Write(strWrite, strLen(strWrite), WYSE);

    tid1 = Receive(NULL,NULL);
    sprintf(strWrite,"Task C got msg from tid1[%d] \n", tid1);
    Write(strWrite, strLen(strWrite), WYSE);

/*
    tid2 = Receive(NULL,NULL);
    sprintf(strWrite,"Task C got msg from tid2[%d] \n", tid2);
    Write(strWrite, strLen(strWrite), WYSE);
*/

    Reply(tid1, NULL,NULL);
    sprintf(strWrite,"Task C Replied to tid1[%d] \n", tid1);
    Write(strWrite, strLen(strWrite), WYSE);

    sprintf(strWrite,">I am TaskC[%d], priority[%d] \n", MyTid(), MyPriority());
    Write(strWrite, strLen(strWrite), WYSE);

    tid2 = Receive(NULL,NULL);
    sprintf(strWrite,"Task C got msg from tid2[%d] \n", tid2);
    Write(strWrite, strLen(strWrite), WYSE);

    Reply(tid2, NULL,NULL);
    sprintf(strWrite,"Task C Replied to tid2[%d] \n", tid2);
    Write(strWrite, strLen(strWrite), WYSE);

    sprintf(strWrite,">>I am TaskC[%d], priority[%d] \n", MyTid(), MyPriority());
    Write(strWrite, strLen(strWrite), WYSE);

    Exit();
}
