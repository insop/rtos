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


#ifndef	_DFS_H_
#define	_DFS_H_

#undef RECURSIVE

#include <stdio.h>
#include "../graph/graph.h"
#include "../graph/stack.h"
#include "../graph/bdBuffer.h"

const int NumQueue = 5;

//
//	Depth First Search 
//
class DFS {

    private:
	Graph *pGrp;
	int id;
	int numQueue;
	int val[NumVertex];
	Stack	stack;
	BdBuffer queue[NumQueue];
	

    public:
	DFS(Graph *_pGrp);
	void testDFS(void);
	void visit(int nVtx);
	void init(void);
	void search(void);
	void visitSP(int start, int end);
	void shortPath(int start, int end);
	void printTraverse(void);
};


#endif	//_DFS_H_
