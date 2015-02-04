////////////////////////////////////////////////////////////////////////////////
// GVGridPoint2D.cpp                                                          //
//                                                                            //
// Implementation file for the two-dimensional grid point.                    //
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "GVGridPoint2D.h"



// GVGridPoint2D ///////////////////////////////////////////////////////////////
GVGridPoint2D::GVGridPoint2D()
{ // GVGridPoint2D
  New(GVGRIDPOINT2D_X, GVGRIDPOINT2D_Y, GVGRIDPOINT2D_HX_2, GVGRIDPOINT2D_HY_2, GVGRIDPOINT2D_VX_2, GVGRIDPOINT2D_VY_2);
} // GVGridPoint2D



GVGridPoint2D::~GVGridPoint2D()
{ // ~GVGridPoint2D
  Delete();
} // ~GVGridPoint2D



void GVGridPoint2D::New(double PX, double PY, double HX_2, double HY_2, double VX_2, double VY_2)
{ // New
  P[GVGRIDPOINT2D_CENTERPOINT].SetX(PX);
  P[GVGRIDPOINT2D_CENTERPOINT].SetY(PY);
  P[GVGRIDPOINT2D_TOPLEFT].SetX(PX - HX_2 - VX_2);
  P[GVGRIDPOINT2D_TOPLEFT].SetY(PY - HY_2 - VY_2);
  P[GVGRIDPOINT2D_TOPRIGHT].SetX(PX + HX_2 - VX_2);
  P[GVGRIDPOINT2D_TOPRIGHT].SetY(PY + HY_2 - VY_2);
  P[GVGRIDPOINT2D_BOTTOMLEFT].SetX(PX - HX_2 + VX_2);
  P[GVGRIDPOINT2D_BOTTOMLEFT].SetY(PY - HY_2 + VY_2);
  P[GVGRIDPOINT2D_BOTTOMRIGHT].SetX(PX + HX_2 + VX_2);
  P[GVGRIDPOINT2D_BOTTOMRIGHT].SetY(PY + HY_2 + VY_2);
} // New



void GVGridPoint2D::Delete(void)
{ // Delete
  New(GVGRIDPOINT2D_X, GVGRIDPOINT2D_Y, GVGRIDPOINT2D_HX_2, GVGRIDPOINT2D_HY_2, GVGRIDPOINT2D_VX_2, GVGRIDPOINT2D_VY_2);
} // Delete



double GVGridPoint2D::GetX(int Index)
{ // GetX
  int i = GVGRIDPOINT2D_CENTERPOINT; // Index Variable

  if ((Index >= GVGRIDPOINT2D_CENTERPOINT) && (Index < GVGRIDPOINT2D_NUMPOINTS)) i = Index;
  return(P[i].GetX());
} // GetX



double GVGridPoint2D::GetY(int Index)
{ // GetY
  int i = GVGRIDPOINT2D_CENTERPOINT; // Index Variable

  if ((Index >= GVGRIDPOINT2D_CENTERPOINT) && (Index < GVGRIDPOINT2D_NUMPOINTS)) i = Index;
  return(P[i].GetY());
} // GetY

