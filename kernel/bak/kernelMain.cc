//
//--
//	FEMOS(Fast EMbedded OS) kernel main  
//--
//
// 	Written by INSOP SONG
//
// 	Systems Design
//	
//	Michael Cole
//	Computer Science
//
// 	University of Waterloo, Canada
//
//	June,July 2001
//--
//

#define	HATE_WYSE

#define	 DEBUG

#include "../include/util/error.h"
#include "kernelMain.h"

#define	PRI_QUEUE_MSG

// flow control for serial control
#undef FLOW_CONTROL
#define  NOT_FLOW_CONTROL

extern "C" {
    initGDT();
    initIDT();
    initClock();
    initSerial();
    setGDTVal(uint, unsigned long, int);
    getCodeSelector( int );
    getDataSelector( int );
    getBaseS( int );
}; // C function interface

    //--
    //	Global Objects
    //--
Bindfile	aBindfile;
Aout		aAoutHeader;
    //--
TaskDescTable   aTDTable;
Kernel		aKernel;
    //--
    // task ready queue ( sinlely linked )
    //--
PriQueue *	pReadyQueue  = &(aTDTable.readyQueue);
    //--
    // clock event queue ( doubly linked )
    //--
DbLinkQueue *	pClkEvtQueue = &(aTDTable.timerQueue);
    //--
    // serial class has four bounded buffer
    //--
Serial		aSerial;
BdBuffer * 	pBDBuf_W_I = &(aSerial.inWyse);
BdBuffer * 	pBDBuf_W_O = &(aSerial.outWyse);
BdBuffer * 	pBDBuf_T_I = &(aSerial.inTrain);
BdBuffer * 	pBDBuf_T_O = &(aSerial.outTrain);

SimpleQueue * 	pQueue_W_I = &(aSerial.inWyseQueue);
SimpleQueue * 	pQueue_W_O = &(aSerial.outWyseQueue);
SimpleQueue * 	pQueue_T_I = &(aSerial.inTrainQueue);
SimpleQueue * 	pQueue_T_O = &(aSerial.outTrainQueue);

    //--
    // pointer for msg queue handling
    //--
PriQueue *	pMsgQueue;

int		interruptTestVal = 0;
//--

int InterruptType;		/* type of the interrupt came */
int SysCallDispatchType;	/* type of the syscall */

int userCS;
int userDS;
int userSS;
int userSP;
int kernelSP;
//--

//--
//  Estabilishement of a table of pointers to functions 
//
// 	for fast switch implementations
//--

int (*switchFunctions[])(void)  = \
      { dispatcher_sysCall, 
	handler_clockEvent, 
	handler_serialEvent_0, 
	handler_serialEvent_1}; 

void (*switchSysCall[])(void)  = \
  { sysCall_invalidSyscall, 
    sysCall_create, 
    sysCall_pass,
    sysCall_exit,
    sysCall_myTid,
    sysCall_myParentTid,
    sysCall_myPriority,
    sysCall_send,
    sysCall_receive,
    sysCall_reply,
    sysCall_validTid,
    sysCall_awaitEvent,
    sysCall_delay,
    sysCall_get,
    sysCall_put,
    sysCall_read,
    sysCall_write,
    sysCall_upTime,
    sysCall_delayUntil,
    sysCall_getAbsMem,
    sysCall_wakeUp,
    sysCall_lastSysCall };
//--

// Read syscall clearence function
void sysCall_read_return( char _port );

//
// XXX maybe outside of this or in here
// maybe C++ or not (just C)
//
//extern int sysCallDispatch();
//extern int clockTickEvent();
//extern int serialEvent();


void Halt(void) {
    Error(ErrorWarning,"Warning, Halt occured ");
    asm("hlt;");
}

//
//------------------------------------------------------------------
// 
// user task stack management functions
//	set return value, get parameter...
//
//------------------------------------------------------------------
//

    //--
void setReturnValue(uint returnValue) {
    int		numParams, check;
    int		stkTop, offset;

    // Check that this task is really executing a system call 
    if( aKernel.pCurrentTask->requestedSysCall == -1 ) {
       Error(ErrorWarning,\
	  "WARNNING !!at setReturnValue invalid requested system call ");
       Halt();
    } // if 

    // no return value check 
    if(RET_PARAM_NUM[aKernel.pCurrentTask->requestedSysCall] == 0 ) {
       Error1(ErrorNone,">>requeseted System call is %d",
			    aKernel.pCurrentTask->requestedSysCall);
       Error(ErrorNone, "SetReturnValue(): No return value expected" );
    } // if 

    numParams = RET_PARAM_NUM[aKernel.pCurrentTask->requestedSysCall];
    Error1(ErrorNone,"setReturnVal return value NUM %d",numParams);

    // Check if the value we're about to overwrite is the magic-number
    // that we expect. 
    stkTop = aKernel.pCurrentTask->pData + aKernel.pCurrentTask->userSP;

    //offset = STORED_REG + 4 + 4*(numParams+1);
    offset = STORED_REG + 4 * (numParams);
    absRead(stkTop+offset, &check, sizeof(int) );

    if( check != MAGIC2 ) {
       Error3(ErrorWarning, 
       "WANNING! setReturnValue() PID [%d] found out the ERROR at user \
       module stack with the : %x, STK_OFFSET %d", 
       aKernel.pCurrentTask->getTid(), check, offset);
       Halt();
    } // if 

    absWrite(stkTop+offset, &returnValue,sizeof(int));
}


    //--
void setReturnValueTo( uint returnValue, Task * pTask) {

    int		numParams, check;
    int		stkTop, offset;

    // Check that this task is really executing a system call */
    if( pTask->requestedSysCall == -1 ) {
       Error(ErrorWarning,\
	  "WARNNING !!at setReturnValue invalid requested system call ");
       Halt();
       //return;
    } // if */

    // no return value check */
    if(RET_PARAM_NUM[pTask->requestedSysCall] == 0 ) {
       Error1(ErrorNone,">>requeseted System call is %d",
			    pTask->requestedSysCall);
       Error(ErrorNone, "SetReturnValue(): No return value expected" );
       //return;
    } // if */

    numParams = RET_PARAM_NUM[pTask->requestedSysCall];
    Error1(ErrorNone,"setReturnVal return value NUM %d",numParams);

    // Check if the value we're about to overwrite is the magic-number
    // that we expect. */
    stkTop = pTask->pData + pTask->userSP;

    /*offset = STORED_REG + 4 + 4*(numParams+1);*/
    offset = STORED_REG + 4 * (numParams);
    absRead(stkTop+offset, &check, 4 );

    if( check != MAGIC2 ) {
       Error3(ErrorWarning, 
       "WANNING! setReturnValue() PID [%d] found out the ERROR at user \
       module stack with the : %x, STK_OFFSET %d", 
       pTask->getTid(), check, offset);
       Halt();
    } // if */

    absWrite(stkTop+offset, &returnValue,4);
}


    //--
uint getParameter(int n)
{
    uint	parameter;
    int	stkTop, offset, numParams;

    // The first parameter is stored at the lowest address */
    numParams = PARAM_NUM[aKernel.pCurrentTask->requestedSysCall];
    Error1(ErrorNone,"getParameter number %d",numParams);

    // Check that 'n' is not out of range **/
    if( n < 1 || n > numParams ) {
	 Error( ErrorWarning, \
	    "getParameter(): inputted n out of range" );
	 Halt();
	 return 0;
    } // if */

    stkTop = aKernel.pCurrentTask->pData +
			    aKernel.pCurrentTask->userSP;
    offset = STORED_REG + 4 + 4*(n);
    absRead( stkTop + offset, &parameter, 4 );

    return parameter;
}

    //
    //--
    //
    //	msg copy from one task to another task for ipc stuffs
    //
    //--
    //
