////////////////////////////////////////////////////////////////////////////////
// GVSensorNetwork2D.cpp                                                      //
//                                                                            //
// Implementation file for a two-dimensional sensor network.                  //
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "GVSensorNetwork2D.h"
#include <iostream.h>


// GVSensorNetwork /////////////////////////////////////////////////////////////
GVSensorNetwork2D::GVSensorNetwork2D()
/*
  GVSensorNetwork2D() creates an empty sensor network.
*/
{ // GVSensorNetwork2D 
  Sensors = NULL;
  Obstacles = NULL;
  NumNeighbors = NULL;
  NeighborIndex = NULL;
  Delete();
} // GVSensorNetwork2D



GVSensorNetwork2D::~GVSensorNetwork2D()
/*
  ~GVSensorNetwork2D() deletes a sensor network.
*/
{ // ~GVSensorNetwork2D
  Delete();
} // ~GVSensorNetwork2D


/*
void GVSensorNetwork2D::New(int NS, double R)
{ // New
  int i = 0; // Index Variable

  Delete();
  if (NS > 0)
  {
    NumSensors = NS;
    SetRadius(R);
	Min.New(GVSENSORNETWORK_MINX, GVSENSORNETWORK_MINY);
	Max.New(GVSENSORNETWORK_MAXX, GVSENSORNETWORK_MAXY);
    Sensors = new GVSensor2D[NumSensors];
    for (i = 0; i < NumSensors; ++i)
    {
      Sensors[i].New(GenerateRandomDouble(Min.GetX(), Max.GetX()), GenerateRandomDouble(Min.GetY(), Max.GetY()));
	  
    }
  }
  CalculateEdges();
} // New
*/

void GVSensorNetwork2D::New(int NS, double R)
{ // New
  int i = 0; // Index Variable
  int j = 0;	
  int PositionIsBlocked = 0;
  int NumObstacles = GVSENSORNETWORK_NUMOBSTACLES;

  double XCoordinate = 0.0;
  double YCoordinate = 0.0;

  Delete();
  if (NS > 0)
  {
    NumSensors = NS;
	SetRadius(R);
	Min.New(GVSENSORNETWORK_MINX, GVSENSORNETWORK_MINY);
	Max.New(GVSENSORNETWORK_MAXX, GVSENSORNETWORK_MAXY);
	
	//Obstacles = new GVObstacle[NumObstacles];
	//Obstacles[0].New(0.0, 0.57, 0.43, 0.77);
   // Obstacle[1].New(0.22, 0.0, 0.40, 0.40);
   // Obstacle[2].New(0.7, 0.32, 1.00, 0.74);


    Sensors = new GVSensor2D[NumSensors];
    i = 0; 
	while( i< NumSensors)
    {
      PositionIsBlocked = 0;
	  XCoordinate = GenerateRandomDouble(Min.GetX(), Max.GetX());
	  YCoordinate = GenerateRandomDouble(Min.GetY(), Max.GetY());
	  
	  //if( !PositionIsBlocked)
	  //if(XCoordinate >= 0.0 && YCoordinate >= 0.57 && XCoordinate <= 0.43 && YCoordinate <= 0.77)
	  //{
	  //}
	  //else 
	  
	  if(XCoordinate >= 0.00 && YCoordinate >= 0.20 && XCoordinate <= 0.50 && YCoordinate <= 0.35)
	  {
	  }
	  else if(XCoordinate >= 0.05 && YCoordinate >= 0.70 && XCoordinate <= 0.20 && YCoordinate <= 1.00)
	  {
	  }
	  else if(XCoordinate >= 0.40 && YCoordinate >= 0.75 && XCoordinate <= 1.00 && YCoordinate <= 0.90)
	  {
	  }
	  else if(XCoordinate >= 0.45 && YCoordinate >= 0.50 && XCoordinate <= 0.85 && YCoordinate <= 0.65)
	  {
	  }
	  else if(XCoordinate >= 0.70 && YCoordinate >= 0.00 && XCoordinate <= 0.85 && YCoordinate <= 0.50)
	  {
	  }
	  else
	  {
		Sensors[i].New(XCoordinate, YCoordinate);
		i++;
	  }
	}
  }
  CalculateEdges();
} // New



