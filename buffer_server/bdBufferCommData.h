//
//	bounded buffer 
//	type : communication data type 
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

#ifndef _BOUNDED_BUFFER_COMM_DATA_H_
#define _BOUNDED_BUFFER_COMM_DATA_H_

#include "../include/i486/cs452.h"
//#include  <stdio.h>
#include "../include/util/errorCode.h"
#include "../include/util/type.h"
#include "../train/ironBird.h"

typedef	CommData	BufferItem;
const int	MaxItemNumCommData 			= 20;

class	BdBufferCommData {

    private:

// buffer type is int
// I need use another way XXX like freiend... or other
// but now I just make multiple buffer.
// This is not a good way of programming...!!!
	BufferItem	buffer[MaxItemNumCommData];

	int	numItem;
	int 	indexHead;
	int 	indexTail;


    public:
	BdBufferCommData();

	inline Boolean  isEmpty( void );
	inline Boolean  isFull( void );
	inline Boolean  isEnoughSpace( int space );

	inline int 	getNumItem( void );
	inline void	setNumItem( int num );
	inline int	getNumLeft( void );

	inline void		addItem( BufferItem item );
	inline BufferItem	getItem( void );



}; // BdBufferCommData class

inline Boolean  BdBufferCommData::isEmpty( void ) {

    return (numItem <= 0 ? True: False);
}

inline Boolean  BdBufferCommData::isFull( void ) {

    return ((numItem >= MaxItemNumCommData) ? True: False);

}

inline Boolean  BdBufferCommData::isEnoughSpace( int space ) {

	// if left space greater than input space >> True
    return ( (( MaxItemNumCommData - numItem ) >= space) ? True : False );
}

inline int 	BdBufferCommData::getNumItem( void ) {

    return numItem;

}
inline void	BdBufferCommData::setNumItem( int num ) {

    numItem = num;

}

inline int	BdBufferCommData::getNumLeft( void ) {

    return ( MaxItemNumCommData - numItem );

}

//
// XXX !!! before use this function
// XXX !!! should check isFull()
//
inline void	BdBufferCommData::addItem( BufferItem item ) {


    numItem++;
    //printf("num: %d ADD TAIL[%d]  %d\n", numItem, indexTail,item);
    buffer[indexTail] = item;
    // end of the buffer
    if( indexTail != MaxItemNumCommData-1) {
    // if not MaxItemNumCommData just increment;
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
inline BufferItem	BdBufferCommData::getItem( void ) {

    BufferItem retVal;
    numItem--;
    retVal = buffer[indexHead];
    //printf("num %d : GET Head[%d]  %d\n", numItem, indexHead,retVal);
    if( indexHead != MaxItemNumCommData-1) {
    // if not MaxItemNumCommData just increment;
	indexHead++;
    } // if
    else {
	// if the end just reset the Head index
	indexHead =0;
    } // else

    return retVal;
}
#endif //_BOUNDED_BUFFER_COMM_DATA_H_
