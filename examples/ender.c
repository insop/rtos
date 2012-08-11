#undef  	DEBUG
#include "../syscall/sysCall.h"
#define NUMEATERS 9
#define NULL 9

main() {
  int EndSequence[NUMEATERS];
  int i;                      /* loop index */
  int eater;                  /* Eater who finnished */
  int replyToPID;             /* PID of eater who finnished */

  cprintf("I am ender\n");
  /*RegisterAs("ender");*/

  for(i = 0; i < NUMEATERS; i++) {
    replyToPID = Receive(&eater, sizeof(int));
    cprintf("GotEater[%d]",eater);
    EndSequence[i] = eater;
    Reply(replyToPID, NULL, NULL);
  }

  console_xy(0, 6);
  cprintf("Finnished in the following sequence: ");
  for (i = 0; i < NUMEATERS; i++) {
    cprintf("%d ",EndSequence[i]);
  }

  while(1) {
      Delay(100);
      cprintf("en");
  }
  Exit();
}
