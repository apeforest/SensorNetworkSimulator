////////////////////////////////////////////////////////////////////////////////
// GVExposureGrid2D.h                                                         //
//                                                                            //
// Header file for two-dimensional grid with exposure attributes.             //
////////////////////////////////////////////////////////////////////////////////



#ifndef GVEXPOSUREGRID2D_H
#define GVEXPOSUREGRID2D_H



// Includes ////////////////////////////////////////////////////////////////////
#include "GVGrid2D.h"
#include "GVExposureGridAttributes.h"



// Defines /////////////////////////////////////////////////////////////////////
#define GVEXPOSUREGRIDATTRIBUTES_MAXLEVEL 32766



// GVExposureGrid2D ////////////////////////////////////////////////////////////
class GVExposureGrid2D : public GVGrid2D
{ // GVExposureGrid2D
  protected:
    int MinHorizontal;                      // Minimum Horizontal Grid Point to Search
    int MinVertical;                        // Minimum Vertical Grid Point to Search
    int MaxHorizontal;                      // Maximum Horizontal Grid Point to Search
    int MaxVertical;                        // Maximum Vertical Grid Point to Search
    int RadiusHorizontal;                   // Size of Radius in Grid Squares Horizontally
    int RadiusVertical;                     // Size of Radius in Grid Squares Vertically
    GVExposureGridAttributes** Attributes;  // Attributes for Each Grid Square

  public:
    GVExposureGrid2D();                                                                                   // Creates an empty  GVExposureGrid2D
    ~GVExposureGrid2D();                                                                                  // Deletes a GVExposureGrid2D
    void New(int NH, int NV, double SX, double SY, double HX, double HY, double VX, double VY, double R); // Creates a new GVExposureGrid2D
    void Delete();                                                                                        // Deletes a GVExposureGrid2D

    int GetMinHorizontal(void);                      // Gets the Minimum Horizontal Value
    int GetMinVertical(void);                        // Gets the Minimum Vertical Value
    int GetMaxHorizontal(void);                      // Gets the Maximum Horizontal Value
    int GetMaxVertical(void);                        // Gets the Maximum Vertcial Value
    int GetRadiusHorizontal(void);                   // Gets the Horizontal Radius
    int GetRadiusVertical(void);                     // Gets the Vertical Radius
    int GetEnqueued(int i, int j);                   // Gets the Enqueued Attribute for a Grid Point
    int GetNearestSensor(int i, int j);              // Gets the Nearest Sensor for a Grid Point
    int GetBorderLevel(int i, int j);                // Gets the Border Level for a Grid Point
    int GetCovered(int i, int j);                    // Gets the Covered Value for a Grid Point
    int GetParentH(int i, int j);                    // Gets the Horizontal Index of the Parent
    int GetParentV(int i, int j);                    // Gets the Vertical Index of the Parent
    double GetCost(int i, int j);                    // Gets the Cost Attribute for a Grid Point
    double GetExposureValue(int i, int j);           // Gets the Exposure Value for a Grid Point
    double GetNormalizedExposureValue(int i, int j); // Gets the Normalized Exposure Value for a Grid Point

    void SetMinHorizontal(int MH);                             // Sets the Minimum Horizontal Value
    void SetMinVertical(int MV);                               // Sets the Minimum Vertical Value
    void SetMaxHorizontal(int MH);                             // Sets the Maximum Horizontal Value
    void SetMaxVertical(int MV);                               // Sets the Maximum Vertical Value
    void SetRadiusHorizontal(int RH);                          // Sets the Horizontal Radius
    void SetRadiusVertical(int RV);                            // Sets the Vertical Radius
    void SetEnqueued(int i, int j, int E);                     // Sets the Enqueued Attribute for a Grid Point
    void SetParent(int i, int j, int H, int V);                // Sets the Parent of Node (i, j) to (H, V)
    void SetNearestSensor(int i, int j, int NS);               // Sets the Nearest Sensor for a Grid Point
    void SetBorderLevel(int i, int j, int BL);                 // Sets the Border Level for a Grid Point
    void SetCovered(int i, int j, int C);                      // Sets the Covered Value for a Grid Point
    void SetCost(int i, int j, double C);                      // Sets the Cost of a Grid Square
    void SetExposureValue(int i, int j, double EV);            // Sets the Exposure Value for a Grid Point
    void SetNormalizedExposureValue(int i, int j, double NEV); // Sets the Normalized Exposure Value for a Grid Point
}; // GVExposureGrid2D



#endif