// AStarNetwork.h /////////////////////////////////////////////////////////////
// Giacomino Veltri                                                          //
// January 12, 2001                                                          //
//                                                                           //
// This file contains a sensor network that evaluates sleep algorithms.      //
///////////////////////////////////////////////////////////////////////////////



#include "SensorNetwork.h"
#include "SensorQueue.h"


#ifndef ASTARNETWORK_H
#define ASTARNETWORK_H



// Define //////////////////////////////////////////////////////////////////////
#define ASTAR_VERSION_NORMAL 0
#define ASTAR_VERSION_SHORT  1
#define ASTAR_VERSION_NUM    2



// AStarMessage Class /////////////////////////////////////////////////////////
class AStarMessage
{ // AStarMessage
  public:
    int NextActiveNode;     // Next Active Node in Network
    int Recipient;          // Recipient of Message
    int NumCommunications;  // Number of Communications
    int NumVisited;         // Number of Visited Nodes
    SensorQueue Queue;      // Queue for AStar Search

    AStarMessage();
    ~AStarMessage();
}; // AStarMessage



// AStarNetwork Class /////////////////////////////////////////////////////////
class AStarNetwork: public SensorNetwork
{ // AStarNetwork
  public:
    int CurrentNode;      // Current Node with Message
    int DestinationNode;  // Destination Node
    int NumHops;          // Number of Hops between Source and Dest
    int* FWParent;        // Floyd-Warshall Parent Array
    double* FWWeight;     // Floyd-Warshall Weight Array
    AStarMessage Message; // Message To Be "Sent" Between Nodes

    // Constructors/Destructors
    AStarNetwork();
    AStarNetwork(int Mode, int Num, double Rad, int Start, int Dest, double PercentSleep);
    ~AStarNetwork();

    // Initialization Functions
    void InitializeAStarNetwork(int Mode, int Num, double Rad, int Start, int Dest, double PercentSleep);
    void InitializeStandard(int Num, double Rad);
    void InitializeManual(void);
    void InitializeRandom2D(void);
    void InitializeGrid(void);
    void InitializeFW(void);
    void CalculateFW(void);
    void CalculateSleepNodes(double PercentSleep);
    void CalcNumHops(void);
    void Delete(void);

    // Algorithm Functions
    void InitializeAStar(int Start, int Dest);
    int CycleAStar(int Version);
    int NextNodeInPath(int Version, int Start, int Dest);
}; // AStarNetwork



#endif