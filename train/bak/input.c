#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"
#include "../train/ironBird.h"

/*
#define	CommandSrv	4
#define	SensorTid	6
*/


main() {

    char ch;

    Exit();
    while(1) {
	ch = Get(WYSE);
	Put(ch, WYSE );
	console_xy(20,9);
	cprintf(": %c",ch);

	/*Send(CommandSrv, &ch, sizeof(char), NULL,NULL);*/
	Send(SensorTid, &ch, sizeof(char), NULL,NULL);
    }


    Exit();

}
