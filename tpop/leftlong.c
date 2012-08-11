/* Copyright (C) 1999 Lucent Technologies */
/* Excerpted from 'The Practice of Programming' */
/* by Brian W. Kernighan and Rob Pike */

/* matchstar: leftmost longest search for c*regexp */
int matchstar(int c, char *regexp, char *text)
{
	char *t;

	for (t = text; *t != '\0' && (*t == c || c == '.'); t++)
		;
	do {	/* * matches zero or more */
		if (matchhere(regexp, t))
			return 1;
	} while (t-- > text);
	return 0;
}
