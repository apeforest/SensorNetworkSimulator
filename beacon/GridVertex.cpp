// GridVertex.cpp: implementation of the GridVertex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridVertex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GridVertex::GridVertex(double X, double Y, int VISITED, int N)
{
  x = X;
  y = Y;
  Visited = VISITED;
  Prev = -1;
  if (N > 0)
  {
    PCost = new double[N];
    PIndex = new int[N];
  }
  else
  {
    PCost = NULL;
    PIndex = NULL;
  }
}

GridVertex::~GridVertex()
{
  if (PCost != NULL)
  {
    delete[] PCost;
  }
  if (PIndex != NULL)
  {
    delete[] PIndex;
  }
}
