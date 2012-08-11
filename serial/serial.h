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

#ifndef	_SERIAL_H_
#define _SERIAL_H_

#include "../include/util/errorCode.h"
#include "../include/util/type.h"
#include "../serial/bdBuffer.h"
#include "../task/task.h"
#include "../task/queue.h" // simple queue for event queue

extern "C" {
    #include "../include/i486/cs452.h"
}; // C function interface

class	Serial {

    private:
	Boolean		readyTHRE0;
	Boolean		readyTHRE1;

	Boolean		readMode0;
	Boolean		readMode1;

	Task *		pTaskReadRequest0;
	Task *		pTaskReadRequest1;

	uint		srcDataRead0;
	uint		srcDataRead1;

    public:
	Serial();

// XXX  later one I may need to add eventQueue
	// Queue;
	
	BdBuffer	inWyse;
	BdBuffer	outWyse;
	BdBuffer	inTrain;
	BdBuffer	outTrain;

	SimpleQueue	inWyseQueue;
	SimpleQueue	outWyseQueue;
	SimpleQueue	inTrainQueue;
	SimpleQueue	outTrainQueue;

	inline Boolean isReady0() {
	    return readyTHRE0;
	};
	inline Boolean isReady1() {
	    return readyTHRE1;
	};
	inline void setReady0() {
	    readyTHRE0 = True;
	};
	inline void setReady1() {
	    readyTHRE1 = True;
	};
	inline void resetReady0() {
	    readyTHRE0 = False;
	};
	inline void resetReady1() {
	    readyTHRE1 = False;
	};

//
// Read mode control stuffs
//	
//	isReadMode();
//	setReadMode();
//	resetReadMode();
//
	inline Boolean isReadMode0() {
	    return readMode0;
	};
	inline Boolean isReadMode1() {
	    return readMode1;
	};
//--

	inline void setReadMode0() {
	    readMode0 = True;
	};

	inline void setReadMode1() {
	    readMode1 = True;
	};
//--

	inline void resetReadMode0() {
	    readMode0 = False;
	};
	inline void resetReadMode1() {
	    readMode1 = False;
	};
//--

	inline void setReadReqTask0( Task * _pTask) {
	    pTaskReadRequest0 = _pTask;
	};

	inline void setReadReqTask1( Task * _pTask) {
	    pTaskReadRequest1 = _pTask;
	};
//--

	inline Task * getReadReqTask0() {
	    return pTaskReadRequest0;
	};

	inline Task * getReadReqTask1() {
	    return pTaskReadRequest1;
	};
//--

	inline void setSrcDataRead0( uint _srcData) {
	    srcDataRead0 = _srcData;
	};

	inline void setSrcDataRead1( uint _srcData) {
	    srcDataRead1 = _srcData;
	};
//--
	inline uint getSrcDataRead0() {
	    return srcDataRead0;
	};

	inline uint getSrcDataRead1() {
	    return srcDataRead1;
	};
//--
}; // Serial


#endif // _SERIAL_H_