void GVSensorNetwork2D::Delete(void)
/*
  Delete() deletes a sensor network and restores default values for parameters.
*/
{ // Delete
  int i = 0; // Index Variable

  if (Sensors != NULL) delete[] Sensors;
  if (Obstacles != NULL) delete[] Obstacles;
  if (NumNeighbors != NULL) delete[] NumNeighbors;
  if (NeighborIndex != NULL)
  {
    for (i = 0; i < NumSensors; ++i)
    {
      if (NeighborIndex[i] != NULL) delete[] NeighborIndex[i];
    }
    delete[] NeighborIndex;
  }
  Min.New(GVSENSORNETWORK_MINX, GVSENSORNETWORK_MINY);
  Max.New(GVSENSORNETWORK_MAXX, GVSENSORNETWORK_MAXY);
  NumSensors = GVSENSORNETWORK_DEFAULT_NUMSENSORS;
  Radius = GVSENSORNETWORK_DEFAULT_RADIUS; 
  Power = GVSENSORNETWORK_DEFAULT_POWER;
  Model = GVSENSORNETWORK_DEFAULT_MODEL;
  Sensors = NULL;
  Obstacles = NULL;
  NumNeighbors = NULL;
  NeighborIndex = NULL;
} // Delete



void GVSensorNetwork2D::CalculateEdges(void)
/*
  CalculateEdges() calculates the edges of a sensor network.  An edge between
  two sensors exists if and only if the distance between the two sensors is
  less than the communication radius.

  It deletes any previous neighbors stored and re-allocates them.
*/
{ // CalculateEdges
  int i = 0; // Index Variable
  int j = 0; // Index Variable

  // Deallocate Memory
  if (NumNeighbors != NULL) delete[] NumNeighbors;
  if (NeighborIndex != NULL)
  {
    for (i = 0; i < NumSensors; ++i)
    {
      if (NeighborIndex[i] != NULL) delete[] NeighborIndex[i];
    }
    delete[] NeighborIndex;
  }

  if ((NumSensors > 0) && (Sensors != NULL))
  {
    // Allocate Memory
    NumNeighbors = new int[NumSensors];
    NeighborIndex = new int* [NumSensors];
    for (i = 0; i < NumSensors; ++i)
    {
      NumNeighbors[i] = 0;
      NeighborIndex[i] = new int[NumSensors];
    }

    // Calculate the Actual Edges
    for (i = 0; i < NumSensors; ++i)
    {
      for (j = i + 1; j < NumSensors; ++j)
      {
        if (Distance2D(Sensors[i].GetX(), Sensors[i].GetY(), Sensors[j].GetX(), Sensors[j].GetY()) <= Radius)
        {
          NeighborIndex[i][NumNeighbors[i]] = j;
          NeighborIndex[j][NumNeighbors[j]] = i;
          ++NumNeighbors[i];
          ++NumNeighbors[j];
        }
      }
    }
  }
} // CalculateEdges



int GVSensorNetwork2D::GetNumSensors(void)
/*
  GetNumSensors() returns the number of sensors in the network.
*/
{ // GetNumSensors
  return(NumSensors);
} // GetNumSensors



int GVSensorNetwork2D::GetNumNeighbors(int Index)
/*
  GetNumNeighbors() returns the number of neighbors for the Sensors[Index].
*/
{ // GetNumNeighbors
  int ReturnVal = -1; // Return Value

  if ((Index >= 0) && (Index < NumSensors))
  {
    if (NumNeighbors != NULL) ReturnVal = NumNeighbors[Index];
  }
  return(ReturnVal);
} // GetNumNeighbors



int GVSensorNetwork2D::GetNeighbor(int Index, int NIndex)
/*
  GetNeighbor() returns the Index'th neighbor for Sensors[Index].
*/
{ // GetNeighbor
  int ReturnVal = -1; // Return Value

  if ((Index >= 0) && (Index < NumSensors))
  {
    if ((NumNeighbors != NULL) && (NeighborIndex != NULL))
    {
      if ((NIndex >= 0) && (NIndex < NumNeighbors[Index]) && (NeighborIndex[Index] != NULL))
      {
        ReturnVal = NeighborIndex[Index][NIndex];
      }
    }
  }
  return(ReturnVal);
} // GetNeighbor



double GVSensorNetwork2D::GetX(int Index)
/*
  GetX() returns the X Coordinate of Sensors[Index].
*/
{ // GetX
  double ReturnVal = GVSENSORNETWORK_MINX; // Return Value

  if ((Sensors != NULL) && (Index >= 0) && (Index < NumSensors)) ReturnVal = Sensors[Index].GetX();
  return(ReturnVal);
} // GetX



