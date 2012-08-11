#ifndef _STEP_GRAPH_H_
#define _STEP_GRAPH_H_

/*****************************************************************
 *
 * name : stepgraph.h
 *
 * authors : Insop Song & Michael Cole
 *
 * functions : support instruction conversions to get from one
 * segment to the next segment.
 *
 * stepgraph.h file includes the following routines :
 *
 * void init_stepgraph(char track)
 * char inst_sequence(node src, node dest, directive * instlist)
 *
 * date : 4 July 2001
 * I love Lei so much!
 *****************************************************************/

#undef Eos
#ifndef Eos
#include <stdio.h>
#endif

#ifdef Eos
#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"
#endif

#include "../pathfind/graphtype.h"

#define NUMSENSORS    80        /* Number of segments now less important */
#define NUMDEST       4         /* MAX number of nodes reachable
                                 * from one sensor
                                 */
#define NUMSWITCH     3         /* MAX number of switches to throw to get
                                 * from any one sensor to any other adjacent
                                 * sensor.
                                 */
#define FORWARD       1         /* Direction train must go in */
#define REVERSE     (-1)        /* See above */
#define BAD         (-2)        /* Could not perform requested operation */

typedef enum {
  straight,
  curve
} sdirection;

typedef struct {
  int sequence;                 /* Switch to throw */
  sdirection setting;           /* direction to throw the switch */
} directive;                    /* list of switches to throw to move one seg */

void init_stepgraph(char track);
  /* Initialize the data structures in the stepgraph routine so that a
   * querey will return a meaningful result - how to get from one segment
   * to the next segment. The variable track should be one of 'a' or 'b'.
   */

char inst_sequence(node src, node dest, directive * instlist);
  /* Given a source (src) sensor and an ADJACENT destination (dest)
   * sensor, inst_sequence will set instlist to contain a list of switches
   * to throw and the direction to throw the switches in. Note instlist
   * should take the following form: directive instlist[NUMSWITCH];
   * Note (2) src.sensorNum should be in {1,2,3,...,16}.
   * Note (3) The return value is REVERSE for reverse or FORWARD for same direction,
   * or BAD.
   */

#endif
