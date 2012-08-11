#ifndef _TYPE_H_
#define _TYPE_H_


typedef unsigned char		UBYTE;		/* 8 bit unsigned */
typedef char			BYTE;		/* 8 bit signed */
typedef unsigned int		UWORD;		/* 16 bit unsigned */
typedef int			WORD;		/* 16 bit signed */
typedef unsigned long		ULONG;		/* 32 bit unsigned */
typedef long			LONG;		/* 32 bit signed */

typedef	char			ErrorCode;	/* for error code */	

enum Boolean {False=0,True=1};
typedef enum Boolean Boolean;


/* state of train, switch tasks */
enum 	State	{Idle, Operating, Waiting, Finishing};
typedef	enum State	State;


/* Control the sequence of the contol
 	train, switch, sensor */
enum ControlFlow {Train,Switch,Sensor};
typedef enum ControlFlow ControlFlow;


#define	FALSE			0
#define	TRUE			1

#define ENABLE			1
#define	FOR_EVER		TRUE

#define EMPTY			(-1)	/* null pointer */

/* for serial initial */
#define ANOTHER_WYSE		2
#define	DUMMY			1	

static BYTE Esc = 27;
#endif	/* _TYPE_H_ */
