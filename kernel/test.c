#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"

main() {
    int i, j;
    int tid, tid1;

    int send=13;
    int reply=0;
    int *pSend = &send;
    int *pReply = &reply;

     printf("delaying 20\n");
     Delay(5);
     printf("delay 20 finished >>\n");


    printf("test tid %d pri %d parent tid %d\n", \
		MyTid(), MyPriority(), MyParentTid());

    tid = Receive(&i, sizeof(int));
    printf("test received << %d \n", i);

    if(i >1) {
	tid1 = Create("Test", 10);
	printf("Test %d created %d\n", MyTid(), tid1);
	j = i - 1;
	Send(tid1, &j, sizeof(int), &j, sizeof(int));
	printf("Test %d gotBack %d\n", MyTid(),j);
	i = i*j;
	Reply(tid, &i, sizeof(int));
    } else
	Reply(tid, &i, sizeof(int));

    while(1) {
	Delay(40);
	printf("test\n");
    }

    Exit();


    *pSend = 6;
    Send(tid, pSend, sizeof(int), pReply, sizeof(int));
    printf("init replyied << %d \n", *pReply);

    Exit();

    for(i=0;i <10;i++) {
	printf("init\n");
    }
    getchar();
    Pass();

    for(i=0;i <10;i++) {
	printf("init\n");
    }
    getchar();

    tid = Create("Init", 63);

    printf("init is exiting new tid %d\n", tid);

    printf("before send , send %d, reply %d\n", *pSend, *pReply);
    Send(2, pSend, sizeof(int), pReply, sizeof(int));
    printf("got replied  , send %d, reply %d\n", *pSend, *pReply);

    tid = Receive( pSend, 9);
    printf("init received from tid %d\n", tid);
    printf("init exitting...\n");
    Exit();
}
