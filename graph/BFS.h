//
//
//	breadth-first search 
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

#ifndef	_BFS_H_
#define	_BDFS_H_

#include <stdio.h>
#include "../graph/graph.h"
#include "../graph/bdBuffer.h"

//
//	Depth First Search 
//
class BFS {

    private:
	Graph *pGrp;
	int id;
	int val[NumVertex];

	// bounded buffer >> queue
	BdBuffer queue;
	

    public:
	BFS(Graph *_pGrp);
	void testBFS(void);
	void visit(int nVtx);
	void init(void);
	void search(void);
	void printTraverse(void);
	void visitShortPath(int nVtx);
	void shortPath(void);
};


#endif	//_BFS_H_
