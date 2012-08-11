#ifndef _ERROR_H_
#define _ERROR_H_
#include <stdio.h>

enum {
  ErrorNone,
  ErrorDebug,
  ErrorWarning,
  ErrorFatal,
  ErrorExit
};


void initDebug(int _errorThreshold);

#ifdef DEBUG
extern int errorThreshold;

#define Error(errorLevel, errorMsg) \
	{ if (errorLevel>=errorThreshold) \
	  cprintf( "(%10s:%4d)\t%s\n", __FILE__, __LINE__, errorMsg ); \
	}

#define Error1(errorLevel, errorMsg, arg1) \
	{ if (errorLevel>=errorThreshold) { \
	    cprintf( "(%10s:%4d)\t", __FILE__, __LINE__ ); \
	    cprintf( errorMsg, arg1 ); \
	    cprintf( "\n" ); \
	  } \
	}

#define Error2(errorLevel, errorMsg, arg1, arg2) \
	{ if (errorLevel>=errorThreshold) { \
	    cprintf( "(%10s:%4d)\t", __FILE__, __LINE__ ); \
	    cprintf( errorMsg, arg1, arg2 ); \
	    cprintf( "\n" ); \
	  } \
	}

#define Error3(errorLevel, errorMsg, arg1, arg2, arg3) \
	{ if (errorLevel>=errorThreshold) {\
	    cprintf( "(%10s:%4d)\t", __FILE__, __LINE__ ); \
	    cprintf( errorMsg, arg1, arg2, arg3 ); \
	    cprintf( "\n" ); \
	  } \
	}

#define Error4(errorLevel, errorMsg, arg1, arg2, arg3, arg4) \
	{ if (errorLevel>=errorThreshold) {\
	    cprintf( "(%10s:%4d)\t", __FILE__, __LINE__ ); \
	    cprintf( errorMsg, arg1, arg2, arg3, arg4 ); \
	    cprintf( "\n" ); \
	  } \
	}

#define Error5(errorLevel, errorMsg, arg1, arg2, arg3, arg4, arg5) \
	{ if (errorLevel>=errorThreshold) { \
	    cprintf( "(%10s:%4d)\t", __FILE__, __LINE__ ); \
	    cprintf( errorMsg, arg1, arg2, arg3, arg4, arg5 ); \
	    printf( "\n" ); \
	  } \
	}

#define Assert(expr)\
	{if (!(expr)) Error(ErrorExit, "Assertion Error");}
#else
#define Error(errorLevel, errorMsg)
#define Error1(errorLevel, errorMsg, arg1)
#define Error2(errorLevel, errorMsg, arg1, arg2)
#define Error3(errorLevel, errorMsg, arg1, arg2, arg3)
#define Error4(errorLevel, errorMsg, arg1, arg2, arg3, arg4)
#define Error5(errorLevel, errorMsg, arg1, arg2, arg3, arg4, arg5)
#define Assert(expr)
#endif

#endif // _ERROR_H_
