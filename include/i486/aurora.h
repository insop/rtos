/* This file is provided for source compatiblity -- use something else for
 * new code. -- PDG
 */
#ifndef _AURORA_h_INCLUDED_
#define _AURORA_h_INCLUDED_

/*  USART REGISTERS  */
#define	USART_0_STATUS	(0x03f8 + 2)		/* updated to reflect PC hardware */
#define	USART_0_DATA	(0x03f8 + 0)
#define USART_1_STATUS	(0x02f8 + 2)
#define USART_1_DATA	(0x02f8 + 0)

/*  STATUS READ register bits  */
#define USART_TX_RDY	0x01		/*  Transmitter ready		*/
#define USART_RX_RDY	0x02		/*  receiver ready		*/
#define	USART_TX_EMPTY	0x04		/*  Transmit buffer empty	*/
#define	USART_PE	0x08		/*  parity error		*/
#define	USART_OE	0x10		/*  overrun error		*/
#define	USART_FE	0x20		/*  framing error		*/
#define	USART_DSR	0x80		/*  data set ready		*/

/*  STATUS WRITE register bits */
#define USART_TXE	0x01
#define USART_DTR	0x02
#define USART_RXE	0x04
#define USART_SBRK	0x08
#define USART_ER	0x10
#define USART_RTS	0x20
#define USART_IR	0x40
#define USART_EH	0x80


/* Important I/O addresses on the Aurora
 */
#define	DAC_COLOUR_REG		0x0E
#define VIDEO_DAC		0xA0
#define VIDEO_DAC_R_TABLE	0xA0
#define VIDEO_DAC_G_TABLE	0xC0
#define VIDEO_DAC_B_TABLE	0xE0

#define	HI_RES_SELECT		0x101
#define LO_RES_SELECT		0x103

#define PAR_PORT_A		0x0C
#define PAR_PORT_B		0x0E
#define PAR_PORT_C		0x1C
#define PAR_PORT_CTL		0x1E

#define	PAR_PORT_CTL_SETUP	0x90	/*  A:input  B,C:output  */
#define	PAR_PORT_B_SETUP	0x00	/*  Nothing special      */

/* Programmable interrupt controller register locations */
#define	INT3	0xff3e
#define	INT2	0xff3c
#define	INT1	0xff3a
#define	INT0	0xff38
#define	TIMER	0xff32
#define	ICSR	0xff30
#define	MASKR	0xff28
#define	EOIR	0xff22

/* Useful timer register constants */
#define TIMER0_MODE_REG    0xff56
#define TIMER1_MODE_REG    0xff5e
#define TIMER2_MODE_REG    0xff66
#define TIMER0_COUNT_REG   0xff50
#define TIMER1_COUNT_REG   0xff58
#define TIMER2_COUNT_REG   0xff60
#define TIMER0_MAX_REG     0xff52
#define TIMER1_MAX_REG     0xff5a
#define TIMER2_MAX_REG     0xff62



/* Parallel port C setup:
 * Disable everything
 * Halts the keyboard click
 * Resets the STI chip
 * Turns off the video
 * Inactivates the PSG
 */
#define PAR_PORT_C_SETUP	0x80

/* Parallel port C bits	*/
#define PPC_VID			0x08	/*  Video enable		*/
#define	PPC_STI			0x20	/*  STI enable			*/
#define	PPC_KCK			0x40	/*  Keyboard clock enable	*/
#define	PPC_KBD			0x80	/*  Keyboard enable		*/


/* Possible commands for system_ctl()	*/
#define VIDEO_ON		 1
#define VIDEO_OFF		 2
#define	KEYBD_EN		 3
#define KEYBD_RESET		 4
#define KCLOCK_RUN		 5
#define KCLOCK_HALT		 6
#define STI_EN			 7
#define STI_RESET		 8
#define NETADDR_EN		 9
#define LO_RES_VIDEO		10
#define	HI_RES_VIDEO		11


/* These are the defines for the Aurora graphics hardware */
#define	FONT_ROM_SEG		0xFFB0
#define	START_OF_VRAM_SEG	0x0040
#define	END_OF_VRAM_SEG		0x0B80
#define	START_OF_VRAM		0x0400
#define	END_OF_VRAM		0xB800

#define	STD_VRAM_SEG		0x0040
#define	AMOUNT_OF_VRAM		0xB400

#define	STD_SCREEN12		0x0000
#define	STD_SCREEN13		0x0040

#define	ROW_LO_RES		36
#define	COL_LO_RES		40
#define	ROW_HI_RES		36
#define	COL_HI_RES		80

#define	PIXELS_Y_LO_RES		288
#define	PIXELS_X_LO_RES		320
#define	PIXELS_Y_HI_RES		288
#define	PIXELS_X_HI_RES		640


/*  Available Colours in Low Resolution mode  */
#define	BLACK			0
#define	RED			1
#define	GREEN			2
#define	YELLOW			3
#define	BLUE			4
#define	PURPLE			5
#define	CYAN			6
#define	WHITE			7


/*  Available colours in High resolution mode  */
#define	HI_RES_BLACK		0
#define	HI_RES_RED		1
#define	HI_RES_GREEN		2
#define	HI_RES_BLUE		3


/*  Adjustment for square pixels  */
#define	FXP_LO_RES_FACTOR	0x0000C8DC	/*  decimal 0.7846  */
#define	FXP_HI_RES_FACTOR	0x000191B7	/*  decimal 1.5692  */


/*  Global Variables  */
extern int  _char_col;		/*  Number of character columns  */
extern int  _char_row;		/*  number of character rows  */
extern int  _hi_resolution;	/*  =0 if low resolution, else high res */
extern int  _screen_seg;	/*  physical segment of colour video memory */
extern long _rep320_array;	/*  used to map low res pixels to memory  */
extern int  _rep640_array;	/*  used to map high res pixels to memory  */

/* io ports defined as global variables */
#if 0
/* port keyword doesn't exist in GCC */
extern port char pp_b;
extern port char pp_c;

extern port char lo_res;
extern port char hi_res;

extern port char pp_ctl;

extern port char u1_status;
extern port char u1_data;
#endif

/* Function prototypes */
void parport_setup();
void system_ctl( int );

int point( int, int, int );
void circle( int, int, int, int );
void line( int, int, int, int, int );
void rect( int, int, int, int, int, int );
void default_colours();
void define_colour( int, int, int, int );
void gcls( int );
int gputchar( int, int, int, int, int, int );
void gputs( int, int, char *, int, int, int );

unsigned char inportb( int );
unsigned int inportw( int );
void outportb( int, unsigned char );
void outportw( int, unsigned int );

char getmemb( int, int );
int  getmemw( int, int );
long int getmeml( int, int );
void putmemb( int, int, char );
void putmemw( int, int, int );
void putmeml( int, int, long int );

#endif