int msgCopy (Task * pTaskSrc, Task * pTaskDesc) {

    int errorCode = OK;
    int srcLen, descLen, copyLen;
    int pMsgSrc, pMsgDesc;

    srcLen   = pTaskSrc->msgSendLen;
    descLen  = pTaskDesc->msgRecvLen;
    pMsgSrc  = pTaskSrc->pMsgSend;
    pMsgDesc = pTaskDesc->pMsgRecv;

    Error1(ErrorNone,"@ CopyMsg XXXX Src Tid 0x%x",pTaskSrc->getTid());
    Error1(ErrorNone,"@ CopyMsg XXXX Desc tid 0x%x",pTaskDesc->getTid());
    Error2(ErrorNone,"@ CopyMsg: Src len %d des len %d " \
			     ,srcLen,descLen);
    Error2(ErrorNone,"@ CopyMsg: src mem 0x%x desc mem 0x%x " \
			     ,pMsgSrc,pMsgDesc);

    if( (srcLen != NULL) && (descLen != NULL) ) {
	// if sorce length is larger than that of destination
	// just copy the length of the destination */
	if(srcLen>descLen)  {
	     copyLen = descLen;
	} else  {
	     copyLen = srcLen;
	} //else 
	 
	absCopy( pTaskDesc->pData+pMsgDesc, 
		  pTaskSrc->pData+pMsgSrc, copyLen);
	// 
	//absRead(ptrFromTask->memoryAddressDS+srcMemPt,&data,copyLen);
	//absWrite(ptrToTask->memoryAddressDS+desMemPt,&data,copyLen);
	//	
	errorCode = copyLen;
    } // if
    else { 
	Error(ErrorNone, "@reply : either s or r length is NULL")
	errorCode = NULL;
    } // else
    return errorCode;
}

//
//------------------------------------------------------------------
// 
// system call dispatch functions
//
//------------------------------------------------------------------
//

/********************************************************************
 * function : pass(void)
 ********************************************************************
 */
void sysCall_pass()
{
    //int i = 0;

    if (aKernel.pCurrentTask == (Task *)NULL) { 
	 Error(ErrorWarning, "NULL TaskToRun");
	 asm("hlt;");
	 while(1);
    }
    aKernel.pCurrentTask->setState(Ready);
    //aTDTable.readyQueue.addPriQueue( aKernel.pCurrentTask );
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );
    //return i;
}

/********************************************************************
 * function : exit(void)
 ********************************************************************
 */
void sysCall_exit()
{
    //int i=0;
    //
    // simply not insert to the ready queue
    aKernel.pCurrentTask->setState(Dead);
    //return i;
}

/********************************************************************
 * function : send(void)
 ********************************************************************
 */
void sysCall_send(void) {
    ErrorCode errorCode = OK;
    Task * pTask = aKernel.pCurrentTask;
    int  rcvTid;

// get tid and conver this into pointer
    rcvTid = getParameter(1);
    pTask->pTaskRecv   = aTDTable.getPtTask(rcvTid);
    //pTask->pTaskRecv   = aTDTable.getPtTask(getParameter(1));
    pTask->pMsgSend    = getParameter(2);
    pTask->msgSendLen  = getParameter(3);

// when this replied, that means received
    pTask->pMsgRecv   = getParameter(4);
    pTask->msgRecvLen = getParameter(5);

    Error1(ErrorNone,"@ send Receiver Tid is 0x%x",
		pTask->pTaskRecv->getTid());
    Error1(ErrorNone,"@ send sender Tid is 0x%x",pTask->getTid());
    Error1(ErrorNone,"@ message send address is 0x%x",pTask->pMsgSend);
    Error1(ErrorNone,"@ message send length is 0x%x",pTask->msgSendLen);
    Error1(ErrorNone,"@ message reply address is 0x%x",pTask->pMsgRecv);
    Error1(ErrorNone,"@ message reply length is 0x%x",pTask->msgRecvLen);

// receiver is not ready
    if(pTask->pTaskRecv->getStateNum() != ReceiveBlocked ) {
	pTask->setState( SendBlocked );

    // add receiver's msg queue 
    // XXX MSG 
#ifndef PRI_QUEUE_MSG
    // one level msg queue.
	pTask->pTaskRecv->addMsgQueue(pTask);
#endif // PRI_QUEUE_MSG

// prioritized msg queue
#ifdef  PRI_QUEUE_MSG
// get the pointer of msg queue of each task
	pMsgQueue = aTDTable.getPtMsgQueue( rcvTid );
	pMsgQueue->addPriQueue( pTask );
	//pMsgQueue->printPriQueue( );

	//printf("added msgqueue of tid %d\n", rcvTid);
	//getchar();

#endif // PRI_QUEUE_MSG
	//printf("msg queue of tid %d\n", pTask->pTaskRecv->getTid());
	//pTask->pTaskRecv->printMsgQueue();
    } // if
// if receiver is waiting, blocked
    else if(pTask->pTaskRecv->getStateNum() == ReceiveBlocked ) {
	msgCopy( pTask, pTask->pTaskRecv);

    // set Return value for receiver sender TID
	setReturnValueTo(pTask->getTid(),
			 aKernel.pCurrentTask->pTaskRecv);

    // set sender pointer for receiver
	pTask->pTaskRecv->pTaskSend = pTask;

    // send task is going to reply blocked , waiting reply

	pTask->setState( ReplyBlocked );
    // set receiver ready and put into ready queue
        pTask->pTaskRecv->setState( Ready ); 

    //aTDTable.readyQueue.addPriQueue( pTask->pTaskRecv );
	pReadyQueue->addPriQueue( pTask->pTaskRecv );
    } // else if
    //return errorCode;
}

/********************************************************************
 * function : receive(void)
 ********************************************************************
 */
void sysCall_receive(void) {

    ErrorCode errorCode = OK;
    Task * pTask = aKernel.pCurrentTask;
    Task * pSenderTask;


    pTask->pMsgRecv    = getParameter(1);
    pTask->msgRecvLen  = getParameter(2);

    Error1(ErrorNone,"@ receive: tid %x",pTask->getTid());
    Error1(ErrorNone,"@ receive: msgRecv address 0x%x",pTask->pMsgRecv);
    Error1(ErrorNone,"@ receive: msgRecv length 0x%x",pTask->msgRecvLen);

    //printf("RECEIVE msgQUEUE\n");
    //pTask->printMsgQueue();

#ifndef PRI_QUEUE_MSG
// sender is not ready, msgQueue is empty
    if( pTask->isMsgQueueEmpty() ) {
	pTask->setState(ReceiveBlocked);
	//printf("tid %d receive blocked \n", pTask->getTid());
    } // if 

// if sender is waiting, msg queue is not empty
    else {

    // XXX MSG 
	pSenderTask = pTask->getMsgQueue();
	msgCopy(pSenderTask, pTask);
#endif	// PRI_QUEUE_MSG

// prioritized msg queue
#ifdef PRI_QUEUE_MSG

    pMsgQueue = aTDTable.getPtMsgQueue( pTask ); 
// sender is not ready, msgQueue is empty
    if( pMsgQueue->isEmpty() ) {
	pTask->setState(ReceiveBlocked);
	//printf("tid %d receive blocked \n", pTask->getTid());
    } // if 

// if sender is waiting, msg queue is not empty
    else {

	//printf("\nrcv tid %d msg queue \n",pTask->getTid());
	//pMsgQueue->printPriQueue( );
	//getchar();

	pSenderTask = pMsgQueue->getNextTask();
	msgCopy(pSenderTask, pTask);

#endif // PRI_QUEUE_MSG

    // send task is going to reply blocked , waiting reply
	pSenderTask->setState( ReplyBlocked );

    // set Return value for receiver for sender TID
	setReturnValue(pSenderTask->getTid());

    // set myself, receiver ready and put into ready queue
        pTask->setState( Ready ); 

    //aTDTable.readyQueue.addPriQueue( pTask );
	pReadyQueue->addPriQueue( pTask );
    } // else
    //return errorCode;
}


/********************************************************************
 * function : reply(void)
 ********************************************************************
 */
void sysCall_reply(void) {

    ErrorCode errorCode = OK;
    Task * pTask = aKernel.pCurrentTask;
    Task * pSenderTask;

//  for return value , real copy data size
    int copyLen = 0;

    pTask->pTaskRecv   = aTDTable.getPtTask(getParameter(1));

    // task should be replied blocked
    if(pTask->pTaskRecv->getStateNum() == ReplyBlocked) {
	pTask->pMsgSend    = getParameter(2);
	pTask->msgSendLen  = getParameter(3);

	Error1(ErrorNone,"@ reply repliee Tid is 0x%x",
		    pTask->pTaskRecv->getTid());
	Error1(ErrorNone,"@ reply replier tid is 0x%x",pTask->getTid());
	Error1(ErrorNone,"@ reply msg add address is 0x%x",pTask->pMsgRecv);
	Error1(ErrorNone,"@ reply: len %d " ,pTask->msgRecvLen);

	copyLen = msgCopy( pTask, pTask->pTaskRecv);

    // XXX set Return value for sender copyied msg size 
	setReturnValueTo(copyLen, pTask->pTaskRecv);


    // set repliee ready and put into ready queue
	pTask->pTaskRecv->setState( Ready );

    //aTDTable.readyQueue.addPriQueue( pTask->pTaskRecv );
	pReadyQueue->addPriQueue( pTask->pTaskRecv );
    } // if
    else {
	Error(ErrorWarning,"reply to invalid task ");
    } // else
    
    // set myself, replier ready and put into ready queue
    pTask->setState( Ready ); 
    pReadyQueue->addPriQueue( pTask );
}

/********************************************************************
 * function : create(void)
 ********************************************************************
 */
void sysCall_create(void) {

    int priority;
    int name;
    char ch, moduleName[NAME_LENGTH+1];
    int i = 0;
    int errorCode = 0;
    Task *pTask;

    name = getParameter(1);
    absRead( aKernel.pCurrentTask->pData+name, &ch, 1 );
    while ( (ch != '\0') && (i < NAME_LENGTH) ) {
	moduleName[i] = ch; 	
	i++;		
	absRead( aKernel.pCurrentTask->pData + name + i, &ch, 1);
    } // while 
    moduleName[i] = '\0';		
    priority = getParameter(2);

    Error1(ErrorNone,"@create modudule name %s",moduleName);
    Error1(ErrorNone,"@create priority  %d",priority);

    // 0 is highest priority*/
    if((priority < HighestPriority) \
		||(priority > LowestPriority)) {
	errorCode = (int)ErrorCreateWithInvalidPriority; 
	Error(ErrorWarning, "at Create(): invalid priority"); 
	Halt();
    } //if i
    else { /* else 1*/
	pTask = loadTask(moduleName,priority); //get tid
	if( (int)pTask < 0 ) {  // if 3
	   Error(ErrorWarning, "LoadTask() failed in Create()" );
	   Halt();
	} // if 3
	else {
	    pTask->setParentTid(aKernel.pCurrentTask->getTid());
	}
    } // else 1
    setReturnValue(pTask->getTid());

//  put into ready queue
    aKernel.pCurrentTask->setState(Ready);
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );
}

