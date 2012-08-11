#include "../pathfind/graph.h"
#include "../syscall/sysCall.h"

/* definitions for train tasks */
#include "../train/ironBird.h"

/* Store nodes as follows:
 * Mod A = 0, Mod B = 8, Mod C = 16, Mod D = 24, Mod E = 32
 * Sensor: 1 = 0, S 3 = 2, S 5 = 3, S 7 = 4,..., S 15 = 7
 * Thus B14 = 8 + 6 (since 14 is in the same segment as 13 and S13 = 6)
 * Now adjacentlist1 stores the next segment with the lowest denomination
 * sensor. Adjacentlist2 stores the next lowest segment. & cetra.
 * Thus in spot adjacentlist1[14] is the sensor closest to B13/14 with the
 * lowest value, 24 = D1/2.
 */
char adjacentlist1[40] = {22, 15, 12, 13, 11, 19, 22, 22,
                          20, 16, 21,  4,  2,  3, 24,  1,
                           9, 18, 15,  2,  8,  1,  0, 18,
                          14, 10, 28, 28, 26, 23,  8, 14,
                          14, 24, 25, 22, 26, 18, 30, 21};

char adjacentlist2[40] = {NIL, 21, 19, 19, 19, NIL, NIL, NIL,
                          30, 20, 25, NIL, NIL, NIL, 31, 18,
                          24, 19, 17,  3,  9, 10,  1, 29,
                          16, 34, 34, 35, 27, 37, 38, 38,
                          16, 34, 26, 27, 27, 19, 31, 32};

char adjacentlist2b[40] ={NIL, 21, 19, 19, 19, 7, NIL, 5,
                          30, 20, 25, NIL, NIL, NIL, 31, 18,
                          24, 19, 17,  3,  9, 10,  1, 29,
                          16, 34, 34, 35, 27, 37, 38, 38,
                          16, 34, 26, 27, 27, 19, 31, 32};

char adjacentlist3[40] = {NIL, 22, NIL, NIL, NIL, NIL, NIL, NIL,
                          NIL, NIL, NIL, NIL, NIL, NIL, 32, 20,
                          32,  NIL,  23,   4,  15, 39,   6, NIL,
                          33,  NIL,  36,  36,  37, NIL, NIL, NIL,
                          39,  NIL,  33, NIL,  38, 28,  36, NIL};

char adjacentlist4[40] = {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL,
                          NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL,
                          NIL, NIL,  37,   5, NIL, NIL,   7, NIL,
                          NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL,
                          NIL, NIL, NIL, NIL, NIL,  29, NIL, NIL};

char adjacentlist5[40] = {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL,
                          NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL,
                          NIL, NIL, NIL,  17, NIL, NIL,  35, NIL,
                          NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL,
                          NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL};

char adjacentlist6[40] = {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL,
                          NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL,
                          NIL, NIL, NIL,  37, NIL, NIL, NIL, NIL,
                          NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL,
                          NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL};

trackSegment trackGraph[NUMSEGMENTS];

void init(char track) {
  /* Initialize the trackGraph so that all segments are live
   * and the connected segments are marked adjacent.
   * Note: If input paramter is 'a' then segment A15 is not adjacent
   * to A11, so we use adjacentlist2. If track == 'b' then we use
   * adjacentlistlist2b.
   */

  int i;                        /* loop index */

  if ((track != 'a') && (track != 'b')) {
    cprintf("Dumb user, bad call to init() in graph.c\n");
  }

  for (i = 0; i < NUMSEGMENTS; i++) {
    trackGraph[i].sensorDescription.module = (i / 8);
    trackGraph[i].sensorDescription.sensorNum = (((i % 8) * 2) + 1);
    trackGraph[i].adjacentList[0] = adjacentlist1[i];
    if (track == 'a') {
      trackGraph[i].adjacentList[1] = adjacentlist2[i];
    } else {                    /* this is the B track */
      trackGraph[i].adjacentList[1] = adjacentlist2b[i];
    }
    trackGraph[i].adjacentList[2] = adjacentlist3[i];
    trackGraph[i].adjacentList[3] = adjacentlist4[i];
    trackGraph[i].adjacentList[4] = adjacentlist5[i];
    trackGraph[i].adjacentList[5] = adjacentlist6[i];
    trackGraph[i].isLive = 1;
  }

}

void isAdjacent(node thisSegment, node * AdjacentList) {
  /* Return all segments adjacent to this segment in AdjacentList,
   * AdjacentList is node array of size 6, i.e. node foo[6].
   */

  int segIndex;                 /* Index into the track graph */
  int i, j;                     /* loop indices */

  segIndex = (thisSegment.module * 8);
  segIndex += ((thisSegment.sensorNum - 1) / 2);

  i = 0;                        /* Adjacent list could be 2 NIL 3... */
  j = 0;                        /* So want to pack the data in AdjacentList */

  if (trackGraph[segIndex].isLive == 0) {
    i = NUMA;                   /* This is broken track */
  }

  while ((i < NUMA) && (j < NUMA)) {
    if (trackGraph[segIndex].adjacentList[i] != NIL) {
      AdjacentList[j].module = (trackGraph[segIndex].adjacentList[i] / 8);
      AdjacentList[j].sensorNum =
        (((trackGraph[segIndex].adjacentList[i] % 8) * 2) + 1);
      i++;
      j++;
    } else {
      i++;
    }
  }

  for(; j < NUMA; j++) {
    AdjacentList[j].sensorNum = NIL;
  }

}

void breakSeg(node thisSegment) {
  /* Mark thisSegment as broken so that isAdjacent will not return this
   * segment. I.e. if a train derailed on thisSegment then an isAdjacent
   * in a formerly adjacent node would NOT return thisSegment so other
   * trains would not be routed over thisSegment.
   */

  int segIndex;                 /* Index into the track graph */
  int i, j;                     /* loop indices */
  int adjacentIndex;            /* Index into adjacent node */

  segIndex = (thisSegment.module * 8);
  segIndex += ((thisSegment.sensorNum - 1) / 2);

  for (i = 0; i < NUMA; i++) {
    adjacentIndex = trackGraph[segIndex].adjacentList[i];
    if (adjacentIndex != NIL) {
      for (j = 0; j < NUMA; j++) {
        if (trackGraph[adjacentIndex].adjacentList[j] == segIndex) {
          trackGraph[adjacentIndex].adjacentList[j] = NIL;
        }
      }
    }
  }
  trackGraph[segIndex].isLive = 0;

}

char isWorking(node thisSegment) {
  /* Return 1 if a train can use this segment, 0 if there has been a
   * derailment or if the segment is marked unavailable for some other
   * reason. (I.e. 0 iff breakSeg() was already called on thisSegment.)
   */

  int segIndex;                 /* Index into the track graph */

  segIndex = (thisSegment.module * 8);
  segIndex += ((thisSegment.sensorNum - 1) / 2);

  return trackGraph[segIndex].isLive;
}
