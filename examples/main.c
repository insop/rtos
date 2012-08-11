#undef		DEBUG
#include <cs452.h>
#include "../syscall/sysCall.h"
#define NUMEATERS 9
#define PRIENDER 10
#define PRIEATER 15
#define NUMPERROW 20
#define NUMROWS 5

char *testStr="1234567890abcdefghijklmnopqrstuvwxyz";

main() {

  int i;                    /* loop index */
  int j;                    /* innder loop index */
  int intSize = sizeof(int);/* optimization */
  int eaterPID;		    /* pid for eater */
  char ch;

  cprintf("I am main PID[%d]\n", MyTid());

  /*printf("registering Main \n");*/
  /*RegisterAs("main");*/
  /*printf("registered Main \n");*/

  console_clear();
  for (i = 0; i < NUMROWS; i++) {
    for (j = 1; j <= NUMPERROW; j++) {
      console_xy((j * 3), i);
      cprintf("%d", ((NUMPERROW * i) + j));
    }
  }

  Create("Ender", PRIENDER);

  for (i = 2; i < (NUMEATERS + 2); i++) {
    eaterPID = Create("Eater", PRIEATER);
    cprintf("Eater created PID[0x%x]\n", eaterPID);
    Send(eaterPID, &i, sizeof(int), NULL, NULL);
  }

i=0;
  while(1) {

	  cprintf(":");
	  Delay(5);
  }

/*
console_xy(100,0);
ch = Get(WYSE);
cprintf(">>%c<<",ch);
*/
  printf("MAIN IS EXITING\n");
  Exit();
}
