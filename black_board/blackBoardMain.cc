//
//--
//
//	blackboard main 
//
//      using FEMOS(Fast EMbedded OS) kernel 
//
//--
//
//      Written by 
//
//	INSOP SONG
//
//      Systems Design
//      
//      Michael Cole Computer Science
//  
//      University of Waterloo, Canada
//  
//      June,July 2001
//
//--
//

#include "blackBoardMain.h"


//
//--
//	GLOBAL OBJECT
//--
//

BlackBoard	aBB;
CommunicateData aCommData;


//
//--
//	GLOBAL Variable
//--
//
int	Tid_BB_Distributor;
int	Tid_BB_Collector;

CommData	request;
CommData	data;
int		ack = ACK;

// blackBoard
int	blackBoard_array[5][16] = {0xff};

const BYTE EndSensorSignal      = 0xf1;



int checkThese(int myTid, BYTE setted, BYTE sen1, BYTE sen2) {

    //BYTE setted, sen1, sen2;


// checked out the sensor
    blackBoard_array[ (setted & 0xf0) >> 4 ][ setted & 0x0f] = 0xff;

// checked sensor;
    if(sen1 != EndSensorSignal && sen2 != EndSensorSignal) {

	if( blackBoard_array[ (sen1 & 0xf0) >> 4][ sen1 & 0x0f] != myTid &&
	  ( blackBoard_array[ (sen1 & 0xf0) >> 4][ sen1 & 0x0f] != 0xff )
	    ) {
	    return 0;
	}
	else {
	    blackBoard_array[ (sen1 & 0xf0) >> 4][ sen1 & 0x0f] = myTid;
	}


	if( blackBoard_array[ (sen2 & 0xf0) >> 4][ sen2 & 0x0f] != myTid &&
	  ( blackBoard_array[ (sen2 & 0xf0) >> 4][ sen2 & 0x0f] != 0xff )) {
	    return 0;
	}
	else { 
	    blackBoard_array[ (sen2 & 0xf0) >> 4][ sen2 & 0x0f] = myTid;
	}

    }


    return 1;
}




main() {

    int 	tid;
    TaskType	trainNo;

    char	strWrite[50];

    char	dataFrom[3];


    int		result;


    RegisterAs( "BlackBoardMain" );


    // XXX I don't create distributor
    //Tid_BB_Distributor = Create("BB_Distributor", BB_DistributorPriority);
    //Create("BB_Distributor", BB_CollectorPriority);
    Delay(10);

    // XXX make this unable
    //tid = Receive(NULL,NULL);
    //Reply(tid, NULL,NULL);

    int temp = 0;

    while(1) {

	tid = Receive(&dataFrom, 3*sizeof(char));


		    // checked sen, sen1, sen2
	result = checkThese(tid, dataFrom[0], dataFrom[1], dataFrom[2]);

	Reply(tid, &result, sizeof(result));




    } // while
}


#undef DISTRIBUTOR_TEST
#ifdef DISTRIBUTOR_TEST

	// do some registring jobs for sending(distributing) data
	
	aCommData.initData(Produce, blackBoard );
	aCommData.packData((CommDataType)(Approval_CheckIn+temp), train52);
	//XXX
	temp++;

	request = aCommData.getData();
	//printf("\n----------------------------------\n");
	//aCommData.printData();
	sprintf(strWrite,"[1BB Send abovedata to distributor....]\n\r");
	Write(strWrite, strLen(strWrite), WYSE);
	//

	Send(Tid_BB_Distributor, &request, sizeof(request),
	   	&ack, sizeof(ack));
	sprintf(strWrite, "[2BB distributted finished ....]\n\r");
	Write(strWrite, strLen(strWrite), WYSE);
	Get(WYSE);
	Delay(10);
#endif // DISTRIBUTOR_TEST
