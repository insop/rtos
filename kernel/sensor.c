#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"

#define	CommandSrv	4
#define TrainTid	7

main() {

    int i;
    char data[11];
    char sendData = 's';
    data[10] = '\0';

    console_xy(0,10);
    cprintf("Sensor Data : ");

    while(1) {

	Send(CommandSrv, &sendData, sizeof(char),NULL,NULL);

	for(i=0;i<10;i++) {
	    data[i] = Get(TRAIN);

	}

	if(data[3] == 1) {
	    console_xy(19,10);
	    cprintf("*");
	    Send(TrainTid, &sendData, sizeof(char),NULL,NULL);
	}

	for(i=0;i<10;i++) {
	    console_xy(20+i,10);
	    cprintf("%x", data[i]);
	    console_xy(19,10);
	    cprintf(" ");
	}
    }
}
	    

