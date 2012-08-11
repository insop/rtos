//
//	stack
//	type : int
//
//
// 	Written by INSOP SONG
//
// 	Systems Design
//
// 	University of Waterloo, Canada
//
//	June,July 2001
//

#ifndef _STACK_H_
#define _STACK_H_

//#include "../include/i486/cs452.h"
#include  <stdio.h>
#include "../include/util/errorCode.h"
#include "../include/util/type.h"

const int	MaxStackItemNum 			= 100;

class	Stack {

    private:
	int	buffer[MaxStackItemNum];
	int	numItem;
	int 	indexTop;

    public:
	Stack();

	inline Boolean  isEmpty( void );
	inline Boolean  isFull( void );
	inline Boolean  isEnoughSpace( int space );

	inline int 	getNumItem( void );
	inline void	setNumItem( int num );
	inline int	getNumLeft( void );

	inline void	addItem( int item );
	inline int	getItem( void );



}; // Stack class

inline Boolean  Stack::isEmpty( void ) {

    return (numItem <= 0 ? True: False);
}

inline Boolean  Stack::isFull( void ) {

    return ((numItem >= MaxStackItemNum) ? True: False);

}

inline Boolean  Stack::isEnoughSpace( int space ) {
	// if left space greater than input space >> True
    return \
    ( (( MaxStackItemNum - numItem ) >= space) ? True : False );
}

inline int 	Stack::getNumItem( void ) {
    return numItem;
}

inline void	Stack::setNumItem( int num ) {
    numItem = num;
}

inline int	Stack::getNumLeft( void ) {
    return ( MaxStackItemNum - numItem );
}

//
// XXX !!! before use this function
// XXX !!! should check isFull()
//
inline void	Stack::addItem( int item ) 
{
    numItem++;
    //printf("num: %d ADD TAIL[%d]  %d\n", numItem, indexTop,item);
    buffer[indexTop++] = item;
}

//
// XXX !!! before use this function
// XXX !!! should check isEmpty()
//
inline int	Stack::getItem( void ) {

    int retVal;
    numItem--;
    retVal = buffer[--indexTop];
    //printf(
	//"num %d : GET Top[%d]  %d\n", numItem, indexTop,retVal);

    return retVal;
}
#endif //_STACK_H_