/********************************************************************
 * function : upTime(void)
 ********************************************************************
 */
void sysCall_upTime( void ) {

    int i=0;

    if (aKernel.pCurrentTask == NULL) { 
	Error(ErrorWarning, "NULL TaskToRun");
	asm("hlt;");
	while(1);
    } // if
    Error1(ErrorNone,"@uptime %d",aKernel.getUpTime());
// put add queue againe
    aKernel.pCurrentTask->setState( Ready );
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

    setReturnValue( aKernel.getUpTime() );
    //return i;
}

/********************************************************************
 * function : delay(void)
 ********************************************************************
 */
void sysCall_delay(void) {

    int errorCode = 0;
    int duration;
    Task *pTask = aKernel.pCurrentTask;

    duration = getParameter(1);

    Error1(ErrorNone,"@delay tid %d >", pTask->getTid());
    Error1(ErrorNone,"@delay timer %d >", duration);
    
    pTask->setTimer( duration + aKernel.getUpTime() );
    pTask->setState( EventBlocked );
    //pTask->printTask();
    pClkEvtQueue->addQueue( pTask );

}

/********************************************************************
 * function : delayUntil(void)
 ********************************************************************
 */
void sysCall_delayUntil(void) {

    int errorCode = 0;
    int directDuration;
    Task *pTask = aKernel.pCurrentTask;

    directDuration= getParameter(1);

    Error1(ErrorNone,"@delay until tid %d >", pTask->getTid());
    Error1(ErrorNone,"@delay  until timer %d >", directDuration);
    
    pTask->setTimer( directDuration );
    pTask->setState( EventBlocked );
    //pTask->printTask();
    pClkEvtQueue->addQueue( pTask );

}

/********************************************************************
 * function : WakeUp(void)
 ********************************************************************
 */
 // if task is event blocked then wake it up 
 // unless just return
 //
void sysCall_wakeUp(void) {

    int errorCode = 0;
    int tid;
    Task *pRmvTask;

    tid = getParameter(1);

    Error1(ErrorNone,"@wakeup tie %d >", tid);

    pRmvTask = aTDTable.getPtTask(tid);

    if( pRmvTask->getStateNum() == EventBlocked ) {

	pClkEvtQueue->removeElement(pRmvTask);

    // set removed task to Ready and put into ready queue
	pRmvTask->setState( Ready );
	pReadyQueue->addPriQueue( pRmvTask );
    }

//  put current task into ready queue
    aKernel.pCurrentTask->setState(Ready);
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

}
/********************************************************************
 * function : getAbsMem(void)
 * 	getting absolute memory address of the task(data area)
 * 	task->pData
 ********************************************************************
 */
void sysCall_getAbsMem(void) {

    int errorCode = 0;
    int targetTid;
    Task *pTask = aKernel.pCurrentTask;
    Task *pTemp;

    targetTid = getParameter(1);

    Error1(ErrorNone,"@absmem tid %d >", pTask->getTid());
    Error1(ErrorNone,"@tid %d >", targetTid);

    pTemp = aTDTable.getPtTask(targetTid);
    //cprintf("tid%d tid %d add %x\n", targetTid, pTemp->getTid(), pTemp->pData);

    setReturnValue(pTemp->pData);
    
//  put into ready queue
    aKernel.pCurrentTask->setState(Ready);
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

}
/********************************************************************
 * function : myTid(void)
 ********************************************************************
 */
void sysCall_myTid( void ) {

    int i=0;

    if (aKernel.pCurrentTask == NULL) { 
	Error(ErrorWarning, "NULL TaskToRun");
	asm("hlt;");
	while(1);
    } // if
// put add queue againe
    aKernel.pCurrentTask->setState(Ready);
    //aTDTable.readyQueue.addPriQueue( aKernel.pCurrentTask );
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

    setReturnValue(aKernel.pCurrentTask->getTid());
    //return i;
}

/********************************************************************
 * function : myPriority(void)
 ********************************************************************
 */
void sysCall_myPriority( void ) {

    int i=0;

    if (aKernel.pCurrentTask == NULL) { 
	Error(ErrorWarning, "NULL TaskToRun");
	asm("hlt;");
	while(1);
    } // if 
// put add queue againe
    aKernel.pCurrentTask->setState(Ready);
    //aTDTable.readyQueue.addPriQueue( aKernel.pCurrentTask );
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

    setReturnValue(aKernel.pCurrentTask->getPriority());
    //return i;
}

/********************************************************************
 * function : myParentTid(void)
 ********************************************************************
 */
void sysCall_myParentTid( void ) {

    int i=0;

    if (aKernel.pCurrentTask == NULL) { 
	Error(ErrorWarning, "NULL TaskToRun");
	asm("hlt;");
	while(1);
    } // if 
    // put add queue againe
    aKernel.pCurrentTask->setState(Ready);
    //aTDTable.readyQueue.addPriQueue( aKernel.pCurrentTask );
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

    setReturnValue(aKernel.pCurrentTask->getParentTid());
    //return i;
}

/********************************************************************
 * function : myParentTid(void)
 ********************************************************************
 */
void sysCall_invalidSyscall(){
    int i;
    Error(ErrorWarning, "Warning!, Invalid system call");
    getchar();
    //return i;
}

void sysCall_validTid(){
    int i;
}

void sysCall_awaitEvent(){
    int i;
}


void sysCall_lastSysCall(){
    int i;
}

