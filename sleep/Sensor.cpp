// Sensor.cpp /////////////////////////////////////////////////////////////////
// Giacomino Veltri                                                          //
// January 12, 2002                                                          //
//                                                                           //
// This file contains the data structure for a generic sensor.               //
///////////////////////////////////////////////////////////////////////////////



#include "StdAfx.h"
#include "Sensor.h"



// Sensor Class Implementation ////////////////////////////////////////////////
Sensor::Sensor()
/*
  This function Initializes a Sensor with the Default Values
*/
{ // Sensor
  Init(0, 0, 0, SENSOR_TYPE_LISTENER, SENSOR_MODE_ON, -1, SENSOR_MSTTYPE_NONE, SENSOR_STATUS_LISTENING);
} // Sensor



Sensor::Sensor(double SX, double SY, double SZ, int T, int M, int P, int SubT, int ST)
/*
  This funciton initializes a Sensor with the provided Values.
*/
{ // Sensor
  Init(SX, SY, SZ, T, M, P, SubT, ST);
} // Sensor



Sensor::~Sensor()
/*
  This function de-initializes a sensor.
*/
{ // ~Sensor()
} // ~Sensor()



void Sensor::Init(double SX, double SY, double SZ, int T, int M, int P, int SubT, int ST)
/*
  This function re-initializes a sensor network
*/
{ // Sensor
  X = SX;
  Y = SY;
  Z = SZ;
  Type = T;
  Mode = M;
  Parent = P;
  Type = SubT;
  Status = ST;
  Enqueued = 0;
  Cost = 0;
  GCost = 0;
  HCost = 0;
} // Sensor
