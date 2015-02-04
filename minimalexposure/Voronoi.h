// voronoi.h: interface for the voronoi class.
//
//////////////////////////////////////////////////////////////////////

#ifndef VORONOI_H
#define VORONOI_H

#include <math.h>
#include "Graph.h"

#define DELETED -2
#define le 0
#define re 1

struct Freenode	{
	Freenode*	nextfree;
};

struct Freelist	{
	Freenode*	head;
	int			nodesize;
};

/* structure used both for sites and for vertices */
struct Site	{
	double	x,y;
	int		sitenbr;
	int		refcnt;
};

struct Edge	{
	double	a,b,c;
	Site 	*ep[2];
	Site	*reg[2];
	int		edgenbr;
};

struct Halfedge {
	Halfedge*	ELleft;
	Halfedge*	ELright;
	Edge*		ELedge;
	int			ELrefcnt;
	char		ELpm;
	Site*		vertex;
	double		ystar;
	Halfedge*	PQnext;
};

class Voronoi   {
public:
	int end;
	int start;
  double startx, starty, endx, endy;

	Voronoi(unsigned int ns);
	~Voronoi();

 	Freelist efl, hfl, sfl;
	Halfedge *ELleftend, *ELrightend;
	Halfedge **ELhash;
	int ELhashsize;
	int PQhashsize;

	Halfedge *PQhash;
	
	int PQcount;
	int PQmin;
	unsigned int num_sites;
    unsigned int num_edges;
    unsigned int num_deledges;
    unsigned int num_vertices;
	unsigned int num_nvertices;

    Site *graph_v;
    Edge *graph_e;

	Site*	sites;
	int		nedges;
	int		nsites;
	int		siteidx;
	int		sqrt_nsites;
	int		nvertices;
	Site*	bottomsite;

	int	  triangulate;
  int bb_xmin, bb_xmax, bb_ymin, bb_ymax;
	double xmin, xmax, ymin, ymax;
	double pxmin, pxmax, pymin, pymax;

	char *myalloc(unsigned n);
	void  makefree(Freenode *curr, Freelist *fl);
	char *getfree(Freelist *fl);
	
	void deref(Site *v);
	void ref(Site *v);
	void out_ep(Edge *e);
	
	Site *nextone();	/* return a single in-storage site */
	
	void assign_sensor(unsigned int i, double x, double y);
	
	int  PQbucket(Halfedge *he);
	void PQinsert(Halfedge *he,Site *v,double  offset);
	void PQdelete(Halfedge *he);
	
	Edge *bisect(Site *s1,Site *s2);
	Site *intersect(Halfedge *el1,Halfedge *el2);
	int   right_of(Halfedge *el,Site *p); /* returns 1 if p is to right of halfedge e */
	void  endpoint(Edge *e,int lr,Site *s);
	double dist(Site *s,Site *t);
	
	Halfedge *HEcreate(Edge *e,int pm);
	void ELinsert(Halfedge *lb,Halfedge *newe);
	void ELdelete(Halfedge *he); /* This delete routine can't reclaim node, since pointers from hash table may be present.   */
	
	Halfedge *ELgethash(int b); /* Get entry from hash table, pruning any deleted nodes */
	Halfedge *ELleftbnd(Site *p);
	Halfedge *ELright(Halfedge *he);
	Halfedge *ELleft(Halfedge *he);
	
	Site *leftreg (Halfedge *he);
	Site *rightreg(Halfedge *he);

	void do_voronoi();
	void add_bb_edges();
	void initialize();
};

#include "Voronoi.cpp"

#endif
