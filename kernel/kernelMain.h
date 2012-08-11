//
//--
//
//
//	FEMOS(Fast EMbedded OS) kernel main  
//
//
//--
//
//
// 	Written by 
//
//	INSOP SONG
//
// 	Systems Design
//	
//	Michael Cole Computer Science
//
// 	University of Waterloo, ON, Canada
//
//	June,July 2001
//
//--
//

#ifndef	_KERNEL_MAIN_H_
#define _KERNEL_MAIN_H_

#include "../hal/bindfile.h"
#include "../hal/aout.h"
#include "../bsp/bsp.h"

#include "../task/task.h"
#include "../task/taskDescTable.h"
#include "../task/priQueue.h"
#include "../task/queue.h" 
#include "../syscall/sysCall.h"

#include "../kernel/kernel.h"

#include "../serial/serial.h"

#include "../include/util/errorCode.h"
#include "../include/util/type.h"

extern "C" {
    #include "../hal/gdt.h"
    //#include "../hal/idt.h"
    #include "../include/i486/cs452.h"
}; // C function interface 


/*
 * include only if it is with the kernel.c
 */
#ifdef _KERNEL_MAIN_H_

/* 
 *definition of the numbers of the sysCall functions input parameter
 */
const int PARAM_NUM[LAST_NUM_SYS_CALL+1] =
{
    0,  /* default */	
    2,  /* create */		
    0,  /* pass */		
    0,  /* exit */		
    0,  /* mypid */		
    0,  /* parentpid */	
    0,  /* mypriority */	
    5,  /* send(tid, &msg,len,&msg,len)*/		
    2,  /* receive */	
    3,  /* reply */		
    1,  /* valid pid */		
    1,  /* await event */		
    1,  /* delay */		
    1,  /* get   */
    2,  /* put */
    2,  /* read */
    3,  /* write */
    0,  /* uptime   */
    1,  /* delayUntil */		
    1,  /* getAbsmem */		
    1,  /* wakeUp */		
    0   /* dummy as last value */
};

/* 
 *definition of the numbers of the sysCall functions output parameter
 */
const int RET_PARAM_NUM[LAST_NUM_SYS_CALL+1] =
{
    0,  /* default */	
    1,  /* create */		
    0,  /* pass */		
    0,  /* exit */		
    1,  /* mypid */		
    1,  /* parentpid */	
    1,  /* mypriority */	
    1,  /* send */		
    1,  /* receive */	
    1,  /* reply */		
    1,  /* valid pid */		
    1,  /* await event */		
    1,  /* delay */		
    1,  /* get */
    0,  /* put */
    1,  /* read */
    1,  /* write */
    1,  /* uptime   */
    0,  /* delayUntil */		
    1,  /* getAbsmem */		
    1,  /* wakeup */		
    0   /* dummy as last value */
};
#endif / _KERNEL_MAIN_H_ 

void initialize( void );
Task * loadTask( char *moduleName,int pri );
int  getNextRequest( void );

void setReturnValue( uint returnVal );
void setReturnValueTo( uint returnVal, Task * pTask );

// forward declaration
// for sysCall dispatch functions
void sysCall_invalidSyscall(void);
void sysCall_create(void);
void sysCall_pass(void);
void sysCall_exit(void); // for exit syscall
void sysCall_myTid(void);
void sysCall_myParentTid(void);
void sysCall_myPriority(void);
void sysCall_send(void);
void sysCall_receive(void);
void sysCall_reply(void);
void sysCall_validTid(void);
void sysCall_awaitEvent(void);
void sysCall_delay(void);
void sysCall_get(void);
void sysCall_put(void);
void sysCall_read(void);
void sysCall_write(void);
void sysCall_upTime(void);
void sysCall_delayUntil(void);
void sysCall_getAbsMem(void);
void sysCall_wakeUp(void);
//--
void sysCall_lastSysCall(void);
//--
int dispatcher_sysCall( void ); 
int handler_clockEvent( void ); 
int handler_serialEvent_0( void ); 
int handler_serialEvent_1( void ); 
#endif	// _KERNEL_MAIN_H_
