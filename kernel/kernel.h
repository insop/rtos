//
//
//--
//
//
//	FEMOS(Fast EMbedded OS) kernel main  
//
//
//--
//
//	Kernel class
//
// 	Written by 
//
//	INSOP SONG
//
// 	Systems Design
//	
//	Michael Cole Computer Science
//
// 	University of Waterloo, Canada
//
//	June,July 2001
//
#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "../task/taskDescTable.h"

#include "../include/util/type.h"
#include "../include/util/errorCode.h"
#include "../include/util/error.h"

extern "C" {
    #include "../include/i486/cs452.h"
};

class Kernel  {

    private:
	// Highest memory position
	int stackTop;	
	//  change into memory top
	int kernelDataStart;
	// kernel oprating time
	unsigned int	upTime;

	// task descriptor table

    public:
	// task descriptor table
	//TaskDescTable   aTDTable;

	int taskCS;
	int taskDS;
	int taskSS;
	int taskSP;

	//int kernelSP;
	int kernelDS;
	int kernelSS;
	int kernelCS;

	int currentTid;
	int parentTid;

	Task * pCurrentTask;
	Task * pKernelTask;

	Kernel();
	void initKernel( int _knlDataStart, int _stkTop );
	inline int getStackTop( void );
	inline int setStackTop( int );
	inline int increaseStackTop( int delta);

	inline int getKernelDataStart( void );
	inline int setKernelDataStart( int );

	inline void setCurrentTask( Task * pTask);
	inline Task * getCurrentTask( void );

	inline int getCurrentTid( void );
	inline void  setCurrentTid( int );

	inline int getParentTid( void );
	inline void  setParentTid( int );

	inline unsigned int getUpTime( void );
	inline void resetUpTime( void );
	inline void incUpTime( void );

};

//
//
//
inline int Kernel::getStackTop( void ) {
    return stackTop;
}

inline int Kernel::setStackTop( int _stkTop ) {
    // XXX add check memory boundary routine
    stackTop = _stkTop;
    return OK;
}

inline int Kernel::increaseStackTop( int _deltaStack ) {
    // XXX add check memory boundary routine
    stackTop += _deltaStack;
    return OK;
}

//
//
inline int Kernel::setKernelDataStart( int _dataStart ) {
    // XXX add check memory boundary routine
    kernelDataStart = _dataStart;
    return OK;
}

inline int Kernel::getKernelDataStart( void ) {
    return kernelDataStart;
}

//
//
inline void Kernel::setCurrentTask( Task * pTask ) {
    pCurrentTask = pTask;
}

inline Task * Kernel::getCurrentTask( void ) {
    return pCurrentTask;
}

//
//
inline int  Kernel::getCurrentTid( void ) {
    return currentTid;
}

inline void Kernel::setCurrentTid( int  tid ) {
    currentTid = tid;
}

//
//
inline void Kernel::setParentTid( int  tid ) {
    parentTid = tid;
}

inline int  Kernel::getParentTid( void ) {
    return parentTid;
}

inline unsigned int  Kernel::getUpTime( void ) {
    return upTime;
}

inline void Kernel::incUpTime( void ) {
    upTime++;
}

inline void Kernel::resetUpTime( void ) {
    upTime=0;
}
#endif //_KERNEL_H_
