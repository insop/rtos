//
//	bounded buffer 
//	type : commnunication data 
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

#include "../buffer_server/bdBufferCommData.h"



BdBufferCommData::BdBufferCommData() {
	numItem = 0;
	indexHead = 0;
	indexTail = 0;
}


