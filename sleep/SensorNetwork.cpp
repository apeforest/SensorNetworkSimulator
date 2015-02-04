// SensorNetwork.cpp //////////////////////////////////////////////////////////
// Giacomino Veltri                                                          //
// January 12, 2001                                                          //
//                                                                           //
// This file contains the data structure for a generic sensor network.       //
///////////////////////////////////////////////////////////////////////////////



#include "StdAfx.h"
#include "SensorNetwork.h"
#include <iostream.h>
#include <math.h>



// Sensor Class Implementation ////////////////////////////////////////////////
SensorNetwork::SensorNetwork()
{ // SensorNetwork
  Radius = 0;
  NumSensors = 0;
  SensorArray = NULL;
  EdgeTable = NULL;
} // SensorNetwork



SensorNetwork::~SensorNetwork()
{ // ~SensorNetwork
  Delete();
} // ~SensorNetwork



void SensorNetwork::Delete()
{ // Delete
  if (SensorArray != NULL)
  {
    delete[] SensorArray;
    SensorArray = NULL;
  }
  if (EdgeTable != NULL)
  {
    delete[] EdgeTable;
    EdgeTable = NULL;
  }
  Radius = 0;
  NumSensors = 0;
} // Delete



double SensorNetwork::Distance(double X1, double Y1, double Z1, double X2, double Y2, double Z2)
{ // Distance
  return(sqrt((X1-X2)*(X1-X2) + (Y1-Y2)*(Y1-Y2) + (Z1-Z2)*(Z1-Z2)));
} // Distance



void SensorNetwork::InitializeSensorNetwork(int Num, double Rad)
{ // InitializeSensorNetwork
  int i = 0;
  int j = 0;

  Delete();
  NumSensors = Num;
  Radius = Rad;
  SensorArray = new Sensor[NumSensors];
  EdgeTable = new int[NumSensors * NumSensors];

  for (i = 0; i < NumSensors; ++i)
  {
    for (j = 0; j < NumSensors; ++j)
    {
      *(EdgeTable + j + i * NumSensors) = -1;
    }
  }
} // InitializeSensorNetwork



void SensorNetwork::CalculateEdges(void)
{ // CalculateEdges
  int i = 0;        // Index Variable
  int j = 0;        // Index Variable
  int Current = 0;  // Current Number of Edges in List
  

  for (i = 0; i < NumSensors; ++i)
  {
    for (j = 0; j < NumSensors; ++j)
    {
      *(EdgeTable + j + i * NumSensors) = -1;
    }
  }
  for (i = 0; i < NumSensors; ++i)
  {
    Current = 0;
    for (j = 0; j < NumSensors; ++j)
    {
      if (i != j)
      {
        if (Distance(SensorArray[i].X, SensorArray[i].Y, SensorArray[i].Z,SensorArray[j].X, SensorArray[j].Y, SensorArray[j].Z) <= Radius)
        {
          *(EdgeTable + Current + i * NumSensors) = j;
          ++Current;
        }
      }
    }
  }
} // CalculateEdges