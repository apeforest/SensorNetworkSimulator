// Sensor Header File //////////////////////////////////////////////////////////
#include <fstream.h>
#include "StdAfx.h"

#ifndef SENSOR_H
#define SENSOR_H

class Sensor
{  // Sensor
  public:
    double x;    // X Coordinate
    double y;    // Y Coordinate
    int Beacon;  // Beacon Status
    double dx;   // Error in X Coordinate
    double dy;   // Error in Y Coordinate

    Sensor()
    {
      x = 0;
      y = 0;
      Beacon = 0;
      dx = 0;
      dy = 0;
    }
}; // Sensor
 
#endif
