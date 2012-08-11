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

#ifndef	_COMMUNICATE_DATA_H_
#define _COMMUNICATE_DATA_H_

extern "C" {
    #include "../include/i486/cs452.h"
    #include "../syscall/sysCall.h"
    //#include "../include/util/type.h"
    // train application definitions
    #include "../train/ironBird.h"
}; // C function interface


//
//
//---
//
// CommunicateData Class
// commnuncation data 
//
//---
//
//

class CommunicateData {

    private:
	CommData	data;

    public:
	
	CommunicateData();
    // initialized data with data type and where this from 
	void initData(TaskType myType);

	inline CommData getData();
	inline void setData( CommData _data);

    //  pack Data with data type with no receiver
	inline void packData(CommDataType _type);

    //  pack Data with data type with specific receiver 
	inline void packData(CommDataType _type, TaskType _receiver);

    // hide ipc functions
	inline void postData( CommData _dataToPost);

    // print out data
	printData();

}; // class CommData


inline CommData CommunicateData::getData() {
    return data;
}

inline void CommunicateData::setData(CommData _data) {
    data = _data;
}

inline void CommunicateData::packData(CommDataType _type) {
    data.type = _type;
}

//  pack Data with data type with specific receiver 
inline void CommunicateData::packData(CommDataType _type, TaskType _receiver){
    data.type = _type;
    data.receiver = _receiver;
}
#endif // _COMMUNICATE_DATA_H_
