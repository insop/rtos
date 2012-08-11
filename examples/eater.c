#undef DEBUG
#include "../syscall/sysCall.h"
#define CLOCKTICK 2 
#define NUMPERROW 20
#define NUMROWS 5
#define DelayWorks
#define NULL 0


main() {
  int EnderPID;             /* PID of the Ender process */
  int myNum;                /* This eater number, in {2,3,..,10} */
  int mainPID;              /* PID of the main task */
  int i;                    /* loop index */
  int row;                  /* row to erase on */
  int col;                  /* column to erase on */
  int EraseThis;            /* Number to erase */
  int delayDuration;        /* How long to delay for */

  EnderPID = 5; 

  mainPID = Receive(&myNum, sizeof(int));
  Reply(mainPID, NULL, NULL);

  console_xy(0, (10 + myNum));


  for(i = 1; i <= ((NUMROWS * NUMPERROW) / myNum); i++) {
    EraseThis = (i * myNum);
    if ((EraseThis % NUMPERROW) != 0) {
      row = (EraseThis / NUMPERROW);
      col = EraseThis - (row * NUMPERROW);
    } else {
      row = ((EraseThis / NUMPERROW) - 1);
      col = NUMPERROW;
    }
    console_xy((col * 3), row);
    cprintf("   ");
#ifdef DelayWorks
    delayDuration = ((i * myNum) * CLOCKTICK);
    Delay(myNum * CLOCKTICK);
#endif
  }

  console_xy(0, (20 + myNum));
  cprintf("Eater %d finnishing \n", myNum);
  Send(EnderPID, &myNum, sizeof(int), NULL, NULL);

  while(1) {
      Delay(40);
      cprintf("ea");
  }
  Exit();
}