double GVSensorNetwork2D::GetY(int Index)
/*
  GetY() returns the Y Coordinate of Sensors[Index].
*/
{ // GetY
  double ReturnVal = GVSENSORNETWORK_MINY; // Return Value

  if ((Sensors != NULL) && (Index >= 0) && (Index < NumSensors)) ReturnVal = Sensors[Index].GetY();
  return(ReturnVal);
} // GetY



double GVSensorNetwork2D::GetMinX(void)
/*
  GetMinX() returns the X Coordinate of the minimum coordinate.
*/
{ // GetMinX
  return(Min.GetX());
} // GetMinX



double GVSensorNetwork2D::GetMinY(void)
/*
  GetMinY() returns the Y Coordinate of the minimum coordinate.
*/
{ // GetMinY
  return(Min.GetY());
} // GetMinY



double GVSensorNetwork2D::GetMaxX(void)
/*
  GetMaxX() returns the X Coordinate of the maximum coordinate.
*/
{ // GetMaxX
  return(Max.GetX());
} // GetMaxX



double GVSensorNetwork2D::GetMaxY(void)
/*
  GetMaxY() returns the Y Coordinate of the maximum coordinate.
*/
{ // GetMaxY
  return(Max.GetY());
} // GetMaxY



double GVSensorNetwork2D::GetPower(void)
/*
  GetPower() returns the power of the exposure function.
*/
{ // GetPower
  return(Power);
} // GetPower



int GVSensorNetwork2D::GetModel(void)
/*
  GetModel() returns the exposure model of the exposure function.
*/
{ // GetModel
  return(Model);
} // GetModel



void GVSensorNetwork2D::SetPower(double Value)
/*
  SetPower() sets the Power for the exposure function to Value if it is valid.
*/
{ // SetPower
  if (Value < GVSENSORNETWORK_MINPOWER) Value = GVSENSORNETWORK_MINPOWER;
  Power = Value;
} // SetPower



void GVSensorNetwork2D::SetExposureModel(int E)
/*
  SetExposureModel() sets the exposure model to E (or a valid exposure model)
*/
{ // SetExposureModel
  if ((E < GVSENSORNETWORK_MODEL_CLOSEST) || (E >= GVSENSORNETWORK_MODEL_NUM)) E = GVSENSORNETWORK_MODEL_CLOSEST;
  Model = E;
} // SetExposureModel



void GVSensorNetwork2D::SetMinCoordinate(double X, double Y)
/*
  SetMinCoordinate() sets the minimum coordinate of the sensor network.
*/
{ // SetMinCoordinate
  Min.New(X, Y);
} // SetMinCoordinate



void GVSensorNetwork2D::SetMaxCoordinate(double X, double Y)
/*
  SetMaxCoordinate() sets the maximum coordinate of the sensor network.
*/
{ // SetMaxCoordinate
  Max.New(X, Y);
} // SetMaxCoordinate



void GVSensorNetwork2D::SetRadius(double R)
/*
  SetRadius() sets the radius to R if it is larger than the minimum radius,
  otherwise it sets the radius to the minimum radius.  It also recalculates
  the edges so that they are always up-to-date.
*/
{ // SetRadius
  if (R < GVSENSORNETWORK_MINRADIUS) R = GVSENSORNETWORK_MINRADIUS;
  Radius = R;
  CalculateEdges();
} // SetRadius