//
//--
// get
//--
//
void sysCall_get( void ) {

    int i=0;
    int port;
    char mcr;
    BYTE data;

    if (aKernel.pCurrentTask == NULL) { 
	Error(ErrorWarning, "NULL TaskToRun");
	asm("hlt;");
	while(1);
    } // if

    port = getParameter(1);
    Error1(ErrorNone, "Get from port %d", port);
    
    // for the train 
    if( port == TRAIN ) {
	if(!pBDBuf_T_I->isEmpty()) {
	    data = pBDBuf_T_I->getItem();
	    setReturnValue(data);
	// put add queue againe
	    aKernel.pCurrentTask->setState(Ready);
	    pReadyQueue->addPriQueue( aKernel.pCurrentTask );
	} // 2nd inner if
	else {
	// if buffer is empty
	    aKernel.pCurrentTask->setState( EventBlocked );
	    pQueue_T_I->addQueue(aKernel.pCurrentTask);
	} // 2nd inner else
    } // if
    else { // ( port == WYSE(=1)) for WYSE 

//
// XXX before I do this I have to check that if this is read mode
// it read mode then just put in the wait queue
//
	if(!aSerial.isReadMode1()) {
	    if(!pBDBuf_W_I->isEmpty()) {
		data = pBDBuf_W_I->getItem();
		setReturnValue(data);
	    // put add queue againe
		aKernel.pCurrentTask->setState(Ready);
		pReadyQueue->addPriQueue( aKernel.pCurrentTask );
	    } // 2nd inner if
	    else {
	    // if buffer is empty
		aKernel.pCurrentTask->setState( EventBlocked );
		pQueue_W_I->addQueue(aKernel.pCurrentTask);
	    } // 2nd inner else
	}// if not read mode 1

	// XXX if READ mode then just wait
	else {
	    aKernel.pCurrentTask->setState( EventBlocked );
	    pQueue_W_I->addQueue(aKernel.pCurrentTask);
	}


    } // else

    //return i;
}

//
//--
// put
//--
//
void sysCall_put( void ) {

    int port;
    char data;
    int i=0;

    char mcr;
    char intSet;

    if (aKernel.pCurrentTask == NULL) { 
	Error(ErrorWarning, "NULL TaskToRun");
	asm("hlt;");
	while(1);
    } // if

    data = (char)getParameter(1);
    port = getParameter(2);
    //Error2(ErrorDebug, "Put port %x data %c\n ", port, data);
    
    // for train port
    if( port == TRAIN ) {
	// if serial port is ready(THRU ready)
	// and no data in buffer
	// then out directly
	if( aSerial.isReady0() && pBDBuf_T_O->isEmpty() ) {
	    //cprintf(" out ");
	    outb( USART_0_DATA , data);
	    aSerial.resetReady0();
	} // if

	else if(!pBDBuf_T_O->isFull()) {
	    //cprintf(" in buf ");
	    pBDBuf_T_O->addItem(data);
	} // else if

	// if buffer is full, then waiting list
	else {
	    cprintf("T_O buffer full !!!! \n");
	    printf("T_O buffer full !!!! \n");
	    // if buffer is full and THRE is not empty
	    // then add into event queue
	    // XXX need to handle waiting list 
	    // XXX
	    //aKernel.pCurrentTask->setState( EventBlocked );
	    //pQueue_W_O->addQueue(aKernel.pCurrentTask);
	} // else
    } // if
    else { // ( port == WYSE(=1)) for WYSE 

	// if serial port is ready(THRU ready)
	// and no data in buffer
	// then out directly
	if( aSerial.isReady1() && pBDBuf_W_O->isEmpty() ) {
	    //cprintf(" out ");
	    outb( USART_1_DATA , data);
	    aSerial.resetReady1();
	} // if

	else if(!pBDBuf_W_O->isFull()) {
	    //cprintf(" in buf ");
	    pBDBuf_W_O->addItem(data);
	} // else if

	// if buffer is full, then waiting list
	else {
	    // if buffer is full and THRE is not empty
	    // then add into event queue
	    // XXX need to handle waiting list 
	    // XXX
	    cprintf("W_O buffer full !!!! \n");
	    printf("W_O buffer full !!!! \n");
	    aKernel.pCurrentTask->setState( EventBlocked );
	    pQueue_W_O->addQueue(aKernel.pCurrentTask);
	} // else
    } // else
    // put add queue again
    aKernel.pCurrentTask->setState(Ready);
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

#undef OLD_PUT
#ifdef OLD_PUT

    // for the train 
    if( port == TRAIN ) {
	if(!pBDBuf_T_O->isFull()) {
	    pBDBuf_T_O->addItem(data);

	    if(!pBDBuf_T_O->isEmpty()) {
		data = pBDBuf_T_O->getItem();
		outb( USART_0_DATA , data);
	    }

	    // put add queue again
	    aKernel.pCurrentTask->setState(Ready);
	    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

	    // THRE 1 is empty already
	    if( aSerial.isReady0() && !pBDBuf_T_O->isEmpty() ) {
		data = pBDBuf_T_O->getItem();
		outb( USART_0_DATA , data);
		aSerial.resetReady0();
	    } // inner if

	} // outter if
	else {
	// if buffer is full
	    if(aSerial.isReady0()) {
		// if THRE is empty then put one out
		data = pBDBuf_T_O->getItem();
		outb( USART_0_DATA , data);
		aSerial.resetReady0();
		// and get data into the buffer
		pBDBuf_T_O->addItem(data);

		// put add queue again
		aKernel.pCurrentTask->setState(Ready);
		pReadyQueue->addPriQueue( aKernel.pCurrentTask );
	    } // inner if
	    else {
		// if buffer is full and THRE is not empty
		// then add into event queue
		aKernel.pCurrentTask->setState( EventBlocked );
		pQueue_T_O->addQueue(aKernel.pCurrentTask);
	    } //  else 
	} //  else 
    } // if

    // WYSE
    else {
	if(!pBDBuf_W_O->isFull()) {
	    pBDBuf_W_O->addItem(data);

	    //cprintf(" NF ");

	    if(!pBDBuf_W_O->isEmpty()) {
		//cprintf(" NE>OUT ");
		data = pBDBuf_W_O->getItem();
		outb( USART_1_DATA , data);
	    }

	    // put add queue again
	    aKernel.pCurrentTask->setState(Ready);
	    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

	    // THRE 1 is empty already
	    if( aSerial.isReady1() && !pBDBuf_W_O->isEmpty() ) {
		data = pBDBuf_W_O->getItem();
		outb( USART_1_DATA , data);
		aSerial.resetReady1();
	    } // inner if

	} // outter if
	else {
	// if buffer is full
	    if(aSerial.isReady1()) {
		// if THRE is empty then put one out
		data = pBDBuf_W_O->getItem();
		outb( USART_1_DATA , data);
		aSerial.resetReady1();
		// and get data into the buffer
		pBDBuf_W_O->addItem(data);

		// put add queue again
		aKernel.pCurrentTask->setState(Ready);
		pReadyQueue->addPriQueue( aKernel.pCurrentTask );
	    } // inner if
	    else {
		// if buffer is full and THRE is not empty
		// then add into event queue
		aKernel.pCurrentTask->setState( EventBlocked );
		pQueue_W_O->addQueue(aKernel.pCurrentTask);
	    } // inner else 
	} // outter else 
    } // main else

    //cprintf("P buffer num %d queue num %d\n",
		//pBDBuf_W_O->getNumItem(), pQueue_W_O->getNumItem());
#endif // OLD_PUT

} // put

//
// XXX in developing
//
// doesn't support multiple calling
// This means that only one task should 
// and can call this primitives.
//
void sysCall_read(){

    int index = 0;
    int errorCode = 0;

    //char readData[MAX_READ_DATA];
    char length;
    uint srcData;
    char port;
    char data;

    port = getParameter( 2 );
    srcData = getParameter( 1 );

    //absRead( aKernel.pCurrentTask->pData + srcData, readData, length);

    if( port == TRAIN ) {
	// if is is not already in read mode
	if(!aSerial.isReadMode0()) {

	    // set read mode
	    aSerial.setReadMode0();
	    // set read requested task
	    aSerial.setReadReqTask0( aKernel.pCurrentTask );
	    // set data address to set read data later on
	    aSerial.setSrcDataRead0( srcData );

	}// if

	// if already ready Mode then error...
	else {

	    cprintf("!!!Warning, Read port0() is already in serve\n");
	    printf("!!!Warning, Read port0() is already in serve\n");

	    //  XXX returned with EMPTY value  
	    setReturnValue( (uint) EMPTY );

	    //  put into ready queue
	    aKernel.pCurrentTask->setState(Ready);
	    pReadyQueue->addPriQueue( aKernel.pCurrentTask );
	} // else 

    } // if train

    else { // ( port == WYSE(=1)) for WYSE 
	// if is is not already in read mode
	if(!aSerial.isReadMode1()) {

	    // set read mode
	    aSerial.setReadMode1();
	    // set read requested task
	    aSerial.setReadReqTask1( aKernel.pCurrentTask );
	    // set data address to set read data later on
	    aSerial.setSrcDataRead1( srcData );

	}// if

	// if already ready Mode then error...
	else {

	    cprintf("!!!Warning, Read port1(WYSE) is already in serve\n");
	    printf("!!!Warning, Read port1(WYSE) is already in serve\n");

	    //  XXX returned with EMPTY value  
	    setReturnValue( (uint) EMPTY );

	    //  put into ready queue
	    aKernel.pCurrentTask->setState(Ready);
	    pReadyQueue->addPriQueue( aKernel.pCurrentTask );
	} // else 

    } // else wyse 

}


