/* 
 * fm.c
 * 
 * 	This stuff controls the "FM" part of the soundblaster.
 * 
 */

/* these are the address/status ports: add one for data port */
#define FM_LEFT_PORT			0x0220
#define FM_RIGHT_PORT			0x0222
#define FM_BOTH_PORT			0x0388

/* from docs:

   20..35       Amp Mod / Vibrato / EG type / Key Scaling / Multiple
   40..55       Key scaling level / Operator output level
   60..75       Attack Rate / Decay Rate
   80..95       Sustain Level / Release Rate
   E0..F5       Wave Select

	Use the function, fmSet() to change these values, for a given 
	channel and operator.
*/

#define FM_CONFIG_AM		0x20
#define FM_CONFIG_SCALE		0x40
#define FM_CONFIG_ATTACK	0x60
#define FM_CONFIG_SUSTAIN	0x80
#define FM_CONFIG_WAVE		0xe0

/* To access these, use functions...

   A0..A8       Frequency (low 8 bits)
   B0..B8       Key On / Octave / Frequency (high 2 bits)
		fmStartNote()
		fmStopNote()

   C0..C8       Feedback strength / Connection type
		fmFeedbackAlg()

	 BD         AM depth / Vibrato depth / Rhythm control
		fmRythme()
*/

/* Valid values for operator number */
#define FM_OPER1			1
#define FM_OPER2			2

/* Channels are numbered from 0..8 inclusive, for a total of 9. Topmost
 * 5 channels may control rhythm section, if enabled.
 */
#define FM_NUM_CHAN			9

/*
 * Here are the notes. You can do much more than just this, of course.
 *
 */
#define FM_NOTE_Cs		0x016B		/*   277.2       C#	*/
#define FM_NOTE_D		0x0181		/*   293.7       D	*/
#define FM_NOTE_Ds		0x0198		/*   311.1       D#	*/
#define FM_NOTE_E		0x01B0		/*   329.6       E	*/
#define FM_NOTE_F		0x01CA		/*   349.2       F	*/
#define FM_NOTE_Fs		0x01E5		/*   370.0       F#	*/
#define FM_NOTE_G		0x0202		/*   392.0       G	*/
#define FM_NOTE_Gs		0x0220		/*   415.3       G#	*/
#define FM_NOTE_A		0x0241		/*   440.0       A	*/
#define FM_NOTE_As		0x0263		/*   466.2       A#	*/
#define FM_NOTE_B		0x0287		/*   493.9       B	*/
#define FM_NOTE_C		0x02AE		/*   523.3       C	*/

/* Octaves range from 0 to 7 */
#define FM_MIN_OCTAVE		0
#define FM_MID_OCTAVE		4
#define FM_MAX_OCTAVE		7

/*** Function prototypes ***/

u_char fmRead(uint port);
void fmWrite(uint port, u_char reg, u_char data);
void fmReset();
void fmSet(u_short port, u_char cmd, u_char chan, u_char oper, u_char value);
void fmStartNote(u_short port, int chan, u_short fnumber, u_char oct);
void fmStopNote(u_short port, int chan);
void fmRhythm(u_short port, u_char value);
void fmFeedbackAlg(u_short port, u_char chan, u_char value);
void fmSimpleSetup(u_short port, u_char chan);
void fmSimple();
