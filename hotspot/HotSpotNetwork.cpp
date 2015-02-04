// HotSpotNetwork.cpp /////////////////////////////////////////////////////////
// Giacomino Veltri                                                          //
// January 12, 2001                                                          //
//                                                                           //
// This file contains a sensor network that evaluates sleep algorithms.      //
///////////////////////////////////////////////////////////////////////////////



#include "StdAfx.h"
#include "HotSpotNetwork.h"
#include <math.h>



// HotSpotNetwork Class Implementation /////////////////////////////////////////
HotSpotNetwork::HotSpotNetwork()
/*
  This function Initializes an HotSpotNetwork
*/
{ // HotSpotNetwork
  FWParent = NULL;
  FWWeight = NULL;
  InitializeHotSpotNetwork(0, 0, 0, 0);
} // HotSpotNetwork



HotSpotNetwork::HotSpotNetworkInitializeHotSpotNetwork(int Mode, int Num, double Rad, double PercentSleep)
/*
  This function Initializes an HotSpotNetwork
*/
{ // HotSpotNetwork
  FWParent = NULL;
  FWWeight = NULL;
  InitializeHotSpotNetwork(Mode, Num, Rad, PercentSleep);
} // HotSpotNetwork



HotSpotNetwork::~HotSpotNetwork()
/*
  This function Deinitializes an HotSpotNetwork
*/
{ // ~HotSpotNetwork
  Delete();
} // ~HotSpotNetwork



void HotSpotNetwork::Delete(void)
/*
  This function deletes the dynamically allocated arrays for
  the Floyd-Warshall computations and erases the HotSpot Network
*/
{ // HotSpotNetwork
  if (FWParent != NULL) delete[] FWParent;
  if (FWWeight != NULL) delete[] FWWeight;
  FWParent = NULL;
  FWWeight = NULL;
  SensorNetwork::Delete();
} // HotSpotNetwork



void HotSpotNetwork::InitializeHotSpotNetwork(int Mode, int Num, double Rad, double PercentSleep)
/*
  This function Initializes an HotSpotNetwork with the specified values.
*/
{ // InitializeHotSpotNetwork
  Delete();
  switch(Mode)
  {
    case 0: // Random2D
      InitializeStandard(Num, Rad);
      InitializeRandom2D();
    break;
    case 1: // Grid
      InitializeStandard(Num, Rad);
      InitializeGrid();
    break;
    case 2: // Manual
      InitializeManual();
    break;
    default: // Random2D
      InitializeStandard(Num, Rad);
      InitializeRandom2D();
    break;
  }
  // Intialize BFS Arrays
  CalculateEdges();
  CalculateSleepNodes(PercentSleep);
  InitializeFW();
  InitializeHotSpot();
} // InitializeHotSpotNetwork



void HotSpotNetwork::InitializeFW(void)
/*
  This function initializes the Floyd-Warshall algorithm components.
*/
{ // InitializeFW
  int i = 0; // Index Variable
  int j = 0; // Index Variable

  // Create New Arrays
  if (NumSensors > 0)
  {
    if (FWWeight != NULL) delete[] FWWeight;
    if (FWParent != NULL) delete[] FWParent;

    FWWeight = new double[NumSensors * NumSensors];
    FWParent = new int[NumSensors * NumSensors];

    for (i = 0; i < NumSensors; ++i)
    {
      for (j = 0; j < NumSensors; ++j)
      {
        if (i == j)
        {
          // Weight is "No Hops", No Parent
          *(FWWeight + i * NumSensors + j) = 0;
          *(FWParent + i * NumSensors + j) = -1;
        }
        else if (Distance(SensorArray[i].X, SensorArray[i].Y, SensorArray[i].Z, SensorArray[j].X, SensorArray[j].Y, SensorArray[j].Z) <= Radius)
        {
          // Weight is "One Hop", Parent of j is i
          *(FWWeight + i * NumSensors + j) = 1;
          *(FWParent + i * NumSensors + j) = j;
        }
        else
        {
          // Weight is "Infinity", No Parent
          *(FWWeight + i * NumSensors + j) = NumSensors + 1;
          *(FWParent + i * NumSensors + j) = -1;
        }
      }
    }
    CalculateFWSleep();
  }
} // InitializeFW



void HotSpotNetwork::CalculateFW(void)
/*
  Run the Floyd-Warshal Algorithm.
*/
{ // CalculateFW
  int i = 0; // Index Variable
  int j = 0; // Index Variable
  int k = 0; // Index Variable

  for (k = 0; k < NumSensors; ++k)
  {
    for (i = 0; i < NumSensors; ++i)
    {
      for (j = 0; j < NumSensors; ++j)
      {
        if (*(FWWeight + NumSensors * i + k) + *(FWWeight + NumSensors * k + j) < *(FWWeight + NumSensors * i + j))
        {
          // Update the Weight and Parent
          *(FWWeight + NumSensors * i + j) = *(FWWeight + NumSensors * i + k) + *(FWWeight + NumSensors * k + j);
          *(FWParent + NumSensors * i + j) = *(FWParent + NumSensors * i + k);
        }
      }
    }
  }
} // CalculateFW



