#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"

#define	SINGLE_SENSOR	0xC1

#define SensorSrv	8
main() {

    int i;
    int tid;
    char ch;
    char setTrain=12;
    char speedy = 0;
    char senCnt = 0;
    char inputted =0;
    char sensorSent = 0;
    char dataSent = 0;
    int x;

    char dataIn = 0;

    int send=13;
    int reply=0;

    console_xy(0,9);
    cprintf("Train Command Srv : ");

    Exit();

    while(1) {

	if( dataIn == 0) {
	    /* if data in not came while sensor working */
	    tid = Receive(&ch,sizeof(char));
	    Reply(tid, NULL,NULL);
	}
	else {
	    /* if data in while sensor working, then reply to him */
	    Reply(tid,NULL,NULL);
	    dataIn = 0;
	}



/* speed and switch */

	if(ch=='A' || ch =='a') { 
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
	    Put( 12 , TRAIN );

	}
	/* EMERGENCY STOP */
	else if (ch=='e' || ch=='E'){
	    Delay(1);
	    Put( 97 , TRAIN );
	}
	/* Release EMERGENCY STOP */
	else if (ch=='r' || ch=='R'){
	    Delay(1);
	    Put( 96 , TRAIN );
	}

	else if(ch==',' || ch=='<') {
	    if(sensorSent == 1) {
		Delay(2);
		sensorSent = 0;
	    }

	    Put( 0x22 , TRAIN );
	    /*Delay(2);*/
	    Put( 6 , TRAIN );
	    Delay(2);
	    Put( 0x20 , TRAIN );
	    dataSent = 1;
	}
	else if(ch=='.' || ch=='>') {
	    if(sensorSent == 1) {
		Delay(2);
		sensorSent = 0;
	    }
	    Put( 0x21 , TRAIN );
	    /*Delay(2);*/
	    Put( 6 , TRAIN );
	    Delay(2);
	    Put( 0x20 , TRAIN );
	    dataSent = 1;
	}
	else if(ch=='m' || ch=='M') {
	    if(sensorSent == 1) {
		Delay(2);
		sensorSent = 0;
	    }
	    Put( 0x21 , TRAIN );
	    /*Delay(2);*/
	    Put( 15 , TRAIN );
	    Delay(2);
	    Put( 0x20 , TRAIN );
	    dataSent = 1;
	}
	else if(ch=='n' || ch=='M') {
	    if(sensorSent == 1) {
		Delay(2);
		sensorSent = 0;
	    }
	    Put( 0x22 , TRAIN );
	    /*Delay(2);*/
	    Put( 15 , TRAIN );
	    Delay(2);
	    Put( 0x20 , TRAIN );
	    dataSent = 1;
	}

/* sensor */
	if( ch == 's') {
	    sensorSent = 1;
	    Put(SINGLE_SENSOR + senCnt, TRAIN);
	    senCnt++;
	    senCnt  %= 5; 
	    /* waiting for sensor acqusition */
	    tid = Receive(&ch,sizeof(char));
	    if( tid == SensorSrv) {
		Reply(tid,NULL,NULL);
	    } else {
		/* if not from sensor srv */
		dataIn = 1;
	    }



	}
	else if( inputted == 1) {

	    Put( speedy , TRAIN );
	    Put( setTrain , TRAIN );

	    dataSent = 1;
	    inputted =0;
	}




    }

}
	    

