////////////////////////////////////////////////////////////////////////////////
// GVSensorNetwork2D.h                                                        //
//                                                                            //
// Header file for a sensor network (graph whose edges are defined by a       //
// communication radius value.  Exposure is defined as a function of the      //
// distance to the sensor (graph vertex).                                     //
////////////////////////////////////////////////////////////////////////////////



#ifndef GVSENSORNETWORK_H
#define GVSENSORNETWORK_H



// Includes ////////////////////////////////////////////////////////////////////
#include "GVUtil.h"
#include "GVSensor2D.h"



// Defines /////////////////////////////////////////////////////////////////////
#define GVSENSORNETWORK_MODEL_CLOSEST      (0                                )
#define GVSENSORNETWORK_MODEL_ALL          (GVSENSORNETWORK_MODEL_CLOSEST + 1)
#define GVSENSORNETWORK_MODEL_FIXED        (GVSENSORNETWORK_MODEL_ALL     + 1)
#define GVSENSORNETWORK_MODEL_NUM          (GVSENSORNETWORK_MODEL_FIXED   + 1)
#define GVSENSORNETWORK_INFINITY           1000000.0
#define GVSENSORNETWORK_MINPOWER           0.0
#define GVSENSORNETWORK_MINRADIUS          0.0
#define GVSENSORNETWORK_MINDISTANCE        0.01
#define GVSENSORNETWORK_MINHEIGHT          0.05
#define GVSENSORNETWORK_MINX               0.0
#define GVSENSORNETWORK_MINY               0.0
#define GVSENSORNETWORK_MAXX               1.0
#define GVSENSORNETWORK_MAXY               1.0
#define GVSENSORNETWORK_DEFAULT_NUMSENSORS 0
#define GVSENSORNETWORK_DEFAULT_RADIUS     0.05
#define GVSENSORNETWORK_DEFAULT_POWER      2.0
#define GVSENSORNETWORK_DEFAULT_MODEL      GVSENSORNETWORK_MODEL_CLOSEST



// GVSensorNetwork /////////////////////////////////////////////////////////////
class GVSensorNetwork2D
{ // GVSensorNetwork
  protected:
    int NumSensors;            // Number of Sensors in the Network
    int Model;                 // Exposure Model for the Network (ALL/CLOSEST)
    int* NumNeighbors;         // Array Containing Number of Neighbors For Each Sensor
    int** NeighborIndex;       // Two-Dimensional Array Storing Neighbors Of All Sensors
    double Radius;             // Radius To Determine Edges
    double Power;              // Power To Determine Exposure
    GVPoint2D Min;             // Minimum Coordinate To Generate Points in Sensor Network
    GVPoint2D Max;             // Maximum Coordinate To Generate Points in Sensor Network
    GVSensor2D* Sensors;       // Array of Sensors

    void CalculateEdges(void);                                                // Calculates the Edges and Neighbors of Sensors in Network
    void CalculateBreadthFirstSearch(int StartIndex);                         // Calculates a Breadth First Search Starting at StartIndex

  public:
    GVSensorNetwork2D();        // Creates an Empty Sensor Network
    ~GVSensorNetwork2D();       // Deletes a Sensor Network
    void New(int NS, double R); // Creates a New Sensor Network
    void Delete(void);          // Deletes a Sensor Network

    int GetNumSensors(void);                // Returns the Number of Sensors
    int GetNumNeighbors(int Index);         // Returns the Number of Neighbors for a Sensor
    int GetNeighbor(int Index, int NIndex); // Returns the Index'th Neighbor of a Sensor
    int GetModel(void);                     // Returns the Model Used to Calculate Exposure
    double GetX(int Index);                 // Returns the X Coordinate of a Sensor
    double GetY(int Index);                 // Returns the Y Coordinate of a Sensor
    double GetMinX(void);                   // Returns the Minimum X Coordinate
    double GetMinY(void);                   // Returns the Minimum Y Coordinate
    double GetMaxX(void);                   // Returns the Maximum X Coordinate
    double GetMaxY(void);                   // Returns the Maximum Y Coordinate
    double GetPower(void);                  // Returns the Power Used for Exposure

    void SetPower(double Value);               // Sets the Exposure Power Value
    void SetRadius(double R);                  // Sets the Communication Radius
    void SetExposureModel(int E);              // Sets the Exposure Model
    void SetMinCoordinate(double X, double Y); // Sets the Minimum Coordinate for Generating Sensors
    void SetMaxCoordinate(double X, double Y); // Sets the Maximum Coordinate for Generating Sensors

    int CalculateNearestSensor(double X, double Y); // Calculates the Nearest Sensor to Point (X, Y).
    double CalculateExposure(double X, double Y);   // Calculates the Exposure for a Point
}; // GVSensorNetwork2D



#endif