void HotSpotNetwork::CalculateFWSleep(void)
/*
  Run the Floyd-Warshal Algorithm, taking into consideration nodes that are
  asleep
*/
{ // CalculateFW
  int i = 0; // Index Variable
  int j = 0; // Index Variable
  int k = 0; // Index Variable

  for (k = 0; k < NumSensors; ++k)
  {
    for (i = 0; i < NumSensors; ++i)
    {
      for (j = 0; j < NumSensors; ++j)
      {
        if ((SensorArray[k].Mode == SENSOR_MODE_ON) && (*(FWWeight + NumSensors * i + k) + *(FWWeight + NumSensors * k + j) < *(FWWeight + NumSensors * i + j)))
        {
          // Update the Weight and Parent
          *(FWWeight + NumSensors * i + j) = *(FWWeight + NumSensors * i + k) + *(FWWeight + NumSensors * k + j);
          *(FWParent + NumSensors * i + j) = *(FWParent + NumSensors * i + k);
        }
      }
    }
  }
} // CalculateFW



void HotSpotNetwork::CalculateSleepNodes(double PercentSleep)
/*
  This function sets a certain number of nodes to sleep.
*/
{ // CalculateSleepNodes
  int SleepNodes = ((int) (NumSensors * PercentSleep));  // Total Asleep
  int Current = 0;                                       // Current Asleep
  int Node = 0;                                          // Current Node
  int i = 0;                                             // Index Variable

  for (i = 0; i < NumSensors; ++i)
  {
    if (SensorArray[i].Mode == SENSOR_MODE_SLEEP)
    {
      SensorArray[i].Mode = SENSOR_MODE_ON;
    }
  }

  if ((SleepNodes < 0) || (SleepNodes > NumSensors)) SleepNodes = 0;
  while (Current < SleepNodes)
  {
    Node = rand() % NumSensors;
    if (SensorArray[Node].Mode != SENSOR_MODE_SLEEP)
    {
      SensorArray[Node].Mode = SENSOR_MODE_SLEEP;
      ++Current;
    }
  }
} // CalculateSleepNodes



void HotSpotNetwork::InitializeStandard(int Num, double Rad)
/*
  This is a standard initialization for All Initialization Routines
*/
{ // InitializeStandard
  InitializeSensorNetwork(Num, Rad);
} // InitializeStandard



void HotSpotNetwork::InitializeManual()
/*
  This is a Manual Initialization of the Sensor Network
*/
{ // InitializeManual
  InitializeStandard(100, 0.1);
  InitializeGrid();
  SensorArray[17].Type = SENSOR_TYPE_CONSUMER;
  SensorArray[27].Type = SENSOR_TYPE_CONSUMER;
  SensorArray[28].Type = SENSOR_TYPE_CONSUMER;
  SensorArray[83].Type = SENSOR_TYPE_CONSUMER;
  SensorArray[84].Type = SENSOR_TYPE_CONSUMER;
  SensorArray[45].Type = SENSOR_TYPE_PRODUCER;
} // InitializeManual



void HotSpotNetwork::InitializeRandom2D(void)
/*
  This initializes the sensor network with Random 2D Positions.
*/
{ // InitializeRandom2D
  int i = 0;            // Index Variable
  int Accuracy = 10000; // Accuracy of Digits
  double Temp = 0;      // Temporary Variable

  for (i = 0; i < NumSensors; ++i)
  {
    SensorArray[i].X = double(rand() % (Accuracy + 1))/(double(Accuracy));
    SensorArray[i].Y = double(rand() % (Accuracy + 1))/(double(Accuracy));
  }
} // InitializeRandom2D



void HotSpotNetwork::InitializeGrid(void)
/*
  This function initializes the sensors in a grid formation
*/
{ // InitializeGrid
  int i = 0;                                // Index Variable
  int j = 0;                                // Index Variable
  int Size = int(sqrt(NumSensors));         // Size of Grid
  double Space = 1.0 / (double (Size + 1)); // Space Between Grid Vertices

  NumSensors = Size * Size;
  for (i = 0; i < Size; ++i)
  {
    for (j = 0; j < Size; ++j)
    {
      SensorArray[j + Size * i].X = (j + 1) * Space;
      SensorArray[j + Size * i].Y = (i + 1) * Space;
    }
  }
} // InitializeGrid



void HotSpotNetwork::InitializeHotSpot(void)
/*
  This function initializes an HotSpot search by clearing the message
  communicated and setting the start and destination nodes.
*/
{ // InitializeHotSpot
  int i = 0;                            // Index Variable
  int j = 0;                            // Index Variable
  int k = 0;                            // Index Variable
  double OldWeight = 0;                 // Old FW Weight
  double NewWeight = 0;                 // New FW Weight
  double Max = 0;                       // Maximum Cost Set To 100%
  double Min = NumSensors * NumSensors; // Minimum Cost Set To 0%
  double Range = 0;                     // Range Of Values

  // Initialize The Values
  for (i = 0; i < NumSensors; ++i)
  {
    SensorArray[i].Cost = 0;
  }

  // Calculate The Values
  for (i = 0; i < NumSensors; ++i)
  {
    for (j = 0; j < NumSensors; ++j)
    {
      OldWeight = *(FWWeight + NumSensors * i + j);
      for (k = 0; k < NumSensors; ++k)
      {
        NewWeight = *(FWWeight + NumSensors * i + k) + *(FWWeight + NumSensors * k + j);;
        if (NewWeight <= OldWeight)
        {
          SensorArray[k].Cost += 1.0;
        }
      }
    }
  }

  // Normalize The Values
  for (i = 0; i < NumSensors; ++i)
  {
    if (SensorArray[i].Cost > Max)
    {
      Max = SensorArray[i].Cost;
    }
    if (SensorArray[i].Cost < Min)
    {
      Min = SensorArray[i].Cost;
    }
  }
  Range = Max - Min;
  if (Range > 0)
  {
    for (i = 0; i < NumSensors; ++i)
    {
      SensorArray[i].Cost -= Min;
      SensorArray[i].Cost = SensorArray[i].Cost / Range;
    }
  }
} // InitializeHotSpot
