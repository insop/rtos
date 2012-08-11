#ifndef	_GDT_H_
#define	_GDT_H_

#include	"../include/util/error.h"
#include	"../include/util/errorCode.h"
#include	"../include/i486/machine/segments.h"
#include	"../include/i486/machine/cpufunc.h"
#include	"../include/i486/cs452.h"

#define GDTTypeCode  		0x1f	/* gdt type for code segments */
#define GDTTypeData  	0x13	/* gdt type for data segments */

#define DefaultGDTSize  	64	/* default GDT size */

#define NumSelectors	 	520 	
				    /* 256(max process) * 2(cs,ds) + 8 */
/*
void initGDT( void );
int getCodeSelector( int tid );
int getDataSelector( int tid );
int getBaseS( int bindToRun );
*/




#endif	/* _GDT_H_ */
