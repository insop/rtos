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

#include "../serial/bdBuffer.h"



BdBuffer::BdBuffer() {
	numItem = 0;
	indexHead = 0;
	indexTail = 0;
}


