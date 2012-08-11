
/* quicksort: sort v[0]..v[n-1] into increasing order */
void quicksort(int v[], int n)
{
    int i, last;

    if(n <= 1) /* nothing to do */
	return;

    swap(v, 0, rand() % n);	/* move pivot elem to v[0] */
    last = 0;
    for(i = 1; i < n; i++)	/* partition */
	if(v[i] < v[0])
	    swap(v, ++last, i);
    swap(v, 0, last);		/* restore pivot */
    quicksort(v, last);		/* recursively sort */
    quicksort(v+last+1, n-last-1); /*	  each part */
}

/* swap:  interchange v[i] and v[j] */
void swap(int v[], int i, int j)
{
    int temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}


/* compare function for qsort library function */

/* scmp: string compare of *p1 and *p2 */
int scmp(const void *p1, const void *p2)
{
    char *v1, *v2;

    v1 = *(char **) p1;
    v2 = *(char **) p2;
    return strcmp(v1, v2);
}

/* icmp: integer compare of *p1 and *p2 */
int icmp(const void *p1, const void *p2)
{
    int v1, v2;

    v1 = *(int *) p1;
    v2 = *(int *) p2;
    if(v1 < v2)
	return -1;
    else if(v1 == v2)
	return 0;
    else 
	return 1;
}



main() {

    char *str[N];

    qsort(str, N, sizeof(str[0]), scmp);


    int arr[N];
    qsort(arr, N, sizeof(arr[0]), icmp);

    // if you use c++ use sort O(n log n)
    int arr[N];
    sort(arr, arr+N);


}




