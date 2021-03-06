//
//--
//
//	distributing buffer server.h
//
//	disribute data(from one or multi server)
//	to the multi clients
//
//	buffer server for commnucation
// 	
//	has 2 buffers
//
//	Int type buffer
//	CommData type buffer
//
//      using FEMOS(Fast EMbedded OS) kernel 
//
//--
//
//      Written by INSOP SONG
//
//      Systems Design
//      
//      Michael Cole
//      Computer Science
//  
//      University of Waterloo, Canada
//  
//      June,July 2001
//
//--
//
#ifndef	_DISTRIBUTOR_SERVER_H_
#define	_DISTRIBUTOR_SERVER_H_

#include "bdBufferInt.h"
#include "bdBufferCommData.h"
#include "communicateData.h"



extern "C" {
    #include "../syscall/sysCall.h"
    #include "../train/ironBird.h"
    #include "../include/i486/cs452.h"
}; // C function interface

#endif // _DISTRIBUTOR_SERVER_H_ 
