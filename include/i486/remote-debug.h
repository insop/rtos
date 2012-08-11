#ifndef __REMOTE_DEBUG_H__
#define __REMOTE_DEBUG_H__

#include <cs452.h>		/* for loadIdt() */

/*
 * Include this file to use remote GDB debugging.
 *
 */

/*
 * Put this as first thing inside "main" to enable debugging.
 */
#define ENABLE_DEBUG		{ loadIdt(); set_debug_traps(); breakpoint(); }


/* Call this function as the first thing in your program. It initializes
 * things so that the remote debugging stub code will catch certain
 * interrupts. To enable debugging, call "breakpoint" next. Communication
 * with a remote instance of gdb will then start.
 */
extern void set_debug_traps();

/* "breakpoint()" can be used anywhere to generate an INT3 and cause GDB to
 * be entered.
 */
extern void breakpoint();

#endif
