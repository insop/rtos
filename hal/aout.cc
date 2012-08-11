#include 	"aout.h"

//	kernel starts from 0x100000;
int	Aout::kernelBaseAddress = 0x100000;

Aout::Aout() {

}

int Aout::getAoutHeader(
	    int num, struct exec *header, Bindfile *pBindfile ) {

    bindEntry		bEntry;
    struct exec		currentHeader;
    int			error;

    if( header == NULL )
	    return -1;

    error = pBindfile->getBindEntry( num, &bEntry );
    if( error < 0 ) {
	    Error(ErrorWarning,
		"Error! Unable to retrieve bind table entry! Aborting" );
	    return ErrorNullAoutHeader;
    }

    // Read a.out header 
    absRead( kernelBaseAddress + bEntry.offset, (char*) &currentHeader,
	    sizeof(struct exec) ); 

    // check magic number of a.out header  
    if( currentHeader.a_midmag != 0x0107 )
	    return ErrorAoutHeaderMagic;

    memcpy( header, &currentHeader, sizeof(struct exec) );
    return OK;
}

void Aout::printAoutHeader( Bindfile *pBindfile ) {

    struct exec		currentHeader;
    int			i, error;

    cprintf("\na.out header contents>>\n");
    cprintf("File :  Magic : Text Size : Data Size : BSS Size : Entry Point\n");
    cprintf("--------------------------------------------------------------\n");

    for( i=0; i < pBindfile->getNumBindfiles() ; i++ ) {
	error = getAoutHeader( i, &currentHeader, pBindfile );
	if( error == ErrorNullAoutHeader ) {
	    Error(ErrorWarning, 
		"Error! Unable to read a.out header. Aborting.");
	    return ;
	} // if
	if( error == ErrorAoutHeaderMagic ) {
	    Error1(ErrorWarning,
	    "Magic number(file=0x%x) error in an a.out file..skipping...",i);
	    continue;
	} // if
	
	cprintf("0x%04x    0x%04x    0x%04x    0x%04x    0x%04x    0x%04x\n", 
		i, currentHeader.a_midmag,
		currentHeader.a_text, currentHeader.a_data,
		currentHeader.a_bss, currentHeader.a_entry );

    } // for
    cprintf("Press any key ... ");
    getchar();
}
