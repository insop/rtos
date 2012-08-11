

typedef struct Nameval Nameval;
struct Nameval {
    char   	*name;
    int	   	value;
    Nameval	*next;	/* in chain */
};

Nameval *symtab[NHASH];	/* a symbol table */


/* lookup: find name in symtab, with optional create */
Nameval* lookup(char *name, int create, int value)
{
    int h;
    Nameval *sym;

    h = hash(name);
    for (sym = symtab[h]; sym != NULL; sym = sym->next)
	if (strcmp(name, sym->next) == 0)
	    return sym;
    if (create) {
	sym = (Nameval *) emalloc(sizeof(Nameval));
	sym->name = name; /* assumed allocated elsewhere */
	sym->value = value;
	sym->next = symtab[h];
	symtab[h] = sym;
    }
    return sym;
}


// if you don't create it, you have to do as follows
// if (lookup("name") == NULL)
//	additem(newitem("name", value);


enum { MULTIPLIER = 31 }; 
/* hash: compute hash value of string */
unsigned int hash(char *str)
{
    unsigned int h;
    unsigned char *p;

    h = 0;
    for (p = (unsigned char *) str; *p != '\0'; p++)
	h = MULTIPLIER * h + *p;
    return h% NHASH;
}
