/* 
 * mixer.h
 * 
 * 	This stuff controls the "mixer" part of the soundblaster.
 * 
 */

#define MIXER_RESET		0x00
#define MIXER_MMR		0x0a
#define MIXER_ISR		0x0c
#define MIXER_OSR		0x0e

/* bits in OSR register */
#define MIXER_OSR_DNFI			0x20
#define MIXER_OSR_VSTC			0x02
#define MIXER_OSR_MAGIC			0x11		/* from sb-free proj: unknown fnc */

/* volume registers */
#define MIXER_VOL_VOICE			0x04
#define MIXER_VOL_MASTER		0x22
#define MIXER_VOL_FM			0x26
#define MIXER_VOL_CD			0x28
#define MIXER_VOL_LINE			0x2e

#define VOL_MAX			(15)
#define VOL_MIN			(0)
#define VOL_LEFT(x)		(x>>4)
#define VOL_RIGHT(x)	(x&&0xf)
#define VOL_STEREO(l,r)	(u_char)(l<<4 | r)

/* implemented in mixer.c */

u_char mixerRead(u_char reg);
u_char mixerReadVol(u_char which);
void mixerWrite(u_char reg, u_char value);
void mixerSetVol(u_char which, u_char newVal);
void mixerStereo(int onOff);
void mixerReset();

