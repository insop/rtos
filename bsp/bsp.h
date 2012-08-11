#ifndef	_BSP_H
#define _BSP_H_

#include "../include/i486/cs452.h"
#include "../include/i486/machine/pio.h"
#include "../include/i486/machine/pc/isareg.h"
#include "../include/i486/machine/serial.h"
#define DEFAULT_FLAG     0x286
/* number of the register stored
 * when context switching 
 */
#define STORED_REG     	 52

typedef struct {            /* Register state at the moment of... */
   short ds, es, fs, gs;    /* ...Context switch */
   uint edi, esi, ebp, esp, ebx, edx, ecx, eax;
   uint eip, cs, eflags;
} RegState;


static int IntNesting = 0;
static int LockNesting = 0;


/* interrupt number of ICU 8259*/
#define CLK_IRQ_NUM	    0
#define SERIAL0_IRQ_NUM	    4
#define SERIAL1_IRQ_NUM	    3


#define SYSCALL_REQUEST 0
#define CLOCK_REQUEST	1
#define SERIAL0_REQUEST 2
#define SERIAL1_REQUEST	3
/*#define INTERRUPT_SYSCALL	__asm volatile("int $69\n\t");*/
#define EXIT_KERNEL_INT	70
/*#define INTERRUPT_EXIT_KERNEL	__asm volatile("int $70\n\t");*/
#define SYSTEM_INT	69
#define INIT_PRIORITY   HIGHEST_PRIORITY
#define IDLE_PRIORITY   7
#define DUMMY_INT	(NOOFINTS-1)
#define DUMMY_CLOCK_INT	(NOOFINTS-2)
#define STORED_REG	52
#define TRUE		1
#define FALSE		0
#define ENABLE		1
#define DISABLE		0
#define FOR_EVER	TRUE


#define SEND_EOI { outb( IO_ICU1, 0x020 ); \
			             outb( IO_ICU2, 0x020 ); }

#define     ENABLE_INTERRUPT	__asm volatile( "sti;\n\t" );
#define     DISABLE_INTERRUPT	__asm volatile( "cli;\n\t" );

/* serial port define */
#define DATA_READY	1	/* LSR Data Ready bit: DR: bit0 */



#define NAME_LENGTH     20              /* maximun length of
				         * binding module name */


/* serial stuffs */

/* port 0 is download port
 * port 1 is console port */
#define LCR             USART_LCR
#define LSR             USART_LSR
#define MCR             USART_MCR
#define MSR             USART_MSR
#define IER             USART_IER
#define FCR             USART_FCR

#define u0_status               inb(USART_0_BASE + USART_LSR)
#define u0_data_r               inb(USART_0_DATA)
#define u0_data_w(x)            outb(USART_0_DATA, x)

#define u1_status               inb(USART_1_BASE + USART_LSR)
#define u1_data_r               inb(USART_1_DATA)
#define u1_data_w(x)            outb(USART_1_DATA, x)


#define UART

typedef struct context {

    /*registers. ..
    //segments...
    //and board specific part
    //...
    */

} context;


void maskIRQ( unsigned char irq, uint _flag);

/*void enterCS( void ) ;*/
/*void exitCS( void );*/

void enterInt( void );
void exitInt( void );

void lockSched( void );
void unlockSched( void );


void enterKernel( void );
void exitKernel( void );

void contextSwitch( void );
/* when context switch if the switched task is the same as now
// you don't need to context change
*/



#endif // _BSP_H_
