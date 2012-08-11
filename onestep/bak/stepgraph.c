#include "stepgraph.h"

/* Store switch directives in the form of:
 *    A1    A2   ...   B13
 * |-----|-----|-...-|-----|...
 * | NIL | NIL |-...-|  D1 |...
 * |undef|undef|-...-| 185 |... (185 == 0x9B + 30) 30 means curve
 * |undef|undef|-...-| 156 |... (156 == 0x9C)
 * |undef|undef|-...-|  E1 |...
 * |undef|undef|-...-| 185 |...
 * |undef|undef|-...-| 186 |...
 *  . . .
 * |_____|_____|_..._|_____|...
 * Note A = 0, B = 16, C = 32, D = 48, E = 64, add the sensor number to
 * get a unique value for lookup in the above data structure.
 */

typedef struct {
  int dest;                     /* one possible destination */
  int instseq[NUMSWITCH];       /* switch instruction sequence */
} destseq;

destseq switchgraph[NUMSENSORS][NUMDEST] =
    {
     /* Sensors in module A */
     {{44, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{42, {44, NIL, NIL}},   {44, {14, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{30, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{38, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{24, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{26, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{38, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{22, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{38, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{38, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     /* Note: for track B the next line should have a 14 in place of the leading NIL */
     {{NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{44, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     /* Note: for track B the next line should have a 10 in place of the leading NIL */
     {{NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{44, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},

     /* Sensors in module B */
     {{60, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{40, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{32, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{40, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{50, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{42, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{ 8, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{ 4, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{ 6, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{48, {185, 156, NIL}},  {64, {185, 186, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{62, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{ 2, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{40, {45, NIL, NIL}},   {36, {15, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},

     /* Sensors in module C */
     {{18, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{48, {185, 156, NIL}},  {64, {185, 186, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{36, {48, NIL, NIL}},   {38, {18, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{34, {36,  5, NIL}},    {46, { 6, NIL, NIL}},   {74, {36, 35, NIL}},    {NIL, {NIL, NIL, NIL}}},
     {{30, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{34, { 5, NIL, NIL}},   {74, {35, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{ 4, { 3, NIL, NIL}},   {6, {33, 32, NIL}},     {8, {33,  2, 31}},      {10, {33,  2, 1}}},
     {{30, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{16, {16, NIL, NIL}},   {18, {46, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{20, {13, NIL, NIL}},   {78, {43, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{ 2, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{70, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{ 0, {11, 12, NIL}},    { 2, {41, NIL, NIL}},   {12, {11, 42, 34}},     {14, {11, 42, 4}}},
     {{58, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{36, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},

     /* Sensors in module D */
     {{28, {183, 184, NIL}},  {32, {183, 154, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{66, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{68, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{20, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{68, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{56, { 8, NIL, NIL}},   {72, {38, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{56, { 8, NIL, NIL}},   {72, {38, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{70, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{74, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{52, {39, NIL, NIL}},   {54, { 9, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{46, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{74, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{16, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{76, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{28, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{76, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},

     /* Sensors in module E */
     {{28, {183, 184, NIL}},  {32, {183, 154, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{78, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{48, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{68, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{52, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{50, {10, NIL, NIL}},   {66, {40, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{54, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{44, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{52, {39, NIL, NIL}},   {54, { 9, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{76, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{56, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{36, {37, 48, NIL}},    {38, {37, 18, NIL}},    {58, { 7, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}},
     {{60, {17, NIL, NIL}},   {62, {47, NIL, NIL}},   {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{72, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{42, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}},
     {{64, {NIL, NIL, NIL}},  {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}, {NIL, {NIL, NIL, NIL}}}
    };

int find_sensor(int row, int sensNum) {
  /* Return the column in the switchgraph that is on the given row with
   * the specified sensor num. Note: row is in {0,1,...,79}, sensNum is
   * in {0,2,4,6,...,78} the return value will be in {NIL,0,1,2,3}.
   */

  int i;                        /* loop index */
  for (i = 0; i < NUMDEST; i++) {
    if (switchgraph[row][i].dest == sensNum) {
      return i;
    }
  }
  return NIL;
}

void init_stepgraph(char track) {
  /* Initialize the data structures in the stepgraph routine so that a
   * querey will return a meaningful result - how to get from one segment
   * to the next segment. The variable track should be one of 'a' or 'b'.
   */
  if ((track != 'a') && (track != 'b')) {
    printf("Incorrect use of init_stepgraph routine.\n");
  }
  if (track == 'b') {           /* Modify switchgraph to support difference between tracks */
    switchgraph[11][0].dest = 14;
    switchgraph[14][0].dest = 10;
  }

  /* Note the above is a pretty damn big data structure and it is probably best
   * not to create yet another one.
   */
}

void copy_setting(int row, int col, directive * instlist) {
  /* Copy switch settings from the switchgraph to the instlist */

  int j;                        /* loop index */
  int sn;                       /* Number of the switch to throw */

  for (j = 0; j < NUMSWITCH; j++) {
    sn = switchgraph[row][col].instseq[j];
    if ((sn < 30) || ((sn > 100) && (sn < 160))) {
      instlist[j].sequence = sn;
      instlist[j].setting = straight;
    } else {
      instlist[j].sequence = (sn - 30);
      instlist[j].setting = curve;
    }
  }

}

char inst_sequence(node src, node dest, directive * instlist) {
  /* Given a source (src) sensor and an ADJACENT destination (dest)
   * sensor, inst_sequence will set instlist to contain a list of switches
   * to throw and the direction to throw the switches in. Note instlist
   * should take the following form: directive instlist[NUMSWITCH];
   */
  int srcindex;                 /* index of the source node in the switch graph */
  int destindex;                /* index of the destination node in the switch graph */
  int i;                        /* loop index */

  srcindex = ((src.module * 16) + (src.sensorNum - 1));
  if ((dest.sensorNum % 2) == 0) {
    destindex = ((dest.module * 16) + (dest.sensorNum - 2));
  } else {                      /* Allow users to specify even sensor numbers as dest */
    destindex = ((dest.module * 16) + (dest.sensorNum - 1));
  }

  i = find_sensor(srcindex, destindex);
  if (i != NIL) {
    copy_setting(srcindex, i, instlist);
    return FORWARD;
  }

  /* Now what if the train has to reverse? Then we want the sensor of
   * opposite parody as source.
   */

  if ((srcindex % 2) == 0) {    /* Odd sensors have even array index */
    srcindex++;
  } else {
    srcindex--;
  }
  i = find_sensor(srcindex, destindex);
  if (i != NIL) {
    copy_setting(srcindex, i, instlist);
    return REVERSE;
  } else {
    return BAD;
  }
}
