#include <stdio.h>
#include "stack.h"

main() {

    Stack buffer;
    char i = 0;
    int j = 0;
    char test;

    printf("BdBuffer testing num%d :Left %d : isEnough %d \n",
    buffer.getNumItem(), buffer.getNumLeft(), buffer.isEnoughSpace(1) );

    while( !buffer.isFull() ) {
	i %= 100;
	buffer.addItem( i );
	i++; 
    }

    printf("BdBuffer testing num%d :Left %d : isEnough %d \n",
    buffer.getNumItem(), buffer.getNumLeft(), buffer.isEnoughSpace(1) );
    getchar();

    for(j = 0; j < 500; j++) {
	i %= 100;
	if(!buffer.isFull()) {
	    buffer.addItem( i );
	}
	if(!buffer.isEmpty()) {
	    test = buffer.getItem(  );
	}
	if(!buffer.isEmpty()) {
	    test = buffer.getItem(  );
	}
	if(!buffer.isEmpty()) {
	    test = buffer.getItem(  );
	}
	if(!buffer.isEmpty()) {
	    test = buffer.getItem(  );
	}
	i++;
    }

    printf("BdBuffer testing num%d :Left %d : isEnough %d \n\n",
    buffer.getNumItem(), buffer.getNumLeft(), buffer.isEnoughSpace(1) );
    getchar();
    while( !buffer.isEmpty() ) {
	test  = buffer.getItem(  );
	//printf("get %d\n", test);
    }

    printf("BdBuffer testing num%d :Left %d : isEnough %d \n",
    buffer.getNumItem(), buffer.getNumLeft(), buffer.isEnoughSpace(1) );
    getchar();

    for(j = 0; j < 500; j++) {
	i %= 100;
	if(!buffer.isFull()) {
	    buffer.addItem( i );
	}
	if(!buffer.isFull()) {
	    buffer.addItem( i );
	}
	if(!buffer.isFull()) {
	    buffer.addItem( i );
	}
	if(!buffer.isEmpty()) {
	    test = buffer.getItem(  );
	    printf(">>get %d\n", test);
	}
	i++;
    }
    printf("BdBuffer testing num%d :Left %d : isEnough %d \n",
    buffer.getNumItem(), buffer.getNumLeft(), buffer.isEnoughSpace(1) );

    

}
