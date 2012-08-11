#include "schedulartype.h"
#define MAXNUM        2147483647/* Want a number bigger than cost could
		                             * possibly be.
		                             */
#define COLISION      (-3)      /* This route will result in a head on colision */
#define TIMETOLERENCE 50        /* Time gap between trains */
#define TICKSPERSEC   20        /* Number of clock ticks per second */
#define STARTTIME     (-60)     /* Less than 0 time - TIMETOLERENCE */

short speed[NUMTRAINS][TRAINSPEEDS] = {
           /* Speed of train 1, (mm / s) */
           {0, 0,  34,  60, 108, 140, 206, 264, 316, 394, 480, 546, 664, 742, 858},
           /* Speed of train 6, (mm / s) */
           {0, 0,  34,  60, 108, 140, 206, 264, 316, 394, 480, 546, 664, 742, 858},
           /* Speed of train 12, (mm / s) */
           {0, 0,  44,  70, 118, 150, 216, 274, 326, 404, 490, 556, 674, 752, 868},
           /* Speed of train 52, (mm / s) */
           {0, 0,  34,  60, 108, 140, 206, 264, 316, 394, 480, 546, 664, 742, 858},
           /* Speed of train 55 (mm / s) */
           {0, 0, 104, 180, 266, 320, 408, 482, 536, 550, 602, 622, 630, 658, 720},
           /* Speed of train 57 (mm / ss) */
           {0, 0,  34,  60, 108, 140, 206, 264, 316, 394, 480, 546, 664, 742, 858}
      };

timedNode trafficMap[NUMSEGMENTS];
                                /* Time when a train is expected at given node */

char trackID;                   /* One of 'a' or 'b' */

char strWrite[50];

void init_schedular(void) {
  /* Initialize all data structures used */
  int i;                        /* loop index */

  for (i = 0; i < NUMSEGMENTS; i++) {
    trafficMap[i].module = (i / 8);
    trafficMap[i].sensorNum = (((i % 8) * 2) + 1);
    trafficMap[i].cost = MAXNUM;
    trafficMap[i].parent = NIL;
    trafficMap[i].trainNum = NIL;
    trafficMap[i].arrivalTime = STARTTIME;
  }
}

void ClearRoute(void) {
  /* Clear the current route from the traffic map so that a new route can be generated */
  int i;                        /* loop index */

  for (i = 0; i < NUMSEGMENTS; i++) {
    trafficMap[i].cost = MAXNUM;
    trafficMap[i].parent = NIL;
  }
}

int WillColl(int locationIndex, int trainNum, int time) {
  /* Return 0 iff train 'trainNum' will NOT collide with some other train.
   * location index is the current location of the train, time is time under
   * examination.
   */

  /* If this train is already in this space, this space is safe */
  if (trainNum == trafficMap[locationIndex].trainNum) return 0;

  if (((time - trafficMap[locationIndex].arrivalTime) < TIMETOLERENCE) &&
      ((trafficMap[locationIndex].arrivalTime - time) < TIMETOLERENCE)) {
    return 1;
  }
  return 0;
}

int CheckSpeed(oneHop * desiredPath, int trainNum, int trainSpeed) {
  /* Check this path to see if train 'trainNum' will collide with someone given
   * the trains current speed, 'trainSpeed'. Assume the train leaves NOW.
   * Return 0 iff train will NOT collide. Otherwise return index into desiredPath
   * where collision takes place.
   */
  int i;                        /* loop index */
  int locationTime;             /* time at particular location */
  int locationIndex;            /* location index into trafficMap */
  char WillCollide = 0;         /* Value is 1 if trains occupie same space conncurently */

  locationTime = GetSmallTime();
  locationTime += ((desiredPath[0].distance * DISTMULT) /
          ((trainSpeed * SPEEDMULT) / TICKSPERSEC));
  for (i = 1; i < NUMSEGMENTS; i++) {
    locationIndex = (desiredPath[i].thisNode.module * 8);
    locationIndex += ((desiredPath[i].thisNode.sensorNum - 1) / 2);
    WillCollide = WillColl(locationIndex, trainNum, locationTime);
    if (WillCollide) break;
    if (desiredPath[i].distance == NIL) break;
    locationTime += ((desiredPath[i].distance * DISTMULT) /
          ((trainSpeed * SPEEDMULT) / TICKSPERSEC));
  }
  if (WillCollide) {
    return i;
  }
  return WillCollide;
}

