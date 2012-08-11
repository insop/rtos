//
//--
//
//	distributing buffer server.cc
// 
//	XXX this is not using now.....
//
//	disribute data(from one or multi server)
//	to the multi clients
//
//	buffer server for commnucation
// 	
//	has 2 types buffers
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

#include "distributorServer.h"


//
//--
//	GLOBAL OBJECT
//--
//

BdBufferInt		aDistTid[trainLast];
BdBufferCommData	aDistData[trainLast];

CommunicateData		aCommData;
//BdBufferCommData	aBufferCommData;

//
//--
//	GLOBAL Variable
//--
//
int		tid;
CommData	request;
CommData	data;
int		ack = ACK;

//int	Tid_BlackBoardMain;



void train52_handler() {

    char 	strWrite[50];

    sprintf(strWrite, "[train 52 want consume]\n\r");
    Write(strWrite, strLen(strWrite), WYSE);
    
    if( aDistData[train52Index].isEmpty() ) {
	// XXX in this case how about reply???
	if(!aDistTid[train52Index].isFull()) {
	    aDistTid[train52Index].addItem( tid );
	}
	// if consumer buffer is full
	else {
	    sprintf(strWrite, "[Consumer buffer is full]\n\r");
	    Write(strWrite, strLen(strWrite), WYSE);
	}
	// if no data for consumer then just ACK reply 
	// XXX no ack when no data
	//Reply( tid, ack, sizeof(ack));
    }
    else {
	data = aDistData[train52Index].getItem();
	Reply( tid, (void *) &data, sizeof(data));
    }
}


train52_handler2() {

    char 	strWrite[50];
    int consumerTid;


    sprintf(strWrite, "[produce for train 52]\n\r");
    Write(strWrite, strLen(strWrite), WYSE);

    if( !aDistTid[train52Index].isEmpty()) {
	consumerTid = aDistTid[train52Index].getItem();
	// distribute data directly to the train
	Reply( consumerTid, (void *) &request, sizeof(request));
    } // if
    else {
	if(!aDistData[train52Index].isFull()) {
	    aDistData[train52Index].addItem(request);
	} //if
	else {
	    sprintf(strWrite, "[Buffer producer buffer is full]\n\r");
	    Write(strWrite, strLen(strWrite), WYSE);
	} // else
    } // else

    Reply( tid, &ack, sizeof(ack));
}



main() {

    char  myName[30];
    char  strWrite[50];


    TaskType consumerType;

    RegisterAs( "BB_Distributor" );
    Delay(10);


    //tid = Receive(&myName, sizeof(myName));
    //RegisterAs(MYNAME)'
    //Reply()

    while(1) {

	tid = Receive( (void *) &request, sizeof(request) );

	// test functions
	sprintf(strWrite, "[Distributor get data]\n\r");
	Write(strWrite, strLen(strWrite), WYSE);
	//aCommData.setData(request);
	//aCommData.printData();
	//printf("\n----------------------------------------\n");


	if( request.fromType == Consume) {
	    // check consumer type
	    // requesting data from which train
	    consumerType = request.from;
	    // switch
	    if( consumerType == train52) {
		train52_handler();
	    } // if
	    else if( consumerType == train55) {
		//train55_handler();
	    } // else if 
	    else if( consumerType == train57) {
		//train57_handler();
	    } // else if 
	    else {
		sprintf(strWrite, 
		    "[train ID error in distribuor %d]\n\r", consumerType);
		Write(strWrite, strLen(strWrite), WYSE);
	    } // else

	} // if
	else if( request.fromType == Produce ) {

	    // distribute data to which train 
	    consumerType = request.to;

	    if( consumerType == train52) {
		train52_handler2();
	    } // if
	    else if( consumerType == train55) {
		//train55_handler2();
	    } // else if
	    else if( consumerType == train57) {
		//train57_handler2();
	    } // else if
	    else {
		sprintf(strWrite,  "[[%d]:train ID error in distribuor
		%d]\n\r",
		    __LINE__,consumerType);
		Write(strWrite, strLen(strWrite), WYSE);
	    } //else
	} // else if


    } // while
}



#if 0

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
#endif
    

