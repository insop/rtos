
// for check number of array elements
#define	NELEMS(array)	(sizeof(array) / sizeof(array[0]))

// double dbuf[100];
// for(i = 0; i < NELEMS(dbuf); i++)
// ...



typedef struct Nameval Nameval;

struct Nameval {
    char	*name;
    int		value;
};

/* HTML characters, e.g. AElig is ligature of A and E. */
/* values are Unicode/ISO10646 dncoding */

Nameval htmlchars[] = {
    "AElig", 0x00c6,
    "Aacute", 0x00c1,
    /* ... */
    "zeta", 0x03b6,
};



    printf("The HTML table has %d words\n", NELEMS(htmlchars));

/* lookup: binary search for name in tab; return index */
int lookup(char *name, Nameval tab[], int ntab)
{
    int low, high, mid, cmp;

    low = 0;
    high = ntab - 1;
    while (low <= high) {
	mid = (low + high) / 2;
	cmp = strcmp(name, tab[mid].name);
	if(cmp < 0)
	    high = mid - 1;
	else if (cmp > 0)
	    low = mid + 1;
	else	/* found match */
	    return mid;
    }
    return -1;	/* no match */
}


/* lookup: use bsearch to find name in tab, return index */
int lookup(char *name, Nameval tab[], int ntab)
{
    Nameval key, *np;

    key.name = name;
    key.value = 0;  /* unused; anything will do */
    np = (Nameval *) bsearch(&key, tab, ntab,
			sizeof(tab[0]), nvcmp);
    if(np == NULL)
	return -1;
    else 
	return np-tab;
}

/* nvcmp: compare two Nameval names */
int nvcmp(const void *va, const void *vb)
{
    const Nameval *a, *b;

    a = (Nameval *) va;
    b = (Nameval *) vb;
    return strcmp(a->name, b->name);
}

main()
{

    half = lookup("frac12", htmlchars, NELEMS(htmlchars));
}
