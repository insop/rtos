#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"

#define	CommandSrv	4

main() {

    int i;
    char data[11];
    char getData = 's';
    char sendData;
    int tid;
    data[10] = '\0';

    console_xy(0,10);
    cprintf("Sensor Data : ");

    while(1) {

	tid = Receive(&getData, sizeof(char));
	Reply(tid,NULL,NULL);

	if(getData == 's') {
	    console_xy(19,10);
	    cprintf("*");
	    /*sendData = 'q';
	    Send(CommandSrv, &sendData, sizeof(char),NULL,NULL);*/
	    sendData = ',';
	    Send(CommandSrv, &sendData, sizeof(char),NULL,NULL);
	}
    }
}
	    

