////////////////////////////////////////////////////////////////////////////////
// GVPoint2D.h                                                                //
//                                                                            //
// Header file for a two-dimensional point.                                   //
////////////////////////////////////////////////////////////////////////////////



#ifndef GVPOINT2D_H
#define GVPOINT2D_H



// Defines /////////////////////////////////////////////////////////////////////
#define GVPOINT2D_DEFAULTX 0.0
#define GVPOINT2D_DEFAULTY 0.0



// GVPoint2D ///////////////////////////////////////////////////////////////////
class GVPoint2D
{ // GVPoint2D
  protected:
    double X; // X Coordinate of Point
    double Y; // Y Coordinate of Point

  public:
    GVPoint2D();                               // Constructor
    ~GVPoint2D();                              // Destructor
    void New(double X1, double Y1);            // Creates a New Point
    void Delete(void);                         // Deletes a Point

    double GetX(void);                         // Gets the X Coordinate
    double GetY(void);                         // Gets the Y Coordinate

    void SetX(double Value);                   // Sets the X Coordinate
    void SetY(double Value);                   // Sets the Y Coordinate
}; // GVPoint2D



#endif