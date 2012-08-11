
#include 	"serial.h"

#define NOT_FLOW_CONTROL



/**********************************************************
 *
 *  setUART()
 *	
 *  set serial port(RS232) for Train Control Unit
 *
 *  2400 baud, 
 *  1 start bit, 
 *  2 stop bits, 
 *  8 data bit
 *  with no parity check
 *  base address in IO space for serial port
 *
 ***********************************************************
 */

void setUSART(
       u_short port,
       unsigned int baud,
       unsigned int data,
       unsigned int parity,
       unsigned int stop)
{
    unsigned mode_word;

    stop = ((stop-1) & 1);
    stop <<= 2;

    baud /= 10;
    baud = 11520 / baud;

    parity <<= 3;
    parity &= 0x018;

    data -= 5;
    data &= 3;

    mode_word = data | stop | parity;

    outb(port + LCR, inb(port + LCR) | LCR_DLAB);
    outb(port, baud % 256);
    outb(port + 1, baud / 256);
    outb(port + LCR, mode_word & 0x7F);
    /* turn on DTR and all other outputs (harmless) */

    outb(USART_1_BASE + USART_IER, 0x0); 

    /* turn off all interrupt sources */
    outb(port + IER, 0);

    /* turn on FIFO, but tell it to make interrupts (if they
    * were enabled when the FIFO has one item.
    */
    outb(port + FCR, 0x0f);

    /* Read pending input and status bits (ignore them)... */
    inb(port + LSR);
    inb(port + MSR);
    inb(port);

    /*printf("train port setted up\n");*/
}



/******************************************************************
 *
 * initialize the serial port
 * for interrupting
 *
 ******************************************************************
 */
void initSerial( int select ) {


#ifdef NOT_FLOW_CONTROL

    char lcr;
    char intSet;
    char data;


    if( select == 1) {
    /* for train serial port */
	setUSART(USART_0_BASE, 2400, 8, 0, 2);  

	intSet = IER_Received_Data | IER_Xmt_Hld_Reg_Empty;
	/*intSet = IER_Received_Data; */

	outb(USART_0_BASE + USART_IER, intSet);
	outb(USART_1_BASE + USART_IER, intSet);

    /* FIFO buffe clear */
	outb( USART_0_BASE + USART_FCR, 0 );
	outb( USART_1_BASE + USART_FCR, 0 );
	/* start data */

    /* just flush the input register */
	data = inb( USART_0_DATA );
	data = inb( USART_0_DATA );
	data = inb( USART_1_DATA );
	data = inb( USART_1_DATA );

	/*cprintf("serial  port setted up\n"); */
    }
    /* setting for another serial terminal */
    else if( select == 2) {

    /* for train serial port */
	setUSART(USART_0_BASE, 2400, 8, 0, 2);  

	/* for another serial terminal */
	setUSART(USART_1_BASE, 115200, 8, 0, 1);

	intSet = IER_Received_Data | IER_Xmt_Hld_Reg_Empty;
	/*intSet = IER_Received_Data; */

	outb(USART_0_BASE + USART_IER, intSet);
	outb(USART_1_BASE + USART_IER, intSet);

    /* FIFO buffe clear */
	outb( USART_0_BASE + USART_FCR, 0 );
	outb( USART_1_BASE + USART_FCR, 0 );
	/* start data */

    /* just flush the input register */
	data = inb( USART_0_DATA );
	data = inb( USART_0_DATA );
	data = inb( USART_1_DATA );
	data = inb( USART_1_DATA );

	/*cprintf("serial  port setted up\n"); */
    } /* else if */


#endif /* NOT_FLOW_CONTROL */




#ifdef FLOW_CONTROL

    char lcr;
    char intSet;
    char data;
    char mcr;

/* for train serial port */
    setUSART(USART_0_BASE, 2400, 8, 0, 2);  


    /* clear irq 
    inb( USART_0_BASE + USART_IIR);
    inb( USART_0_BASE + USART_LSR);
    inb( USART_0_BASE + USART_MSR);
    */
    /* set modem control register */
    mcr = MCR_rts | MCR_dtr | MCR_Out_1 | MCR_Out_2;
    outb(USART_0_BASE + USART_MCR, mcr); 
    
    /*intSet = IER_Received_Data; */

    /* try to use modem status for serial port 0 for train*/
    /* intSet = IER_Received_Data | IER_Xmt_Hld_Reg_Empty |
     * IER_Modem_Status;*/
    /*intSet = 0x0f; */
    /*intSet = IER_Received_Data | IER_Modem_Status |
     * IER_Xmt_Hld_Reg_Empty;*/
    intSet = 0x0d; 
    outb(USART_0_BASE + USART_IER, intSet);

    intSet = IER_Received_Data | IER_Xmt_Hld_Reg_Empty;
    outb(USART_1_BASE + USART_IER, intSet);

/* FIFO buffe clear */
    outb( USART_0_BASE + USART_FCR, 0 );
    outb( USART_1_BASE + USART_FCR, 0 );
    /* start data */

/* just flush the input register */
    data = inb( USART_0_DATA );
    data = inb( USART_0_DATA );
    data = inb( USART_1_DATA );
    data = inb( USART_1_DATA );

    /*cprintf("serial  port setted up\n"); */

#endif /* FLOW_CONTROL */
}


/* enable interrupt, now bit0, 1, 2 are turned on 
* 
*bit 7	6	5	4	3	2	1	0	
*--------------------------------------------------------------
*set	0	0	0	0	0	1	1	1
*
* 	bit0 : received data available interrupt
*	bit1 : Transmitter Holding Register Empty
*	bit2 : receiver line status
*	bit3 : modem status interrupt 
*	bit4-7: don't care
*/