void sysCall_read_return( char port ) {

    Task * pTask;
    char   readData[MAX_READ_DATA];
    int	length=0;
    int i;
    uint returnReadData;

    if(port == TRAIN) {
	// if is is already in read mode
	if(aSerial.isReadMode0()) {

	    // reset read mode
	    aSerial.resetReadMode0();
	    // get read requested task
	    pTask = aSerial.getReadReqTask0();
	    // set data address to set read data later on
	    returnReadData = aSerial.getSrcDataRead0();

	    // get number of the item from bounded buffer
	    length = pBDBuf_T_I->getNumItem();

	    for(i = 0; i < length; i++) {
		// get the item
		readData[i] = pBDBuf_T_I->getItem();
	    }
	    readData[length++] = '\0';

	    // XXX debug display
	    //cprintf(">>> Read data [%s]\n", readData);

	    // copy the read data to the user task data space 
	    //absCopy(pTask->pData+returnReadData, readData,length);
	    absWrite(pTask->pData+returnReadData, readData,length);
	    
	    // return the number of getting data
	    setReturnValueTo( length, pTask );

	    //  put into ready queue
	    pTask->setState( Ready );
	    pReadyQueue->addPriQueue( pTask );

	}// if

	// if it is not already ready Mode then error...
	else {

	    cprintf("!!!Warning, Read port0 isnot in readmode\n");
	    printf("!!!Warning, Read port0 isnot in readmode\n");

	} // else 

    }
    else if(port == WYSE ){
	// if is is already in read mode
	if(aSerial.isReadMode1()) {

	    // reset read mode
	    aSerial.resetReadMode1();
	    // get read requested task
	    pTask = aSerial.getReadReqTask1();
	    // set data address to set read data later on
	    returnReadData = aSerial.getSrcDataRead1();

	    // get number of the item from bounded buffer
	    length = pBDBuf_W_I->getNumItem();

	    for(i = 0; i < length; i++) {
		// get the item
		readData[i] = pBDBuf_W_I->getItem();
	    }
	    readData[length++] = '\0';

	    // XXX debug display
	    cprintf(">>> Read data [%s]\n", readData);

	    // copy the read data to the user task data space 
	    //absCopy(pTask->pData+returnReadData, readData,length);
	    absWrite(pTask->pData+returnReadData, readData,length);
	    
	    // return the number of getting data
	    setReturnValueTo( length, pTask );

	    //  put into ready queue
	    pTask->setState( Ready );
	    pReadyQueue->addPriQueue( pTask );

	}// if

	// if it is not already ready Mode then error...
	else {

	    cprintf("!!!Warning, Read port1(WYSE) isnot in readmode\n");
	    printf("!!!Warning, Read port1(WYSE) isnot in readmode\n");

	} // else 

    } // main else
//  NOT make ready until it gets CR(return) XXX XXX 
//  put into ready queue
    //aKernel.pCurrentTask->setState(Ready);
    //pReadyQueue->addPriQueue( aKernel.pCurrentTask );
}


void sysCall_write(void) {

    int index = 0;
    int errorCode = 0;

    char writeData[MAX_WRITE_DATA];
    char length;
    uint srcData;
    char port;
    char data;

    port = getParameter( 3 );
    length = getParameter( 2 );
    srcData = getParameter( 1 );

    absRead( aKernel.pCurrentTask->pData + srcData, writeData, length);

    //cprintf("\nWrite::: %s . len:%d\n", writeData, length);

    if( port == TRAIN ) {
	// if serial port is ready(THRU ready)
	// and no data in buffer
	// then out directly
	if( aSerial.isReady0() && pBDBuf_T_O->isEmpty() ) {
	    //cprintf(" out ");
	    outb( USART_0_DATA , writeData[index++]);
	    aSerial.resetReady0();
	} // if

	while(!pBDBuf_T_O->isFull() && index < length) {
	    //cprintf(" in buf ");
	    pBDBuf_T_O->addItem(writeData[index++]);
	} // while

	// if buffer is full, then waiting list
	if(index < length) {
	    // if buffer is full and THRE is not empty
	    // then add into event queue
	    // XXX need to handle waiting list 
	    // XXX
	    cprintf("T_O buffer full data is truncated!!!! \n");
	    printf("T_O buffer full is truncated !!!! \n");
	    aKernel.pCurrentTask->setState( EventBlocked );
	    pQueue_W_O->addQueue(aKernel.pCurrentTask);
	} // if
    } // if
    else { // ( port == WYSE(=1)) for WYSE 

	// if serial port is ready(THRU ready)
	// and no data in buffer
	// then out directly
	if( aSerial.isReady1() && pBDBuf_W_O->isEmpty() ) {
	    //cprintf(" out ");
	    outb( USART_1_DATA , writeData[index++]);
	    aSerial.resetReady1();
	} // if

	while(!pBDBuf_W_O->isFull() && index < length) {
	    //cprintf(" in buf ");
	    pBDBuf_W_O->addItem(writeData[index++]);
	} // while

	// if buffer is full, then waiting list
	if(index < length) {
	    // if buffer is full and THRE is not empty
	    // then add into event queue
	    // XXX need to handle waiting list 
	    // XXX
	    cprintf("W_O buffer full data is truncated!!!! \n");
	    printf("W_O buffer full is truncated !!!! \n");
	    aKernel.pCurrentTask->setState( EventBlocked );
	    pQueue_W_O->addQueue(aKernel.pCurrentTask);
	} // if
    } // else

//  put into ready queue
    aKernel.pCurrentTask->setState(Ready);
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );
}

//
//------------------------------------------------------------------
// 
// inner kernel interrupt dispatch functions (servers)
//	clock server and serial 0 1 server 
// 
//------------------------------------------------------------------
//

int handler_clockEvent() {

    int i=0, itemSize;
    Task * pTask;
    Task * pRmvTask;

    // increase upTime()
    aKernel.incUpTime();

    aKernel.pCurrentTask->setState(Ready);
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

    pTask = pClkEvtQueue->getHead();

#define TIMER_INCREMENT_WAY
#ifdef TIMER_INCREMENT_WAY
    while( pTask != (Task *)Empty) {
	if (!pTask->isTimeOut( aKernel.getUpTime() )) {
	    pTask = pTask->pNext;
	} //if
	else {

	// if time out then remove 
	    pRmvTask = pTask;
	    pTask = pTask->pNext;
	    pClkEvtQueue->removeElement(pRmvTask);

	// set removed task to Ready and put into ready queue
	    pRmvTask->setState( Ready );
	    pReadyQueue->addPriQueue( pRmvTask );
	} // else
    } // while
#endif // TIMER_INCREMENT_WAY

#undef TIMER_DECREMET_WAY
#ifdef TIMER_DECREMET_WAY
    while( pTask != (Task *)Empty) {
	if (!pTask->isTimeOut()) {
	    pTask->decreaseTimer();
	    pTask = pTask->pNext;
	} //if
	else {

	// if time out then remove 
	    pRmvTask = pTask;
	    pTask = pTask->pNext;
	    pClkEvtQueue->removeElement(pRmvTask);

	// set removed task to Ready and put into ready queue
	    pRmvTask->setState( Ready );
	    pReadyQueue->addPriQueue( pRmvTask );
	} // else
    } // while
#endif // TIMER_DECREMET_WAY
    
    // notify that End Of Interrupt to ICU
    SEND_EOI;
    return i;
}


