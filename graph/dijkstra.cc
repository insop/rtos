//
//
//	dijkstra shortpath
//
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
#include "../graph/dijkstra.h"


Dijkstra::Dijkstra(Graph *_pGrp) 
{
    printf("Dijkstra Constructor\n");
    pGrp = _pGrp;

}

void Dijkstra::visitShortPath(int nVtx)
{

    Vertex *pVtx;
    EleVertex *pEleVtx;
    int k;

    queue.addItem(nVtx);
    do {
	k = queue.getItem();
	id++;
	val[k] = id;
	pVtx = pGrp->getVertex(k);
	pEleVtx = pVtx->getNextElem();
	//pEleVtx->print();
	while(pEleVtx != NULL) {
	    if ( val[pEleVtx->getNum()] == 0 ) {
		queue.addItem(pEleVtx->getNum());
		val[pEleVtx->getNum()] = -1;

		//printf("enqueue >>>\n");
		//pEleVtx->print();
	    }// if
	    pEleVtx = pEleVtx->getNext();
	} // while
	//printf(">>\n");
    } while(!queue.isEmpty());
}

void Dijkstra::shortPath(int start)
{

    init(start);
    visitShortPath(start);
}

void Dijkstra::init(int init)
{
    for(int i=0; i < pGrp->getMaxNum(); i++) {
	if(i != init) 
	    val[i] = Infinity;
	else 
	    val[i] = 0;
    }
}


//
// display traverse order
//
void Dijkstra::printTraverse(void) 
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

void Dijkstra::testDijkstra(void) 
{
    pGrp->print(1);
    pGrp->print(58);
}
