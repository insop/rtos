
//
//	Serial Class 
//
//	has event queue, bounded buffer 
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

#include "../serial/serial.h"

Serial::Serial() {
    readyTHRE0 = False;
    readyTHRE1 = False;

    readMode0 = False;
    readMode1 = False;

    pTaskReadRequest0 = (Task *) EMPTY;
    pTaskReadRequest1 = (Task *) EMPTY;

}

