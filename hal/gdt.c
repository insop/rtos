#include 	"gdt.h"

int freeGDT = 8;	
/* first free entry of the GDT */

struct segment_descriptor SegDesc[NumSelectors];
struct region_descriptor	regDesc;

/*
//	read GDT table 
// 	and then copy to the kernel space
//	for the size of mazimum task number
*/


void initGDT( void ) {

    struct region_descriptor * pRegDesc=&regDesc;
    int baseS;

    sgdt( (void*) &regDesc );
    absRead( regDesc.rd_base, SegDesc, regDesc.rd_limit);

    regDesc.rd_limit = 8 * NumSelectors - 1;
    regDesc.rd_base = baseDS;
    regDesc.rd_base += (int) SegDesc;
    lgdt( (void*) &regDesc );
}

/*
//	return code selector 
//	ex> kernel tid is 0 and code selector is 0x30
*/
int getCodeSelector( int tid ) {

    int selNum;

    selNum = 8 * ( 2 * ( tid + 3 ) );
    if( selNum >= 8 * NumSelectors ) {
	    Error1( ErrorWarning, 
		"tid[0x%x] gdt code selector over the range", tid );
    } 

    return selNum;
}

/*
//	return data selector 
//	ex> kernel tid is 0 and data selector is 0x38
*/
int getDataSelector( int tid ) {

    int selNum;

    selNum = 8 * ( 2 * ( tid + 3 ) + 1 );
    if( selNum >= 8 * NumSelectors ) {
	    Error1( ErrorWarning, 
		"tid[0x%x] gdt data selector over the range", tid );
    } 

    return selNum;
}


/* 
// return real addreas
*/
int getBaseS( int i ) {
    return(SegDesc[i].sd_hibase<<24)|(SegDesc[i].sd_lobase);
}

int setGDTVal(unsigned long limit, unsigned long base, int type) {
    /* Set SegDesc[entry] to have limit base and type as listed in the calling
    * parameters. Return the GDT index number of the GDT entry modified.
    */

    SegDesc[freeGDT].sd_lolimit = limit&0xffff;
    SegDesc[freeGDT].sd_lobase = base&0xffffff;
    SegDesc[freeGDT].sd_type = type;
    /* Suggested value in the course notes */
    SegDesc[freeGDT].sd_dpl = 0;            
    /* Suggested value in the course notes */
    SegDesc[freeGDT].sd_p = 1;              
    SegDesc[freeGDT].sd_hilimit = (limit >> 16)&0xf;
    /* Suggested value in the course notes */
    SegDesc[freeGDT].sd_def32 = 1;          
    /* Don't need paging - so gran is by byte */
    SegDesc[freeGDT].sd_gran = 0;           
    SegDesc[freeGDT].sd_hibase = (base >> 24)&0xff;

    freeGDT++;
    return (freeGDT - 1);
}


