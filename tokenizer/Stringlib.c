#include "Stringlib.h"

int len(char * str) {
  /* Return the number of bytes in str, less \0. Thus the string:
   * "train 12 -s b1 -e a1" would be of size 20, whereas string:
   * "train" would be of size 5.
   */
  int i;                        /* Loop index */
  for (i = 0; i < STRLEN; i++) {
    if (str[i] == '\0') {
      return i;
    }
  }
  return OVERRUN;
}

int StrCmp(char * strA, char * strB) {
  /* Return 1 if and only if, for all i <= max( len(strA), len(strB) )
   * strA[i] == strB[i].
   * Else return 0.
   */
  int Len;                      /* length of the two strings */
  int i;                        /* loop index */

  Len = len(strA);
  if (Len != len(strB)) {
    return 0;
  }

  for (i = 0; i <= Len; i++) {
    if (strA[i] != strB[i]) {
      return 0;
    }
  }
  return 1;

}
int TruncStrCmp(int n, char * strA, char * strB) {
  /* Return 1 if and only if, for all i <= n, strA[i] == strB[i]
   * Else return 0.
   */
  int i;                        /* loop index */
  int LenA;                     /* length of string A */
  int LenB;                     /* length of string B */

  LenA = len(strA);
  LenB = len(strB);

  if (LenA < n) return OVERRUN;
  if (LenB < n) return OVERRUN;

  for (i = 0; i < n; i++) {
    if (strA[i] != strB[i]) {
      return 0;
    }
  }
  return 1;

}
int DubTruncStrCmp(int m, int n, char * strA, char * strB) {
  /* Return 1 if and only if, for all m <= i <= n, j = (i - m),
   * strA[i] == strB[j]
   * Else return 0.
   */
  int i;                        /* loop index */
  int LenA;                     /* length of string A */
  int LenB;                     /* length of string B */

  LenA = len(strA);
  LenB = len(strB);

  if (n < m) return OVERRUN;
  if (LenA < n) return OVERRUN;
  if (LenB < (n - m)) return OVERRUN;

  for (i = (m - 1); i < n; i++) {
    if (strA[i] != strB[(i - (m - 1))]) {
      return 0;
    }
  }
  return 1;
}

int FindFirst(char * str, char symtofind) {
  /* Return the location of the first symtofind in str */
  int i;                        /* loop index */

  for (i = 0; i < STRLEN; i++) {
    if (str[i] == symtofind) {
      return (i + 1);
    }
  }
  return OVERRUN;
}

void StringCpy(char * strA, char * strB) {
  /* For all i < min( STRLEN, (len(strB) + 1) ) copy strB[i] -> strA[i] */
  int i;                        /* loop index */

  for (i = 0; i < STRLEN; i++) {
    strA[i] = strB[i];
    if (strB[i] == '\0') break;
  }
}
