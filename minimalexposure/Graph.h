// Graph Header File ///////////////////////////////////////////////////////////
//
// Seapahn Meguerdichian
// May 10, 2000
// Combined linked list, queue, and stack into one class
// Then removed list completely due to Template problems
// Optimized for fast performance
// Corrected Algorithmic errors
//
// Based on initial code by
// By Giacomino Veltri
// Last Updated: April 19, 2000
//

#ifndef GRAPH_H
#define GRAPH_H

#define MAX_EDGES    100
#define MAX_PATHLEN  300
#define MAX_VERTICES 2500

// Constants for breadth first search
#define LOWER_BOUND_SEARCH 0
#define UPPER_BOUND_SEARCH 1

struct GVEdge {		// Edge
    int    Dest;	// Second Vertex in the Edge
    double Cost;	// Weight of the Edge
};


class Vertex {
public:
	Vertex();
	~Vertex();
	void initialize();
	void Init(int name);
	void AddEdge(double W, int d);

	int Name;							// Vertex Name
	int Previous;						// -1 if not visited, else previous Vertex name in path
	int adj_count;
    GVEdge Adjacent[MAX_EDGES];			// Adjacent Vertices
};


class Graph {
public:
    Graph();
    ~Graph();
	void reset();
	void initialize();
    void ClearData();
	int  FindVertex(int name);
	int  AddEdge(int S,int E, double W);
	int  BreadthFirst(int S,int E, double Bount, int method);
	double MaxPath(int S, int E, double L, double U);
	double MinPath(int S, int E, double L, double U);
	double MarkCritical(int method);
  double GetMaxWeight(CString *Status, CProgressCtrl *Progress);
	
    Vertex *V;		
    int P[MAX_PATHLEN];
	int p_count;
	int v_count;
	int critical;
}; 

#include "Graph.cpp"

#endif