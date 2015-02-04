// SensorNetwork.h ////////////////////////////////////////////////////////////
// Giacomino Veltri                                                          //
// January 12, 2001                                                          //
//                                                                           //
// This file contains the data structure for a generic sensor network.       //
///////////////////////////////////////////////////////////////////////////////



#include "Sensor.h"



#ifndef SENSORNETWORK_H
#define SENSORNETWORK_H



// Sensor Class ///////////////////////////////////////////////////////////////
class SensorNetwork
{ // Sensor
  public:
    int NumSensors;      // Number of Sensors in Network
    int* EdgeTable;      // Table to Maintain Number of Edges
    double Radius;       // Communication Radius of Sensors
    Sensor* SensorArray; // Array to Maintain Sensor Data

    // Constructors/Destructors
    SensorNetwork();
    ~SensorNetwork();

    // Initialization and Deletion
    void InitializeSensorNetwork(int Num, double Rad);
    void Delete();

    // Functions
    double Distance(double X1, double Y1, double Z1, double X2, double Y2, double Z2);
    void CalculateEdges(void);
}; // SensorNetwork



#endif