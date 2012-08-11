#ifndef _SCHEDULAR_TYPE_H_
#define _SCHEDULAR_TYPE_H_

/*****************************************************************
 *
 * name : schedulartype.h
 *
 * authors : Insop Song & Michael Cole
 *
 * functions : supports graph abstraction of trains on track.
 * (Associates an arrival time with each node on the track.)
 *
 * date : July 19 2001
 * I love Lei beyond what words can say!
 ****************************************************************/

#include "../pathfind/graphtype.h"
#include "../pathfind/graph.h"

#define SPEEDMULT     100       /* Multiply speed by Speed Mult to get
                                 * speed in um / s.
                                 */
#define DISTMULT      100       /* Multiply distance by this num to get
                                 * distance in um.
                                 */
#define NUMTRAINS     6         /* Number of working trains */
#define TRAINSPEEDS   15        /* All the speeds a train can go at includes speed 0 */

typedef struct {                /* Node with an arrival time */
  modName module;               /* Sensor letter (enum: A, B, C, D, E) */
  char sensorNum;               /* One of 1, 3, 5, 7, 9, 11, 13, 15 */
  unsigned int cost;            /* cost to reach this point from ancestor node */
  int parent;                   /* parent index number - between 0 and 39 */
  int trainNum;                 /* Train on this space at this time */
  int arrivalTime;              /* Time the train is expected to arrive
                                 * at this node.
                                 */
} timedNode;                    /* standard node, with arrivalTime */

typedef struct {                /* Basic node, with speed */
  modName module;               /* Sensor letter (enum: A, B, C, D, E) */
  char sensorNum;               /* One of 1, 3, 5, 7, 9, 11, 13, 15 */
  int speed;                    /* If < 0 then this is a new path and must go through
                                 * one step. If > 0 the oneHop path provided is valid
                                 * and this entry is the speed the train should go at.
                                 */
} speedNode;

#endif
