////////////////////////////////////////////////////////////////////////////////
// GridSquare.cpp
// Implementation file for GridSquare class
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "GridSquare.h"



// GridSquare //////////////////////////////////////////////////////////////////
GridSquare::GridSquare()
{ // GridSquare
  XL = 0.0;
  YL = 0.0;
  XU = 0.0;
  YU = 0.0;
  CenterPointX = 0.0;
  CenterPointY = 0.0;
  OffsetX = 0.0;
  OffsetY = 0.0;
  InitialOffset = 0.0;
} // GridSquare



GridSquare::~GridSquare()
{ // ~GridSquare
  Deinit();
} // ~GridSquare



void GridSquare::Init(double CX, double CY, double X1, double Y1, double X2, double Y2, double OX, double OY)
{ // Init
  Deinit();
  CenterPointX = CX;
  CenterPointY = CY;
  XL = X1;
  YL = Y1;
  XU = X2;
  YU = Y2;
  OffsetX = OX;
  OffsetY = OY;
  InitialOffset = OX;
} // Init



void GridSquare::Deinit()
{ // Deinit
  XL = 0.0;
  YL = 0.0;
  XU = 0.0;
  YU = 0.0;
  CenterPointX = 0.0;
  CenterPointY = 0.0;
  OffsetX = 0.0;
  OffsetY = 0.0;
  InitialOffset = 0.0;
} // Deinit



void GridSquare::SetCenterPoint(double CX, double CY)
{ // SetCenterPoint
  CenterPointX = CX;
  CenterPointY = CY;
} // SetCenterPoint



void GridSquare::SetOffset(double OX, double OY)
{ // SetOffset
  InitialOffset = OX;
  OffsetX = OX;
  OffsetY = OY;
  if (OffsetX > 0.0) OffsetX = 0.0;
  if (OffsetY > 0.0) OffsetY = 0.0;
} // SetOffset



double GridSquare::GetCenterPointX(void)
{ // GetCenterPointX
  return(CenterPointX);
} // GetCenterPointX



double GridSquare::GetCenterPointY(void)
{ // GetCenterPointY
  return(CenterPointY);
} // GetCenterPointY



double GridSquare::GetLX(void)
{ // GetLX
  return(XL);
} // GetLX



double GridSquare::GetLY(void)
{ // GetLY
  return(YL);
} // GetLY



double GridSquare::GetUX(void)
{ // GetUX
  return(XU);
} // GetUX



double GridSquare::GetUY(void)
{ // GetUY
  return(YU);
} // GetUY



double GridSquare::GetOffsetX(void)
{ // GetOffsetX
  return(OffsetX);
} // GetOffsetX



double GridSquare::GetOffsetY(void)
{ // GetOffsetY
  return(OffsetY);
} // GetOffsetY
