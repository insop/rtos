
typedef struct Nameval Nameval;
struct Nameval {
    char   	*name;
    int	   	value;
    Nameval	*left;	/* lesser */
    Nameval	*right;	/* greater */
};

/* insert: insert newp in treep, return treep */
Nameval *insert(Nameval *treep, Nameval *newp)
{
    int cmp;

    if (treep == NULL)
	return newp;
    cmp = strcmp(newp->name, treep->name);
    if (cmp == 0)
	weprintf("insert: duplicate entry %s ignored",
	    newp->name);
    else if (cmp < 0)
	treep->left = insert(treep->left, newp);
    else 
	treep->right = insert(treep->right, newp);
    return treep;
}

/* lookup: look up name in tree treep */
Nameval *lookup(Nameval *treep, char *name)
{
    int cmp;

    if (treep == NULL)
	return NULL;
    cmp = strcmp(newp->name, treep->name);
    if (cmp == 0)
	return treep;
    else if (cmp < 0)
	treep->left = insert(treep->left, newp);
    else 
	treep->right = insert(treep->right, newp);
}

/* nlookup: non-recursively look up name in tree treep */
Nameval *nlookup(Nameval *treep, char *name)
{
    int cmp;

    while (treep != NULL) {
	cmp = strcmp(newp->name, treep->name);
	if (cmp == 0)
	    return treep;
	else if (cmp < 0)
	    tree = treep->left;
	else 
	    tree = treep->right;
    }
    return NULL;
}


/* applyinorder: inorder application of fn to treep */
void applyinorder(Nameval *treep,
	    void (*fn)(Nameval*, void*), void *arg)
{
    if (treep == NULL)
	return;
    applyinorder(treep->left, fn, arg);
    (*fn)(treep, arg);
    applyiinorder(treep->right, fn, arg);
}

// applyinorder(treep, printnv, "%s: %x\n");

/* applypostorder: postorder application of fn to treep */
void applypostorder(Nameval *treep,
	    void (*fn)(Nameval*, void*), void *arg)
{
    if (treep == NULL)
	return;
    applyinorder(treep->left, fn, arg);
    applyiinorder(treep->right, fn, arg);
    (*fn)(treep, arg);
}


// postorder is useful for parsing