void MarkRoute(oneHop * desiredPath, int trainNum, int trainSpeed) {
  /* This route is cleared for going on, so we want to mark this route as taken */
  int i;                        /* loop index */
  int locationTime;             /* time at particular location */
  int locationIndex;            /* location index into trafficMap */

  locationTime = GetSmallTime();
  locationTime += ((desiredPath[0].distance * DISTMULT) /
        ((trainSpeed * SPEEDMULT) / TICKSPERSEC));
  for (i = 1; i < NUMSEGMENTS; i++) {
    locationIndex = (desiredPath[i].thisNode.module * 8);
    locationIndex += ((desiredPath[i].thisNode.sensorNum - 1) / 2);
    trafficMap[locationIndex].trainNum = trainNum;
    trafficMap[locationIndex].arrivalTime = locationTime;
    if (desiredPath[i].distance == NIL) break;
    locationTime += ((desiredPath[i].distance * DISTMULT) /
        ((trainSpeed * SPEEDMULT) / TICKSPERSEC));
  }
}

int speedLookup(int trainNum, int Tspeed) {
  /* Return the speed in mm/s of specified train given a speed between 0 and 14 */
  if (trainNum == 1) return speed[0][Tspeed];
  if (trainNum == 6) return speed[1][Tspeed];
  if (trainNum == 12) return speed[2][Tspeed];
  if (trainNum == 52) return speed[3][Tspeed];
  if (trainNum == 55) return speed[4][Tspeed];
  return speed[5][Tspeed];
}

