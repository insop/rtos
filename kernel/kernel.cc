//
//--
//
//	FEMOS(Fast EMbedded OS) kernel main  
//
//--
//
//	Kernel class
//
//
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
#include  "kernel.h"

Kernel::Kernel() {
    kernelCS = 0x30;
    kernelDS = kernelSS = 0x38;
    parentTid = 0;
    currentTid = 0;
    upTime = 0;

}

void Kernel::initKernel( int _knlDataStart, int _stkTop ) {
    setKernelDataStart( _knlDataStart );
    setStackTop( _stkTop );
}
