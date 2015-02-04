// voronoi.cpp: implementation of the voronoi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "voronoi.h"

int __cdecl scomp(const void *p1,const void *p2) {
	Site *s1=(Site *)p1;
	Site *s2=(Site *)p2;
	if(s1 -> y < s2 -> y) return(-1);
	if(s1 -> y > s2 -> y) return(1);
	if(s1 -> x < s2 -> x) return(-1);
	if(s1 -> x > s2 -> x) return(1);
	return(0);
}

int __cdecl sxcomp(const void *p1,const void *p2) {
	Site *s1=(Site *)p1;
	Site *s2=(Site *)p2;
	if(s1 -> x < s2 -> x) return(-1);
	if(s1 -> x > s2 -> x) return(1);
	return(0);
}

int __cdecl sycomp(const void *p1,const void *p2) {
	Site *s1=(Site *)p1;
	Site *s2=(Site *)p2;
	if(s1 -> y < s2 -> y) return(-1);
	if(s1 -> y > s2 -> y) return(1);
	return(0);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Voronoi::Voronoi(unsigned int ns) : num_sites(ns) {
	nsites=num_sites;

    num_deledges=0;
   	num_edges=0;
    num_vertices=0;
	num_nvertices=999;

    graph_e=NULL;
	sites = NULL;
    graph_v=NULL;

    sfl.head = (Freenode *) NULL;
	sfl.nodesize = sizeof Site;

    /*
    bb_xmin=-1000;
    bb_xmax= 1000;
    bb_ymin=-1000;
    bb_ymax= 1000;
    */
    bb_xmin= 0;
    bb_xmax= 1;
    bb_ymin= 0;
    bb_ymax= 1;
}

Voronoi::~Voronoi() {
	if (sites)   delete sites;
    if (graph_e) delete graph_e;
    if (graph_v) delete graph_v;
}

#define DELETED -2
#define le 0
#define re 1

void Voronoi::initialize() {
	if (sites==NULL) {
		sites = new Site[num_sites];
		if (sites==NULL) 
			AfxMessageBox("Error, Not Enough Memory");
	}

	if (graph_e==NULL) {
		graph_e=new Edge[1000];
		if (graph_e==NULL) 
			AfxMessageBox("Error, Not Enough Memory");
	}

	if (graph_v==NULL) {
	    graph_v=new Site[1000];
		if (graph_v==NULL) 
			AfxMessageBox("Error, Not Enough Memory");
	}

    num_deledges=0;
   	num_edges=0;
    num_vertices=0;
	num_nvertices=999;

    /*
    bb_xmin=-1000;
    bb_xmax= 1000;
    bb_ymin=-1000;
    bb_ymax= 1000;
    */
    bb_xmin= 0;
    bb_xmax= 1;
    bb_ymin= 0;
    bb_ymax= 1;
    startx = 0;
    starty = 0;
    endx = 1;
    endy = 1;
}


char *Voronoi::myalloc(unsigned n) {
	char *t=new char[n];
	if (t==NULL) {
		AfxMessageBox("Insufficient memory for Voronoi Processing.");
		exit(1);
	}
	return(t);
}

void Voronoi::makefree(Freenode *curr, Freelist *fl) {
	curr -> nextfree = fl -> head;
	fl -> head = curr;
}


void Voronoi::deref(Site *v) {
	v -> refcnt--;
	if (v->refcnt == 0)
		makefree((Freenode *)v, &sfl);
}

void Voronoi::ref(Site *v) {
	v -> refcnt++;
}

void Voronoi::out_ep(Edge *e) {
    graph_e[num_deledges].reg[0]=(Site*) e->reg[0]->sitenbr;
    graph_e[num_deledges].reg[1]=(Site*) e->reg[1]->sitenbr;
    num_deledges++;

	double x1,y1,x2,y2;
	if (e->ep[0]!=NULL && e->ep[1]!=NULL) {
		x1=e->ep[0]->x;
		y1=e->ep[0]->y;
		x2=e->ep[1]->x;
		y2=e->ep[1]->y;
	}
	else if (e->ep[0]!=NULL && e->ep[1]==NULL) {
		x1=e->ep[0]->x;
		y1=e->ep[0]->y;
		x2=1000.0f;
		y2=(e->b==0) ? y1 : (e->c-e->a*x2)/e->b ;
	}
	else if (e->ep[0]==NULL && e->ep[1]!=NULL) {
		x2=e->ep[1]->x;
		y2=e->ep[1]->y;
		x1=-1000.0f;
		y1=(e->b==0) ? y2 : (e->c-e->a*x1)/e->b ;
	}
	else
		return;

	if (x1<=bb_xmin && x2<=bb_xmin) return;
	if (x1>=bb_xmax && x2>=bb_xmax) return;
	if (y1<=bb_ymin && y2<=bb_ymin) return;
	if (y1>=bb_ymax && y2>=bb_ymax) return;

	// Clip to bounding box 
	if (e->b==0) {
		if (x1<bb_xmin) x1=bb_xmin; 
		if (x1>bb_xmax) x1=bb_xmax;
		if (x2<bb_xmin) x2=bb_xmin;
		if (x2>bb_xmax) x2=bb_xmax;
	}
	else {
		if (x1<bb_xmin) { x1=bb_xmin; y1=(e->c-e->a*x1)/e->b; }
		if (x1>bb_xmax) { x1=bb_xmax; y1=(e->c-e->a*x1)/e->b; }
		if (x2<bb_xmin) { x2=bb_xmin; y2=(e->c-e->a*x2)/e->b; }
		if (x2>bb_xmax) { x2=bb_xmax; y2=(e->c-e->a*x2)/e->b; }
	}

	if (e->a==0) {
		if (y1<bb_ymin) y1=bb_ymin; 
		if (y1>bb_ymax) y1=bb_ymax;
		if (y2<bb_ymin) y2=bb_ymin;
		if (y2>bb_ymax) y2=bb_ymax;
	}
	else {
		if (y1<bb_ymin) { y1=bb_ymin; x1=(e->c-e->b*y1)/e->a; }
		if (y1>bb_ymax) { y1=bb_ymax; x1=(e->c-e->b*y1)/e->a; }
		if (y2<bb_ymin) { y2=bb_ymin; x2=(e->c-e->b*y2)/e->a; }
		if (y2>bb_ymax) { y2=bb_ymax; x2=(e->c-e->b*y2)/e->a; }
	}

	if (e->ep[0]==NULL || e->ep[0]->x!=x1 || e->ep[0]->y!=y1) {
		graph_v[num_nvertices].x= (double) x1;
		graph_v[num_nvertices].y= (double) y1;
		graph_e[num_edges].ep[0]= (Site *)num_nvertices;
		graph_v[num_nvertices].sitenbr=num_nvertices;
		num_nvertices--;
	}
	else
		graph_e[num_edges].ep[0]= (Site *)e->ep[0]->sitenbr;

	if (e->ep[1]==NULL || e->ep[1]->x!=x2 || e->ep[1]->y!=y2) {
		graph_v[num_nvertices].x= (double) x2;
		graph_v[num_nvertices].y= (double) y2;
		graph_e[num_edges].ep[1]= (Site *)num_nvertices;
        graph_v[num_nvertices].sitenbr=num_nvertices;
		num_nvertices--;
	}
	else
		graph_e[num_edges].ep[1]= (Site *)e->ep[1]->sitenbr;

	graph_e[num_edges].edgenbr=num_edges;
	num_edges++;
}

char *Voronoi::getfree(Freelist *fl) {
	Freenode *t;
	if(fl->head == (Freenode *) NULL) {
		t = (Freenode *) myalloc(sqrt_nsites * fl->nodesize);
		for(int i=0; i<sqrt_nsites; i+=1) 	
			makefree((Freenode *)((char *)t+i*fl->nodesize), fl);
	}
	t = fl -> head;
	fl -> head = (fl -> head) -> nextfree;
	return((char *)t);
}


/* return a single in-storage site */
Site *Voronoi::nextone() {
	for ( ;siteidx<nsites; siteidx++) {
		if (siteidx==0 || sites[siteidx].x!=sites[siteidx-1].x || sites[siteidx].y!=sites[siteidx-1].y) {
			siteidx ++;
			return (&sites[siteidx-1]);
		}
	}
	return(NULL);
}


/* all sites, sort, and compute xmin, xmax, ymin, ymax */
void Voronoi::assign_sensor(unsigned int i, double x, double y) {
	if (i>=num_sites) return;
	sites[i].x=x;
	sites[i].y=y;
	sites[i].sitenbr = i;
	sites[i].refcnt = 0;
}

int Voronoi::PQbucket(Halfedge *he) {
	int bucket;
	if (he->ystar < ymin) bucket = 0;
	else if	(he->ystar >= ymax) bucket = PQhashsize-1;
	else bucket = (int)((he->ystar - ymin)/(ymax-ymin) * PQhashsize);
	if (bucket<0) bucket = 0;
	if (bucket>=PQhashsize) bucket = PQhashsize-1 ;
	if (bucket < PQmin) PQmin = bucket;
	return(bucket);
}


void Voronoi::PQinsert(Halfedge *he,Site *v,double offset) {
	Halfedge *last, *next;
	he -> vertex = v;
	ref(v);
	he -> ystar = v -> y + offset;
	last = &PQhash[PQbucket(he)];
	while ((next = last -> PQnext) != (Halfedge *) NULL && (he -> ystar  > next -> ystar  || (he -> ystar == next -> ystar && v -> x > next->vertex->x))) {
		last = next;
	}
	
	he -> PQnext = last -> PQnext; 
	last -> PQnext = he;
	PQcount++;
}

void Voronoi::PQdelete(Halfedge *he) {
	Halfedge *last;
	if(he ->  vertex != (Site *) NULL) {
		last = &PQhash[PQbucket(he)];
		while (last -> PQnext != he) last = last -> PQnext;
		last -> PQnext = he -> PQnext;
		PQcount -= 1;
		deref(he -> vertex);
		he -> vertex = (Site *) NULL;
	}
}


Edge *Voronoi::bisect(Site *s1,Site *s2) {
	double dx,dy,adx,ady;
	Edge *newedge;

	newedge = (Edge *) getfree(&efl);

	newedge -> reg[0] = s1;
	newedge -> reg[1] = s2;
	ref(s1); 
	ref(s2);
	newedge -> ep[0] = NULL;
	newedge -> ep[1] = NULL;

	dx = s2->x - s1->x;
	dy = s2->y - s1->y;
	adx = dx>0 ? dx : -dx;
	ady = dy>0 ? dy : -dy;

	newedge -> c = s1->x * dx + s1->y * dy + (dx*dx + dy*dy)*0.5;
	if (adx>ady) {
		newedge -> a = 1.0; 
		newedge -> b = dy/dx; 
		newedge -> c /= dx;
	}
	else {
		newedge -> b = 1.0;
		newedge -> a = dx/dy;
		newedge -> c /= dy;
	}

	newedge -> edgenbr = nedges++;
    return(newedge);
}


Site *Voronoi::intersect(Halfedge *el1,Halfedge *el2) {
	Edge *e1,*e2, *e;
	Halfedge *el;
	double d, xint, yint;
	int right_of_site;
	Site *v;

	e1 = el1 -> ELedge;
	e2 = el2 -> ELedge;
	if(e1 == (Edge*)NULL || e2 == (Edge*)NULL) return ((Site *) NULL);
	if (e1->reg[1] == e2->reg[1]) return ((Site *) NULL);

	d = e1->a * e2->b - e1->b * e2->a;
	/* printf("intersect: d=%g\n", d); */
	if (-1.0e-17<d && d<1.0e-17) 
		{return ((Site *) NULL);};

	xint = (e1->c*e2->b - e2->c*e1->b)/d;
	yint = (e2->c*e1->a - e1->c*e2->a)/d;

	if( (e1->reg[1]->y < e2->reg[1]->y) || (e1->reg[1]->y == e2->reg[1]->y && e1->reg[1]->x < e2->reg[1]->x)) {
		el = el1; e = e1;
	}
	else {	
		el = el2; e = e2;
	}

	right_of_site = xint >= e -> reg[1] -> x;
	if ((right_of_site && el -> ELpm == le) || (!right_of_site && el -> ELpm == re)) 
		return ((Site *) NULL);

	v = (Site *) getfree(&sfl);
	v -> refcnt = 0;
	v -> x = (double) xint;
	v -> y = (double) yint;
	return(v);
}

int Voronoi::right_of(Halfedge *el,Site *p) {/* returns 1 if p is to right of halfedge e */
	Edge *e;
	Site *topsite;
	int right_of_site, above, fast;
	double dxp, dyp, dxs, t1, t2, t3, yl;

	e = el -> ELedge;
	topsite = e -> reg[1];
	right_of_site = p -> x > topsite -> x;
	if( right_of_site && el->ELpm==le) return(1);
	if(!right_of_site && el->ELpm==re) return (0);

	if (e->a == 1.0) {
		dyp = p->y - topsite->y;
		dxp = p->x - topsite->x;
		fast = 0;
		if ((!right_of_site & (e->b<0.0)) | (right_of_site & (e->b>=0.0)) ) {
			above = dyp>= e->b*dxp;	
			fast  = above;
		}
		else {
			above = p->x + p->y*e->b > e-> c;
			if(e->b<0.0) above = !above;
			if (!above) fast = 1;
		}
		if (!fast) {
			dxs = topsite->x - (e->reg[0])->x;
			above = e->b * (dxp*dxp - dyp*dyp) < dxs*dyp*(1.0+2.0*dxp/dxs + e->b*e->b);
			if(e->b<0.0) above = !above;
		}
	}
	else  /*e->b==1.0 */ {
		yl = e->c - e->a*p->x;
		t1 = p->y - yl;
		t2 = p->x - topsite->x;
		t3 = yl - topsite->y;
		above = t1*t1 > t2*t2 + t3*t3;
	}
	return (el->ELpm==le ? above : !above);
}

void Voronoi::endpoint(Edge *e,int lr,Site *s) {
	e -> ep[lr] = s;
	ref(s);
	if (e->ep[re-lr]==(Site *) NULL) return;
	out_ep(e);
	deref(e->reg[le]);
	deref(e->reg[re]);
	makefree((Freenode *)e, &efl);
}

double Voronoi::dist(Site *s,Site *t) {
	double dx = s->x - t->x;
	double dy = s->y - t->y;
	return (double)sqrt(dx*dx + dy*dy);
}

Halfedge *Voronoi::HEcreate(Edge *e,int pm) {
	Halfedge *answer;
	answer = (Halfedge *) getfree(&hfl);
	answer -> ELedge = e;
	answer -> ELpm = pm;
	answer -> PQnext = (Halfedge *) NULL;
	answer -> vertex = (Site *) NULL;
	answer -> ELrefcnt = 0;
	return answer;
}

void Voronoi::ELinsert(Halfedge *lb, Halfedge *newe) {
	newe -> ELleft = lb;
	newe -> ELright= lb -> ELright;
	(lb -> ELright) -> ELleft = newe;
	lb -> ELright = newe;
}

/* Get entry from hash table, pruning any deleted nodes */
Halfedge *Voronoi::ELgethash(int b) {
	if(b<0 || b>=ELhashsize) return NULL;
	Halfedge *he = ELhash[b]; 
	if (he == (Halfedge *) NULL || he -> ELedge != (Edge *) DELETED ) return (he);

	/* Hash table points to deleted half edge.  Patch as necessary. */
	ELhash[b] = NULL;
	if ((he -> ELrefcnt -= 1) == 0) makefree((Freenode *)he, &hfl);
	return NULL;
}	

Halfedge *Voronoi::ELleftbnd(Site *p) {
	int i, bucket;
	Halfedge *he;

	/* Use hash table to get close to desired halfedge */
	bucket = (int)((p->x - xmin)/(xmax-xmin) * ELhashsize);
	if(bucket<0) bucket =0;
	if(bucket>=ELhashsize) bucket = ELhashsize - 1;
	he = ELgethash(bucket);
	if(he == (Halfedge *) NULL) {
		for(i=1; 1 ; i++) {
			if ((he=ELgethash(bucket-i)) != (Halfedge *) NULL) break;
			if ((he=ELgethash(bucket+i)) != (Halfedge *) NULL) break;
	    }
	}

	/* Now search linear list of halfedges for the corect one */
	if (he==ELleftend  || (he != ELrightend && right_of(he,p))) {
		do {he = he -> ELright;} while (he!=ELrightend && right_of(he,p));
		he = he -> ELleft;
	}
	else 
		do {
			he = he -> ELleft;
		} while (he!=ELleftend && !right_of(he,p));

	/* Update hash table and reference counts */
	if(bucket > 0 && bucket <ELhashsize-1) {
		if(ELhash[bucket] != (Halfedge *) NULL) 
			ELhash[bucket] -> ELrefcnt -= 1;
		ELhash[bucket] = he;
		ELhash[bucket] -> ELrefcnt++;
	}
	return (he);
}

	
/* This delete routine can't reclaim node, since pointers from hash
   table may be present.   */
void Voronoi::ELdelete(Halfedge *he) {
	(he -> ELleft) -> ELright = he -> ELright;
	(he -> ELright) -> ELleft = he -> ELleft;
	he -> ELedge = (Edge *)DELETED;
}

Halfedge *Voronoi::ELright(Halfedge *he) { return (he -> ELright); }
Halfedge *Voronoi::ELleft (Halfedge *he) { return (he -> ELleft);  }

Site *Voronoi::leftreg(Halfedge *he) {
	if(he ->ELedge==NULL) return(bottomsite);
	return ( he -> ELpm == le ? he -> ELedge -> reg[le] : he -> ELedge -> reg[re]);
}

Site *Voronoi::rightreg(Halfedge *he){
	if(he -> ELedge == (Edge *)NULL) return(bottomsite);
	return ( he -> ELpm == le ? he -> ELedge -> reg[re] : he -> ELedge -> reg[le]);
}


/* implicit parameters: nsites, sqrt_nsites, xmin, xmax, ymin, ymax.
   Performance suffers if they are wrong; better to make nsites, and deltay too big than too small. */

void Voronoi::do_voronoi() {
	Site *newsite, *bot, *top, *temp, *p, *v;
	Site newintstar;
	Halfedge *lbnd, *rbnd, *llbnd, *rrbnd, *bisector;
	Edge *e;
	int pm, i;

   	num_edges=0;
    num_vertices=0;
	num_nvertices=999;
    num_deledges=0;

	qsort(sites, nsites, sizeof Site,scomp);

	xmin=sites[0].x; 
	xmax=sites[0].x;
	for(i=1; i<nsites; i++) {
		if(sites[i].x<xmin) xmin=sites[i].x;
		if(sites[i].x>xmax) xmax=sites[i].x;
	}
	ymin = sites[0].y;
	ymax = sites[nsites-1].y;
	
	siteidx = 0;
	nedges = 0;
	nvertices = 0;
	sqrt_nsites = (int)sqrt(nsites + 4);
	
    sfl.head = (Freenode *) NULL;
	sfl.nodesize = sizeof Site;

	efl.head = (Freenode *) NULL;
	efl.nodesize = sizeof Edge;

	hfl.head = NULL;
	hfl.nodesize = sizeof **ELhash;

	/////////////////////////// PQinitialize();
	PQcount = 0;
	PQmin = 0;
	PQhashsize = 4 * sqrt_nsites;
	PQhash = (Halfedge *) myalloc(PQhashsize * (sizeof *PQhash));
	for(i=0; i<PQhashsize; i++) PQhash[i].PQnext = NULL;
	////////////////////////////
	
	bottomsite = nextone();
	
	////////////////// ELinitialize();
	ELhashsize = 2 * sqrt_nsites;
	ELhash = (Halfedge **) myalloc(ELhashsize * (sizeof *ELhash));
	for(i=0; i<ELhashsize; i++) ELhash[i]=NULL;

	ELleftend  = HEcreate(NULL, 0);
	ELrightend = HEcreate(NULL, 0);
	ELleftend -> ELleft = NULL;
	ELleftend -> ELright= ELrightend;
	ELrightend-> ELleft = ELleftend;
	ELrightend-> ELright= NULL;
	ELhash[0]  = ELleftend;
	ELhash[ELhashsize-1] = ELrightend;
	////////////////////

	newsite = nextone();
	while(1) {
		if(!(PQcount==0)) { // Used to be PQMIN
			while(PQhash[PQmin].PQnext == (Halfedge *)NULL) PQmin ++;
			newintstar.x = (float) PQhash[PQmin].PQnext -> vertex -> x;
			newintstar.y = (float) PQhash[PQmin].PQnext -> ystar;
		}

		/* new site is smallest */
		if (newsite != NULL && (PQcount==0 || newsite->y<newintstar.y || (newsite->y == newintstar.y && newsite->x<newintstar.x))) {
			lbnd = ELleftbnd(newsite);
			rbnd = ELright(lbnd);
			bot = rightreg(lbnd);
			e = bisect(bot, newsite);
			bisector = HEcreate(e, le);
			ELinsert(lbnd, bisector);
			if ((p = intersect(lbnd, bisector)) != (Site *) NULL)  {
				PQdelete(lbnd);
				PQinsert(lbnd, p, dist(p,newsite));
			}
			lbnd = bisector;
			bisector = HEcreate(e, re);
			ELinsert(lbnd, bisector);
			if ((p = intersect(bisector, rbnd)) != (Site *) NULL) {
				PQinsert(bisector, p, dist(p,newsite));	
			}
			newsite = nextone();	
		}
		else if (!(PQcount==0)) { /* intersection is smallest */
			lbnd = PQhash[PQmin].PQnext;
			PQhash[PQmin].PQnext = lbnd -> PQnext;
			PQcount--;

            llbnd = ELleft(lbnd);
			rbnd = ELright(lbnd);
			rrbnd = ELright(rbnd);
			bot = leftreg(lbnd);
			top = rightreg(rbnd);

			// FOR delaunay
			// out_triple(bot, top, rightreg(lbnd));
			// printf("%d %d %d\n", s1->sitenbr, s2->sitenbr, s3->sitenbr);

			v = lbnd->vertex;
    		v -> sitenbr = nvertices++;
            graph_v[num_vertices++]= *v;

            endpoint(lbnd->ELedge,lbnd->ELpm,v);
			endpoint(rbnd->ELedge,rbnd->ELpm,v);
			ELdelete(lbnd); 
			PQdelete(rbnd);
			ELdelete(rbnd); 
			pm = le;
			if (bot->y > top->y) {
				temp = bot; 
				bot = top; 
				top = temp; 
				pm = re;
			}
			e = bisect(bot, top);
			bisector = HEcreate(e, pm);
			ELinsert(llbnd, bisector);
			endpoint(e, re-pm, v);
			deref(v);

			if((p = intersect(llbnd, bisector)) != (Site *) NULL) {
				PQdelete(llbnd);
				PQinsert(llbnd, p, dist(p,bot));
			}
			if ((p = intersect(bisector, rrbnd)) != (Site *) NULL) {
				PQinsert(bisector, p, dist(p,bot));
			}
		}
		else 
			break;
	}

	for(lbnd=ELright(ELleftend); lbnd != ELrightend; lbnd=ELright(lbnd)) {
		e = lbnd -> ELedge;
		out_ep(e);
	}

	add_bb_edges();
}


void Voronoi::add_bb_edges() { 
    graph_v[num_nvertices].x= (double) bb_xmin;
	graph_v[num_nvertices].y= (double) bb_ymin;
    graph_v[num_nvertices].sitenbr=num_nvertices--;
	
    graph_v[num_nvertices].x= (double) bb_xmax;
	graph_v[num_nvertices].y= (double) bb_ymin;
    graph_v[num_nvertices].sitenbr=num_nvertices--;
    
	graph_v[num_nvertices].x= (double) bb_xmax;
	graph_v[num_nvertices].y= (double) bb_ymax;
    graph_v[num_nvertices].sitenbr=num_nvertices--;
	
	graph_v[num_nvertices].x= (double) bb_xmin;
	graph_v[num_nvertices].y= (double) bb_ymax;
    graph_v[num_nvertices].sitenbr=num_nvertices--;

	Site *vt=new Site[1000]; int nvt=0;
	Site *vb=new Site[1000]; int nvb=0;
	Site *vr=new Site[1000]; int nvr=0;
	Site *vl=new Site[1000]; int nvl=0;

	if (vt==NULL || vb==NULL || vr==NULL || vl==NULL) {
		AfxMessageBox("Error, Not Enough Memory");
		return;
	}

	graph_v[num_nvertices].x= (double) startx;
	graph_v[num_nvertices].y= (double) starty;
    graph_v[num_nvertices].sitenbr=num_nvertices;
	start=num_nvertices--;
	
	graph_v[num_nvertices].x= (double) endx;
	graph_v[num_nvertices].y= (double) endy;
    graph_v[num_nvertices].sitenbr=num_nvertices;
	end=num_nvertices--;

	for (int i=num_nvertices+1; i<=999; i++) {
		if (graph_v[i].x==bb_xmin) vl[nvl++]=graph_v[i];
		if (graph_v[i].x==bb_xmax) vr[nvr++]=graph_v[i];
		if (graph_v[i].y==bb_ymin) vb[nvb++]=graph_v[i];
		if (graph_v[i].y==bb_ymax) vt[nvt++]=graph_v[i];
	}

	qsort(vl,nvl,sizeof Site,sycomp);
	qsort(vr,nvr,sizeof Site,sycomp);
	
	qsort(vb,nvb,sizeof Site,sxcomp);
	qsort(vt,nvt,sizeof Site,sxcomp);

	// Top and bottom bounding box edges
	for (i=0; i<nvt-1; i++) {
		graph_e[num_edges].ep[0]=(Site *)(vt[i].sitenbr);
		graph_e[num_edges].ep[1]=(Site *)(vt[i+1].sitenbr);
		num_edges++;
	}
    for (i=0; i<nvb-1; i++) {
		graph_e[num_edges].ep[0]=(Site *)(vb[i].sitenbr);
		graph_e[num_edges].ep[1]=(Site *)(vb[i+1].sitenbr);
		num_edges++;
	}

	// Left and right bb edges
	/*for (i=0; i<nvr-1; i++) {
		graph_e[num_edges].ep[0]=(Site *)(vr[i].sitenbr);
		graph_e[num_edges].ep[1]=(Site *)(vr[i+1].sitenbr);
		num_edges++;
	}
	for (i=0; i<nvl-1; i++) {
		graph_e[num_edges].ep[0]=(Site *)(vl[i].sitenbr);
		graph_e[num_edges].ep[1]=(Site *)(vl[i+1].sitenbr);
		num_edges++;
	}*/

	// edges to source and sink
	for (i=0; i<nvr; i++) {
		graph_e[num_edges].ep[0]=(Site *)end;
		graph_e[num_edges].ep[1]=(Site *)(vr[i].sitenbr);
		num_edges++;
	}
	for (i=0; i<nvl; i++) {
		graph_e[num_edges].ep[0]=(Site *)start;
		graph_e[num_edges].ep[1]=(Site *)(vl[i].sitenbr);
		num_edges++;
	}

	delete vt;
	delete vb;
	delete vr;
	delete vl;
}

