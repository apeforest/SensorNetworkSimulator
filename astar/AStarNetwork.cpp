// AStarNetwork.cpp ///////////////////////////////////////////////////////////
// Giacomino Veltri                                                          //
// January 12, 2001                                                          //
//                                                                           //
// This file contains a sensor network that evaluates sleep algorithms.      //
///////////////////////////////////////////////////////////////////////////////



#include "StdAfx.h"
#include "AStarNetwork.h"
#include <math.h>



// AStarMessageClass Implementation ///////////////////////////////////////////
AStarMessage::AStarMessage()
/*
  Initialize the Beginning Message
*/
{ // AStarMessage
  NextActiveNode = -1;
  Recipient = -1;
  NumCommunications = 0;
  NumVisited = 0;
} // AStarMessage



AStarMessage::~AStarMessage()
/*
  This function De-Initializes an AStar Message
*/
{ // ~AStarMessage
} // ~AStarMessage







// AStarNetwork Class Implementation //////////////////////////////////////////
AStarNetwork::AStarNetwork()
/*
  This function Initializes an AStarNetwork
*/
{ // AStarNetwork
  FWParent = NULL;
  FWWeight = NULL;
  InitializeAStarNetwork(0, 0, 0, -1, -1, 0);
} // AStarNetwork



AStarNetwork::AStarNetwork(int Mode, int Num, double Rad, int Start, int Dest, double PercentSleep)
/*
  This function Initializes an AStarNetwork
*/
{ // AStarNetwork
  FWParent = NULL;
  FWWeight = NULL;
  InitializeAStarNetwork(Mode, Num, Rad, Start, Dest, PercentSleep);
} // AStarNetwork



AStarNetwork::~AStarNetwork()
/*
  This function Deinitializes an AStarNetwork
*/
{ // ~AStarNetwork
  Delete();
} // ~AStarNetwork



void AStarNetwork::Delete(void)
/*
  This function deletes the dynamically allocated arrays for
  the Floyd-Warshall computations and erases the AStar Network
*/
{ // AStarNetwork
  if (FWParent != NULL) delete FWParent;
  if (FWWeight != NULL) delete FWWeight;
  FWParent = NULL;
  FWWeight = NULL;
  SensorNetwork::Delete();
} // AStarNetwork



void AStarNetwork::InitializeAStarNetwork(int Mode, int Num, double Rad, int Start, int Dest, double PercentSleep)
/*
  This function Initializes an AStarNetwork with the specified values.
*/
{ // InitializeAStarNetwork
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
  CalculateEdges();
  CalculateSleepNodes(PercentSleep);
  InitializeFW();
  InitializeAStar(Start, Dest);
} // InitializeAStarNetwork



void AStarNetwork::InitializeFW(void)
/*
  This function initializes the Floyd-Warshall algorithm components.
*/
{ // InitializeFW
  int i = 0; // Index Variable
  int j = 0; // Index Variable

  // Create New Arrays
  if (NumSensors > 0)
  {
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
    CalculateFW();
  }
} // InitializeFW



void AStarNetwork::CalculateFW(void)
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



