#include	"bindfile.h"


Bindfile::Bindfile( )
{
    bndEntry = 0;
    numFiles = 0;

    //cprintf("Bind entry : 0x%x, num of files 0x%x\n", bndEntry, numFiles);
}
Bindfile::Bindfile( uint _bindEntry, int _numFiles )
{
    bndEntry = _bindEntry;
    numFiles = _numFiles;

    //cprintf("Bind entry : 0x%x, num of files 0x%x\n", bndEntry, numFiles);
}

Bindfile::initBindfile( uint _bindEntry, int _numFiles )
{
    bndEntry = _bindEntry;
    numFiles = _numFiles;

    //cprintf("Bind entry : 0x%x, num of files 0x%x\n", bndEntry, numFiles);
}


ErrorCode Bindfile::getBindEntry( int num, bindEntry *entry ) {

    if( num < 0 || num == numFiles ) {
	    //Error1(ErrorWarning,"Invalid bind entry number %d",num);
	    cprintf("Invalid bind entry number %d\n",num);
	    return ErrorInvalidBindFileNum;
    } // if
    if( entry == NULL ) {
	    //Error(ErrorWarning,"Null BindEntry");
	    cprintf("Null BindEntry\n");
	    return ErrorNullBindEntry;
    } // if
    absRead( (uint)bndEntry + (num * sizeof(bindEntry)),
	    (void *) entry, sizeof(bindEntry) );
    return OK;
}

void Bindfile::printBindfile ( void ) {

    bindEntry	bEntry;
    int		i, error;
    
    if( bndEntry==0 || numFiles==0 ) {
	Error(ErrorWarning,
	" bind entry variables are not initialized yet");
	//cprintf(" bind entry variables are not initialized yet\n");
    } // if
    cprintf("Printing Bind Entries>>\n");
    cprintf( "No   Offset    Stack     Filename        Module \n");
    cprintf( "------------------------------------------------\n");

    for( i = 0; i < numFiles; i++ ) {
	error = getBindEntry( i, &bEntry );
	if( error != OK) {
		Error(ErrorWarning,
		    "Error! Unable to retrieve bind table entry! Aborting" );
		return;
	} // if

	cprintf( " %d : 0x%06x : 0x%04x : %-16s : %-32s\n",
		i, bEntry.offset, bEntry.stackSize,
		bEntry.filename, bEntry.module );
    } // for 
    cprintf("[Press a key]");
    getchar();
}






