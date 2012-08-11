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
//


//	
//
//	[Graph]	: this has All(Vertex, EleVertex, Edge)
//
//	[Edge] : this has weight edge information
//
//	[Vertext] : this Vertext has all information about node
//	|		
//	[EleVertex] : just pointing element
//	|
//	[EleVertex]
//
//

#include <stdio.h>

#ifndef	_GRPAH_H_
#define	_GRPAH_H_


const int	NumVertex	= 60;
const int	NumEdge		= (NumVertex - 1);
const int	NumEleVertex	= 500;
const int	NIL		= -1;

enum	VertexType { SensorType, SwitchType };
typedef VertexType VertexType;


//
//	Vertex node for list
//	element of vertex list
//
class	EleVertex {

    private:
	int	nVertex;
	EleVertex *next;

    public:
	EleVertex();
	EleVertex *getNext(){ return next;}
	int 	getNum(void) { return nVertex; }
	void	setNum(int n) { nVertex = n;}
	void 	setNext(EleVertex *n) { next = n;}
	void 	print();


	
};

//
//	Vertex Class
//	has vertex(node) information 
//	such as number and type and so on.
//
class	Vertex {
    
    private:
	int		nVertex;
	VertexType	type;
	// number of type according to sensor or switch
	int		nType;

	EleVertex 	*pEleVertex;
	int		nEleVertex;
	

    public:
	Vertex();
	int getNum(void) { return nVertex;}
	int getNumElem(void) { return nEleVertex;}
	EleVertex *getNextElem(void) { return pEleVertex;}
	void setNum(int n) { nVertex = n;}
	void addElem(EleVertex *e);
	void print();
	void setSensor() { type = SensorType;}
	void setSwitch() { type = SwitchType;}
};

//
//	Edge class 
//	has distance information
//
class	Edge {
    private:

    public:
	Edge();

};

//
//	Graph class
// 	has vertexes and edges
//
class	Graph {

    private:
	Vertex	   vertex[NumVertex];
	EleVertex  eleVertex[NumEleVertex];	
	int	   nElem;
	Edge	   edge[NumEdge];

    public:
	Graph();
	//void setVertex(Vertex *v1, Vertex *v2);
	void setVertex(int v1, int v2);
	void setSensor(int v);
	void setSwitch(int v);
	int getMaxNum(void) { return NumVertex;}
	Vertex *getVertex(int n);

	EleVertex *getNextElem();
	
	void print(int nVertex);
	int getNumList(int nVertex);

};


#endif	// _GRPAH_H_

