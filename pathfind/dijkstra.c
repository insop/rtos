/*****************************************************************
 *
 * name : dijkstra.c
 *
 * authors : Insop Song & Michael Cole
 *
 * functions : find shortest path from some point 'A' to some point
 *   (possibly the same as 'A') 'B'.
 *
 * dijkstra.c file includes the following routines :
 *
 * void init_graph(void)
 * void FindShortest(int startPoint, int parentCost)
 * void findPath(int * path, int start, int end)
 * main()
 *
 * date : 30 June 2001
 * I really miss Lei
 *****************************************************************/

#include "../pathfind/graph.h"
#include "../syscall/sysCall.h"

/* definitions for train tasks */
#include "../train/ironBird.h"

#define MAXNUM	      2147483647 /* Want a number bigger than cost could
		                              * possibly be.
		                              */
#define INSTSIZE      3          /* Size of user instruction to pathfind */

typedef struct {
  unsigned int cost;            /* cost to reach this point from 'A' */
  modName sensorLet;            /* This segments sensor letter */
  char sensorNum;               /* This segments sensor number */
  int parent;                   /* Parent number (between 0 and 39) */
} dNode;

dNode weightedGraph[NUMSEGMENTS];

void init_graph(void) {
  /* Initialize all the values in the weighted Graph */
  int i;                        /* loop index */

  for (i = 0; i < NUMSEGMENTS; i++) {
    weightedGraph[i].cost = MAXNUM;
    weightedGraph[i].sensorLet = (i / 8);
    weightedGraph[i].sensorNum = (((i % 8) * 2) + 1);
    weightedGraph[i].parent = NIL;
  }

}

void FindShortest(int startPoint, int parentCost) {
  /* Tour through all the children, shorten their cost if possible */
  int i;                        /* loop index */
  node children[NUMA];          /* List of adjacent nodes */
  int childIndex;               /* Index to the child's number */

  children[0].module = (startPoint / 8);
  children[0].sensorNum = (((startPoint % 8) * 2) + 1);
  isAdjacent(children[0], children);

  if (parentCost > NUMSEGMENTS) {
    return;
  }

  for (i = 0; i < NUMA; i++) {
    if (children[i].sensorNum != NIL) {
      childIndex = (children[i].module * 8);
      childIndex += ((children[i].sensorNum - 1) / 2);
      if ((weightedGraph[childIndex].cost - 1) > parentCost) {
        weightedGraph[childIndex].cost = (parentCost + 1);
        weightedGraph[childIndex].parent = startPoint;
        FindShortest(childIndex, weightedGraph[childIndex].cost);
      }
    }
  }

}

void findPath(int * path, int start, int end) {
  /* Find the shortest path from start to end.
   * Note: Since we have parent nodes but not child nodes
   * users should interchange start and end. */
  int i = 0;                    /* loop index */
  int current;                  /* current place in graph */

  current = end;
  while(current != start) {
    if (i == NUMSEGMENTS) {
      path[0] = NIL;
      break;
    }
    path[i] = current;
    current = weightedGraph[current].parent;
    i++;
  }

  if (i < NUMSEGMENTS) {
    path[i] = start;
    i++;
  }

  for(; i < NUMSEGMENTS; i++) {
    path[i] = NIL;
  }
}

void convertPath(int * path, node * newpath) {
  /* Convert path into a user readable form */
  int i;                        /* loop index */

  for (i = 0; i < NUMSEGMENTS; i++) {
    if (path[i] != NIL) {
      newpath[i].module = (path[i] / 8);
      newpath[i].sensorNum = (((path[i] % 8) * 2) + 1);
    } else {
      newpath[i].sensorNum = NIL;
    }
  }
}

main() {
  char trackId;                 /* Which track is this, A or B */
  char instruction[INSTSIZE];   /* What the client task wants us to do */
  int start;                    /* Current location */
  int sensNum;                  /* Number of user sensor */
  int end;                      /* Where we want to go */
  int path[NUMSEGMENTS];        /* Path to follow */
  node newpath[NUMSEGMENTS];    /* Path returned to the user */
  int centralControlTID;        /* obvious */
  int clientTID;                /* TID of the client process */
  node segToBreak;              /* Break this segment */
  int temp;                     /* Start sens number - backup */
  

  RegisterAs( "PathFind" );
  Delay(10);

  centralControlTID = Receive(&trackId, sizeof(char));
  Reply(centralControlTID, NULL, NULL);
  init(trackId);

  while(1) {
    clientTID = Receive(&instruction, (INSTSIZE * sizeof(char)));
    if (instruction[0] == 'b') {
      segToBreak.module = (instruction[1] / 16);
      sensNum = (instruction[1] % 16);
      if ((sensNum % 2) == 0) {
        segToBreak.sensorNum = sensNum + 1;
      } else {
        segToBreak.sensorNum = sensNum;
      }
      breakSeg(segToBreak);
      Reply(clientTID, NULL, NULL);
    } else if (instruction[0] == 's') {
      start = ((instruction[1] / 16) * 8);
      sensNum = (instruction[1] % 16);

      if ((sensNum % 2) == 0) {
        start += ((sensNum + 1) / 2);
      } else {
        start += (sensNum / 2);
      }
      temp = sensNum;

      end = ((instruction[2] / 16) * 8);
      sensNum = (instruction[2] % 16);
      if ((sensNum % 2) == 0) {
        end += ((sensNum + 1) / 2);
      } else {
        end += (sensNum / 2);
      }
      init_graph();
      FindShortest(end, 0);
      findPath(path, end, start);
      convertPath(path, newpath);
      newpath[0].sensorNum = (temp + 1);

      Reply(clientTID, &newpath, (sizeof(node) * NUMSEGMENTS));
    } else {
      Reply(clientTID, NULL, NULL);
      cprintf("Bad useage of dijkstra.c line %d\n", __LINE__);
      Exit();
    }
  }
}

