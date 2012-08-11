#include "stepgraph.h"

/*******************************************************************
 *
 * file : onestepmain.c
 *
 * Insop Song & Michael Cole
 *
 * On startup requires the central controler to send a single char,
 * a or b to determine which track we are on.
 *
 * Given a list of of nodes, where one node is adjacent to the previous
 * node in the list, and the first node has the correct parity of the train
 * onestepmain.c will return a set of instructions to get from the start
 * to the end of the given list.
 *
 *******************************************************************/

char parity(node src, node dest) {
  /* Determine the parity of the destination node. Note
   * that parity indicates the direction the train _will_
   * be going in when it arrives at the destination.
   * Return 0 means parity is the same, else return 1.
   */

  directive dummy[NUMSWITCH];   /* Not used, required by inst_sequence */

  if (inst_sequence(dest, src, dummy) == FORWARD) {
    return 1;
  } else {
    return 0;
  }
}

main() {
  char trackId;                 /* Should be 'a' or 'b' */
  int centralControlTID;        /* Hmmmmm... what could this integer be? */
  int clientTID;                /* Callers TID */
  node instruction[NUMSEGMENTS];/* Actual request made by user */
  node src;                     /* Source node - specified by user */
  node dest;                    /* Adjacent to src */
  oneHop DToSend[NUMSEGMENTS];  /* List of instructions to execute */
  int i;                        /* loop index */

  int tid_centralControl;

  RegisterAs( "OneStep" );
  Delay(10);

#ifdef Eos
  centralControlTID = Receive(&trackId, sizeof(char));
  Reply(centralControlTID, NULL, NULL);
#else
  trackId = 'a';
#endif

  init_stepgraph(trackId);

  while(1) {
#ifdef Eos
    clientTID = Receive(&instruction, (NUMSEGMENTS * sizeof(node)));
#endif

    src = instruction[0];
    DToSend[0].thisNode = src;  /* Assume first node has correct parity */

    for (i = 1; i < NUMSEGMENTS; i++) {
      if (instruction[i].sensorNum == NIL) {
        break;
      }
      src = DToSend[(i - 1)].thisNode;
      dest = instruction[i];
      if (parity(src, dest)) {
        if ((dest.sensorNum % 2) == 0) dest.sensorNum--;
        else dest.sensorNum++;
      }
      DToSend[i].thisNode = dest;
      DToSend[i].distance = NIL;  /* Mark the end of list */
      DToSend[(i - 1)].distance = distance_between(src, dest);
      DToSend[(i - 1)].direction =
        inst_sequence(src, dest, DToSend[(i - 1)].instlist);
    }
#ifdef Eos
    Reply(clientTID, &DToSend, (NUMSEGMENTS * sizeof(oneHop)));
#else
    break;
#endif
  }
}
