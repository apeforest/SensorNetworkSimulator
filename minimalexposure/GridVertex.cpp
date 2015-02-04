// GridVertex.cpp: implementation of the GridVertex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Exposure.h"
#include "GridVertex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GridVertex::GridVertex(double X, double Y, double COST, int VISITED)
{
  x = X;
  y = Y;
  Cost = COST;
  Visited = VISITED;
  Prev = -1;
}

GridVertex::~GridVertex()
{

}
