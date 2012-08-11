#include "bsp.h"

#define enterCS() __asm__ __volatile__ ("cli");

#define exitCS() __asm__ __volatile__ ("sti");

//int IntNesting = 0;
//int LockNesting = 0;

//inline enterCS( void ) {
//}

//inline exitCS( void ) {
//}



void maskIRQ( unsigned char irq, unsigned int flag ) {

    unsigned int    port = IO_ICU1+1;
    unsigned char   val;

    if (irq>=8) {                 /* an irq on the secondary ICU */
	port = IO_ICU2+1;
	irq -= 8;
    }                             /* if  */

    val = inb(port);

    if (!flag)
	val |= (1<<irq);            /* turn off the interrupt */
    else
	val &= ~(1<<irq);           /* turn on the interrupt */

    outb(port, val);              /* mask the interrupt enable */
}

inline void enterInt( void ) {
    enterCS();
    IntNesting++;
    exitCS();
}

inline void exitInt( void ) {
    enterCS();
    if (( --IntNesting | LockNesting) == 0) { // reschedule only if all ISRs
					      // completed & not locked
	// context switch
    }
    exitCS();
}

inline void lockSched( void ) {
    enterCS();
    LockNesting++;
    exitCS();
}

inline void unlockSched( void ) {
    enterCS();
    if ( LockNesting != 0) {
	LockNesting--;
	if (( LockNesting | IntNesting) == 0) {
	    exitCS();
	    //Sched();
	} else {
	    exitCS();
	}
    } else {
	exitCS();
    }
}
