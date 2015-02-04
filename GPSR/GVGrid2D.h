////////////////////////////////////////////////////////////////////////////////
// GVGrid2D.h                                                                 //
//                                                                            //
// Header file for the two-dimensional grid.                                  //
////////////////////////////////////////////////////////////////////////////////



#ifndef GVGRID2D_H
#define GVGRID2D_H



// Includes ////////////////////////////////////////////////////////////////////
#include "GVUtil.h"
#include "GVGridPoint2D.h"



// GVGrid2D ////////////////////////////////////////////////////////////////////
class GVGrid2D
{ // GVGrid2D
  protected:
    int SizeH;                  // Number of Horizontal (With Respect To Horizontal Axis) Grid Points
    int SizeV;                  // Number of Vertical (With Respect To Vertical Axis) Grid Points
    int NumPoints;              // Total Number of Grid Points
    GVPoint2D Start;            // Starting Coordinate of Grid
    GVPoint2D Horizontal;       // Horizontal Axis Vector
    GVPoint2D Vertical;         // Vertical Axis Vector
    GVGridPoint2D** GridPoints; // Two-Dimensional Array for Storing Grid Points

  public:
    GVGrid2D();                                                                                 // Creates an Empty GVGrid2D
    ~GVGrid2D();                                                                                // Deletes a GVGrid2D
    void New(int NH, int NV, double SX, double SY, double HX, double HY, double VX, double VY); // Creates A New GVGrid2D
    void Delete();                                                                              // Deletes A GVGrid2D

    int GetSizeH(void);                              // Returns the Horizontal Size of the Grid
    int GetSizeV(void);                              // Returns the Vertical Size of the Grid
    double GetPointX(int i, int j, int PointNumber); // Returns the X-Coordinate of the Grid Point (Either Center, Top Left, Top Right, Bottom Left, Bottom Right)
    double GetPointY(int i, int j, int PointNumber); // Returns the Y-Coordinate of the Grid Point (Either Center, Top Left, Top Right, Bottom Left, Bottom Right)
    double GetStartX(void);                          // Returns the Starting X Coordinate
    double GetStartY(void);                          // Returns the Starting Y Coordinate
    double GetHorizontalVectorX(void);               // Returns the Horizontal Vector X Coordinate
    double GetHorizontalVectorY(void);               // Returns the Horizontal Vector Y Coordinate
    double GetVerticalVectorX(void);                 // Returns the Vertical Vector X Coordinate
    double GetVerticalVectorY(void);                 // Returns the Vertical Vector Y Coordinate
}; // GVGrid2D



#endif

