//
//
//	graph class	
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


char *strType[] = {"SENsor", "SWitch"};

EleVertex::EleVertex() {

    nVertex 	= 0;
    next 	= NULL;

}

void EleVertex::print(){
    printf("eleVtx num %d:\n", nVertex);
}

Vertex::Vertex() 
{
    nVertex 	= 0;
    type 	= (VertexType) NULL;
    nType 	= NULL;
    pEleVertex 	= NULL;
    nEleVertex	= 0;
}

void Vertex::addElem(EleVertex *e)
{
    e->setNext(pEleVertex);
    pEleVertex = e;
    nEleVertex++;
}

void Vertex::print() 
{
    EleVertex *nextEle = pEleVertex;

    //printf(
	//"[Vertex number %d, type %s, type number %d element number %d]\n",
	//nVertex, strType[type], nType, nEleVertex);
    printf("%s [%02d] : ", strType[type], nVertex);
    for(int i = 0; i < nEleVertex; i++) {
	printf("->[%02d]", nextEle->getNum());
	nextEle = nextEle->getNext();
    }
    printf("\n");
}

Edge::Edge() 
{

    //for(int i = 0; i < NumVertex; i++) 
	//vertex

}


Graph::Graph() 
{

    for(int i=0; i < NumVertex; i++)
	vertex[i].setNum(i);

    nElem = 0;

}

EleVertex *Graph::getNextElem() 
{
    if( nElem < NumEleVertex)
	return &eleVertex[nElem++];
    else
	return NULL;
}

Vertex *Graph::getVertex(int n)
{
    return &vertex[n];
}

void Graph::setSensor(int v) 
{
    vertex[v].setSensor();
}
void Graph::setSwitch(int v)
{
    vertex[v].setSwitch();
}
void Graph::setVertex(int v1, int v2)
{
    EleVertex *e1;
    EleVertex *e2;

    e1 = getNextElem();
    if(e1 == NULL) {
	printf("WARNING! NO element left \n");
    }
    e2 = getNextElem();
    if(e1 == NULL) {
	printf("WARNING! NO element left \n");
    }

    e1->setNum(v1);
    e2->setNum(v2);

    vertex[v1].addElem(e2);
    vertex[v2].addElem(e1);


}

void Graph::print(int nVertex)
{
    vertex[nVertex].print();
}

int Graph::getNumList(int nVertex) {
    return vertex[nVertex].getNumElem();
}