int SetArrivalTime(oneHop * desiredPath, int trainNum) {
  /* Take in the path desired by a task and determine if this path is feasible.
   * If the path is feasible, mark the arrival times in the trafficMap and return
   * speed train should go at. If there is a colision return COLISION.
   * If route is not clear, route is not marked in the path, but collision point is
   * marked broken.
   */

  int WillCollide;              /* Is this train going to collide with anything */
  int Tspeed;                   /* speed to set the train to */
  int CollisionPoint;           /* If there is a collision where it will take place */

  Tspeed = 6;
  WillCollide = CheckSpeed(desiredPath, trainNum, speedLookup(trainNum, Tspeed));
  if (!WillCollide) {
    MarkRoute(desiredPath, trainNum, speedLookup(trainNum, Tspeed));
    return Tspeed;
  }
  Tspeed = 2;
  WillCollide = CheckSpeed(desiredPath, trainNum, speedLookup(trainNum, Tspeed));
  if (!WillCollide) {
    MarkRoute(desiredPath, trainNum, speedLookup(trainNum, Tspeed));
    return Tspeed;
  }
  Tspeed = 10;
  WillCollide = CheckSpeed(desiredPath, trainNum, speedLookup(trainNum, Tspeed));
  if (!WillCollide) {
    MarkRoute(desiredPath, trainNum, speedLookup(trainNum, Tspeed));
    return Tspeed;
  }
  /* Picking different speeds is not doing much good, lets find the point
   * of collision and try to avoid it.
   */
  CollisionPoint = CheckSpeed(desiredPath, trainNum, speedLookup(trainNum, 6));
  breakSeg(desiredPath[CollisionPoint].thisNode);
  return COLISION;
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
      if ((trafficMap[childIndex].cost - 1) > parentCost) {
        trafficMap[childIndex].cost = (parentCost + 1);
        trafficMap[childIndex].parent = startPoint;
        FindShortest(childIndex, trafficMap[childIndex].cost);
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
    current = trafficMap[current].parent;
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

int router(int * newPath, int trainNum, oneHop * desiredPath) {
  /* See if it is possible to travel desiredPath. If it is possible return
   * the speed the train should move along the desiredPath. If it is not
   * possible attempt to generate a new path and return COLISION this way we
   * know we have to use newPath and generate a new desiredPath.
   */
  int ReturnVal;                /* Will either be the speed the train should
                                 * go at or COLISION.
                                 */
  int start;                    /* Start node between 0 and 40 */
  int end;                      /* End node between 0 and 40 */
  int i;                        /* loop index */

  for (i = 0; i < NUMSEGMENTS; i++) newPath[i] = NIL;

  ReturnVal = SetArrivalTime(desiredPath, trainNum);
  if (ReturnVal != COLISION) return ReturnVal;

  for (i = 0; i < NUMSEGMENTS; i++) {
    if (desiredPath[i].distance == NIL) break;
  }                             /* i is now known to be the end of the path */

  start = (desiredPath[0].thisNode.module * 8);
  start += ((desiredPath[0].thisNode.sensorNum - 1) / 2);
  end = (desiredPath[i].thisNode.module * 8);
  end += ((desiredPath[i].thisNode.sensorNum - 1) / 2);
  ClearRoute();
  FindShortest(end, 0);
  findPath(newPath, end, start);

  return ReturnVal;
}

void convertPath(int * path, speedNode * newpath, int Tspeed) {
  /* Convert path into a user readable form */
  int i;                        /* loop index */
/*cprintf("The path generated by schedular is: ");*/
  for (i = 0; i < NUMSEGMENTS; i++) {
    if (path[i] != NIL) {
      newpath[i].module = (path[i] / 8);
      newpath[i].sensorNum = (((path[i] % 8) * 2) + 1);
    } else {
      newpath[i].sensorNum = NIL;
    }
    newpath[i].speed = Tspeed;
    /*cprintf("%c:%d,%d ", (newpath[i].module + 'A'), newpath[i].sensorNum,
        newpath[i].speed);*/
  }
  /*cprintf("\n");*/
}

main() {
  /* Unbundle message, attempt to route, if route is feasible reply with single postive
   * integer which will represent speed. If route is unfeasible reply with COLISON and
   * new path which must be passed through the onestep task.
   * Message formats:
   * Always send an array of oneHop, this array should be of size NUMSEGMENTS.
   * If the first entry, oneHop[0].distance == NIL then oneHop[0].thisNode is
   * a section of track that a train derailed on, the section of track will not
   * be reused. Otherwise this task should receive an array of oneHops that corespond
   * to the route the train wishes to take. (The Reply() from onestep.) HOWEVER in
   * the last used record of the oneHop array this task should also take the train
   * number of this train - in the direction field. (This will be the same record that
   * has distance == NIL).
   */
  int genPath[NUMSEGMENTS];     /* Path generated by Dijkstra here */
  speedNode nPath[NUMSEGMENTS]; /* Path generated here, to be passed to client */
  oneHop desPath[NUMSEGMENTS];  /* Path passed in by client */
  node sBreak[NUMSEGMENTS];     /* Segments to break - trains derailed here */
  int centralControlTID;        /* What could this ambigiously named variable be? */
  int clientTID;                /* TID of the client checking the proposed schedule */
  int fspeed;                   /* final speed the train should go at */
  int trainNum;                 /* Number of the train to process */
  int i;                        /* loop index */
  int brkseg = 0;               /* number of broken segments */

  RegisterAs("Schedular");
  Delay(10);
  centralControlTID = Receive(&trackID, sizeof(char));
  Reply(centralControlTID, NULL, NULL);
  init_schedular();

  while (1) {
    clientTID = Receive(&desPath, (NUMSEGMENTS * sizeof(oneHop)));
    if (desPath[0].distance == NIL) {
      Reply(clientTID, NULL, NULL);
      sBreak[brkseg] = desPath[0].thisNode;
      brkseg++;                 /* Just added a broken segment to the list */
    } else {
      init(trackID);
      for (i = 0; i < brkseg; i++) {
        breakSeg(sBreak[i]);
      }
      for (i = 0; i < NUMSEGMENTS; i++) {
        if (desPath[i].distance == NIL) {
          trainNum = desPath[i].direction;
          break;
        }
      }
      fspeed = router(genPath, trainNum, desPath);
      convertPath(genPath, nPath, fspeed);
      Reply(clientTID, &nPath, (NUMSEGMENTS * sizeof(speedNode)));
    }
  }
}
