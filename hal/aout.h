#ifndef	_A_OUT_H_
#define	_A_OUT_H_

#include	"bindfile.h"
#define		Debug
#include	"../include/util/error.h"
#include	"../include/util/errorCode.h"

extern "C" {
    #include	"../include/i486/bind486.h"
    #include 	"../include/i486/sys/exec_aout.h"
};



// 
//	class for a.out data of each process
//
class	Aout	{
    private:
	struct exec	aOutHeader;

    public:
	Aout();
	static	int	kernelBaseAddress;
	int getAoutHeader(
		    int num, struct exec *header , Bindfile *pBindfile);
	void printAoutHeader( Bindfile * pBindfile );


};

#endif	// _A_OUT_H_
