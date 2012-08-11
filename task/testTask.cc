#include	<stdio.h>
#include	"task.h"
#include	"taskDescTable.h"
#include	"priQueue.h"



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
#undef	FIRST_TEST
#ifdef  FIRST_TEST
    Task 	aTask[MaxTaskNum];
    PriQueue	aTaskPriQueue;

    Task *	pTask;


    //initDebug(ErrorDebug);
    for(i = 0 ; i < 30 ; i++)
	aTask[i].printTask();

    pTask = &aTask[1];

    printf("\n");
    pTask->printTask();

    pTask->hello();

    printf("\ntask testing\n");
	pTask->setState(Ready);
    char * taskState = pTask->getState();
    printf("task state [%s] \n", taskState);

    //pTask->priority = 340;
    if( pTask->isValidPriority(LowestPriority) != OK )
	printf("NOT valid priority\n");
    else
	printf("valid priority\n");

    aTask[45].priority = 1;
    aTask[4].priority = 254;
    aTask[5].priority = 0;
    aTask[14].priority = 1;
    aTask[37].priority = 1;
    aTask[99].priority = 0;
    aTask[78].priority = 4;
    for(i = 3; i < 80; i++) {
	pTask = &aTask[i]; 
	// XXX 
	pTask->setState(Ready);
	aTaskPriQueue.addPriQueue(pTask);
    }
    
    aTaskPriQueue.printQueue(255);
    aTaskPriQueue.printQueue(254);
    aTaskPriQueue.printQueue(99);
    aTaskPriQueue.printQueue(78);
    aTaskPriQueue.printQueue(37);
    aTaskPriQueue.printQueue(45);
    aTaskPriQueue.printQueue(14);
    //aTaskPriQueue.printQueue(3);
    aTaskPriQueue.printQueue(1);
    aTaskPriQueue.printQueue(0);

    //aTaskPriQueue.getHighPriority();

    printf("Damm i will call getNextTask\n");
    pTask = aTaskPriQueue.getNextTask();
    pTask = aTaskPriQueue.getNextTask();
    pTask = aTaskPriQueue.getNextTask();
	// XXX 
	aTask[5].setState(Ready);
	aTaskPriQueue.addPriQueue(&aTask[5]);
    pTask = aTaskPriQueue.getNextTask();
    pTask = aTaskPriQueue.getNextTask();
    pTask = aTaskPriQueue.getNextTask();
    pTask = aTaskPriQueue.getNextTask();
    pTask = aTaskPriQueue.getNextTask();

    aTaskPriQueue.printQueue(1);
    //printf("getNext task %d\n", pTask->tid);
    //aTaskPriQueue.removeQueue(1);

    //aTaskPriQueue.getHighestPriority();


    printf("\nfirst version of test finished \n");
    getchar();
#endif // FIRST_TEST

    TaskDescTable aTDTable;

    //
    // test getNextFreeTask();
    //
    printf(" testing getNextFreeTask\n");
    Task * pTempTask = aTDTable.getNextFreeTask();
    printf("First free task tid %d\n", pTempTask->getTid());
    aTDTable.readyQueue.addPriQueue(pTempTask);
    //for(i = 1; i< aTDTable.getMaxNumTasks()+2; i++) {
    for(i = 1; i< 100; i++) {
	pTempTask = aTDTable.getNextFreeTask();
	if( pTempTask != (Task *) Empty) {
	    printf(" %d: free task tid %d\n", i, pTempTask->getTid());
	    pTempTask->setPriority(i%10);
	    pTempTask->setState(Ready);
	    aTDTable.readyQueue.addPriQueue(pTempTask);
	}
    }

    printf(" priqueu testing \n");
    getchar();
    aTDTable.readyQueue.printQueue(255);
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

    i=0;
    do {
	printf("%d ",i);
	pTempTask = aTDTable.readyQueue.getNextTask();
	i++;
    } while( !aTDTable.readyQueue.isEmpty() );
    //} while( pTempTask != (Task *) Empty);


    printf("\nsecond version of test finished \n");
    getchar();



}
