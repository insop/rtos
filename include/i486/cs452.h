#ifndef __CS452_H__
#define __CS452_H__

/* Basic include file for all CS452 programs.
 *
 * You can think of this as a replacement for "stdio.h", but on an
 * embedded system, file IO is exactly what you don't have.
 * Still, this file defines useful stuff like NULL that normally is
 * found in stdio.h
 *
 * In particular, there is no "FILE *" data type, so all functions
 * starting with "f" are omitted. Also, "stdin" et al are not
 * defined.
 *
 */

#if !defined(_ANSI_SOURCE) && !defined(__STRICT_ANSI__)
#include <sys/types.h>
#endif

#include "sys/cdefs.h"

#include "machine/ansi.h"
#ifdef	_BSD_SIZE_T_
typedef	_BSD_SIZE_T_	size_t;
#undef	_BSD_SIZE_T_
#endif

#ifndef NULL
#define	NULL	0
#endif

/*
 * Functions defined in ANSI C standard.
 */
__BEGIN_DECLS
int	 getchar __P((void));
char	*gets __P((char *));
int	 putchar __P((int));
int	 puts __P((const char *));
int	 puts_plain __P((const char *));		/* no appended NL */
int	 scanf __P((const char *, ...));
int	 sscanf __P((const char *, const char *, ...));
int	 cprintf __P((const char *, ...));	/* to attached monitor, not serial */
int	 printf __P((const char *, ...));
int	 sprintf __P((char *, const char *, ...));
int	 vprintf __P((const char *, _BSD_VA_LIST_));
int	 vsprintf __P((char *, const char *, _BSD_VA_LIST_));
__END_DECLS

/*
 * Routines that are common, but not ANSI
 */
#if !defined (_ANSI_SOURCE) && !defined(_POSIX_SOURCE)
__BEGIN_DECLS
int	 snprintf __P((char *, size_t, const char *, ...));
int	 vsnprintf __P((char *, size_t, const char *, _BSD_VA_LIST_));
int	 vscanf __P((const char *, _BSD_VA_LIST_));
int	 vsscanf __P((const char *, const char *, _BSD_VA_LIST_));
__END_DECLS
#endif


/*
 * Things that are really just CS452 related. Nothing to do with ANSI
 * here.
 * 
 */
__BEGIN_DECLS

/* Read/write absolute (linear) address in memory into local address space */
void absRead(uint from, void *to, int count);
void absWrite(uint to, void *from, int count);
void absCopy(uint to, uint from, int count);	/* .. or from-to abs loc */
void absZero(uint at, int count);				/* .. or clear abs block */

/* IDT management */
void loadIdt();				/* should, don't have to call this first */
void setIdt(uint idx, void (*func)(), uint typ, uint dpl);

/* Set by crt0.S for the first file in bindfile */
extern uint baseDS;		/* linear address of start of DS thanks to bootstrap */
extern uint dataTop;	/* first byte after end of data+bss segments (sbrk) */

/* Text console output */
void console_init(void);				/* optional */
void console_clear(void);
void console_putchar(int c);
void console_xy(int x, int y);		/* position cursor */

/* IBM keyboard input */
void console_kbdreset();			/* call first */
int  console_kbhit();				/* return T if char waiting */
char console_getchar();				/* returns ASCII for keys as pressed */

/* Unix compatibility */
int getpagesize();			/* always returns 16 (for malloc's benefit) */
caddr_t sbrk(int incr);		/* "allocates" more memory from stack depth */

/* Other stuff */
void soft_cpu_reset();		/* reset the machine from software, handy   */
caddr_t MemMap(uint offset, uint length);		/* map physical memory */

/* Wrapper version of printf that calls "Put", a cs452 kernel primitive */
int	 Printf __P((const char *, ...));

__END_DECLS

#endif /* __CS452_H__ */