//
//--
//  serial terminal DA** German TOY train control 
//--
//
int handler_serialEvent_0() {

#ifdef NOT_FLOW_CONTROL
    int i=2;
    BYTE iir, lsr;
    BYTE dummy, data, item;
    BYTE loop = 3;
    Task * pTask;

    Boolean CR_inputted = False;
    
    Error(ErrorNone,"Serial0");

// check this is for receive or transmit 
    iir = inb( USART_0_BASE + USART_IIR) & 0xf;

    // first check that this is read event 
    // check recieved data first 
    if(iir == 0x04) { /* bit 2-1 : 0 10 0 : received data  */

// XXX if this is not read mode then normal operation

	if(!aSerial.isReadMode0()) {

	    Error(ErrorNone," S0I ");

	    //do {

	    lsr = inb( USART_0_BASE + USART_LSR);
	    if( lsr & 0x1e)	 {
		dummy = inb( USART_0_BASE + USART_LSR );

	    }  //if
	    else if ( LSR_Data_Ready & lsr) { 
		data = inb( USART_0_DATA );
		//cprintf("%c",data);
		if( !pBDBuf_T_I->isFull() ) {
		// put the inputted data into bound buffer
		    pBDBuf_T_I->addItem( data );
		    // XXX check serial event queue
		} // if

	    } //  else if

	    iir = inb(USART_0_BASE + USART_IIR) & 0xf;

	    //} while( iir && (--loop > 0));

	    //cprintf("buffer num %d queue num %d\n",
			//pBDBuf_W_I->getNumItem(), pQueue_W_I->getNumItem());

    // check blocked queue
	    while( !pQueue_T_I->isEmpty() &&
			!pBDBuf_T_I->isEmpty()) {
		pTask = pQueue_T_I->getItem();
		// set ready state again
		pTask->setState( Ready );
		pReadyQueue->addPriQueue( pTask );

		//get the buffered data
		item = pBDBuf_T_I->getItem();
		setReturnValueTo( data, pTask );
		Error2(ErrorNone,"get data (%c) tid %d", item, pTask->getTid());
	    }
	}// if read mode 
// XXX if read mode then just read the data and put into buffer and
// check the data. if CR(=13) comes the call syscall_read_return with
// port number
	else {
	    do {

		lsr = inb( USART_0_BASE + USART_LSR);
		if( lsr & 0x1e)	 {
		    dummy = inb( USART_0_BASE + USART_LSR );

		}  //if
		else if ( LSR_Data_Ready & lsr) { 
		    data = inb( USART_0_DATA );

		    // if the data is not CR
		    if( data != 13)  { 
			//cprintf("%c",data);
			if( !pBDBuf_T_I->isFull() ) {
			// put the inputted data into bound buffer
			    pBDBuf_T_I->addItem( data );
			    // XXX check serial event queue
			} // if
		    }
		    // if CR is inputted
		    else {
			CR_inputted = True;

		    }

		} //  else if

		iir = inb(USART_0_BASE + USART_IIR) & 0xf;

	    } while( iir && (--loop > 0) && !(CR_inputted));

	    // if cr is inputted the call clearence function 
	    // of Read syscall
	    if(CR_inputted) {
		sysCall_read_return( TRAIN );
	    }
	    //cprintf("buffer num %d queue num %d\n",
			//pBDBuf_W_I->getNumItem(), pQueue_W_I->getNumItem());
	    
	} // processing Read syscall

    } // if 
///
/// bit 2-1 : 0 01 0 : THRE  
/// means output 
///
    else if(iir == 0x02) { 
	//cprintf("I buffer num %d queue num %d\n",
		    //pBDBuf_W_O->getNumItem(), pQueue_W_O->getNumItem());
	Error(ErrorNone," <<<S00>>> ");
	if(!pBDBuf_T_O->isEmpty()) {
	    data = pBDBuf_T_O->getItem();
	    outb( USART_0_DATA , data);
	}
	else {
	    // in case of empty set ready flag
	    aSerial.setReady0();
	}

    } // else if 

    aKernel.pCurrentTask->setState(Ready);
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

    SEND_EOI;
    return i;


#endif // NOT_FLOW_CONTROL


#ifdef FLOW_CONTROL
    int i=2;
    BYTE iir, lsr;

    BYTE msr;
    BYTE mcr;
    char ier;
    BYTE dummy, data, item;
    BYTE loop = 3;
    Task * pTask;

    char intSet;
    

// check this is for receive or transmit 
    iir = inb( USART_0_BASE + USART_IIR) & 0xf;

// XXX flow test

    Error2(ErrorDebug,"1st, Serial0 iir %x  msr %x", iir, msr);


    // first check that this is read event 
    // check recieved data first 
    if(iir == 0x04) { /* bit 2-1 : 0 10 0 : received data  */

	Error(ErrorDebug," S0I ");

	do {

	lsr = inb( USART_0_BASE + USART_LSR);
	if( lsr & 0x1e)	 {
	    dummy = inb( USART_0_BASE + USART_LSR );

	}  //if
	else if ( LSR_Data_Ready & lsr) { 
	    data = inb( USART_0_DATA );
	    //cprintf("%c",data);

	    // XXX flow
	    mcr = 0x0; //~MCR_rts
	    outb(USART_0_BASE + USART_IER, mcr);

	    if( !pBDBuf_T_I->isFull() ) {
	    // put the inputted data into bound buffer
		pBDBuf_T_I->addItem( data );
		// XXX check serial event queue
	    } // if

	} //  else if

	iir = inb(USART_0_BASE + USART_IIR) & 0xf;

	} while( iir && (--loop > 0));

	//cprintf("buffer num %d queue num %d\n",
		    //pBDBuf_W_I->getNumItem(), pQueue_W_I->getNumItem());


// check blocked queue
	while( !pQueue_T_I->isEmpty() &&
		    !pBDBuf_T_I->isEmpty()) {
	    pTask = pQueue_T_I->getItem();
	    // set ready state again
	    pTask->setState( Ready );
	    pReadyQueue->addPriQueue( pTask );

	    //get the buffered data
	    item = pBDBuf_T_I->getItem();
	    setReturnValueTo( data, pTask );
	    Error2(ErrorNone,"get data (%c) tid %d", item, pTask->getTid());
	}

    } // if 
///
/// bit 2-1 : 0 01 0 : THRE  
/// means output 
///

    //else if( iir == 0x00 || iir == 0x02 ) { 
	//msr = inb(USART_0_BASE + USART_MSR);
	//Error2(ErrorDebug,"2nd Serial0 iir %x  msr %x", iir, msr);

	//else if(iir == 0x00 && msr & 0x10) {  // test... ctr ??
	else if((iir == 0x00) ) {  // test... ctr ??

	    msr = inb(USART_0_BASE + USART_MSR) & 0xf0;

	    //cprintf("I buffer num %d queue num %d\n",
			//pBDBuf_W_O->getNumItem(), pQueue_W_O->getNumItem());
	    Error(ErrorDebug," <<<S00>>> ");
	    // XXX flow control
	    //msr = inb(USART_0_BASE + USART_MSR);
	    //cprintf(" <%x>", msr);
	    // XXX


	    if(msr & 0x10) {

	    if(!pBDBuf_T_O->isEmpty()) {
		data = pBDBuf_T_O->getItem();
		outb( USART_0_DATA , data);
		Error1(ErrorDebug," %x out ", data);

		lsr = inb( USART_0_BASE + USART_LSR);
		// XXX test flow control
		//mcr = 0x0; //~MCR_rts
		//outb(USART_0_BASE + USART_IER, mcr);

		msr = inb(USART_0_BASE + USART_MSR) & 0xf0;

		Error2(ErrorDebug,"after Serial0 iir %x  msr %x", iir, msr);
	    }
	    else {
		// in case of empty set ready flag
		aSerial.setReady0();
	    }
	    }
	} // test if

    //} // else if 

    aKernel.pCurrentTask->setState(Ready);
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

    SEND_EOI;
    return i;

#endif // FLOW_CONTROL

}


