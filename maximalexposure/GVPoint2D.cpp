////////////////////////////////////////////////////////////////////////////////
// GVPoint2D.cpp                                                              //
//                                                                            //
// Implementation File for GVPoint2D Class.                                   //
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "GVPoint2D.h"



// GVPoint2D ///////////////////////////////////////////////////////////////////
GVPoint2D::GVPoint2D()
/*
  GVPoint2D() creates a new default point.
*/
{ // GVPoint
  New(GVPOINT2D_DEFAULTX, GVPOINT2D_DEFAULTY);
} // GVPoint



GVPoint2D::~GVPoint2D()
/*
  ~GVPoint2D() restores the point to its default value.
*/
{ // ~GVPoint
  Delete();
} // ~GVPoint



void GVPoint2D::New(double X1, double Y1)
/*
  New() creates a new point with the specified coordinates (X1, Y1).
*/
{ // New
  Delete();
  X = X1;
  Y = Y1;
} // New



void GVPoint2D::Delete()
/*
  Delete() restores the point to its default value.
*/
{ // Delete
  X = GVPOINT2D_DEFAULTX;
  Y = GVPOINT2D_DEFAULTY;
} // Delete



double GVPoint2D::GetX(void)
/*
  GetX() returns the X Coordinate of the Point.
*/
{ // GetX
  return(X);
} // GetX



double GVPoint2D::GetY(void)
/*
  GetY() returns the Y Coordiante of the Point.
*/
{ // GetY
  return(Y);
} // GetY



void GVPoint2D::SetX(double Value)
/*
  SetX() sets the X Coordinate to Value.
*/
{ // SetX
  X = Value;
} // SetX



void GVPoint2D::SetY(double Value)
/*
  SetY() sets the Y Coordinate to Value.
*/
{ // SetY
  Y = Value;
} // SetY
