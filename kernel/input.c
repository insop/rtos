#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"

#define	CommandSrv	4


main() {

    char ch;

    while(1) {
	ch = Get(WYSE);
	console_xy(20,9);
	cprintf(": %c",ch);

	Send(CommandSrv, &ch, sizeof(char), NULL,NULL);
    }


    Exit();

}
