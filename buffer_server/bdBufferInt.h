//
//	bounded buffer 
//	type : int
//
//
// 	Written by INSOP SONG
//
// 	Systems Design
//
//	Michael Cole
//	Computer science
//
// 	University of Waterloo, Canada
//
//	June,July 2001
//

#ifndef _BOUNDED_BUFFER_INT_H_
#define _BOUNDED_BUFFER_INT_H_

#include "../include/i486/cs452.h"
//#include  <stdio.h>
#include "../include/util/errorCode.h"
#include "../include/util/type.h"

typedef	int	BufferItemInt;
const int	MaxItemNumInt 			= 20;

class	BdBufferInt {

    private:

// buffer type is int
// I need use another way XXX like freiend... or other
// but now I just make multiple buffer.
// This is not a good way of programming...!!!
	BufferItemInt	buffer[MaxItemNumInt];

	int	numItem;
	int 	indexHead;
	int 	indexTail;


    public:
	BdBufferInt();

	inline Boolean  isEmpty( void );
	inline Boolean  isFull( void );
	inline Boolean  isEnoughSpace( int space );

	inline int 	getNumItem( void );
	inline void	setNumItem( int num );
	inline int	getNumLeft( void );

	inline void		addItem( BufferItemInt item );
	inline BufferItemInt	getItem( void );



}; // BdBufferInt class

inline Boolean  BdBufferInt::isEmpty( void ) {

    return (numItem <= 0 ? True: False);
}

inline Boolean  BdBufferInt::isFull( void ) {

    return ((numItem >= MaxItemNumInt) ? True: False);

}

inline Boolean  BdBufferInt::isEnoughSpace( int space ) {

	// if left space greater than input space >> True
    return ( (( MaxItemNumInt - numItem ) >= space) ? True : False );
}

inline int 	BdBufferInt::getNumItem( void ) {

    return numItem;

}
inline void	BdBufferInt::setNumItem( int num ) {

    numItem = num;

}

inline int	BdBufferInt::getNumLeft( void ) {

    return ( MaxItemNumInt - numItem );

}

//
// XXX !!! before use this function
// XXX !!! should check isFull()
//
inline void	BdBufferInt::addItem( BufferItemInt item ) {


    numItem++;
    //printf("num: %d ADD TAIL[%d]  %d\n", numItem, indexTail,item);
    buffer[indexTail] = item;
    // end of the buffer
    if( indexTail != MaxItemNumInt-1) {
    // if not MaxItemNumInt just increment;
	indexTail++;
    } // if
    else {
	// if the end just reset the tail index
	indexTail =0;
    } // else
}

//
// XXX !!! before use this function
// XXX !!! should check isEmpty()
//
inline BufferItemInt	BdBufferInt::getItem( void ) {

    BufferItemInt retVal;
    numItem--;
    retVal = buffer[indexHead];
    //printf("num %d : GET Head[%d]  %d\n", numItem, indexHead,retVal);
    if( indexHead != MaxItemNumInt-1) {
    // if not MaxItemNumInt just increment;
	indexHead++;
    } // if
    else {
	// if the end just reset the Head index
	indexHead =0;
    } // else

    return retVal;
}
#endif //_BOUNDED_BUFFER_INT_H_
