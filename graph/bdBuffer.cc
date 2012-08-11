//
//	bounded buffer 
//	type : int
//
//
// 	Written by INSOP SONG
//
// 	Systems Design
//
//
// 	University of Waterloo, Canada
//
//	June,July 2001
//

#include "../graph/bdBuffer.h"



BdBuffer::BdBuffer() 
{
    numItem = 0;
    indexHead = 0;
    indexTail = 0;
}

void BdBuffer::reset(void) 
{
    numItem = 0;
    indexHead = 0;
    indexTail = 0;
}


