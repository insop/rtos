//
//
//	graph main
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
#include "../graph/BFS.h"

int SensorVertex[] = {
    1, 2, 4, 7, 8, 9, 11, 12, 14, 15, 17, 18, 20, 22, 
    24, 25, 27, 28, 29, 31, 32, 34, 36, 37, 40, 41, 42,
    43, 45, 46, 47, 49, 50, 51, 52, 54, 55, 56, 57, 59,
    NIL
};

int SwitchVertex[] = {
    0, 3, 5, 6, 10, 13, 16, 19, 21, 23, 26, 30, 33, 35, 38, 
    39, 44, 48, 53, 58,
    NIL
};
// 
// vertex connect information for
//	adjacency list structure
//
int VerSet[]=
{
     0,47,
     0,58,
     0,59,
     1,2,
     2,3, 
     3,4,
     3,10,
     4,5,
     5,16,
     5,6,
     6,7,
     6,19,
     8,9,
     10,9,
     10,13,
     11,12,
     12,13,
     13,14,
     16,15,
     15,21,
     16,17,
     17,18,
     18,19,
     19,20,
     20,29,
     21,22,
     21,31,
     22,23,
     23,24,
     23,41,
     24,25,
     25,26,
     26,27,
     26,50,
     27,28,
     28,30,
     29,30,
     30,48,
     31,32,
     32,33,
     33,34,
     33,53,
     34,35,
     35,36,
     35,42,
     36,37,
     37,38,
     38,46,
     38,39,
     39,40,
     39,49,
     40,41,
     42,43,
     43,44,
     44,51,
     44,45,
     45,46,
     48,52,
     48,56,
     49,50,
     51,52,
     53,54,
     53,58,
     54,55,
     55,56,
     57,58,
     
     NIL,NIL
};

//
// Graph Object
//
Graph	aGraph;

//
// Graph Search Object
// BreadthFirstSearch
//
BFS	aBFS(&aGraph);

//
// Graph Search Object
// DepthFirstSearch
//
DFS	aDFS(&aGraph);


void initGraph(void)
{
    int i=0;
    while(VerSet[i] != NIL){
	aGraph.setVertex(VerSet[i++], VerSet[i++]);
    }
    i = 0;
    while(SensorVertex[i] != NIL){
	aGraph.setSensor(SensorVertex[i++]);
    }
    i = 0;
    while(SwitchVertex[i] != NIL){
	aGraph.setSwitch(SwitchVertex[i++]);
    }
}

void printGrapth()
{
    for(int i=0; i < aGraph.getMaxNum(); i++) {
	if(aGraph.getNumList(i) != 0)
	    aGraph.print(i);
    }
}

main()
{
    Vertex v1,v2;

    printf("\n\nGraph Main\n");

    initGraph();

    printGrapth();
    //aGraph.printGraph(1);

    //aDFS.testDFS();

    //aDFS.shortPath(1, 7);

    aDFS.search();

}
