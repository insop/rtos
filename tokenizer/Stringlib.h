#ifndef _STRING_LIBRARY_H_
#define _STRING_LIBRARY_H_

/*****************************************************************
 *
 * name : Stringlib.h
 *
 * authors : Insop Song & Michael Cole
 *
 * functions : Supports the scanner, that is, will read data typed
 *             by the user and convert data into meaningful format.
 *             This provides an interface to a stripped down string
 *             library.
 *
 * notes : All strings must take the form of: c*\0, that is some
 *         sequence of characters terminated by the \0 char.
 *         Also all strings must be of length no more than STRLEN chars,
 *         or (STRLEN - 1) chars followed by a \0.
 *
 * Stringlib.h file includes the following routines :
 *
 *  int len(char * str)
 *  int StrCmp(char * strA, char * strB)
 *  int TruncStrCmp(int n, char * strA, char * strB)
 *  int DubTruncStrCmp(int m, int n, char * strA, char * strB)
 *  void StringCpy(char * strA, char * strB)
 *
 * date : 22 July 2001
 * I can't wait to see Lei
 *****************************************************************/

#define STRLEN      23          /* Max size of any string supported */
#define OVERRUN     (-5)        /* Value all functions, will return if
                                 * result of performing operation will
                                 * cause function to Seg fault.
                                 */
#define MISSION     0           /* Statement "mission" found in the input string */
#define RANDOM      1           /* Statement "random" found in the input string */
#define INIT        2           /* Statement "init" found in the input string */
#define NOOFABOVE   3           /* None of the above found in input string */

int len(char * str);
  /* Return the number of bytes in str, less \0. Thus the string:
   * "train 12 -s b1 -e a1" would be of size 20, whereas string:
   * "train" would be of size 5.
   */

int StrCmp(char * strA, char * strB);
  /* Return 1 if and only if, for all i <= max( len(strA), len(strB) )
   * strA[i] == strB[i].
   * Else return 0.
   */

int TruncStrCmp(int n, char * strA, char * strB);
  /* Return 1 if and only if, for all i <= n, strA[i] == strB[i]
   * Else return 0.
   */

int DubTruncStrCmp(int m, int n, char * strA, char * strB);
  /* Return 1 if and only if, for all m <= i <= n, j = (i - m),
   * strA[i] == strB[j]
   * Else return 0.
   */

int FindFirst(char * str, char symtofind);
  /* Return the location of the first symtofind in str */

void StringCpy(char * strA, char * strB);
  /* For all i < min( STRLEN, (len(strB) + 1) ) copy strB[i] -> strA[i] */

#endif
