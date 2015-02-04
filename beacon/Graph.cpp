// Graph Implementation ////////////////////////////////////////////////////////
// By Giacomino Veltri
// Last Updated: April 19, 2000
//
// Modified by Seapahn Meguerdichian
// Removed templates
// Optimized binary search 
// May 10, 2000
//
#include "stdafx.h"
#include "Graph.h"

Vertex::Vertex() { 
	Previous= -1;
	adj_count= 0;
} 

void Vertex::initialize() {
	Previous= -1;
	adj_count= 0;
}

void Vertex::AddEdge(double W, int d) {
	if (adj_count==MAX_EDGES)
		AfxMessageBox("Error, Vertex Edge count exceeded.");
	else {
		Adjacent[adj_count  ].Cost=W;
		Adjacent[adj_count++].Dest=d;
	}
}

void Vertex::Init(int name) {
	Name     = name;
	Previous = -1;
	adj_count= 0;
}

Vertex::~Vertex() {
}

////////////////////////////////////////

Graph::Graph() {
	V=NULL;
	v_count=0;
	p_count=0;
}


Graph::~Graph() { 
	if (V) delete [] V;
}

void Graph::initialize() {
	if (V==NULL) V=new Vertex[MAX_VERTICES];
	if (V==NULL) AfxMessageBox("Error, Not Enough Memory");
	for (int i=0; i<MAX_VERTICES; i++) V[i].initialize();    // this is redundant but makes me feel better!
}

void Graph::reset() {
	ClearData();
	v_count=0;
	p_count=0;
}

void Graph::ClearData() { // Sets All Vertices to Not Visited
	for (int i=0; i<v_count; ++i) V[i].Previous = -1;
}


int Graph::AddEdge(int S,int E,double W) { 
	int i=FindVertex(S);
	if (i==v_count)	V[v_count++].Init(S);
	V[i].AddEdge(W,E);

	i=FindVertex(E);
	if (i==v_count)	V[v_count++].Init(E);
	V[i].AddEdge(W,S);

	return 1;
}

int Graph::FindVertex(int name) {
	for (int i=0; i<v_count && V[i].Name!=name; ++i);
	return i;
}


int  Graph::BreadthFirst(int S, int E, double Bound,int method) {
	int VQ[MAX_VERTICES];			// Queue to Check Next Vertices
	int i,next,v, Success=0;

	ClearData();					// Clear All Visited Node Data
	
	// Check to see if start and end are valid vertices
	if (       FindVertex(E) ==v_count)	return 0; 
	if ((VQ[0]=FindVertex(S))==v_count) return 0; 

	int VQ_start=0;					// Enqueued the start node
	int VQ_end  =1;

    V[VQ[VQ_start]].Previous=-2;		

    while (VQ_start<VQ_end && !Success) {
		v=VQ[VQ_start++];					// dequeue a vertex
		for (i=0; i<V[v].adj_count && Success==0; i++) {
			if ((V[v].Adjacent[i].Cost>=Bound && method==LOWER_BOUND_SEARCH) || 
				(V[v].Adjacent[i].Cost<=Bound && method==UPPER_BOUND_SEARCH)) {
				next=FindVertex(V[v].Adjacent[i].Dest);
				if (V[next].Previous==-1) {
					V[next].Previous=v;
					VQ[VQ_end++]=next;        // Enqueue vertex
					if (V[next].Name==E)
						Success = 1;
				}
			}
		}
	}

	if (Success) {
		P[0]=V[next].Name;
		for (p_count=1; V[next].Previous>=0; p_count++) {
			next=V[next].Previous;
			P[p_count]=V[next].Name;
		}
	}
	return Success;
} 



double Graph::MaxPath(int S, int E, double L, double U) {
    double M, nL;  // MidPoint of the Weights
	int exists=1;
	
	if (L>U || !BreadthFirst(S,E,L,LOWER_BOUND_SEARCH)) return -1;

    nL=(U+L)/2;
    for (M=(U-nL)/2; M>0.5; M=M/2) {
        if (exists=BreadthFirst(S,E,nL,LOWER_BOUND_SEARCH))
            nL=nL+M;
        else
            nL=nL-M;
	}
	while (!exists)	exists=BreadthFirst(S,E,--nL,LOWER_BOUND_SEARCH);
	return (MarkCritical(LOWER_BOUND_SEARCH));
}


double Graph::MinPath(int S, int E, double L, double U) {
    double M, nU;  // MidPoint of the Weights
	int exists=1;
	
	if (L>U || !BreadthFirst(S,E,U,UPPER_BOUND_SEARCH)) return -1;

    nU=(U+L)/2;
    for (M=(nU-L)/2; M>0.5; M=M/2) {
        if (exists=BreadthFirst(S,E,nU,UPPER_BOUND_SEARCH))
            nU=nU-M;
        else
            nU=nU+M;
	}
	while (!exists)	exists=BreadthFirst(S,E,++nU,UPPER_BOUND_SEARCH);
	return (MarkCritical(UPPER_BOUND_SEARCH));
}


double Graph::MarkCritical(int method) {
	double weight,extreme;
	int i,j,v;

	if (method==UPPER_BOUND_SEARCH) extreme=-10000; else extreme=10000;
	for (i=p_count-2; i>1; --i) {
		v=FindVertex(P[i]);
		for (j=0; j<V[v].adj_count && V[v].Adjacent[j].Dest!=P[i-1]; j++);
		weight=V[v].Adjacent[j].Cost;
		if ((method==UPPER_BOUND_SEARCH && weight>=extreme) ||
			(method==LOWER_BOUND_SEARCH && weight<=extreme)) {
			extreme=weight;
      
			critical=i;
		}
	}
	return extreme;
}


double Graph::GetMaxWeight(CString *Status, CProgressCtrl *Progress)
{ // GetMaxWeight
  int g = 0;         // Index Variable
  int h = 0;         // Index Variable
  int S = 0;         // Start Vertex
  int E = 0;         // End Vertex
  int T = 0;         // Temp Vertex
  double Weight = 0; // Weight of the Path

  // Set the Status Bar
  Status->Format("Calculating Total Cost of Path...");
  // Set the Progress Bar
  Progress->SetRange(0, p_count - 1);
  Progress->SetStep(1);
  Progress->SetPos(0);
  // Add the Weight of Each Edge
  for (g = 0; g < p_count - 1; ++g)
  {
    // Store the Start and End Vertices
    S = FindVertex(P[g]);
    E = FindVertex(P[g + 1]);
    // Check Each Vertex in the Adjacency List
    for (h = 0; (h < V[S].adj_count) && (T != E); ++h)
    {
      // Get the Next Vertex in the Adjacency List
      T = FindVertex(V[S].Adjacent[h].Dest);
      // If the Current Vertex Equals the Destination Vertex
      if (T == E)
      {
        // Add the Weight to the total Cost
        Weight += V[S].Adjacent[h].Cost;
      }
    }
    Progress->StepIt();
  }
  // Return the Weight of the Path
  return Weight;
} // GetMaxWeight