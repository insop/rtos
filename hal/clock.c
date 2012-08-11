#include 	"clock.h"

void initClock( void ) {


    /*printf(">>>PIT SETTING \n");   enable the Interrupt */

    /* set timer 0 as rate generator 
     * when the counter comes to zero int 32 will 
     * take place */
    outb(TIMER_MODE, TIMER_SEL0|TIMER_RATEGEN|TIMER_16BIT);

    outb(TIMER_CNTR0, TIMER_DIV(20)%256);
    outb(TIMER_CNTR0, TIMER_DIV(20)/256);

/*
// maybe different system clock

    unsigned short counter;
    unsigned short ushort;
    unsigned char  uchar;

    counter = SYSTEM_100_Hz;
    ushort = counter & 0xFF;
    uchar  = (unsigned char) ushort;
    outb( TIMER_CNTR0, uchar );

    ushort = (counter & 0xFF00)>>8;
    uchar  = (unsigned char) ushort;
    outb( TIMER_CNTR0, uchar );
*/

}