double GVSensorNetwork2D::CalculateExposure(double X, double Y)
/*
  CalculateExposure() calculates the exposure of a given point with respect
  to an exposure model.
*/
{ // CalculateExposure
  int i = 0;                                   // Index Variable
  double D = 0.0;                              // Current Distance
  double MinD = GVSENSORNETWORK_INFINITY;      // Minimum Distance
  double ReturnVal = 0.0;                      // Return Value
  double NX = X - 0.25;                        // FIXED EXPOSURE - Offset With Respect to Starting Point
  double NY = Y - 0.25;                        // FIXED EXPOSURE - Offset With Respect to Starting Point
  double SH = ((NX * .05 + NY * .05) / 0.005); // FIXED EXPOSURE - Non-Integer Grid Square
  double SV = ((NY * .05 - NX * .05) / 0.005); // FIXED EXPOSURE - Non-Integer Grid Square
  int SquareH = (int) SH;                      // FIXED EXPOSURE - Integer Grid Square
  int SquareV = (int) SV;                      // FIXED EXPOSURE - Integer Grid Square
  if (SH < 0) SquareH -= 1;
  if (SV < 0) SquareV -= 1;

  if (Sensors != NULL)
  {
    switch(Model)
    {
      case GVSENSORNETWORK_MODEL_ALL:
        for (i = 0; i < NumSensors; ++i)
        {
          D = Distance3D(X, Y, 0.0, Sensors[i].GetX(), Sensors[i].GetY(), GVSENSORNETWORK_MINHEIGHT);
          ReturnVal += pow(D, -Power);
        }
      break;

      case GVSENSORNETWORK_MODEL_CLOSEST:
      default:
        for (i = 0; i < NumSensors; ++i)
        {
          D = Distance3D(X, Y, 0.0, Sensors[i].GetX(), Sensors[i].GetY(), GVSENSORNETWORK_MINHEIGHT);
          if (D < MinD) MinD = D;
        }
        ReturnVal = pow(MinD, -Power);
      break;

      case GVSENSORNETWORK_MODEL_FIXED:
        if ((SquareV == 0) && (((SquareH >= 0) && (SquareH <= 3)) || ((SquareH >= 6) && (SquareH <= 9)))) ReturnVal = 1.0;
        else if ((SquareV == 1) && (SquareH >= 3) && (SquareH <= 6)) ReturnVal = 1.0;
      break;
    }
  }

  return(ReturnVal);
} // CalculateExposure


void GVSensorNetwork2D::CalculateBreadthFirstSearch(int StartIndex)
/*
  CalculateBreadthFirstSearch() performs a breadth first search on the sensor
  network known sensors.  The result is that the sensor attributes (Cost and
  Parent) are updated to reflect the breadth first serach cost from StartIndex
  to each sensor and the associated parent of each sensor.
*/
{ // CalculateBreadthFirstSearch
  int i = 0;                 // Index Variable
  int CurIndex = StartIndex; // Current Sensor Being Examined in Breadth First Search
  int NbrIndex = 0;          // Neighboring Sensor to be Examined in Breadth First Search
  int CurQueue = 0;          // Current Position in Search Queue
  int NumQueue = 0;          // Number of Elements in Search Queue
  int* IndexQueue = NULL;    // Search Queue for Breadth First Search

  if ((StartIndex >= 0) && (StartIndex < NumSensors))
  {
    // Initialize Breadth-First Search
    IndexQueue = new int[NumSensors];
    for (i = 0; i < NumSensors; ++i) Sensors[i].Initialize();
    IndexQueue[NumQueue++] = CurIndex;
    Sensors[CurIndex].SetVisited(1);

    // Perform the Breadth-First Search
    while(CurQueue < NumQueue)
    {
      CurIndex = IndexQueue[CurQueue];
      for (i = 0; i < NumNeighbors[CurIndex]; ++i)
      {
        NbrIndex = NeighborIndex[CurIndex][i];
        if ((Sensors[NbrIndex].GetKnown()) && (!Sensors[NbrIndex].GetVisited()))
        {
          IndexQueue[NumQueue++] = NbrIndex;
          Sensors[NbrIndex].SetCost(Sensors[CurIndex].GetCost() + 1);
          Sensors[NbrIndex].SetParent(CurIndex);
          Sensors[NbrIndex].SetVisited(1);
        }
      }
      ++CurQueue;
    }
  }
  if (IndexQueue != NULL) delete[] IndexQueue;
} // CalculateBreadthFirstSearch


int GVSensorNetwork2D::CalculateNearestSensor(double X, double Y)
/*
  CalculateNearestSensor() calculates the nearest sensor to point (X, Y).
*/
{ // CalculateNearestSensor
  int i = 0;                                     // Index Variable
  int ReturnVal = -1;                            // Closest Sensor
  double D = 0.0;                                // Current Distance
  double MinDistance = GVSENSORNETWORK_INFINITY; // Closest Sensor Distance

  for (i = 0; i < NumSensors; ++i)
  {
    D = Distance2D(X, Y, Sensors[i].GetX(), Sensors[i].GetY());
    if (D < MinDistance)
    {
      MinDistance = D;
      ReturnVal = i;
    }
  }
  return(ReturnVal);
} // CalculateNearestSensor