//
//--
//  WYSE terminal
//--
//
int handler_serialEvent_1() {
    int i=2;
    BYTE iir, lsr;
    BYTE dummy, data, item;
    BYTE loop = 3;
    Task * pTask;

    Boolean CR_inputted = False;
    
    Error(ErrorNone,"Serial1");

// check this is for receive or transmit 
    iir = inb( USART_1_BASE + USART_IIR) & 0xf;

    // first check that this is read event 
    // check recieved data first 
    if(iir == 0x04) { /* bit 2-1 : 0 10 0 : received data  */

// XXX if this is not read mode then normal operation

	if(!aSerial.isReadMode1()) {
    
	    Error(ErrorNone," S1I ");

	    do {

	    lsr = inb( USART_1_BASE + USART_LSR);
	    if( lsr & 0x1e)	 {
		dummy = inb( USART_1_BASE + USART_LSR );

	    }  //if
	    else if ( LSR_Data_Ready & lsr) { 
		data = inb( USART_1_DATA );
		//cprintf("%c",data);
		if( !pBDBuf_W_I->isFull() ) {
		// put the inputted data into bound buffer
		    pBDBuf_W_I->addItem( data );
		    // XXX check serial event queue
		} // if

	    } //  else if

	    iir = inb(USART_1_BASE + USART_IIR) & 0xf;

	    } while( iir && (--loop > 0));

	    //cprintf("buffer num %d queue num %d\n",
			//pBDBuf_W_I->getNumItem(), pQueue_W_I->getNumItem());

    // check blocked queue
	    while( !pQueue_W_I->isEmpty() &&
			!pBDBuf_W_I->isEmpty()) {
		pTask = pQueue_W_I->getItem();
		// set ready state again
		pTask->setState( Ready );
		pReadyQueue->addPriQueue( pTask );

		//get the buffered data
		item = pBDBuf_W_I->getItem();
		setReturnValueTo( data, pTask );
		Error2(ErrorNone,"get data (%c) tid %d", item, pTask->getTid());
	    }
	}// if read mode 

// XXX if read mode then just read the data and put into buffer and
// check the data. if CR(=13) comes the call syscall_read_return with
// port number
	else {
	    do {

		lsr = inb( USART_1_BASE + USART_LSR);
		if( lsr & 0x1e)	 {
		    dummy = inb( USART_1_BASE + USART_LSR );

		}  //if
		else if ( LSR_Data_Ready & lsr) { 
		    data = inb( USART_1_DATA );

		    // if the data is not CR
		    if( data != 13)  { 
			//cprintf("%c",data);
			if( !pBDBuf_W_I->isFull() ) {
			// put the inputted data into bound buffer
			    pBDBuf_W_I->addItem( data );
			    // XXX check serial event queue
			} // if
		    }
		    // if CR is inputted
		    else {
			CR_inputted = True;

		    }

		} //  else if

		iir = inb(USART_1_BASE + USART_IIR) & 0xf;

	    } while( iir && (--loop > 0) && !CR_inputted);

	    // if cr is inputted the call clearence function 
	    // of Read syscall
	    if(CR_inputted) {
		sysCall_read_return( WYSE );
	    }
	    //cprintf("buffer num %d queue num %d\n",
			//pBDBuf_W_I->getNumItem(), pQueue_W_I->getNumItem());
	    
	} // processing Read syscall

    } // if 
///
/// bit 2-1 : 0 01 0 : THRE  
/// means output 
///
    else if(iir == 0x02) { 
	//cprintf("I buffer num %d queue num %d\n",
		    //pBDBuf_W_O->getNumItem(), pQueue_W_O->getNumItem());
	Error(ErrorNone," <<<W0>>> ");
	if(!pBDBuf_W_O->isEmpty()) {
	    data = pBDBuf_W_O->getItem();
	    outb( USART_1_DATA , data);
	    // XXX
	    //lsr = inb( USART_1_BASE + USART_LSR);

	}
	else {
	    // in case of empty set ready flag
	    aSerial.setReady1();
	}
	// XXX NEED TO DO waiting list processing

    } // else if 

    aKernel.pCurrentTask->setState(Ready);
    pReadyQueue->addPriQueue( aKernel.pCurrentTask );

    SEND_EOI;
    return i;
}


//
//--
// system call dispatch
//--
//
int dispatcher_sysCall( void ) {

    int i=0;

//  fast switch for dispatching interrupt
    switchSysCall[SysCallDispatchType]();

    return i;
}


//********************************************************************
//
// function : loadTask()
//
//	set up TDT(task descriptor table)s and stack for 
//	context switching
// 
// *******************************************************************
// xloadtask

Task * loadTask(char *moduleName,int pri)
{
    int bindToRun;           /* Entry in the bind file to run */
    int tid; 			/* indicating process id value */
    int i, cs, ds;                          /* Loop index */
    int baseS;                      /* Data Segment, base real pointer */
    int dummyParm;                  /* Segment setting */
     
    struct exec aHeader;
    struct exec *pHeader = &aHeader;
    bindEntry aEntry;
    bindEntry *pEntry = &aEntry;
    Task      *pTask;
    Error1(ErrorNone, "LoadTask %s",moduleName);
    bindToRun = aBindfile.findModule(moduleName);

    // check bindToRun number is valid 
    if(bindToRun < 0) {
	 Error1(ErrorWarning,
	     "WARNING!!! @ loadTask: bindToRun num error%d",bindToRun);   
	 Error (ErrorWarning,"MUST create with invalid module name");
	 //Halt();
    } // if
    Error1(ErrorNone,"@loadTask: bindToRun NUMBER : %d",bindToRun);
    // read aout header
    aAoutHeader.getAoutHeader( bindToRun, pHeader, &aBindfile);

    aBindfile.getBindEntry( bindToRun, pEntry);
    // XXX  check this is index 0, 1 ... or 0x30, 0x38 ...
    // Create a GDT entry for the code 
    //aKernel.taskCS = setGDTVal(
    cs = setGDTVal(
     (pHeader->a_text + pHeader->a_data + sizeof(struct exec) + pHeader->a_bss),
     (pEntry->offset + aAoutHeader.kernelBaseAddress),GDTTypeCode);
    // Create a GDT entry for the data segment 
    //aKernel.taskDS = setGDTVal(
    //cprintf("before setting GDT stacktop 0x%x \n", aKernel.getStackTop());
    ds = setGDTVal(
	(pHeader->a_data + pHeader->a_bss + pEntry->stackSize), 
	aKernel.getStackTop(), GDTTypeData);

    pTask = aTDTable.getNextFreeTask();
    Error1(ErrorNone,"getNextFreeTask tid %d", pTask->getTid());
// 
//  init TDT table
//  I need pHeader, pEntry
//
    pTask->codeSize = pHeader->a_text + pHeader->a_data + 
				 sizeof(struct exec) + pHeader->a_bss;
    pTask->dataTotalSize = pHeader->a_data + pHeader->a_bss +
			    pEntry->stackSize;
    pTask->entryPoint = pHeader->a_entry;
    pTask->stackSize = pEntry->stackSize;
    pTask->userCS = aKernel.taskCS;
    pTask->parentTid = aKernel.parentTid;

    tid = aKernel.currentTid = pTask->getTid();
    aKernel.parentTid = aKernel.currentTid;

    RegState regSet;                /* Register set at startup */
    baseS = getBaseS( ds );

    userCS = getCodeSelector( pTask->getTid() );
    userDS = getDataSelector( pTask->getTid() );
    userSS = userDS;
    userSP = pTask->dataTotalSize - 4;

    Error1(ErrorNone,"baseS of User module1 : 0x%x",baseS);
    Error1(ErrorNone,"DS of User module1 : 0x%x",userDS);
    Error1(ErrorNone,"CS of User module1 : 0x%x",userCS);
    Error1(ErrorNone,"total DATA size of User mocule1 : 0x%x",\
	 pTask->dataTotalSize);

    regSet.gs=regSet.fs=0;

    regSet.es = userDS;
    regSet.ds = userDS;
    regSet.cs = userCS;
    regSet.eflags = DEFAULT_FLAG;
    regSet.eip = pTask->entryPoint;

    regSet.eax = regSet.ebx = regSet.ecx = \
    regSet.edx = regSet.ebp = regSet.esi = \
    regSet.edi = 0;

    dummyParm=0;
    /* bindTable and numFiles information */
    absWrite((int) baseS+userSP - 12, &dummyParm,4);
    absWrite((int) baseS+userSP - 8, &dummyParm,4);
    /* DS as third parameter */
    dummyParm = userDS;
    absWrite((int) baseS + userSP - 4, &dummyParm, 4);
    absWrite((int) baseS + userSP - 12 - sizeof(regSet), 
		&regSet, sizeof(regSet));

    userSP -= (3 * sizeof(int) + sizeof(regSet) );

    pTask->pData    = baseS;
    pTask->setPriority(pri);
    pTask->userSP   = userSP;
    pTask->userSS   = userDS;
    pTask->userDS   = userDS;
    pTask->userCS   = userCS;
    //TDTable[freeTD].msgQueueHead = nil;
    //TDTable[freeTD].msgQueueTail = nil;

//  set state into Ready and put into ready queue
    pTask->setState(Ready);
    //aTDTable.readyQueue.addPriQueue( pTask );
    pReadyQueue->addPriQueue( pTask );

    Error3(ErrorNone,"@loadTask: PID[%d] : %s module starting pt :%x"\
		 ,tid,moduleName,pEntry->offset);
// update real memeory top (stack top)
    aKernel.increaseStackTop(
		pHeader->a_data + pHeader->a_bss + pEntry->stackSize );
    return pTask;	// return loaded task pointer 
}


