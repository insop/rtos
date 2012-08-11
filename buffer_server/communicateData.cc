//
//--
//	Communication Data class
//	
//	wrapper class for CommData Struct
//
//	using for data communication with 
//	train task and higher level tasks
//	
//	using FEMOS(Fast EMbedded OS) kernel main  
//
//
// 	Written by INSOP SONG
//
// 	Systems Design
//	
//	Michael Cole
//	Computer Science
//
// 	University of Waterloo, Canada
//
//	June,July 2001
//--
//

#include	"communicateData.h"

char *strCommDataType[] = {
    "Initial", 
/* as KQML style */
    "Ask_if",
    "Ready",
    "Deny",
    "Uninsert",
/* ............. */
    "Approval_CheckIn", 
    "Approval_CheckOut",
    "Ack_Report",

    "Command_Slow_Down",
    "Command_Speed_Up",
    "Command_Stop",

    "Requeset_CheckIn", 
    "Requeset_CheckOut", 

    "Report_Normal", 
    "Report_Emergency", 
    "Report_InMyWay_Ahead1",
    "Report_InMyWay_Ahead2",
    "Report_InMyWay_Behind1",
    "Report_InMyWay_Behind2",

    "Report_Mision_Complished",
    "Report_Mision_Uncomplished",

    "Route_toAll",
    "Route_toLeader",
    "Route_toWindman",
    "Route_toSolo",

    "Route_toAhead",
    "Route_toBehind",

    "Finished"};
    


CommunicateData::CommunicateData() {
    // dummy constructor
}

void CommunicateData::initData(TaskType myType) {
    data.sender 	  = myType;
}

CommunicateData::printData() {

    //printf("BufferType 	 : %s   ", strBufferType(data.fromType));
    printf("CommDataType : %s   \n", strCommDataType[data.type]);
    //printf("BufferType 	 : %d   ", data.fromType);
    //printf("CommDataType : %d   \n", data.type);
    printf("Sender  	 : %d    ", data.sender);
    printf("Receiver   	 : %d    ", data.receiver);
    printf("Current      : [%c%d]  \n", SEN_MODULE_NUM(data.sensor),
					SEN_NUMBER(data.sensor));
    printf("NextSensor   : [%c%d]  \n", SEN_MODULE_NUM(data.nextSensor),
					SEN_NUMBER(data.nextSensor));
    printf("Next2ndSensor: [%c%d]  \n", SEN_MODULE_NUM(data.next2ndSensor),
					SEN_NUMBER(data.next2ndSensor));
    printf("position  	 : %d    ", data.position);
    printf("time    	 : %d    ", data.time);

}

