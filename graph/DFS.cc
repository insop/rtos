//
//
//	depth-first search 
//	using adjancy list graph representation
//
//
// 	Written by 
//
//	INSOP SONG
//
// 	Systems Design
//
//
// 	University of Waterloo, Canada
//
//	August 2001
//

#include <stdio.h>
#include "../graph/graph.h"
#include "../graph/DFS.h"



DFS::DFS(Graph *_pGrp) 
{

    //printf("DFS Constructor\n");
    numQueue = 0;
    pGrp = _pGrp;

}

// 
//  DFS using stack(non-recursive)
//
void DFS::visit(int nVtx)
{
    Vertex *pVtx;
    EleVertex *pEleVtx;
    int k;

    stack.addItem(nVtx);
    do {
	k = stack.getItem();
	id++;
	val[k] = id;
	pVtx = pGrp->getVertex(k);
	pEleVtx = pVtx->getNextElem();
	//pEleVtx->print();
	while(pEleVtx != NULL) {
	    if ( val[pEleVtx->getNum()] == 0 ) {
		stack.addItem(pEleVtx->getNum());
		val[pEleVtx->getNum()] = -1;
		//printf("enqueue >>>\n");
		//pEleVtx->print();
	    }// if
	    pEleVtx = pEleVtx->getNext();
	} // while
	//printf(">>\n");
    } while(!stack.isEmpty());
}

void DFS::init(void)
{
    id = 0;
    for(int i=0; i < pGrp->getMaxNum(); i++) {
	val[i] = 0;
    }
}

void DFS::search(void)
{
    init();
    visit(1);
    printTraverse();
}

void DFS::visitSP(int nVtx, int nVtx2)
{
    Vertex *pVtx;
    EleVertex *pEleVtx;
    int k;
    char found = 0;

    stack.addItem(nVtx);
    do {
	k = stack.getItem();
	id++;
	val[k] = id;
	pVtx = pGrp->getVertex(k);
	pEleVtx = pVtx->getNextElem();

	printf("\n[%d] : ",pEleVtx->getNum());

	//if ( pEleVtx->getNum() == nVtx2) {
	    //printf("BINGO\n");
	    //break;
	//}

	//pEleVtx->print();
	while(pEleVtx != NULL) {

	    if ( pEleVtx->getNum() == nVtx2) {
		printf("BINGO\n");
		found = 1;
		break;
	    }

	    if ( val[pEleVtx->getNum()] == 0 ) {
		stack.addItem(pEleVtx->getNum());
		val[pEleVtx->getNum()] = -1;
		printf("[%d] > ",pEleVtx->getNum());
		//pEleVtx->print();
	    }// if
	    pEleVtx = pEleVtx->getNext();
	} // while
	//printf(">>\n");
    } while(!stack.isEmpty() && found == 0);

    printf("\n");

#ifdef	RECURSIVE
    Vertex *pVtx;
    EleVertex *pEleVtx;

    id++;
    val[nVtx] = id;
    pVtx = pGrp->getVertex(nVtx);
    pEleVtx = pVtx->getNextElem();

    pEleVtx->print();

    while(pEleVtx != NULL) {
	if ( pEleVtx->getNum() == nVtx2) {
	    printf("BINGO\n");
	    break;
	}
	if ( val[pEleVtx->getNum()] == 0 ) {
	    visitSP( pEleVtx->getNum(), nVtx2 );
	}
	pEleVtx = pEleVtx->getNext();
    }
#endif // RECURSIVE

}

void DFS::shortPath(int start, int end)
{
    init();

    //for(int i = 0; i < pGrp->getMaxNum(); i++) {
	visitSP(start, end);
    //}

    printTraverse();
}

void DFS::printTraverse(void) 
{

    char finish;
    for(int i = 0; i < pGrp->getMaxNum(); i++) {
	finish = 0;
	for(int j = 0; 
	    j < pGrp->getMaxNum() && finish == 0; j++) {
	    if(val[j] == i+1){
		printf("%d : vertex: %d\n", i,j);
		finish = 1;
	    } // if
	} // for j
    } // for i
}

void DFS::testDFS(void) 
{
    pGrp->print(1);
    pGrp->print(58);
}
