// GridVertex.h: interface for the GridVertex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDVERTEX_H__2A5292DE_391E_4E62_8E8A_A5973F5C70AE__INCLUDED_)
#define AFX_GRIDVERTEX_H__2A5292DE_391E_4E62_8E8A_A5973F5C70AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GridVertex  
{
  public:
	  GridVertex(double X = 0, double Y = 0, double COST = -1, int VISITED = 0);
	  virtual ~GridVertex();

    double x;     // X Coordinate of Vertex
    double y;     // Y COordinate of Vertex
    double Cost;  // Cost it takes to get to Current Vertex
    int Visited;  // If the Vertex Has Been Checked or Not
    int Prev;     // Index of Previous Vertex in Path
};

#endif // !defined(AFX_GRIDVERTEX_H__2A5292DE_391E_4E62_8E8A_A5973F5C70AE__INCLUDED_)
