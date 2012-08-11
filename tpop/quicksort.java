interface Cmp {
    int cmp(Object x, Objecty);
}


// Icmp: Interger comparison
class Icmp implements Cmp {
    public int cmp(Object o1, Object o2)
    {
	int i1 = ((Integer) o1).intValue();
	int i2 = ((Integer) o2).intValue();
	if (i1 < i2)
	    return -1;
	else if (i1 == i2)
	    return 0;
	else
	    return 1;
    }
}


// Scmp: String comparison
class Scmp implements Cmp {
    public int cmp(Object o1, Object o2)
    {
	String s1 = (String) o1;
	String s2 = (String) o2;
	return s1.compareTo(s2);
    }
}

class Quicksort {

    // Quicksort.sort: quicksort v[left]..v[right]
    static void sort(Object[] v, int left, int right, Cmp cmp)
    {
	int i, last;

	if (left >= right)	// nothing to do
	    return;
	swap(v, left, rand(left, right));	// move pivot elem
	last = left;				//    to v[left]
	for(i = left+1; i <= right; i++) 	// partition
	    if (cmp.cmp(v[i], v[left]) < 0)
		    swap(v, ++last, i);
	swap(v, left, last);			// restore pivot elem
	sort(v, left, last-1, cmp);		// recursively sort
	sort(v, last+1, right, cmp);		//   each part
    }

    // Quicksort.swap: swap v[i] and v[j]
    static void swap(Object[] v, int i, int j)
    {
	Object temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
    }

    static Random rgen = new Random();

    // Quicksort.rand: return random integer in [left, right]
    static int rand(int left, int right)
    {
	return left + Math.abs(rgen.nextInt())%(right-left+1);
    }
}


String[] sarr = new String[n];

// fill n elements of sarr...

Quicksort.sort(sarr, 0, sarr.length-1, new Scmp());


