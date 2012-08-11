#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"

#include "../train/ironBird.h"

typedef struct {
	char	name[ NAME_SIZE + 1 ];
	int	tid;
}NameEntry;

#define		NAME_TABLE_SIZE		50

NameEntry NameEntryList[ NAME_TABLE_SIZE ];	
int	  RegisteredNameNum = 0;


int registerTask( char *name, int tid ) {

    int	pos;		
    int	result;

    if( RegisteredNameNum == NAME_TABLE_SIZE ) {
	printf( "NameServer: Unable to register %s"
	"Name server is full " , name );
	return -1;
    } /* if */

    for( pos=0; pos < RegisteredNameNum; pos++ ) {
	result = strcmp( name, NameEntryList[pos].name );
	if ( result == 0 ) {
	    cprintf("NameServer: %s already registered"
		    "...registration failed", name );
	    printf("NameServer: %s already registered"
		    "...registration failed", name );
	    return -1;
	}
	else if ( result > 0 ) {
	    break;
	}
    }


    if( pos < RegisteredNameNum ) {
	memmove(&NameEntryList[pos+1], &NameEntryList[pos],
		(RegisteredNameNum-pos)*sizeof(NameEntry) );

    } 

    NameEntryList[pos].tid = tid;
    strcpy( NameEntryList[pos].name, name );
    RegisteredNameNum++;

/*
    cprintf("NameServer: Successfully registered"
	    " %s with ID %d", name, tid);
*/

    return 1;

}

main() {

    int	tid;
    int	reply;	
    int	index;
    NameServiceMsg	request;

    /* initialize the table */
    for( index=0; index<NAME_TABLE_SIZE; index++ ) {
	NameEntryList[ index ].tid = -1;
	strcpy(NameEntryList[ index ].name, "");
    }

    while( 1 ) {

	tid = Receive( (void *) &request, sizeof( request ) );
	/* cprintf("NS GOT req from tid%d\n", tid);
	Get(WYSE); */


	if ( request.type == Register ) {  
	    reply = registerTask( request.name, tid );
	    /*cprintf("reg  tid%d\n", tid);*/

	}
	else if ( request.type == LookUp ) {
	    reply = tidOf( request.name );
	    /*cprintf("BINGGO  tid%d\n", reply);*/
	}
	else {
	    /* unknown request...this should never happen */
	    cprintf("NameServer: %d invalid request", request.type );
	    printf("NameServer: %d invalid request", request.type );
	    reply = -1;
	}

	Reply( tid, (void *) &reply, sizeof(reply) );

    } /* for */


} /* main */




int tidOf( char *name ) {
    int pos;

    pos = lookup(name);
    if ( pos == -1 ) {
	cprintf( "NameServer: %s is not registered yet", name );
	printf( "NameServer: %s is not registered yet", name );
	return -1;
    }
    else 
	return NameEntryList[pos].tid;
} 


int lookup( char *name ) {
    int first, last;
    int mid;
    int found = 0;
    int result;
    
    last = RegisteredNameNum;
    first = 0;
    
    while ( (first <= last) && (!found) ) {
	mid = ( first + last ) / 2;
	result = strcmp( NameEntryList[mid].name, name );
	if ( result == 0 )
	    found = 1;
	else
	    if ( result > 0 )
		first = mid + 1;
	    else
		last = mid - 1;
    }
    
    if ( found )
	return mid;
    else
	return -1;
	
}
