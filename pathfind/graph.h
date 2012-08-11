#ifndef _GRAPH_H_
#define _GRAPH_H_

/*****************************************************************
 *
 * name : graph.h
 *
 * authors : Insop Song & Michael Cole
 *
 * functions : support graph abstraction of the train track
 *
 * graph.h file includes the following routines :
 *
 *  void init(char track)
 *  void isAdjacent(node thisSegment, node * AdjacentList)
 *  void breakSeg(node thisSegment)
 *  char isWorking(node thisSegment)
 *
 * date : 27 June 2001
 * I really love Lei
 *****************************************************************/

#define Eos
#ifndef Eos
#include <stdio.h>
#endif

#ifdef Eos
#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"
#endif

#include "graphtype.h"

void init(char track);
  /* Initialize the trackGraph so that all segments are live
   * and the connected segments are marked adjacent.
   * Note: If input paramter is 'a' then segment A15 IS NOT adjacent
   * to A11. If input paramter is 'b' then we assume that segment A15 IS
   * adjacent to A11. If anything is input then we have a bad user!
   */

void isAdjacent(node thisSegment, node * AdjacentList);
  /* Return all segments adjacent to this segment in AdjacentList,
   * AdjacentList is node array of size 6, i.e. node foo[6].
   * Note: if thisSegment is of degree 2 say then
   * foo[3].sensorNum == NIL
   */

void breakSeg(node thisSegment);
  /* Mark thisSegment as broken so that isAdjacent will not return this
   * segment. I.e. if a train derailed on thisSegment then an isAdjacent
   * in a formerly adjacent node would NOT return thisSegment so other
   * trains would not be routed over thisSegment.
   */

char isWorking(node thisSegment);
  /* Return 1 if a train can use this segment, 0 if there has been a
   * derailment or if the segment is marked unavailable for some other
   * reason. (I.e. 0 iff breakSeg() was already called on thisSegment.)
   */

#endif
