////////////////////////////////////////////////////////////////////////////////
// GVGridPoint2D.h                                                            //
//                                                                            //
// Header file for the two-dimensional grid point.                            //
////////////////////////////////////////////////////////////////////////////////



#ifndef GVGRIDPOINT2D_H
#define GVGRIDPOINT2D_H



// Includes ////////////////////////////////////////////////////////////////////
#include "GVPoint2D.h"



// Defines /////////////////////////////////////////////////////////////////////
#define GVGRIDPOINT2D_X    0.0
#define GVGRIDPOINT2D_Y    0.0
#define GVGRIDPOINT2D_HX_2 0.5
#define GVGRIDPOINT2D_HY_2 0.0
#define GVGRIDPOINT2D_VX_2 0.0
#define GVGRIDPOINT2D_VY_2 0.5

#define GVGRIDPOINT2D_CENTERPOINT (0                            )
#define GVGRIDPOINT2D_TOPLEFT     (GVGRIDPOINT2D_CENTERPOINT + 1)
#define GVGRIDPOINT2D_TOPRIGHT    (GVGRIDPOINT2D_TOPLEFT     + 1)
#define GVGRIDPOINT2D_BOTTOMLEFT  (GVGRIDPOINT2D_TOPRIGHT    + 1)
#define GVGRIDPOINT2D_BOTTOMRIGHT (GVGRIDPOINT2D_BOTTOMLEFT  + 1)
#define GVGRIDPOINT2D_NUMPOINTS   (GVGRIDPOINT2D_BOTTOMRIGHT + 1)



// GVGridPoint2D ///////////////////////////////////////////////////////////////
class GVGridPoint2D
{ // GVGridPoint2D
  protected:
    GVPoint2D P[GVGRIDPOINT2D_NUMPOINTS]; // Center Point and Four Corners of Grid Square

  public:
    GVGridPoint2D();                                                                    // Creates Default Grid Point
    ~GVGridPoint2D();                                                                   // Deletes Point (Sets To Default)
    void New(double PX, double PY, double HX_2, double HY_2, double VX_2, double VY_2); // Creates Grid Point at (PX, PY) with Grid Square Corners Defined By (HX_2, HY_2) and (VX_2, VY_2)
    void Delete(void);                                                                  // Deletes Point (Sets To Default)

    double GetX(int Index); // Gets The X Value For a Point (Center, Top Left, Top Right, Bottom Left, Bottom Right)
    double GetY(int Index); // Gets The Y Value For a Point (Center, Top Left, Top Right, Bottom Left, Bottom Right)
}; // GVGridPoint2D



#endif