void AStarNetwork::CalculateSleepNodes(double PercentSleep)
/*
  This function sets a certain number of nodes to sleep.
*/
{ // CalculateSleepNodes
  int SleepNodes = ((int) (NumSensors * PercentSleep));  // Total Asleep
  int Current = 0;                                       // Current Asleep
  int Node = 0;                                          // Current Node

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



void AStarNetwork::InitializeStandard(int Num, double Rad)
/*
  This is a standard initialization for All Initialization Routines
*/
{ // InitializeStandard
  InitializeSensorNetwork(Num, Rad);
} // InitializeStandard



void AStarNetwork::InitializeManual()
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



void AStarNetwork::InitializeRandom2D(void)
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



void AStarNetwork::InitializeGrid(void)
/*
  This function initializes the sensors in a grid formation
*/
{ // InitializeGrid
  int i = 0;                            // Index Variable
  int j = 0;                            // Index Variable
  int Size = int(sqrt(NumSensors));     // Size of Grid
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



void AStarNetwork::InitializeAStar(int Start, int Dest)
/*
  This function initializes an AStar search by clearing the message
  communicated and setting the start and destination nodes.
*/
{ // InitializeAStar
  int i = 0; // Initialize AStar

  // Initialize the Message Queue
  Message.Queue.Init();
  Message.NextActiveNode = Start;
  Message.Recipient = Start;
  Message.NumCommunications = 0;
  // Set the Source and Destination Nodes
  CurrentNode = Start;
  DestinationNode = Dest;
  // Initialize the Actual Sensors
  for (i = 0; i < NumSensors; ++i)
  {
    SensorArray[i].Parent = -1;
    SensorArray[i].Enqueued = 0;
    SensorArray[i].Cost = NumSensors + 1;
  }
  if ((NumSensors >= 0) && (Start >= 0) && (Start < NumSensors))
  {
    SensorArray[Start].GCost = 0;
    SensorArray[Start].HCost = 0;
    SensorArray[Start].Cost = 0;
    SensorArray[Start].Enqueued = 1;
    SensorArray[Start].Mode = SENSOR_MODE_ON;
    Message.NumVisited = 1;
  }
  if ((NumSensors >= 0) && (DestinationNode >= 0) && (DestinationNode < NumSensors))
  {
    SensorArray[DestinationNode].Mode = SENSOR_MODE_ON;
  }
} // InitializeAStar



int AStarNetwork::CycleAStar(int Version)
/*
  This funciton cycles through one iteration of the AStar search.
  It goes one iteration at a time so that the graphics can be updated
  externally.
*/
{ // CycleAStar
  int i = 0;           // Index Node
  int Neighbor = -1;   // Neighbor of Current Node
  int Complete = 0;    // Is the Cycling Complete?
  double NewCost = 0;  // New Cost for Node
  double NewGCost = 0; // New Prior Cost
  double NewHCost = 0; // New Heuristic Cost

  if ((CurrentNode == DestinationNode) || (CurrentNode < 0))
  {
    Complete = 1;
  }
  else if (CurrentNode == Message.Recipient)
  {
    // Dequeue the Current Node
    Message.Queue.Dequeue(CurrentNode);
    if (CurrentNode == Message.NextActiveNode)
    {
      // Mark the Node as Visited
      SensorArray[CurrentNode].Enqueued = 1;
      ++Message.NumVisited;
      // Update Each Neighbor
      NewGCost = SensorArray[CurrentNode].GCost + 1;
      for (i = 0, Neighbor = *(EdgeTable + CurrentNode * NumSensors + i); Neighbor >= 0; Neighbor = *(EdgeTable + CurrentNode * NumSensors + (++i)))
      {
        // Calculate the New Cost for the Neighbor
        switch(Version % ASTAR_VERSION_NUM)
        {
          case ASTAR_VERSION_NORMAL:
            NewHCost = Distance(SensorArray[Neighbor].X, SensorArray[Neighbor].Y, SensorArray[Neighbor].Z, SensorArray[DestinationNode].X, SensorArray[DestinationNode].Y, SensorArray[DestinationNode].Z) / Radius;
          break;
          case ASTAR_VERSION_SHORT:
            NewHCost = *(FWWeight + Neighbor * NumSensors + DestinationNode);
          break;
          default:
            NewHCost = Distance(SensorArray[Neighbor].X, SensorArray[Neighbor].Y, SensorArray[Neighbor].Z, SensorArray[DestinationNode].X, SensorArray[DestinationNode].Y, SensorArray[DestinationNode].Z) / Radius;
          break;
        }
        NewCost = NewGCost + NewHCost;
        // Update the Cost if Necessary
        if ((NewCost < SensorArray[Neighbor].Cost) && (SensorArray[Neighbor].Mode == SENSOR_MODE_ON))
        {
          Message.Queue.Dequeue(Neighbor);
          SensorArray[Neighbor].GCost = NewGCost;
          SensorArray[Neighbor].HCost = NewHCost;
          SensorArray[Neighbor].Cost = NewCost;
          SensorArray[Neighbor].Parent = CurrentNode;
          Message.Queue.Enqueue(Neighbor, NewCost);
        }
      }
    }
    // Set the Next Active Node and Recipient
    if (Message.Queue.Head->Next != NULL)
    {
      Message.NextActiveNode = Message.Queue.Head->Next->NodeNumber;
    }
    else
    {
      Complete = 1;
    }
    CurrentNode = NextNodeInPath(Version, CurrentNode, Message.NextActiveNode);
    // Send the Message and Increment the Number of Sent Messages
    Message.Recipient = CurrentNode;
    ++Message.NumCommunications;
  }
  // Return if the Algorithm is Complete
  return(Complete);
} // CycleAStar



void AStarNetwork::CalcNumHops(void)
/*
  This function returns the number of *Intermediate* nodes on the path from the
  source to the destination.  That's why NumHops starts at -2, and < 0 means that
  there is no path from the source to the destination.  If the NumHops is 0, it
  means that the source and destination are immediately connected.
*/
{ // CalcNumHops
  int Current = 0;
  
  for (Current = DestinationNode, NumHops = -2; Current >= 0; Current = SensorArray[Current].Parent)
  {
    ++NumHops;
  }
} // CalcNumHops



int AStarNetwork::NextNodeInPath(int Version, int Start, int Dest)
/*
  This function returns the next node in the path.  It first calculates
  to see if the two nodes are connected by the current path, and if they
  are, it chooses the node on the path to the destination, otherwise, it
  chooses the parent node of the starting node (i.e. go back a level).
  If the parent node is unavailable, it returns the current node.
*/
{ // NextNodeInPath
  int CurrentNode = Dest; // Current Node In Path

  while ((SensorArray[CurrentNode].Parent != -1) && (SensorArray[CurrentNode].Parent != Start))
  {
    CurrentNode = SensorArray[CurrentNode].Parent;
  }
  // If the Node Is Not on the Path, Set the Current Node to the Parent (if Available)
  if ((SensorArray[CurrentNode].Parent == -1) && (SensorArray[Start].Parent != -1))
  {
    CurrentNode = SensorArray[Start].Parent;
  }
  // Return the Current Node (Next Node in Path)
  return(CurrentNode);
} // NextNodeInPath