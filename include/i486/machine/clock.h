/* things to do with keep time on a PC/AT class machine */
#include <cs452.h>
#include <machine/pio.h>

#define CMOS_ADDR_PORT		0x070
#define CMOS_DATA_PORT		0x071

inline uint readCmos(uint offset)
{
	/* must output address first to port 0x070, then read data on
	 * port 0x071... Don't do one without the other!
	 */

	outb(CMOS_ADDR_PORT, offset & 0x07f);
	return inb(CMOS_DATA_PORT);
}

inline void writeCmos(uint offset, uint val)
{
	/* must output address first to port 0x070, then write data on
	 * port 0x071... Don't do one without the other!
	 */

	outb(CMOS_ADDR_PORT, offset & 0x07f);
	outb(CMOS_DATA_PORT, val);
}

#define CMOS_SECOND			0
#define CMOS_MINUTE			2
#define CMOS_HOUR			4
#define CMOS_WEEKDAY		6
#define CMOS_MONTHDAY		7
#define CMOS_MONTH			8
#define CMOS_YEAR			9			/* 0 -> 99 */

inline uint readCmosBcd(uint offset)
{
	/* read a value from the CMOS and convert the BCD value received
	 * into binary
	 */
	uint val = readCmos(offset);
	return (val & 0x0f) + ((val >> 4)*10);
}

/*
 *
 * Programmable Interval Timer: Intel 8253 chip
 *
 */

#define PIT_COUNT0		0x040			/* usable counter */
#define PIT_COUNT1		0x041			/* RAM refresh rate */
#define PIT_COUNT2		0x042			/* output connected to speaker */
#define PIT_CONTROL		0x043			/* write only */

inline void pitSetup() {
	/* no control over rate, just "read" mode: set it to read the
	 * high 8 bits of the counter0, when you read counter0...
	 */

	outb(PIT_CONTROL, 0x022);		/* select counter zero, rate div. */
}
