#include "../syscall/sysCall.h"
#include "../include/i486/cs452.h"


int StartPos = 131;
int IndexStar = 0;
unsigned int IdleCnt = 0L;

char *StarShap[4]={"/","-","\\","|"};

#define STAT_TID	3
void star() {
    
    console_xy(StartPos,0);
    cprintf(StarShap[IndexStar]);
    IndexStar++;
    if(IndexStar == 4)
	IndexStar = 0;
}

void main( void ) 
{
    int i=0;
    int * pIdleCnt = &IdleCnt;

    Send(STAT_TID, &pIdleCnt, sizeof(int), NULL,NULL); 

    /*cprintf("___IDLE TASK___\n"); */

    IdleCnt =0;
    while(1){
	IdleCnt++;
    }

    while(1){
	star();
    }
}
