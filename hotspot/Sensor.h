// Sensor.h ///////////////////////////////////////////////////////////////////
// Giacomino Veltri                                                          //
// January 12, 2001                                                          //
//                                                                           //
// This file contains the data structure for a generic sensor.               //
///////////////////////////////////////////////////////////////////////////////



#ifndef SENSOR_H
#define SENSOR_H



// Constants //////////////////////////////////////////////////////////////////
#define SENSOR_TYPE_PRODUCER    0
#define SENSOR_TYPE_LISTENER    1
#define SENSOR_TYPE_CONSUMER    2
#define SENSOR_MODE_OFF         0
#define SENSOR_MODE_INFLUENCED  1
#define SENSOR_MODE_SLEEP       2
#define SENSOR_MODE_STANDBY     3
#define SENSOR_MODE_ON          4
#define SENSOR_MSTTYPE_NONE     0
#define SENSOR_MSTTYPE_INTERIOR 1
#define SENSOR_MSTTYPE_LEAF     2
#define SENSOR_STATUS_LISTENING 0
#define SENSOR_STATUS_HEARD     1
#define SENSOR_STATUS_DONE      2

// Sensor Class ///////////////////////////////////////////////////////////////
class Sensor
{ // Sensor
  public:
    int Type;      // Main Type of Sensor
    int SubType;   // Sub Type of Sensor
    int Parent;    // Parent Node for Routing
    int Status;    // Status of Sensor
    int Mode;      // Mode Sensor is in (Off, Influence, Sleep, StandBy, On)
    int Enqueued;  // Enqued Flag for Breadth-First-Search
    double Cost;   // Cost
    double GCost;  // Prior Cost
    double HCost;  // Heuristic Cost
    double X;      // X Coordinate of Sensor
    double Y;      // Y Coordinate of Sensor
    double Z;      // Z Coordinate of Sensor

    // Constructors/Destructors
    Sensor();
    Sensor(double SX, double SY, double SZ, int T, int M, int P, int SubT, int ST);
    ~Sensor();

    // Initialization
    void Init(double SX, double SY, double SZ, int T, int M, int P, int SubT, int ST);
}; // Sensor



#endif