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

#include "../buffer_server/bdBufferInt.h"



BdBufferInt::BdBufferInt() {
	numItem = 0;
	indexHead = 0;
	indexTail = 0;
}


