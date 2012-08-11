//
//
//	Max priority queue
//
//
// 	Written by 
//
//	INSOP SONG
//
// 	Systems Design
//
//
// 	University of Waterloo, Canada
//
//	August 2001
//

#ifndef _MAX_PRIORITY_QUEUE_H_
#define _MAX_PRIORITY_QUEUE_H_

#include "../include/util/type.h"

#include <stdio.h>
//#include "../include/i486/cs452.h"


class	MaxPriQueue {

    private:

    public:
	MaxPriQueue();
	Boolean	isEmpty();
	int	size();
	int	getMax();
	void	put();
	int	removeMzx();

};





#endif //_MAX_PRIORITY_QUEUE_H_
