//
//--
//	buffer server.h
//
//	buffer server for commnucation
// 	
//	has 2 buffers
//
//	Int type buffer
//	CommData type buffer
//
//      using FEMOS(Fast EMbedded OS) kernel 
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

#include "bufferServer.h"


//
//--
//	GLOBAL OBJECT
//--
//

BdBufferInt		aConsumer;
BdBufferCommData	aProducer;
//BdBufferCommData	aBufferCommData;



main() {

    char* myName[30];

    int	tid;
    int consumerTid;

    CommData	request;
    CommData	data;

    //tid = Receive(&myName, sizeof(myName));
    //RegisterAs(MYNAME)'
    //Reply()

    while(1) {

	tid = Receive( (void *) &request, sizeof(request) );

	if( request.fromType == Consume) {
	    if( aProducer.isEmpty() ) {
		// XXX in this case how about reply???
		if(!aConsumer.isFull()) {
		    aConsumer.addItem( tid );
		}
		// if consumer buffer is full
		else {
		    printf("Consumer buffer is full\n");
		}

		//Reply( tid, ACK, sizeof(ACK));
	    }
	    else {
		data = aProducer.getItem();
		Reply( tid, (void *) &data, sizeof(data));
	    }
	}
	else if( request.fromType == Produce ) {
	    if( !aConsumer.isEmpty()) {
		consumerTid = aConsumer.getItem();
		Reply( consumerTid, (void *) &request, sizeof(request));
	    } // if
	    else {
		if(!aProducer.isFull()) {
		    aProducer.addItem(request);
		} //if
		else {
		    printf("Buffer producer buffer is full\n");
		} // else
	    } // else

	    //Reply( tid, ACK, sizeof(ACK));
	} // else if
    } // while
}




    

