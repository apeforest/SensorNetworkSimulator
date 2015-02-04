// HotSpotNetwork.h ///////////////////////////////////////////////////////////
// Giacomino Veltri                                                          //
// January 12, 2001                                                          //
//                                                                           //
// This file contains a sensor network that evaluates sleep algorithms.      //
///////////////////////////////////////////////////////////////////////////////



#include "SensorNetwork.h"
#include "SensorQueue.h"


#ifndef HOTSPOTNETWORK_H
#define HOTSPOTNETWORK_H



// Define //////////////////////////////////////////////////////////////////////
#define HOTSPOT_VERSION_NORMAL 0
#define HOTSPOT_VERSION_SHORT  1
#define HOTSPOT_VERSION_GSPR   2
#define HOTSPOT_VERSION_NUM    3
#define HOTSPOT_PI             3.14159

// HotSpotNetwork Class ////////////////////////////////////////////////////////
class HotSpotNetwork: public SensorNetwork
{ // HotSpotNetwork
  public:
    int* FWParent;        // Floyd-Warshall Parent Array
    double* FWWeight;     // Floyd-Warshall Weight Array

    // Constructors/Destructors
    HotSpotNetwork();
    HotSpotNetworkInitializeHotSpotNetwork(int Mode, int Num, double Rad, double PercentSleep);
    ~HotSpotNetwork();

    // Initialization Functions
    void InitializeHotSpotNetwork(int Mode, int Num, double Rad, double PercentSleep);
    void InitializeHotSpot(void);
    void InitializeStandard(int Num, double Rad);
    void InitializeManual(void);
    void InitializeRandom2D(void);
    void InitializeGrid(void);
    void InitializeFW(void);
    void CalculateFW(void);
    void CalculateFWSleep(void);
    void CalculateSleepNodes(double PercentSleep);
    void Delete(void);
}; // HotSpotNetwork



#endif