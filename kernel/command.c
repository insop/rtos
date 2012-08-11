#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"

main() {

    int i;
    int tid;
    char ch;
    char setTrain=12;
    char speedy = 0;
    char inputted =0;
    int x;

    int send=13;
    int reply=0;

    console_xy(0,9);
    cprintf("Train Command Srv : ");

    while(1) {
	tid = Receive(&ch,sizeof(char));
	Reply(tid, NULL,NULL);

/* sensor */
	if( ch == 's') {
	    Delay(1);
	    Put(0x85, TRAIN);
	    Delay(1);
	}

/* speed and switch */

	else if(ch=='A' || ch =='a') { 
	    setTrain = 12;
	    inputted =1;
	}
	else if(ch=='B' || ch=='b'){
	    setTrain = 55;
	    inputted =1;
	}
	else if(ch=='C' || ch=='c'){
	    setTrain = 57;
	    inputted =1;
	}
	else if(ch=='1'){
	    speedy = 1;
	    inputted =1;
	}
	else if(ch=='2'){
	    speedy = 2;
	    inputted =1;
	}
	else if(ch=='3'){
	    speedy = 3;
	    inputted =1;
	}
	else if(ch=='4'){
	    speedy = 4;
	    inputted =1;
	}
	else if(ch=='5'){
	    speedy = 5;
	    inputted =1;
	}
	else if(ch=='6'){
	    speedy = 6;
	    inputted =1;
	}
	else if(ch=='7'){
	    speedy = 7;
	    inputted =1;
	}
	else if(ch=='8'){
	    speedy = 8;
	    inputted =1;
	}
	else if(ch=='9'){
	    speedy = 9;
	    inputted =1;
	}
	else if(ch=='0'){
	    speedy = 0;
	    inputted =1;
	}
	else if(ch=='-' || ch=='_'){
	    speedy = 10;
	    inputted =1;
	}
	else if(ch=='='|| ch=='+'){
	    speedy = 11;
	    inputted =1;
	}
	else if(ch==']'|| ch=='}'){
	    speedy = 14;
	    inputted =1;
	}
	else if(ch=='q' || ch=='Q'){
	    Put( 0, TRAIN );
	    Delay(1);
	    Put( 12 , TRAIN );
	    Delay(1);

/*
	    Put( 15, TRAIN );
	    Delay(1);
	    Put( 12 , TRAIN );
	    Delay(1);

	    Put( 14, TRAIN );
	    Delay(1);
	    Put( 12 , TRAIN );
	    Delay(1);

	    Put( 0, TRAIN );
	    Delay(1);
	    Put( 12 , TRAIN );
	    Delay(1);

	    Put( 15, TRAIN );
	    Delay(1);
	    Put( 12 , TRAIN );
	    Delay(1);

	    Put( 0, TRAIN );
	    Delay(1);
	    Put( 12 , TRAIN );
	    Delay(1);
	    */
	    /*
	    Delay(1);
	    Put( 0 , TRAIN );
	    Delay(1);
	    Put( 55 , TRAIN );
	    Delay(1);
	    Put( 0 , TRAIN );
	    Delay(1);
	    Put( 57 , TRAIN );
	    */
	}
	/* EMERGENCY STOP */
	else if (ch=='e' || ch=='E'){
	    Put( 97 , TRAIN );
	}
	/* Release EMERGENCY STOP */
	else if (ch=='r' || ch=='R'){
	    Put( 96 , TRAIN );
	}

	else if(ch==',' || ch=='<') {
	    Put( 0x22 , TRAIN );
	    Delay(2);
	    Put( 6 , TRAIN );
	    Delay(2);
	    Put( 0x20 , TRAIN );
	    Delay(2);
	}
	else if(ch=='.' || ch=='>') {
	    Put( 0x21 , TRAIN );
	    Delay(2);
	    Put( 6 , TRAIN );
	    Delay(2);
	    Put( 0x20 , TRAIN );
	    Delay(2);
	}
	else if(ch=='m' || ch=='M') {
	    Put( 0x21 , TRAIN );
	    Delay(2);
	    Put( 15 , TRAIN );
	    Delay(2);
	    Put( 0x20 , TRAIN );
	}
	else if(ch=='n' || ch=='M') {
	    Put( 0x22 , TRAIN );
	    Delay(2);
	    Put( 15 , TRAIN );
	    Delay(2);
	    Put( 0x20 , TRAIN );
	}
	if(inputted ==1) {
	
	    Put( speedy , TRAIN );
	    Delay(1);
	    Put( setTrain , TRAIN );
	    inputted =0;
	}



    }

}
	    

