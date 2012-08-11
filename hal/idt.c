#include 	"idt.h"

/* 
*	interrupt process functions 
*/
extern uint entryPtEnterKernel;
extern uint entryPtExitKernel;
extern uint entryPtClockTick;
extern uint entryPtSerial0;
extern uint entryPtSerial1;
extern uint entryPtKeyInt;
extern uint entryPtInt0;
extern uint entryPtInt1;
extern uint entryPtInt2;
extern uint entryPtInt3;
extern uint entryPtInt4;
extern uint entryPtInt6;
extern uint entryPtInt8;
extern uint entryPtInt12;
extern uint entryPtInt13;

/*
//	initialize IDT (Interrupt Descriptor Table)
//	>> interrupt vector
*/


/* 
* Initialize the Interrupt Descriptor Table, should be called during
* kernel initialization.
*
*/
void initIDT( void ) {

  loadIdt();
  /*DISABLE_INTERRUPT*/
  setIdt(SYS_CALL_INT,	 (void *) (uint) &entryPtEnterKernel,SDT_SYS386IGT,0);

  setIdt(CLOCK_TICK_INT, (void *) (uint) &entryPtClockTick,  SDT_SYS386IGT,0);
  setIdt(SERIAL0_INT,    (void *) (uint) &entryPtSerial0,    SDT_SYS386IGT,0);
  setIdt(SERIAL1_INT,    (void *) (uint) &entryPtSerial1,    SDT_SYS386IGT,0);

  /* key int for reboot */
  /*setIdt(33,             (void *) (uint) &entryPtKeyInt,  SDT_SYS386IGT,0);*/


  /*
  setIdt(DIVIDE_INT,    (void *) (uint) &entryPtInt0,SDT_SYS386IGT,0);
  setIdt(DEBUG_INT,     (void *) (uint) &entryPtInt1,SDT_SYS386IGT,0);
  setIdt(NMI_INT,	(void *) (uint) &entryPtInt2,SDT_SYS386IGT,0);
  setIdt(BREAKPOINT_INT,(void *) (uint) &entryPtInt3,SDT_SYS386IGT,0);
  setIdt(OVERFLOW_INT,	(void *) (uint) &entryPtInt4,SDT_SYS386IGT,0);
  setIdt(OPCODE_INT,	(void *) (uint) &entryPtInt6,SDT_SYS386IGT,0);
  setIdt(DOUBLE_INT,	(void *) (uint) &entryPtInt8,SDT_SYS386IGT,0);
  setIdt(STACK_INT,	(void *) (uint) &entryPtInt12,SDT_SYS386IGT,0);
  setIdt(GPF_INT,	(void *) (uint) &entryPtInt13,SDT_SYS386IGT,0);
  */
}


