#include "../../syscall/sysCall.h"
#include "../../include/i486/cs452.h"

#define	CommandSrv	4
#define	SensorSrv	8
#define	Sensor		6
#define TrainTid	7

main() {

/* need add name handling jobs here */

    while( 1 ) {
	/* make the sensor job 1 tick(50 msec) period */
	Send(Sensor,NULL,NULL,NULL,NULL);
	Delay( 1 );
    } /* while */


}
	    

