#include "stepgraph.h"

main() {
  node src, dest;
  int c;

  printf("Please select today's track.\n");
  c = getchar();
  getchar();
  init_stepgraph(c);
  for(;;) {
    printf("Please select a src module (A = 0, B = 1, C = 2, D = 3, E = 4).\n");
    src.module = (getchar() - '0');
    getchar();
    printf("Please select a src num (1, 2, 3, ..., 16).\n");
    src.sensorNum = (getchar() - '0');
    getchar();
    printf("Please select a dest module (A = 0, B = 1, C = 2, D = 3, E = 4).\n");
    dest.module = (getchar() - '0');
    getchar();
    printf("Please select a dest num (1, 2, 3, ..., 16).\n");
    dest.sensorNum = (getchar() - '0');
    getchar();
    printf("The distance between selected nodes is: %d00 um\n",
        distance_between(src, dest));
  }
}
  
