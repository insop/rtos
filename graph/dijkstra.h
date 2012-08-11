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
#include "../graph/bdBuffer.h"

const int	Infinity	= 0xffffff;

//
//	Dijkstra short path
//
class Dijkstra {

    private:
	Graph *pGrp;
	int id;
	int val[NumVertex];

	// bounded buffer >> queue
	BdBuffer queue;
	

    public:
	Dijkstra(Graph *_pGrp);
	void testDijkstra(void);
	void init(int init);
	void printTraverse(void);
	void visitShortPath(int nVtx);
	void shortPath(int start);
};


