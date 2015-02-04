////////////////////////////////////////////////////////////////////////////////
// GVGrid2D.cpp                                                               //
//                                                                            //
// Implementation file for the two-dimensional grid.                          //
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "GVGrid2D.h"



// GVGrid2D ////////////////////////////////////////////////////////////////////
GVGrid2D::GVGrid2D()
/*
  GVGrid2D() deletes a grid.  It does not create a default grid due to speed.
*/
{ // GVGrid2D
  GridPoints = NULL;
  Delete();
} // GVGrid2D



GVGrid2D::~GVGrid2D()
/*
  ~GVGrid2D deletes the grid.
*/
{ // ~GVGrid2D
  Delete();
} // ~GVGrid2D



void GVGrid2D::New(int NH, int NV, double SX, double SY, double HX, double HY, double VX, double VY)
/*
  New() creates a new grid as follows:
    (NX, NY): The number of grid squares Horizontally and Verically.
    (SX, SY): The origin of the grid in space.  This is the coordinate of the
              Top Left grid square represented by the grid point (0, 0).
    (HX, HY): Defines the Horizontal Axis of the Grid
    (VX, VY): Defines the Vertical Axis of the Grid
*/
{ // New
  int i = 0;                         // Index Variable
  int j = 0;                         // Index Variable
  double HX_2 = HX / 2.0;            // Half of the Horizontal Vector (X Coordinate)
  double HY_2 = HY / 2.0;            // Half of the Horizontal Vector (Y Coordinate)
  double VX_2 = VX / 2.0;            // Half of the Vertical Vector (X Coordinate)
  double VY_2 = VY / 2.0;            // Half of the Vertical Vector (Y Coordinate)
  double OffsetX = SX + HX_2 + VX_2; // X Offset To Make The Top Left Point Line Up Appropriately With The Starting Point
  double OffsetY = SY + HY_2 + VY_2; // Y Offset To Make The Top Left Point Line Up Appropriately With The Starting Point

  Delete();
  Start.New(SX, SY);
  Horizontal.New(HX, HY);
  Vertical.New(VX, VY);
  SizeH = NH;
  SizeV = NV;
  NumPoints = SizeH * SizeV;
  GridPoints = new GVGridPoint2D* [SizeH];
  for (i = 0; i < SizeH; ++i)
  {
    GridPoints[i] = new GVGridPoint2D[SizeV];
    for (j = 0; j < SizeV; ++j)
    {
      GridPoints[i][j].New(OffsetX + i * HX + j * VX, OffsetY + i * HY + j * VY, HX_2, HY_2, VX_2, VY_2);
    }
  }
} // New


void GVGrid2D::Delete()
/*
  Delete() erases the grid and frees up allocated memory.
*/
{ // Delete
  int i = 0; // Index Variable

  if (GridPoints != NULL)
  {
    for (i = 0; i < SizeH; ++i)
    {
      if (GridPoints[i] != NULL) delete[] GridPoints[i];
    }
    delete[] GridPoints;
  }
  SizeH = 0;
  SizeV = 0;
  NumPoints = 0;
  Start.Delete();
  Horizontal.Delete();
  Vertical.Delete();
  GridPoints = NULL;
} // Delete



int GVGrid2D::GetSizeH(void)
/*
  GetSizeH() returns the horizontal size of the grid.
*/
{ // GetSizeH
  return(SizeH);
} // GetSizeH



int GVGrid2D::GetSizeV(void)
/*
  GetSizeV() returns the vertical size of the grid.
*/
{ // GetSizeV
  return(SizeV);
} // GetSizeV



double GVGrid2D::GetPointX(int i, int j, int PointNumber)
/*
  GetPointX() returns the X coordinate of a grid square's point.

  (i, j): Used to reference the grid square (i.e. (0, 0) ).
  PointNumber: GVGRIDPOINT2D_CENTERPOINT - Center of the Grid Square
               GVGRIDPOINT2D_TOPLEFT     - Top Left Point of the Grid Square
               GVGRIDPOINT2D_TOPRIGHT    - Top Right Point of the Grid Square
               GVGRIDPOINT2D_BOTTOMLEFT  - Bottom Left Point of the Grid Square
               GVGRIDPOINT2D_BOTTOMRIGHT - Bottom Right Point of the Grid Square
*/
{ // GetPointX
  double ReturnVal = -1.0; // Return Value

  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeV) && (PointNumber >= GVGRIDPOINT2D_CENTERPOINT) && (PointNumber < GVGRIDPOINT2D_NUMPOINTS)) ReturnVal = GridPoints[i][j].GetX(PointNumber);
  return(ReturnVal);
} // GetPointX



double GVGrid2D::GetPointY(int i, int j, int PointNumber)
/*
  GetPointY() returns the Y coordinate of a grid square's point.

  (i, j): Used to reference the grid square (i.e. (0, 0) ).
  PointNumber: GVGRIDPOINT2D_CENTERPOINT - Center of the Grid Square
               GVGRIDPOINT2D_TOPLEFT     - Top Left Point of the Grid Square
               GVGRIDPOINT2D_TOPRIGHT    - Top Right Point of the Grid Square
               GVGRIDPOINT2D_BOTTOMLEFT  - Bottom Left Point of the Grid Square
               GVGRIDPOINT2D_BOTTOMRIGHT - Bottom Right Point of the Grid Square
*/
{ // GetPointX
  double ReturnVal = -1.0; // Return Value

  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeV) && (PointNumber >= GVGRIDPOINT2D_CENTERPOINT) && (PointNumber < GVGRIDPOINT2D_NUMPOINTS)) ReturnVal = GridPoints[i][j].GetY(PointNumber);
  return(ReturnVal);
} // GetPointX



double GVGrid2D::GetStartX(void)
/*
  GetStartX() returns the starting X coordinate.
*/
{ // GetStartX
  return(Start.GetX());
} // GetStartX



double GVGrid2D::GetStartY(void)
/*
  GetStartY() returns the starting Y coordinate.
*/
{ // GetStartY
  return(Start.GetY());
} // GetStartY



double GVGrid2D::GetHorizontalVectorX(void)
/*
  GetHorizontalX() returns the horizontal vector X coordinate.
*/
{ // GetHorizontalVectorX
  return(Horizontal.GetX());
} // GetHorizontalVectorX



double GVGrid2D::GetHorizontalVectorY(void)
/*
  GetHorizontalY() returns the horizontal vector Y coordinate.
*/
{ // GetHorizontalVectorY
  return(Horizontal.GetY());
} // GetHorizontalVectorY



double GVGrid2D::GetVerticalVectorX(void)
/*
  GetVerticalX() returns the vertical vector X coordinate.
*/
{ // GetVerticalVectorX
  return(Vertical.GetX());
} // GetVerticalVectorX



double GVGrid2D::GetVerticalVectorY(void)
/*
  GetVerticalY() returns the vertical vector Y coordinate.
*/
{ // GetVerticalVectorY
  return(Vertical.GetY());
} // GetVerticalVectorY
