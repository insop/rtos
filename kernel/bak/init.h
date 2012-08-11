#ifndef _init_h_
#define _init_h_

typedef struct {
  int delayFor;         /* How many more ticks until wake up */
  int PID;              /* Process PID to wake up */
} ClockEntry;

#endif
