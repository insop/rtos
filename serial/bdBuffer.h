//
//	bounded buffer 
//	type : char(=BYTE)
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

#ifndef _BOUNDED_BUFFER_H_
#define _BOUNDED_BUFFER_H_

#include "../include/i486/cs452.h"
//#include  <stdio.h>
#include "../include/util/errorCode.h"
#include "../include/util/type.h"

const int	MaxItemNum 			= 2000;

class	BdBuffer {

    private:
	BYTE	buffer[MaxItemNum];
	int	numItem;
	int 	indexHead;
	int 	indexTail;


    public:
	BdBuffer();

	inline Boolean  isEmpty( void );
	inline Boolean  isFull( void );
	inline Boolean  isEnoughSpace( int space );

	inline int 	getNumItem( void );
	inline void	setNumItem( int num );
	inline int	getNumLeft( void );

	inline void	addItem( BYTE item );
	inline BYTE	getItem( void );



}; // BdBuffer class

inline Boolean  BdBuffer::isEmpty( void ) {

    return (numItem <= 0 ? True: False);
}

inline Boolean  BdBuffer::isFull( void ) {

    return ((numItem >= MaxItemNum) ? True: False);

}

inline Boolean  BdBuffer::isEnoughSpace( int space ) {

	// if left space greater than input space >> True
    return ( (( MaxItemNum - numItem ) >= space) ? True : False );
}

inline int 	BdBuffer::getNumItem( void ) {

    return numItem;

}
inline void	BdBuffer::setNumItem( int num ) {

    numItem = num;

}

inline int	BdBuffer::getNumLeft( void ) {

    return ( MaxItemNum - numItem );

}

//
// XXX !!! before use this function
// XXX !!! should check isFull()
//
inline void	BdBuffer::addItem( BYTE item ) {


    numItem++;
    //printf("num: %d ADD TAIL[%d]  %d\n", numItem, indexTail,item);
    buffer[indexTail] = item;
    // end of the buffer
    if( indexTail != MaxItemNum-1) {
    // if not MaxItemNum just increment;
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
inline BYTE	BdBuffer::getItem( void ) {

    BYTE retVal;
    numItem--;
    retVal = buffer[indexHead];
    //printf("num %d : GET Head[%d]  %d\n", numItem, indexHead,retVal);
    if( indexHead != MaxItemNum-1) {
    // if not MaxItemNum just increment;
	indexHead++;
    } // if
    else {
	// if the end just reset the Head index
	indexHead =0;
    } // else

    return retVal;
}
#endif //_BOUNDED_BUFFER_H_
