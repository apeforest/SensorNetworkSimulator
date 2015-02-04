////////////////////////////////////////////////////////////////////////////////
// GVSensor2D.h                                                               //
//                                                                            //
// Header file for a two-dimensional sensor.                                  //
////////////////////////////////////////////////////////////////////////////////



#ifndef GVSENSOR2D_H
#define GVSENSOR2D_H



// Includes ////////////////////////////////////////////////////////////////////
#include "GVPoint2D.h"



// Defines /////////////////////////////////////////////////////////////////////
#define GVSENSOR2D_INITIALIZE_PARENT  -1
#define GVSENSOR2D_INITIALIZE_COST    0
#define GVSENSOR2D_INITIALIZE_VISITED 0



// GVSensor2D //////////////////////////////////////////////////////////////////
class GVSensor2D: public GVPoint2D
{ // GVSensor2D
  protected:
    int Parent;  // Breadth First Search Attribute: Parent
    int Cost;    // Breadth First Search Attribute: Cost (Hops)
    int Visited; // Breadth First Search Attribute: Visited
    int Known;   // Is the Sensor Known

  public:
    GVSensor2D();                   // Creates a Blank Sensor
    ~GVSensor2D();                  // Deletes a Sensor
    void New(double X1, double Y1); // Creates a New Sensor at (X1, Y1)
    void Delete(void);              // Deletes a Sensor
    void Initialize(void);          // Deletes Only Sensor Attributes (Not Point Attributes)

    int GetParent(void);  // Gets the Breadth First Search Parent of the Sensor
    int GetCost(void);    // Gets the Breadth First Search Cost of the Sensor
    int GetVisited(void); // Gets the Breadth First Search Visited of the Sensor
    int GetKnown(void);   // Gets the Known Attribute of the Sensor

    void SetParent(int P);  // Sets the Breadth First Search Parent of the Sensor
    void SetCost(int C);    // Sets the Breadth First Search Cost of the Sensor
    void SetVisited(int V); // Sets the Breadth First Search Visited of the Sensor
    void SetKnown(int K);   // Sets the Known Attribute of the Sensor
}; // GVSensor2D



#endif