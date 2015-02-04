////////////////////////////////////////////////////////////////////////////////
// GVSensor2D.cpp                                                             //
//                                                                            //
// Implementation file for a two-dimensional sensor.                          //
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "GVSensor2D.h"



// GVSensor2D //////////////////////////////////////////////////////////////////
GVSensor2D::GVSensor2D()
/*
  GVSensor2D() creates a default sensor.
*/
{ // GVSensor2D
  New(GVPOINT2D_DEFAULTX, GVPOINT2D_DEFAULTY);
} // GVSensor2D



GVSensor2D::~GVSensor2D()
/*
  ~GVSensor2D() deletes the sensor.
*/
{ // ~GVSensor2D
  Delete();
} // ~GVSensor2D



void GVSensor2D::New(double X1, double Y1)
/*
  New() creates a new sensor at (X1, Y1) with breadth first search attributes
  initialized.
*/
{ // New
  GVPoint2D::New(X1, Y1);
  Known = 0;
  Initialize();
} // New



void GVSensor2D::Delete(void)
/*
  Delete() deletes a sensor.
*/
{ // Delete
  GVPoint2D::Delete();
  Known = 0;
  Initialize();
} // Delete



void GVSensor2D::Initialize(void)
/*
  Initialize() deletes (initializes) breadth first search attributes.
*/
{ // Initialize
  Parent = GVSENSOR2D_INITIALIZE_PARENT;
  Cost = GVSENSOR2D_INITIALIZE_COST;
  Visited = GVSENSOR2D_INITIALIZE_VISITED;
} // Initialize



int GVSensor2D::GetParent(void)
/*
  GetParent() returns the breadth first search parent of the sensor.
*/
{ // GetParent
  return(Parent);
} // GetParent



int GVSensor2D::GetCost(void)
/*
  GetCost() returns the breadth first search cost of the sensor.
*/
{ // GetCost
  return(Cost);
} // GetCost



int GVSensor2D::GetVisited(void)
/*
  GetVisited() returns the breadth first search visited of the sensor.
*/
{ // GetVisited
  return(Visited);
} // GetVisited



int GVSensor2D::GetKnown(void)
/*
  GetKnown() returns the known attribute of the sensor.
*/
{ // GetKnown
  return(Known);
} // GetKnown



void GVSensor2D::SetParent(int P)
/*
  SetParent() sets the breadth first search parent of the sensor to P.
*/
{ // SetParent
  Parent = P;
} // SetParent



void GVSensor2D::SetCost(int C)
/*
  SetCost() sets the breadth first search cost to C.
*/
{ // SetCost
  Cost = C;
} // SetCost



void GVSensor2D::SetVisited(int V)
/*
  SetVisited() sets the breadth first search visited to V.
*/
{ // SetVisited
  Visited = V;
} // SetVisited



void GVSensor2D::SetKnown(int K)
/*
  SetKnown() sets the known attribute of the sensor to K.
*/
{ // SetKnown
  Known = K;
} // SetKnown