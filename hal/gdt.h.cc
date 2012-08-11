#ifndef	_GDT_H_
#define	_GDT_H_

#include	"../include/util/type.h"
#include	"../include/util/error.h"
#include	"../include/util/errorCode.h"
extern "C" {
    #include	"../include/i486/machine/segments.h"
    #include	"../include/i486/machine/cpufunc.h"
    #include	"../include/i486/cs452.h"
};


void init( void );
int getCodeSelector( int tid );
int getDataSelector( int tid );



#endif	// _GDT_H_
