#include	<stdio.h>
#include	"task.h"
#include	"taskDescTable.h"
#include	"priQueue.h"
#include	"dbLinkQueue.h"



int processNodeA(void){
    int i = 0;
    printf("TEST 0\n");
    return i;
}
int processNodeB(void) {
    int i= 1;
    printf("TEST 1\n");
    return i;
}
int processNodeC(void) {
    int i= 2;
    printf("TEST2 \n");
    return i;
}
int processNodeD(void) {
    int i= 3;
    printf("TEST3 \n");
    return i;
}
int processNodeE(void) {
    int i= 4;
    printf("TEST4 \n");
    return i;
}
int processNodeF(void) {
    int i= 5;
    printf("TEST5 \n");
    return i;
}
int processNodeG(void) {
    int i= 6;
    printf("TEST6 \n");
    return i;
}
int getNodeType(void) {
    int i;
    printf("TEST\n");
    return i;
}


//
//  Estabilishement of a table of pointers to functions 
//
// 	for fast switch implementations
//

int (*switchFunctions[])(void)  = \
		{ processNodeA, processNodeB, processNodeC, 
		 processNodeD, processNodeE, processNodeF, processNodeG};
// function pointer test
int (*fpTest)(void) = (int(*)(void))processNodeC();

main() {

    int i;
    int j;
    *fpTest;
    getchar();
// 
//  fast switch implementation
//
    for(j=0;j<7; j++) {
	i= switchFunctions[j]();
	printf("return val %d\n",i);
    } // for 
    getchar();

    TaskDescTable aTDTable;

    //
    // test getNextFreeTask();
    //
    printf(" testing getNextFreeTask\n");
    Task * pTempTask = aTDTable.getNextFreeTask();
    printf("First free task tid %d\n", pTempTask->getTid());
    aTDTable.readyQueue.addPriQueue(pTempTask);


/* not doing timer check

    printf("Timer check \n");
    pTempTask->setTimer(3);
    while(!pTempTask->isTimeOut()) {
	pTempTask->decreaseTimer();
	printf("decreased and isEmpty timer %d , timer %d \n", 
		    pTempTask->isTimeOut(), pTempTask->getTimer());
	getchar();
    }
*/

    Task * pAnotherTask = aTDTable.getNextFreeTask();
    printf("Second free task tid %d\n", pAnotherTask->getTid());

    pTempTask->addMsgQueue( pAnotherTask );

    printf("msg queue of tid %d\n", pTempTask->getTid());
    pTempTask->printMsgQueue();
    getchar();

    for(i = 1; i< 30; i++) {
	pAnotherTask = aTDTable.getNextFreeTask();
	if( pAnotherTask != (Task *) Empty) {
	    printf(" %d: free task tid %d\n", i, pAnotherTask->getTid());
	    pAnotherTask->setPriority(i%5);
	    pAnotherTask->setState(Ready);
	    //pAnotherTask->setTimer(10);
	    aTDTable.readyQueue.addPriQueue(pAnotherTask);
	    //aTDTable.timerQueue.addQueue( pAnotherTask );
	}
    }


    printf(" priqueu testing \n");
    getchar();
    aTDTable.readyQueue.printQueue(63);
    aTDTable.readyQueue.printQueue(9);
    aTDTable.readyQueue.printQueue(8);
    aTDTable.readyQueue.printQueue(7);
    aTDTable.readyQueue.printQueue(6);
    aTDTable.readyQueue.printQueue(5);
    aTDTable.readyQueue.printQueue(4);
    aTDTable.readyQueue.printQueue(3);
    aTDTable.readyQueue.printQueue(2);
    aTDTable.readyQueue.printQueue(1);
    aTDTable.readyQueue.printQueue(0);

    getchar();

int returnVal;
    for(i = 10; i<20; i++) {
	pAnotherTask = aTDTable.getPtTask(i);
	returnVal = aTDTable.readyQueue.removePriQueue(pAnotherTask);
	aTDTable.readyQueue.printQueue(pAnotherTask->getPriority());
	printf("\n i = %d removed retVal %d\n\n\n", i, returnVal);
	getchar();
    }

    pAnotherTask = aTDTable.getPtTask(30);
    returnVal = aTDTable.readyQueue.removePriQueue(pAnotherTask);
    aTDTable.readyQueue.printQueue(pAnotherTask->getPriority());
    printf("removed retVal %d\n\n", returnVal);
    getchar();
    pAnotherTask = aTDTable.getPtTask(20);
    returnVal = aTDTable.readyQueue.removePriQueue(pAnotherTask);
    aTDTable.readyQueue.printQueue(pAnotherTask->getPriority());
    printf("removed retVal %d\n", returnVal);
    pAnotherTask = aTDTable.getPtTask(10);
    returnVal = aTDTable.readyQueue.removePriQueue(pAnotherTask);
    aTDTable.readyQueue.printQueue(pAnotherTask->getPriority());
    printf("removed retVal %d\n", returnVal);
    getchar();

    Task * pTask;
    printf("Damm i will call getNextTask\n");
    for(i=0;i < 99; i++) {
	pTask = aTDTable.readyQueue.getNextTask();
	printf("Got tid %d pri %d \n", pTask->getTid(), pTask->getPriority());
    aTDTable.readyQueue.printQueue(pTask->getPriority());
	getchar();
	if(i%10 ==0)
	    aTDTable.readyQueue.addPriQueue(pTask);
    }
    printf("end for \n");
	pTask->setState(Ready);
	    pTask->setPriority(18);
    printf("insert tid %d pri %d \n", pTask->getTid(), pTask->getPriority());
	aTDTable.readyQueue.addPriQueue(pTask);
	
    pTask = aTDTable.readyQueue.getNextTask();
    printf("Got tid %d pri %d \n", pTask->getTid(), pTask->getPriority());
	getchar();
    pTask = aTDTable.readyQueue.getNextTask();
    pTask = aTDTable.readyQueue.getNextTask();
    pTask = aTDTable.readyQueue.getNextTask();
    pTask = aTDTable.readyQueue.getNextTask();
    pTask = aTDTable.readyQueue.getNextTask();

    aTDTable.readyQueue.printQueue(0);

    getchar();














    printf("timer queue of tid %d>>>\n", pTempTask->getTid());
    aTDTable.timerQueue.printQueue();

    getchar();
    printf("timer queue of tid %d<<<\n", pTempTask->getTid());

    Task *pTestTask;
    while( !aTDTable.timerQueue.isTravEnd()) {
	pTestTask = aTDTable.timerQueue.getNextElement();
	if(pTestTask->getTid() == 5) {
	    pTestTask->setTimer(0);
	    //aTDTable.timerQueue.removeElement(pTestTask);
	    printf("tid 5 timer set 0 \n");
	}
	printf("timer queue traversing, get tid %d timer[%d]<<<\n", 
		pTestTask->getTid(), pTestTask->getTimer());
    }
    // should reset
    aTDTable.timerQueue.resetTravPointer();
    getchar();
    printf("timer queue traverse finished tid %d\n", pTempTask->getTid());


}
