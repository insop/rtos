#undef DEBUG
#include "../syscall/sysCall.h"
#define CLOCKTICK 2 
#define NUMPERROW 20
#define NUMROWS 5
#define DelayWorks
#define NULL 0

#define TASK_C 	7


main() {
    int taskB, taskC;

    char strWrite[50];


    sprintf(strWrite, "I am TaskB[%d], priority[%d] \n", MyTid(), MyPriority());
    Write(strWrite, strLen(strWrite), WYSE);

    Send(TASK_C, NULL,NULL,NULL,NULL);

    sprintf(strWrite,"TaskB replied from TaskC\n");
    Write(strWrite, strLen(strWrite), WYSE);

    sprintf(strWrite,"I am TaskB[%d], priority[%d] \n", MyTid(), MyPriority());
    Write(strWrite, strLen(strWrite), WYSE);

    Exit();
}