extern uint _entryPtEnterKernel;
extern uint _entryPtExitKernel;
extern uint _entryPtClockTick;
extern uint _entryPtSerial0;
extern uint _entryPtSerial1;

/********************************************************************
*
* function : getNextRequest()
*
*	get next highest ready task 
*	and exit from the kernel
*	And enter from task into kernel
*
*******************************************************************
*xgetnextrequest
*/
int getNextRequest(void) {

    Task * pTaskToRun;

// get next highest task
    //pTaskToRun = aTDTable.readyQueue.getNextTask();
    pTaskToRun = pReadyQueue->getNextTask();
// set state as ACTIVE
    pTaskToRun->setState(Active);

    aKernel.setCurrentTask( pTaskToRun );

    Error3(ErrorNone,\
    "Before INTERRUPT CS : 0x%x , DS : 0x%x , ESP : 0x%x ",userCS,
    userDS, userSP); 

    Error2(ErrorNone,"@ nextToRun return Value TID %d, priority %d", \
      pTaskToRun->getTid(), pTaskToRun->getPriority());

    /* set SS,ES,ESP for the process candidate */
    userSS=pTaskToRun->userSS;
    userSP=pTaskToRun->userSP;
    userDS=pTaskToRun->userDS;

    Error3(ErrorNone,"DS: 0x%x, SS: 0x%x, ESP: 0x%x",pTaskToRun->userDS,\
	 pTaskToRun->userSS, pTaskToRun->userSP);


/********************************************************************
*
* exitKernel
*
*******************************************************************
*xexitkernel
*/
interruptTestVal++;
asm volatile("
      .text
      .global _entryPtExitKernel
_entryPtExitKernel:
      cli
      pushal
      pushw %gs
      pushw %fs
      pushw %es
      pushw %ds

# store kernel's sp 
      movl %esp,_kernelSP

# get user process's sp and ss
      movw _userDS,%ss
      movl _userSP,%esp

      movw _userDS, %ax
      movw  %ax,%ds

      popw %ds
      popw %es
      popw %fs
      popw %gs
      popal
      iretl

      ");

/*************************************************************
*
* enterKernel
*
**************************************************************
*xenterkernel
*/
asm volatile("
      .text
      .global _entryPtEnterKernel
      .global _entryPtClockTick
      .global _entryPtSerial0
      .global _entryPtSerial1
_entryPtEnterKernel:
      cli
      pushal
      movl  $0, %ebx
      jmp CommonPartHandler

# entry point for clock tick (PIT) type 0
_entryPtClockTick:
      cli
      pushal
      movl  $1, %ebx
      jmp CommonPartHandler

      nop

# entry point for Serial 0 int type 2
_entryPtSerial0:
      cli
      pushal
      movl  $2, %ebx
      jmp CommonPartHandler

      nop

# entry point for Serial 1  type 3
_entryPtSerial1:
      cli
      pushal
      movl  $3, %ebx
      jmp CommonPartHandler

      nop

CommonPartHandler:
      pushw %gs
      pushw %fs
      pushw %es
      pushw %ds

      pushw %ds

# set kernel DS
      movl  $0x38,%eax
      movw  %ax,%ds
      movw  %ebx,_InterruptType

# set user DS
      popw  _userDS

# save user SS, SP
      movw  %ss,_userSS
      movl  %esp,_userSP

# set kernel SS SP
      movw  %ds,%ax
      movw  %ax,%ss
      movl  _kernelSP,%esp

# restor kernel's registers set
      popw  %ds
      popw  %es
      popw  %fs
      popw  %gs
      popal
      ");
// this value should be ZERO(=0) when you are in kernel 
interruptTestVal--;
    // or interruptTestVal is minus because interrup happened 
    // when you are in the Kernel area
    if(interruptTestVal < 0) {
	Error(ErrorWarning,"interrupted when you are in Kernel!!!");
	cprintf("interrupted when you are in Kernel!!!\n");
	printf("interrupted when you are in Kernel!!!\n");
	getchar();
    } // if

    //DISABLE_INTERRUPT

    Error(ErrorNone," Back to Kernel");

    /* save user SS ES ESP */
    pTaskToRun->userSS=userSS;
    pTaskToRun->userSP=userSP;
    pTaskToRun->userDS=userDS;

    if( InterruptType == 0) {
	absRead( pTaskToRun->pData+ pTaskToRun->userSP + STORED_REG,
				 &SysCallDispatchType, 4 );
	pTaskToRun->requestedSysCall = SysCallDispatchType;
	Error1(ErrorNone,"SysCall type is %x",SysCallDispatchType);
    } /* if*/
    return InterruptType;

#undef NOT_NEEDED
#ifdef NOT_NEEDED

    else if(InterruptType == 1) {
     Error1(ErrorNone,"@getNextRequest : Clock InterruptType is %d"
		 ,InterruptType);
	return InterruptType;
    } /* else if */

    else if(InterruptType == 2) {
     Error1(ErrorNone,"@getNextRequest : InterruptType is %d"
		 ,InterruptType);
	return InterruptType;
    } /* else if */

    else if(InterruptType == 3) {
     Error1(ErrorNone,"@getNextRequest : InterruptType is %d"
		 ,InterruptType);
    } /* else if */
    return InterruptType;
#endif // NOT_NEEDED

} // getNextRequest();

// 
//	hardware initialization
//
void initialize( void ) {

    char inputChar;



    initGDT();
    initIDT();
    initClock();
    initSerial( DUMMY );

/*
#ifdef	HATE_WYSE 
    cprintf("\n\nDo you want to use another serial terminal ?(y/n)\n");
    inputChar = getchar();
    */

    /* another serial terminal setting */
    /*
    if(inputChar == 'Y' || inputChar =='y') {
	*/
	//initSerial( ANOTHER_WYSE );
	/*

#else
    }
    */
/*#endif  HATE_WYSE */

}

//
//  set irq mask for interrupt starting
//
void setMaskIRQ() {
    maskIRQ(CLK_IRQ_NUM, ENABLE);
    maskIRQ(SERIAL0_IRQ_NUM, ENABLE);
    maskIRQ(SERIAL1_IRQ_NUM, ENABLE);
}

//
// hardware abstract layer test
//
void halTest() {

//  Bindtable and aout header check
    cprintf("\nKernel's code selector 0x%x\n", getCodeSelector(0));
    cprintf("Kernel's data selector 0x%x\n", getDataSelector(0));

    aBindfile.findModule("Kernel");
    aBindfile.printBindfile();

    aAoutHeader.printAoutHeader(&aBindfile);
}


//
//------------------------------------------------------------------------
//
//	kernel main function
//
//------------------------------------------------------------------------
//

main(uint bindTable,int numFiles ) {
    
    aBindfile.initBindfile( bindTable, numFiles );

    int i;
    int request, status;

// debug message init
    initDebug(ErrorDebug); 

// hardware initialization
    initialize();

// hal(hardware abstraction layer) test
    //halTest();

//
//  kernel initialization
//
// baseDS : start data area address  dataTop : top of the data 
// from crt0.S  kernel module(file) module number is 0
//i = aBindfile.getStackSize( 0 );
    aKernel.setStackTop( baseDS + dataTop + aBindfile.getStackSize( 0 ) );
    aKernel.setKernelDataStart( baseDS );

    aKernel.pKernelTask = aTDTable.getNextFreeTask();
    loadTask( "Init", UserPriority);
    loadTask( "Idle", IdleTaskPriority);

    setMaskIRQ();

    while (FOR_EVER) {                
    // fast switch statement
    // main dispatching
    // terse, simple pretty nice ?
	status= switchFunctions[getNextRequest()]();

	//request = getNextRequest();     
	//status= switchFunctions[request]();
    } // while

} // main
