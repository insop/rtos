#ifndef	_IDT_H_
#define	_IDT_H_

#include	"../include/util/error.h"
#include	"../include/i486/cs452.h"
#include "../include/i486/machine/segments.h"

/*
void initIDT( void );
*/

/* XXX should be changed */
#define SYS_CALL_INT	69
#define EXIT_KERNEL_INT	70

#define CLOCK_TICK_INT	32
#define SERIAL0_INT	36
#define SERIAL1_INT	35

#define DIVIDE_INT	0
#define DEBUG_INT	1
#define NMI_INT		2		
#define BREAKPOINT_INT	3
#define OVERFLOW_INT	4
#define OPCODE_INT	6
#define DOUBLE_INT	8
#define STACK_INT	12
#define GPF_INT		13


#endif	/* _IDT_H_ */
