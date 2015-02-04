////////////////////////////////////////////////////////////////////////////////
// Point.cpp
// Implementation file for Point class.
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "Point.h"



// Point ///////////////////////////////////////////////////////////////////////
Point::Point()
{ // Point
  Deinit();
} // Point



Point::~Point()
{ // ~Point
  Deinit();
} // ~Point



void Point::Init(double PX, double PY, double PZ)
{ // Init
  Deinit();
  SetX(PX);
  SetY(PY);
  SetZ(PZ);
} // Init



void Point::Deinit(void)
{ // Deinit
  X = 0.0;
  Y = 0.0;
  Z = 0.0;
} // Deinit



void Point::SetX(double PX)
{ // SetX
  X = PX;
} // SetX



void Point::SetY(double PY)
{ // SetY
  Y = PY;
} // SetY



void Point::SetZ(double PZ)
{ // SetZ
  Z = PZ;
} // SetZ



double Point::GetX(void)
{ // GetX
  return(X);
} // GetX



double Point::GetY(void)
{ // GetY
  return(Y);
} // GetY



double Point::GetZ(void)
{ // GetZ
  return(Z);
} // GetZ
