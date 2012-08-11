#ifndef _GRAPH_TYPE_H_
#define _GRAPH_TYPE_H_

/*****************************************************************
 *
 * name : graphtype.h
 *
 * authors : Insop Song & Michael Cole
 *
 * functions : support graph abstraction of the train track
 *
 * graphtype.h file includes no routines but does have
 * several type definitions.
 *
 * date : July 3, 2001
 * I love Lei so much!
 *****************************************************************/

#define NUMSEGMENTS     40      /* Two sensors on every segment */
#define NIL            (-1)     /* Since arrays have ele'm at 0 */
#define NUMA            6       /* MAX number of adjacent nodes */

typedef enum {
  A, B, C, D, E
} modName;

typedef struct {                /* Type to be returned to user code */
  modName module;               /* this way users don't have to sort */
  char sensorNum;               /* through annoying number schemes. */
} node;

typedef struct {
  node sensorDescription;       /* This segment's sensor module & number */
  char isLive;                  /* If there is a derailment, set this
                                 * to zero, else it should be 1 */
  int adjacentList[NUMA];       /* Note that most segments only */
} trackSegment;                 /* adjacent to two other segments, */
                                /* but segments near switches adjacent */
                                /* to three sensors and segments near */
                                /* switches 1, 2, 3, 4, 5, 6, 12 are */
                                /* adjacent to up to 6 sensors. */
#endif
