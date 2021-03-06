#define		DEBUG

#include	"bindfile.h"
#include	"aout.h"
#include	"gdt.h"


extern "C" {
    #include	"../include/i486/bind486.h"
    #include	"../include/i486/cs452.h"
}

extern "C" {
    initGDT();
    getCodeSelector(int);
    getDataSelector(int);
}

main(uint bindTable,int numFiles ) {

    Bindfile	aBindfile( bindTable, numFiles );
    Aout	aAoutHeader;
    //GDT		aGDT;
    initGDT();
    getchar();


    initDebug(ErrorDebug);

    //aBindfile.printBindfile();
    //aAoutHeader.printAoutHeader(&aBindfile);
    getchar();
    aBindfile.findModule("Kernel");

    printf("\nKernel's code selector 0x%x\n",
	getCodeSelector(0));
    printf("Kernel's data selector 0x%x\n",
	getDataSelector(0));

    getchar();

    aBindfile.printBindfile();
    aAoutHeader.printAoutHeader(&aBindfile);



}
