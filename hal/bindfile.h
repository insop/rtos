
#ifndef	_BINDFILE_H_
#define	_BINDFILE_H_
//#define	DEBUG
#include "../include/util/type.h"
#include "../include/util/error.h"
#include "../include/util/errorCode.h"

extern "C" {
    #include "../include/i486/cs452.h"
    #include "../include/i486/bind486.h"
};

//
//	bindfile class
//	for read modules, size, stack and offsets
//

class Bindfile {
    private:
	uint	bndEntry;
	int	numFiles;

    public:
	Bindfile();
	Bindfile(uint _bindEntry, int _numFiles);
	initBindfile(uint _bindEntry, int _numFiles);
	ErrorCode getBindEntry( int num, bindEntry *entry );
	inline int getNumBindfiles( void );
	inline int getStackSize( int num );
	void printBindfile( void );
	inline int findModule( char* moduleName);

};

inline int Bindfile::getNumBindfiles ( void ) {
    return numFiles;
}

inline int Bindfile::findModule( char * moduleName ) {

    int i, error;         /* loop index */
    bindEntry bEntry; /* Want an easy to modify bind table entry */

    for (i = 0; i < numFiles; i++) {
	error = getBindEntry( i, &bEntry);
	if ( error != OK ) {
	    //Error1(ErrorWarning,"getting number %d module error",i);
	    cprintf("Warning:: getting number %d module error\n",i);
	    getchar();
	    return ErrorModuleRead;
	} // if
	if ( strcmp(bEntry.module, moduleName) == 0) {
	  return i;
	} // if 
    } // for 
    return ErrorFindModule;
}

inline int Bindfile::getStackSize( int num) {
    bindEntry	bEntry;
    int		i, error;
    
    if( bndEntry==0 || numFiles==0 ) {
	Error(ErrorWarning,
	" bind entry variables are not initialized yet");
	//cprintf(" bind entry variables are not initialized yet\n");
    } // if

    error = getBindEntry( num, &bEntry );
    if( error != OK) {
	    Error(ErrorWarning,
		"Error! Unable to retrieve bind table entry! Aborting" );
	    getchar();
	    return Error;
    } // if

    return bEntry.stackSize;
}

#endif	// _BINDFILE_H_
