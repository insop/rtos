#include "../include/i486/cs452.h"
#include "../include/i486/bind486.h"

#ifndef	_HAL_L_
#define	_HAL_L_

//
//	Hardware abstraction layer class
//

class	HAL {
    private:

	Bindtable	aBindtable;
	GDT		aGDT;
	Aout		aAout;


    public:
	HAL();
	printHAL();


}// HAL

#endif // _HAL_L_
