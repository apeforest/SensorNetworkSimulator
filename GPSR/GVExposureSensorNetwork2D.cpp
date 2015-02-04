////////////////////////////////////////////////////////////////////////////////
// GVExposureSensorNetwork2D.cpp                                              //
//                                                                            //
// Implementation file for exposure sensor network.                           //
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "GVExposureSensorNetwork2D.h"

// GVExposureSensorNetwork2D /////////////////////////////////////////////////////
int __cdecl CompareHoleGridPoints(const void* Element1, const void* Element2)
/*
  CompareHoleGridPoints() returns:
    -1: Element1 < Element2
     0: Element1 = Element2
     1: Element1 > Element2
*/
{ // CompareHoleGridPoints
  int ReturnVal = 0;
  double DAngle = ((GVExposureSensorNetwork2D_Hole*) Element1)->Angle - ((GVExposureSensorNetwork2D_Hole*) Element2)->Angle;

  if (DAngle > 0) ReturnVal = 1;
  else if (DAngle < 0) ReturnVal = -1;

	return(ReturnVal);
} // CompareHoleGridPoints



GVExposureSensorNetwork2D::GVExposureSensorNetwork2D()
/*
  GVExposureSensorNetwork2D() creates a blank exposure sensor network.
*/
{ // GVExposureSensorNetwork2D
  GVSensorNetwork2D::GVSensorNetwork2D();
  MinExposureInitialized = 0;
  MaxExposureInitialized = 0;
  GPSRInitialized = 0;
  LocMinExposureMode = GVEXPOSURESENSORNETWORK2D_LOCMINEXPOSUREMODE_DEFAULT;
  LocMaxExposureMode = GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_HEURISTIC;
  CenMinExposureMode = GVEXPOSURESENSORNETWORK2D_CENMINEXPOSUREMODE_DEFAULT;
  CenMaxExposureMode = GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_ADJUSTED;
  NumCommunications = 0;
  CurrentIteration = 0;
  CurrentNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
  NextNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
  SourceNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
  DestinationNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
  MessageMode = GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH;
  AlgorithmStage = GVEXPOSURESENSORNETWORK2D_MODE_INIT;
  AlgorithmMode = GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL;
  MaxLength = 0;
  NumCenMinPath = 0;
  NumCenMaxPath = 0;
  NumLocMinPath = 0;
  NumLocMaxPath = 0;
  CenMinPath = NULL;
  CenMaxPath = NULL;
  LocMinPath = NULL;
  LocMaxPath = NULL;
  GPSR_Path = NULL;
  Start.Delete();
  End.Delete();
  Grid.Delete();
  Delete();
} // GVExposureSensorNetwork2D



GVExposureSensorNetwork2D::~GVExposureSensorNetwork2D()
/*
  ~GVExposureSensorNetwork2D() deletes the exposure sensor network.
*/
{ // ~GVExposureSensorNetwork2D
  Delete();
} // ~GVExposureSensorNetwork2D



void GVExposureSensorNetwork2D::New(int NS, double R)
/*
  New() creates a new sensor network with NS number of sensors and communication
  radius of R
*/
{ // New
  Delete();
  GVSensorNetwork2D::New(NS, R);
} // New



void GVExposureSensorNetwork2D::Delete()
/*
  Delete() deletes the exposure sensor network.
*/
{ // Delete
  if (CenMinPath != NULL) delete[] CenMinPath;
  if (CenMaxPath != NULL) delete[] CenMaxPath;
  if (LocMinPath != NULL) delete[] LocMinPath;
  if (LocMaxPath != NULL) delete[] LocMaxPath;
  MinExposureInitialized = 0;
  MaxExposureInitialized = 0;
  GPSRInitialized = 0;
  LocMinExposureMode = GVEXPOSURESENSORNETWORK2D_LOCMINEXPOSUREMODE_DEFAULT;
  LocMaxExposureMode = GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_HEURISTIC;
  CenMinExposureMode = GVEXPOSURESENSORNETWORK2D_CENMINEXPOSUREMODE_DEFAULT;
  CenMaxExposureMode = GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_ADJUSTED;
  NumCommunications = 0;
  CurrentIteration = 0;
  CurrentNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
  NextNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
  SourceNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
  DestinationNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
  MessageMode = GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH;
  AlgorithmStage = GVEXPOSURESENSORNETWORK2D_MODE_INIT;
  AlgorithmMode = GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL;
  MaxLength = 0;
  NumCenMinPath = 0;
  NumCenMaxPath = 0;
  NumLocMinPath = 0;
  NumLocMaxPath = 0;
  CenMinPath = NULL;
  CenMaxPath = NULL;
  LocMinPath = NULL;
  LocMaxPath = NULL;
  GPSR_Path = NULL;
  Event = NULL;
  Start.Delete();
  End.Delete();
  Grid.Delete();
  GVSensorNetwork2D::Delete();
} // Delete



void GVExposureSensorNetwork2D::Reset(void)
/*
  Reset() marks the status of the algorihtm as uninitialized.
*/
{ // Reset
  MinExposureInitialized = 0;
  MaxExposureInitialized = 0;
  GPSRInitialized = 0;
  AlgorithmStage = GVEXPOSURESENSORNETWORK2D_MODE_INIT;
} // Reset



void GVExposureSensorNetwork2D::OutputResults(char* FileName)
/*
  OutputResults() calculates the required results and outputs them to the
  specified text file (FileName).
*/
{ // OutputResults
  int i = 0;                   // Index Variable
  int j = 0;                   // Index Variable
  int SensorsTotal = 0;        // Total Number of Sensors in Ellipse
  int SensorsUnknown = 0;      // Number of Sensors Unknown in Ellipse
  int SensorsKnown = 0;        // Number of Sensors Known (and Useless) in Ellipse
  int SensorsVisited = 0;      // Number of Sensors Visited in Ellipse
  int AreaTotal = 0;           // Total Area in Ellipse
  int AreaUncovered = 0;       // Total Uncovered Area in Ellipse
  int AreaCovered = 0;         // Total Covered Area in Ellipse
  int GridH = Grid.GetSizeH(); // Number of Horizontal Grid Squares
  int GridV = Grid.GetSizeV(); // Number of Vertical Grid Squares
  double D = 0.0;              // Temporary Distance
  double X = 0.0;              // Temporary X Coordinate
  double Y = 0.0;              // Temporary Y Coordinate
  double SX = Start.GetX();    // Starting X Coordinate
  double SY = Start.GetY();    // Starting Y Coordinate
  double EX = End.GetX();      // End X Coordinate
  double EY = End.GetY();      // End Y Coordinate
  ofstream OutFile;            // Output File

  // Calculate Sensor Information
  for (i = 0; i < NumSensors; ++i)
  {
    X = Sensors[i].GetX();
    Y = Sensors[i].GetY();
    D = Distance2D(X, Y, Start.GetX(), Start.GetY()) + Distance2D(X, Y, End.GetX(), End.GetY());
    if (D < MaxLength)
    {
      ++SensorsTotal;
      switch(Sensors[i].GetKnown())
      {
        case GVEXPOSURESENSORNETWORK2D_SENSOR_UNKNOWN:
          ++SensorsUnknown;
        break;

        case GVEXPOSURESENSORNETWORK2D_SENSOR_USELESS:
        case GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN:
          ++SensorsKnown;
        break;

        case GVEXPOSURESENSORNETWORK2D_SENSOR_VISITED:
          ++SensorsVisited;
        break;
      }
    }
  }

  // Calculate Area Information
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    {
      X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
      Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
      D = Distance2D(X, Y, SX, SY) + Distance2D(X, Y, EX, EY);
      if (D < MaxLength)
      {
        ++AreaTotal;
        switch(Grid.GetCovered(i, j))
        {
          case GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED:
            ++AreaUncovered;
          break;

          case GVEXPOSURESENSORNETWORK2D_GRID_COVERED:
            ++AreaCovered;
          break;
        }
      }
    }
  }

  // Output Results
  OutFile.open(FileName, ios::app);
  OutFile << "NS"         << GVEXPOSURESENSORNETWORK2D_SPACE
          << "R"             << GVEXPOSURESENSORNETWORK2D_SPACE
          << "H"    << GVEXPOSURESENSORNETWORK2D_SPACE
		  << "V"    << GVEXPOSURESENSORNETWORK2D_SPACE
          << "SX"       << GVEXPOSURESENSORNETWORK2D_SPACE
          << "SY"       << GVEXPOSURESENSORNETWORK2D_SPACE
          << "EX"         << GVEXPOSURESENSORNETWORK2D_SPACE
          << "EY"         << GVEXPOSURESENSORNETWORK2D_SPACE
          << "MaxL"          << GVEXPOSURESENSORNETWORK2D_SPACE
          << "SenTotal"       << GVEXPOSURESENSORNETWORK2D_SPACE
          << "SenUnknown"     << GVEXPOSURESENSORNETWORK2D_SPACE
          << "SenKnown"       << GVEXPOSURESENSORNETWORK2D_SPACE
          << "SenVisited"     << GVEXPOSURESENSORNETWORK2D_SPACE
          << "NumComms"  << GVEXPOSURESENSORNETWORK2D_SPACE
          << endl;

  OutFile << NumSensors         << GVEXPOSURESENSORNETWORK2D_SPACE
          << Radius             << GVEXPOSURESENSORNETWORK2D_SPACE
          << Grid.GetSizeH()    << GVEXPOSURESENSORNETWORK2D_SPACE
          << Grid.GetSizeV()    << GVEXPOSURESENSORNETWORK2D_SPACE
          << Start.GetX()       << GVEXPOSURESENSORNETWORK2D_SPACE
          << Start.GetY()       << GVEXPOSURESENSORNETWORK2D_SPACE
          << End.GetX()         << GVEXPOSURESENSORNETWORK2D_SPACE
          << End.GetY()         << GVEXPOSURESENSORNETWORK2D_SPACE
          << MaxLength          << GVEXPOSURESENSORNETWORK2D_SPACE
          << SensorsTotal       << GVEXPOSURESENSORNETWORK2D_SPACE
          << SensorsUnknown     << GVEXPOSURESENSORNETWORK2D_SPACE
          << SensorsKnown       << GVEXPOSURESENSORNETWORK2D_SPACE
          << SensorsVisited     << GVEXPOSURESENSORNETWORK2D_SPACE
          << NumCommunications  << GVEXPOSURESENSORNETWORK2D_SPACE
          << endl;
  OutFile.close();
} // OutputResults



int GVExposureSensorNetwork2D::MaxExposure_UpdateCurrentNode(int Next)
/*
  UpdateCurrentNode() sets the current node equal to the next node.  It then
  sets all the node's neighbors to visited and marks the area as covered.
*/
{ // MaxExposure_UpdateCurrentNode
  int i = 0;                                                                   // Index Variable
  int j = 0;                                                                   // Index Variable
  int ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS;              // Return Value
  int GridH = Grid.GetSizeH();                                                 // Number of Horizontal Grid Squares
  int GridV = Grid.GetSizeV();                                                 // Number of Vertical Grid Squares
  int NbrNum = NumNeighbors[Next];                                             // Number of Neighbors
  int NbrIndex = 0;                                                            // Index of Current Neighbor
  int iSensor = 0;                                                             // Grid Square Horizontal Value of Current Sensor
  int jSensor = 0;                                                             // Grid Square Vertical Value of Current Sensor
  int iMin = 0;                                                                // Minimum i Value
  int iMax = 0;                                                                // Minimum j Value
  int jMin = 0;                                                                // Maximum i Value
  int jMax = 0;                                                                // Maximum j Value
  int GridRadiusH = Grid.GetRadiusHorizontal();                                // Horizontal Radius Grid Squares
  int GridRadiusV = Grid.GetRadiusVertical();                                  // Vertical Radius Grid Squares
  double GridHX = Grid.GetHorizontalVectorX();                                 // Horizontal Grid Vector X Coordinate
  double GridHY = Grid.GetHorizontalVectorY();                                 // Horizontal Grid Vector Y Coordinate
  double GridVX = Grid.GetVerticalVectorX();                                   // Vertical Grid Vector X Coordinate
  double GridVY = Grid.GetVerticalVectorY();                                   // Vertical Grid Vector Y Coordinate
  double GridHLength = Distance2D(0.0, 0.0, GridHX, GridHY);                   // Length of Grid Horizontal Vector
  double GridVLength = Distance2D(0.0, 0.0, GridVX, GridVY);                   // Length of Grid Vertical Vector
  double D = 0.0;                                                              // Temporary Distance
  double X = 0.0;                                                              // Temporary X Coordinate
  double Y = 0.0;                                                              // Temporary Y Coordinate
  double SensorX = Sensors[Next].GetX();                                       // Sensor X Coordinate
  double SensorY = Sensors[Next].GetY();                                       // Sensor Y Coordinate
  double GridSensorX = SensorX - Grid.GetPointX(0, 0, GVGRIDPOINT2D_TOPLEFT);  // Sensor X Coordinate With Respect to Grid Start
  double GridSensorY = SensorY - Grid.GetPointY(0, 0, GVGRIDPOINT2D_TOPLEFT);  // Sensor Y Coordinate With Respect to Grid Start

  // Calculate Relevant Grid Squares: iMin to iMax, jMin to jMax (Significantly faster on high-resolution grids)
  GridHLength = Distance2D(0.0, 0.0, GridHX, GridHY);
  GridVLength = Distance2D(0.0, 0.0, GridVX, GridVY);
  iSensor = ((int) ((GridHX * GridSensorX + GridHY * GridSensorY) / (GridHLength * GridHLength)));
  jSensor = ((int) ((GridVX * GridSensorX + GridVY * GridSensorY) / (GridVLength * GridVLength)));
  iMin = iSensor - GridRadiusH;
  iMax = iSensor + GridRadiusH;
  jMin = jSensor - GridRadiusV;
  jMax = jSensor + GridRadiusV;
  if (iMin < 0) iMin = 0;
  if (iMax >= GridH) iMax = GridH - 1;
  if (jMin < 0) jMin = 0;
  if (jMax >= GridV) jMax = GridV - 1;

  // Set Next Sensor Attributes
  if (Sensors[Next].GetParent() < 0) Sensors[Next].SetParent(CurrentNode);

  // Set Algorithm Parameters
  CurrentNode = Next;
  ++NumCommunications;

  // Set Current Sensor Attributes
  Sensors[CurrentNode].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_VISITED);

  // Set Neighbors to Known
  for (i = 0; i < NbrNum; ++i)
  {
    NbrIndex = NeighborIndex[CurrentNode][i];
    if (!Sensors[NbrIndex].GetKnown()) Sensors[NbrIndex].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN);
  }

  // Set Area to Covered
  for (i = iMin; i <= iMax; ++i)
  {
    for (j = jMin; j <= jMax; ++j)
    {
      if (!Grid.GetCovered(i, j))
      {
        X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
        Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
        D = Distance2D(X, Y,SensorX, SensorY);
        if (D <= Radius) Grid.SetCovered(i, j, GVEXPOSURESENSORNETWORK2D_GRID_COVERED);
      }
    }
  }
  ReturnVal = MaxExposure_CalculateDone();
  return(ReturnVal);
} // MaxExposure_UpdateCurrentNode



int GVExposureSensorNetwork2D::MaxExposure_CalculateDone(void)
/*
  MaxExposure_CalculateDone() calculates if the algorithm is done.  It checks to see if
  any new area exists, and if new area exists, the a
  lgorithm is not done.
  
  MaxExposure_CalculateDone() returns 1 to indicate "Keep Going" (not done) and
  0 to indicate "Stop" (done).
*/
{ // MaxExposure_CalculateDone
  int ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_DONE; // Return Value
  int i = 0;                                                // Index Variable

  for (i = 0; (!ReturnVal) && (i < NumSensors); ++i)
  {
    if (Sensors[i].GetKnown() == GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN)
    {
      if (MaxExposure_CalculatePotentialArea(i, GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED) > 0)
      {
        ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS;
      }
    }
  }
  return(ReturnVal);
} // MaxExposure_CalculateDone



int GVExposureSensorNetwork2D::MaxExposure_CalculatePotentialArea(int Index, int TempValue)
/*

*/
{ // MaxExposure_CalculatePotentialArea
  int i = 0;                                                                  // Index Variable
  int j = 0;                                                                  // Index Variable
  int iSensor = 0;                                                            // Sensor Horizontal Grid Square Coordinate
  int jSensor = 0;                                                            // Sensor Vertical Grid Square Coordinate
  int iMin = 0;                                                               // Minimum Horizontal Grid Value
  int jMin = 0;                                                               // Minimum Vertical Grid Value
  int iMax = 0;                                                               // Maximum Horizontal Grid Value
  int jMax = 0;                                                               // Maximum Vertical Grid Value
  int ReturnVal = 0;                                                          // Return Value (New Area)
  int GridH = Grid.GetSizeH();                                                // Number of Horizontal Grid Squares
  int GridV = Grid.GetSizeV();                                                // Number of Vertical Grid Squares
  int GridRadiusH = Grid.GetRadiusHorizontal();                               // Horizontal Radius Grid Squares
  int GridRadiusV = Grid.GetRadiusVertical();                                 // Vertical Radius Grid Squares
  double GridHX = Grid.GetHorizontalVectorX();                                // Horizontal Grid Vector X Coordinate
  double GridHY = Grid.GetHorizontalVectorY();                                // Horizontal Grid Vector Y Coordinate
  double GridVX = Grid.GetVerticalVectorX();                                  // Vertical Grid Vector X Coordinate
  double GridVY = Grid.GetVerticalVectorY();                                  // Vertical Grid Vector Y Coordinate
  double GridHLength = Distance2D(0.0, 0.0, GridHX, GridHY);                  // Length of Grid Horizontal Vector
  double GridVLength = Distance2D(0.0, 0.0, GridVX, GridVY);                  // Length of Grid Vertical Vector
  double D = 0.0;                                                             // Temporary Distance
  double L = 0.0;                                                             // Temporary Length
  double X = 0.0;                                                             // Temporary X Coordinate
  double Y = 0.0;                                                             // Temporary Y Coordinate
  double StartX = Start.GetX();                                               // Starting X Coordinate
  double StartY = Start.GetY();                                               // Starting Y Coordinate
  double EndX = End.GetX();                                                   // Starting X Coordinate
  double EndY = End.GetY();                                                   // Starting Y Coordinate
  double SensorX = Sensors[Index].GetX();                                     // Sensor X Coordinate
  double SensorY = Sensors[Index].GetY();                                     // Sensor Y Coordinate
  double GridSensorX = SensorX - Grid.GetPointX(0, 0, GVGRIDPOINT2D_TOPLEFT); // Sensor X Coordinate With Respect to Grid
  double GridSensorY = SensorY - Grid.GetPointY(0, 0, GVGRIDPOINT2D_TOPLEFT); // Sensor Y Coordinate With Respect to Grid

  // Calculate Relevant Grid Squares: iMin to iMax, jMin to jMax (Significantly faster on high-resolution grids)
  iSensor = ((int) ((GridHX * GridSensorX + GridHY * GridSensorY) / (GridHLength * GridHLength)));
  jSensor = ((int) ((GridVX * GridSensorX + GridVY * GridSensorY) / (GridVLength * GridVLength)));
  iMin = iSensor - GridRadiusH;
  iMax = iSensor + GridRadiusH;
  jMin = jSensor - GridRadiusV;
  jMax = jSensor + GridRadiusV;
  if (iMin < 0) iMin = 0;
  if (iMax >= GridH) iMax = GridH - 1;
  if (jMin < 0) jMin = 0;
  if (jMax >= GridV) jMax = GridV - 1;

  for (i = iMin; i <= iMax; ++i)
  {
    for (j = jMin; j <= jMax; ++j)
    {
      if (Grid.GetCovered(i, j) == GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED)
      {
        X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
        Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
        D = Distance2D(SensorX, SensorY, X, Y);
        L = Distance2D(X, Y, StartX, StartY) + Distance2D(X, Y, EndX, EndY);
        if ((L <= MaxLength) && (D <= Radius))
        {
          Grid.SetCovered(i, j, TempValue);
          ++ReturnVal;
        }
      }
    }
  }
  return(ReturnVal);
} // MaxExposure_CalculatePotentialArea



int GVExposureSensorNetwork2D::MaxExposure_CalculateHoleCreation(int SensorIndex)
/*
  MaxExposure_CalculateHoleCreation() determines if the next move would create
  a hole.  It does this by examining the squares just outside the area that would
  be covered and divides them into groups.  Groups are defined as squares that are
  consecutive in terms of angle and have the same covered attribute.  A hole is
  created if there are more than two groups (the number of groups must be either
  1 or even).
*/
{ // MaxExposure_CalculateHoleCreation
  int i = 0;                                                                                                // Index Variable
  int j = 0;                                                                                                // Index Variable
  int k = 0;                                                                                                // Index Variable
  int iSensor = 0;                                                                                          // Horizontal Grid Square of Sensor
  int jSensor = 0;                                                                                          // Vertical Grid Square of Sensor
  int iMin = 0;                                                                                             // Minimum Horizontal Grid Squiare
  int jMin = 0;                                                                                             // Minimum Vertical Grid Squiare
  int iMax = 0;                                                                                             // Maximum Horizontal Grid Squiare
  int jMax = 0;                                                                                             // Maximum Vertical Grid Squiare
  int iNext = 0;                                                                                            // Next Grid Square Horizontal Index
  int jNext = 0;                                                                                            // Next Grid Square Vertical Index
  int GridH = Grid.GetSizeH();                                                                              // Number of Horizontal Grid Squares
  int GridV = Grid.GetSizeV();                                                                              // Number of Vertical Grid Squares
  int ReturnVal = 0;                                                                                        // Return Value (1 if Hole Exists, 0 Otherwise)
  int NumElements = 0;                                                                                      // Number of Elements in Hole Array
  int iOffset[4] = { 1, -1, 0, 0 };                                                                         // Horizontal Neighbor Grid Square Offsets
  int jOffset[4] = { 0, 0, 1, -1 };                                                                         // Vertical Neighbor Grid Square Offsets
  int ijAdded = 0;                                                                                          // Was a Grid Square Added (Short-Circuit Mechanism)
  int KeepGoing = 0;                                                                                        // Keep Searching Through Arrays (Short-Circuit Mechanism)
  int NumGroups = 0;                                                                                        // Number of Groups in Hole Array
  int StartCovered = 0;                                                                                     // Starting Covered Value in Hole Array
  int CurrentCovered = 0;                                                                                   // Current Covered Value in Hole Array
  int StartIndex = 0;                                                                                       // Starting Element in Hole Array
  int CurrentIndex = 0;                                                                                     // Current Element in Hole Array
  int GridRadiusH = Grid.GetRadiusHorizontal();                                                             // Radius Horizontal Grid Squares
  int GridRadiusV = Grid.GetRadiusVertical();                                                               // Radius Vertical Grid Squares
  double StartX = Start.GetX();                                                                             // Starting X Coordinate
  double StartY = Start.GetY();                                                                             // Starting Y Coordinate
  double EndX = End.GetX();                                                                                 // Ending X Coordinate
  double EndY = End.GetY();                                                                                 // Ending Y Coordinate
  double D = 0.0;                                                                                           // Temporary Distance
  double L = 0.0;                                                                                           // Temporary Length
  double X = 0.0;                                                                                           // Temporary X Coordinate
  double Y = 0.0;                                                                                           // Temporary Y Coordinate
  double GX = 0.0;                                                                                          // Grid Square X Coordinate Used to Calculate Angle of Grid Square
  double GY = 0.0;                                                                                          // Grid Square Y Coordinate Used to Calculate Angle of Grid Square
  double VX = 0.0;                                                                                          // Vector X Coordinate Used to Calculate Angle of Grid Square
  double VY = 0.0;                                                                                          // Vector Y Coordinate Used to Calculate Angle of Grid Square
  double GridHX = Grid.GetHorizontalVectorX();                                                              // X Coordinate of Horizontal Vector
  double GridHY = Grid.GetHorizontalVectorY();                                                              // Y Coordinate of Horizontal Vector
  double GridVX = Grid.GetVerticalVectorX();                                                                // X Coordinate of Vertical Vector
  double GridVY = Grid.GetVerticalVectorY();                                                                // Y Coordinate of Vertical Vector
  double GridHLength = Distance2D(0.0, 0.0, GridHX, GridHY);                                                // Length of Grid Horizontal Vector
  double GridVLength = Distance2D(0.0, 0.0, GridVX, GridVY);                                                // Length of Grid Vertical Vector
  double SensorX = Sensors[SensorIndex].GetX();                                                             // Sensor X Coordinate
  double SensorY = Sensors[SensorIndex].GetY();                                                             // Sensor Y Coordinate
  double DNext = 0.0;                                                                                       // Distance of Neighbor Grid Square
  double LNext = 0.0;                                                                                       // Length of Neighbor Grid Square
  double LSensor = Distance2D(SensorX, SensorY, StartX, StartY) + Distance2D(SensorX, SensorY, EndX, EndY); // Path Length of the Sensor (Inside/Outside Ellipse)
  double Angle = 0.0;                                                                                       // Current Angle of Grid Square
  double GridSensorX = SensorX - Grid.GetPointX(0, 0, GVGRIDPOINT2D_TOPLEFT);                               // Sensor X Coordinate With Respect to Grid
  double GridSensorY = SensorY - Grid.GetPointY(0, 0, GVGRIDPOINT2D_TOPLEFT);                               // Sensor Y Coordinate With Respect to Grid
  double ToDegrees = 180.0 / 3.14159;
  GVExposureSensorNetwork2D_Hole* HoleArray = NULL;

  // Calculate Relevant Grid Squares: iMin to iMax, jMin to jMax (Significantly faster on high-resolution grids)
  iSensor = ((int) ((GridHX * GridSensorX + GridHY * GridSensorY) / (GridHLength * GridHLength)));
  jSensor = ((int) ((GridVX * GridSensorX + GridVY * GridSensorY) / (GridVLength * GridVLength)));
  iMin = iSensor - GridRadiusH;
  iMax = iSensor + GridRadiusH;
  jMin = jSensor - GridRadiusV;
  jMax = jSensor + GridRadiusV;
  if (iMin < 0) iMin = 0;
  if (iMax >= GridH) iMax = GridH - 1;
  if (jMin < 0) jMin = 0;
  if (jMax >= GridV) jMax = GridV - 1;

  // Add the Bordering Grid Squares to the Array For Checking
  HoleArray = new GVExposureSensorNetwork2D_Hole[GridH * GridV];
  for (i = iMin; i <= iMax; ++i)
  {
    for (j = jMin; j <= jMax; ++j)
    {
      X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
      Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
      L = Distance2D(X, Y, StartX, StartY) + Distance2D(X, Y, EndX, EndY);
      if (L <= MaxLength)
      {
        D = Distance2D(X, Y, SensorX, SensorY);
        for (ijAdded = 0, k = 0; (!ijAdded) && (k < 4); ++k)
        {
          iNext = i + iOffset[k];
          jNext = j + jOffset[k];
          if ((iNext >= 0) && (iNext < GridH) && (jNext >= 0) && (jNext < GridV))
          {
            GX = Grid.GetPointX(iNext, jNext, GVGRIDPOINT2D_CENTERPOINT);
            GY = Grid.GetPointY(iNext, jNext, GVGRIDPOINT2D_CENTERPOINT);
            DNext = Distance2D(GX, GY, SensorX, SensorY);
            LNext = Distance2D(GX, GY, StartX, StartY) + Distance2D(GX, GY, EndX, EndY);
            if ((D > Radius) && (DNext <= Radius) && (L < MaxLength))
            {
              VX = X - SensorX;
              VY = Y - SensorY;
              Angle = (acos(VX / Distance2D(0.0, 0.0, VX, VY))) * ToDegrees;
              if (VY < 0) Angle = 360.0 - Angle;
              HoleArray[NumElements].Angle = Angle;
              if (L <= MaxLength) HoleArray[NumElements].Covered = Grid.GetCovered(i, j);
              else HoleArray[NumElements].Covered = GVEXPOSURESENSORNETWORK2D_GRID_COVERED;
              ++NumElements;
              ijAdded = 1;
            }
            else if ((LSensor <= MaxLength) && (D <= Radius) && (LNext > MaxLength))
            {
              VX = X - SensorX;
              VY = Y - SensorY;
              Angle = (acos(VX / Distance2D(0.0, 0.0, VX, VY))) * ToDegrees;
              if (VY < 0) Angle = 360.0 - Angle;
              HoleArray[NumElements].Angle = Angle;
              HoleArray[NumElements].Covered = GVEXPOSURESENSORNETWORK2D_GRID_COVERED;
              ++NumElements;
              ijAdded = 1;
            }
          }
        }
      }
    }
  }

  if (NumElements > 0)
  {
    // Sort the Array
    qsort(HoleArray, NumElements, sizeof(GVExposureSensorNetwork2D_Hole), CompareHoleGridPoints);
    StartIndex = 0;
    StartCovered = (HoleArray[StartIndex].Covered != GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED);
    KeepGoing = 1;
    // Start at the first complete group (don't start in the middle of a group)
    while(KeepGoing)
    {
      if (StartIndex >= NumElements) KeepGoing = 0;
      else if (StartCovered != HoleArray[StartIndex].Covered) KeepGoing = 0;
      else StartIndex = (StartIndex + 1) % NumElements;
      if (!StartIndex) KeepGoing = 0;
    }
    // Count Number of Groups
    CurrentIndex = StartIndex;
    CurrentCovered = (HoleArray[CurrentIndex].Covered != GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED);
    NumGroups = 1;
    ReturnVal = 0;
    do
    {
      if (HoleArray[CurrentIndex].Covered != CurrentCovered)
      {
        ++NumGroups;
        CurrentCovered = (HoleArray[CurrentIndex].Covered != GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED);
        if (NumGroups >= 3) ReturnVal = 1;
      }
      CurrentIndex = (CurrentIndex + 1) % NumElements;
    } while ((!ReturnVal) && (CurrentIndex != StartIndex));
  }
  // Free Up Array
  if (HoleArray != NULL) delete[] HoleArray;
  // Return 1 if Hole Would Be Created, 0 Otherwise
  return(ReturnVal);
} // MaxExposure_CalculateHoleCreation



int GVExposureSensorNetwork2D::MaxExposure_CalculateIndex(int Index)
/*
  MaxExposure_CalculateIndex() calculates the index value of a sensor.  It is
  defined as the lowest index value grid square within the radius.
*/
{ // MaxExposure_CalculateIndex
  int i = 0;                                                                  // Index Variable
  int j = 0;                                                                  // Index Variable
  int GridH = Grid.GetSizeH();                                                // Horizontal Grid Squares
  int GridV = Grid.GetSizeV();                                                // Vertical Grid Squares
  int ReturnVal = GVEXPOSUREGRIDATTRIBUTES_MAXLEVEL;                          // Best Level
  int NextLevel = 0;                                                          // Next Level
  int iSensor = 0;                                                            // Grid Square of Horizontal Sensor
  int jSensor = 0;                                                            // Grid Square of Vertical Sensor
  int iMin = 0;                                                               // Minimum Horizontal Grid Square
  int jMin = 0;                                                               // Maximum Horizontal Grid Square
  int iMax = 0;                                                               // Minimum Vertical Grid Square
  int jMax = 0;                                                               // Maximum Vertical Grid Square
  int GridRadiusH = Grid.GetRadiusHorizontal();                               // Horizontal Radius Grid Squares
  int GridRadiusV = Grid.GetRadiusVertical();                                 // Vertical Radius Grid Squares
  double D = 0.0;                                                             // Temporary Distance
  double L = 0.0;                                                             // Temporary Length
  double X = 0.0;                                                             // Temporary X Coordinate
  double Y = 0.0;                                                             // Temporary Y Coordinate
  double StartX = Start.GetX();                                               // Starting X Coordinate
  double StartY = Start.GetY();                                               // Starting Y Coordinate
  double EndX = End.GetX();                                                   // Ending X Coordinate
  double EndY = End.GetY();                                                   // Ending Y Coordinate
  double GridHX = Grid.GetHorizontalVectorX();                                // X Coordinate of Horizontal Vector
  double GridHY = Grid.GetHorizontalVectorY();                                // Y Coordinate of Horizontal Vector
  double GridVX = Grid.GetVerticalVectorX();                                  // X Coordinate of Vertical Vector
  double GridVY = Grid.GetVerticalVectorY();                                  // Y Coordinate of Vertical Vector
  double GridHLength = Distance2D(0.0, 0.0, GridHX, GridHY);                  // Length of Horizontal Grid Vector
  double GridVLength = Distance2D(0.0, 0.0, GridVX, GridVY);                  // Length of Vertical Grid Vector
  double SensorX = Sensors[Index].GetX();                                     // Sensor X Coordinate
  double SensorY = Sensors[Index].GetY();                                     // Sensor Y Coordinate
  double GridSensorX = SensorX - Grid.GetPointX(0, 0, GVGRIDPOINT2D_TOPLEFT); // Sensor X Coordinate With Respect to Grid
  double GridSensorY = SensorY - Grid.GetPointY(0, 0, GVGRIDPOINT2D_TOPLEFT); // Sensor Y Coordinate With Respect to Grid

  // Calculate Relevant Grid Squares: iMin to iMax, jMin to jMax (Significantly faster on high-resolution grids)
  iSensor = ((int) ((GridHX * GridSensorX + GridHY * GridSensorY) / (GridHLength * GridHLength)));
  jSensor = ((int) ((GridVX * GridSensorX + GridVY * GridSensorY) / (GridVLength * GridVLength)));
  iMin = iSensor - GridRadiusH;
  iMax = iSensor + GridRadiusH;
  jMin = jSensor - GridRadiusV;
  jMax = jSensor + GridRadiusV;
  if (iMin < 0) iMin = 0;
  if (iMax >= GridH) iMax = GridH - 1;
  if (jMin < 0) jMin = 0;
  if (jMax >= GridV) jMax = GridV - 1;

  // Find the Smallest Index of the Relevant Squares
  for (i = iMin; i <= iMax; ++i)
  {
    for (j = jMin; j <= jMax; ++j)
    {
      if (Grid.GetCovered(i, j) != GVEXPOSURESENSORNETWORK2D_GRID_COVERED)
      {
        X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
        Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
        L = Distance2D(X, Y, StartX, StartY) + Distance2D(X, Y, EndX, EndY);
        D = Distance2D(X, Y, SensorX, SensorY);
        if ((D <= Radius) && (L <= MaxLength))
        {
          NextLevel = Grid.GetBorderLevel(i, j);
          if (NextLevel < ReturnVal) ReturnVal = NextLevel;
        }
      }
    }
  }
  return(ReturnVal);
} // MaxExposure_CalculateIndex



void GVExposureSensorNetwork2D::MaxExposure_CalculateLevels(void)
/*
  MaxExposure_CalculateLevels() calculates the levels of the grid squares.
  All inside-ellispe grid squares start at infinity and all outside-ellipse
  grid squares start at 0.  Then, iteratively update grid squares by setting
  its level to 1 plus the lowest neighboring level.  Do this until all levels
  no longer change.

  The result is that squares closer to the outside of the ellipse have a lower
  index value and squares closer to the inside have a higher index value.
*/
{ // MaxExposure_CalculateLevels
  int i = 0;                              // Index Variable
  int j = 0;                              // Index Variable
  int GridH = Grid.GetSizeH();            // Horizontal Number of Grid Squares
  int GridV = Grid.GetSizeV();            // Vertical Number of Grid Squares
  int KeepGoing = 1;                      // Keep Adjusting Levels
  int CurrentLevel = 0;                   // Current Level of Square
  int NextLevel = 0;                      // Next Level of Square
  int GridHMin = Grid.GetMinHorizontal(); // Minimum Horizontal Square to Search
  int GridHMax = Grid.GetMaxHorizontal(); // Maximum Horizontal Square to Search
  int GridVMin = Grid.GetMinVertical();   // Minimum Vertical Square to Search
  int GridVMax = Grid.GetMaxVertical();   // Maximum Vertical Square to Search
  double Length = 0.0;                    // Length from Square to Foci
  double X = 0.0;                         // Temporary X Coordinate
  double Y = 0.0;                         // Temporary Y Coordinate
  double StartX = Start.GetX();           // Starting X Coordinate
  double StartY = Start.GetY();           // Starting Y Coordinate
  double EndX = End.GetX();               // Ending X Coordinate
  double EndY = End.GetY();               // Ending Y Coordinate

  for (i = GridHMin; i <= GridHMax; ++i)
  {
    for (j = GridVMin; j <= GridVMax; ++j)
    {
      X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
      Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
      Length = Distance2D(X, Y, StartX, StartY) + Distance2D(X, Y, EndX, EndY);
      if ((Length > MaxLength) || (Grid.GetCovered(i, j) == GVEXPOSURESENSORNETWORK2D_GRID_COVERED))
      {
        Grid.SetBorderLevel(i, j, 0);
      }
      else
      {
        Grid.SetBorderLevel(i, j, GVEXPOSUREGRIDATTRIBUTES_MAXLEVEL);
      }
    }
  }

  while(KeepGoing)
  {
    KeepGoing = 0;
    for (i = GridHMin; i <= GridHMax; ++i)
    {
      for (j = GridVMin; j <= GridVMax; ++j)
      {
        CurrentLevel = Grid.GetBorderLevel(i, j);
        NextLevel = Grid.GetBorderLevel(i + 1, j) + 1;
        if (NextLevel < CurrentLevel)
        {
          CurrentLevel = NextLevel;
          KeepGoing = 1;
        }
        NextLevel = Grid.GetBorderLevel(i - 1, j) + 1;
        if (NextLevel< CurrentLevel)
        {
          CurrentLevel = NextLevel;
          KeepGoing = 1;
        }
        NextLevel = Grid.GetBorderLevel(i, j + 1) + 1;
        if (NextLevel < CurrentLevel)
        {
          CurrentLevel = NextLevel;
          KeepGoing = 1;
        }
        NextLevel = Grid.GetBorderLevel(i, j - 1) + 1;
        if (NextLevel < CurrentLevel)
        {
          CurrentLevel = NextLevel;
          KeepGoing = 1;
        }
        Grid.SetBorderLevel(i, j, CurrentLevel);
      }
    }
  }
} // MaxExposure_CalculateLevels



void GVExposureSensorNetwork2D::MaxExposure_ErasePotentialArea(int Index, int TempValue)
/*
  MaxExposure_ErasePotentialArea() erases areas marked as TempValue within the
  radius defined by Index.
*/
{ // MaxExposure_ErasePotentialArea
  int i = 0;                                                 // Index Variable
  int j = 0;                                                 // Index Variable
  int GridH = Grid.GetSizeH();                               // Number of Horizontal Grid Squares
  int GridV = Grid.GetSizeV();                               // Number of Vertical Grid Squares
  int iSensor = 0;                                           // Sensor Horizontal Grid Square Coordinate
  int jSensor = 0;                                           // Sensor Vertical Grid Square Coordinate
  int iMin = 0;                                              // Minimum Horizontal Grid Value
  int jMin = 0;                                              // Minimum Vertical Grid Value
  int iMax = GridH - 1;                                      // Maximum Horizontal Grid Value
  int jMax = GridV - 1;                                      // Maximum Vertical Grid Value
  int ReturnVal = 0;                                         // Return Value (New Area)
  int GridRadiusH = Grid.GetRadiusHorizontal();              // Horizontal Radius Grid Squares
  int GridRadiusV = Grid.GetRadiusVertical();                // Vertical Radius Grid Squares
  double SensorX = 0.0;                                      // Sensor X Coordinate
  double SensorY = 0.0;                                      // Sensor Y Coordinate
  double GridHX = Grid.GetHorizontalVectorX();               // Horizontal Grid Vector X Coordinate
  double GridHY = Grid.GetHorizontalVectorY();               // Horizontal Grid Vector Y Coordinate
  double GridVX = Grid.GetVerticalVectorX();                 // Vertical Grid Vector X Coordinate
  double GridVY = Grid.GetVerticalVectorY();                 // Vertical Grid Vector Y Coordinate
  double GridHLength = Distance2D(0.0, 0.0, GridHX, GridHY); // Length of Grid Horizontal Vector
  double GridVLength = Distance2D(0.0, 0.0, GridVX, GridVY); // Length of Grid Vertical Vector
  double GridSensorX = 0.0;                                  // Sensor X Coordinate With Respect to Grid
  double GridSensorY = 0.0;                                  // Sensor Y Coordinate With Respect to Grid

  // Calculate Relevant Grid Squares: iMin to iMax, jMin to jMax (Significantly faster on high-resolution grids)
  if ((Index >= 0) && (Index < NumSensors))
  {
    SensorX = Sensors[Index].GetX();
    SensorY = Sensors[Index].GetY();
    GridSensorX = SensorX - Grid.GetPointX(0, 0, GVGRIDPOINT2D_TOPLEFT);
    GridSensorY = SensorY - Grid.GetPointY(0, 0, GVGRIDPOINT2D_TOPLEFT);
    iSensor = ((int) ((GridHX * GridSensorX + GridHY * GridSensorY) / (GridHLength * GridHLength)));
    jSensor = ((int) ((GridVX * GridSensorX + GridVY * GridSensorY) / (GridVLength * GridVLength)));
    iMin = iSensor - GridRadiusH;
    iMax = iSensor + GridRadiusH;
    jMin = jSensor - GridRadiusV;
    jMax = jSensor + GridRadiusV;
    if (iMin < 0) iMin = 0;
    if (iMax >= GridH) iMax = GridH - 1;
    if (jMin < 0) jMin = 0;
    if (jMax >= GridV) jMax = GridV - 1;
  }

  // Erase the Grid Squares
  for (i = iMin; i <= iMax; ++i)
  {
    for (j = jMin; j <= jMax; ++j)
    {
      if (Grid.GetCovered(i, j) == TempValue)
      {
        Grid.SetCovered(i, j, GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED);
      }
    }
  }
} // MaxExposure_ErasePotentialArea



void GVExposureSensorNetwork2D::MaxExposure_UpdateLevels(void)
/*
  MaxExposure_UpdateLevels updates the border levels of the grid and updates
  the minimum and maximum search areas for speed.
*/
{ // MaxExposure_UpdateLevels
  int i = 0;                                      // Index Variable
  int j = 0;                                      // Index Variable
  int Level = 0;                                  // Current Border Level Value
  int MinVal = GVEXPOSUREGRIDATTRIBUTES_MAXLEVEL; // Minimum Border Level Value
  int GridHMin = Grid.GetMinHorizontal();         // Minimum Horizontal Grid Square to Search
  int GridHMax = Grid.GetMaxHorizontal();         // Maximum Horizontal Grid Square to Search
  int GridVMin = Grid.GetMinVertical();           // Minimum Vertical Grid Square to Search
  int GridVMax = Grid.GetMaxVertical();           // Maximum Vertical Grid Square to Search
  int CurrentHMin = GridHMax;                     // Current Minimum Horizontal Grid Square
  int CurrentHMax = GridHMin;                     // Current Maximum Horizontal Grid Square
  int CurrentVMin = GridVMax;                     // Current Minimum Vertical Grid Square
  int CurrentVMax = GridVMin;                     // Current Maximum Vertical Grid Square
  double D = 0.0;                                 // Temporary Distance
  double X = 0.0;                                 // Temporary X Coordinate
  double Y = 0.0;                                 // Temporary Y Coordinate
  double StartX = Start.GetX();                   // Starting X Coordinate
  double StartY = Start.GetY();                   // Starting Y Coordinate
  double EndX = End.GetX();                       // Ending X Coordinate
  double EndY = End.GetY();                       // Ending Y Coordinate

  for (i = GridHMin; i <= GridHMax; ++i)
  {
    for (j = GridVMin; j <= GridVMax; ++j)
    {
      X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
      Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
      D = Distance2D(X, Y, StartX, StartY) + Distance2D(X, Y, EndX, EndY);
      if ((D <= MaxLength) && (!Grid.GetCovered(i, j)))
      {
        Level = Grid.GetBorderLevel(i, j);
        if (Level < MinVal) MinVal = Level;
        if (i < CurrentHMin) CurrentHMin = i;
        if (i > CurrentHMax) CurrentHMax = i;
        if (j < CurrentVMin) CurrentVMin = j;
        if (j > CurrentVMax) CurrentVMax = j;
      }
    }
  }
  Grid.SetMinHorizontal(CurrentHMin);
  Grid.SetMaxHorizontal(CurrentHMax);
  Grid.SetMinVertical(CurrentVMin);
  Grid.SetMaxVertical(CurrentVMax);
  if (MinVal > 1)
  {
    MaxExposure_CalculateLevels();
  }
} // MaxExposure_UpdateLevels



void GVExposureSensorNetwork2D::MinExposure_CalculateGrid(int GH, int GV)
/*
  MinExposure_CalculateGrid() places a GH x GV grid from 0.0 to 1.1 and
  calculates the exposure.
*/
{ // MinExposure_CalculateGrid
  Grid.New(GH, GV, 0, 0, 1.0 / ((double) GH), 0.0, 0.0, 1.0 / ((double) GV), Radius);
  CalculateGridExposure();
} // MinExposure_CalculateGrid



void GVExposureSensorNetwork2D::MaxExposure_CalculateGrid(int GH, int GV)
/*
  MaxExposure_CalculateGrid() creates the grid based off the ellipse formed
  by the starting and ending coordinates and the maximum length.
*/
{ // MaxExposure_CalculateGrid
  int NewGridH = GH - 2;                                  // Horizontal Grid Squares Covering Ellipse
  int NewGridV = GV - 2;                                  // Vertical Grid Squares Covering Ellipse
  double StartX = Start.GetX();                           // Starting X Coordinate
  double StartY = Start.GetY();                           // Starting Y Coordinate
  double EndX = End.GetX();                               // Ending X Coordinate
  double EndY = End.GetY();                               // Ending Y Coordinate
  double Length = Distance2D(StartX, StartY, EndX, EndY); // Distance from Start to End
  double A = MaxLength / 2.0;                             // Ellipse A Parameter
  double B = sqrt(A * A - (Length * Length / 4.0));       // Ellipse B Parameter
  double VX = (EndX - StartX) / Length;                   // X Coordinate of Unit Vector From Start to End
  double VY = (EndY - StartY) / Length;                   // Y Coordinate of Unit Vector From Start to End
  double AD = 2.0 * A / ((double) NewGridH);              // Length of A Vector
  double BD = 2.0 * B / ((double) NewGridV);              // Length of B Vector
  double CX = (StartX + EndX) / 2.0;                      // X Coordinate of C Vector
  double CY = (StartY + EndY) / 2.0;                      // Y Coordinate of C Vector
  double X = 0.0;                                         // Temporary X Coordinate
  double Y = 0.0;                                         // Temporary Y Coordinate
  double GridHX = VX * AD;                                // X Coordinate of Horizontal Vector
  double GridHY = VY * AD;                                // Y Coordinate of Horizontal Vector
  double GridVX = -VY * BD;                               // X Coordinate of Vertical Vector
  double GridVY = VX * BD;                                // Y Coordinate of Vertical Vector
  
  X = CX - ((GridHX / 2.0) * NewGridH) - GridHX - ((GridVX / 2.0) * NewGridV) - GridVX;
  Y = CY - ((GridHY / 2.0) * NewGridH) - GridHY - ((GridVY / 2.0) * NewGridV) - GridVY;
  Grid.New(GH, GV, X, Y, GridHX, GridHY, GridVX, GridVY, Radius);
  MaxExposure_CalculateLevels();
  CalculateGridExposure();
} // MaxExposure_CalculateGrid



double GVExposureSensorNetwork2D::MaxExposure_CalculateHeuristic(int Area, int Hops, int Index)
/*
  MaxExposure_CalculateHeuristic() returns the heuristic value with respect to
  the Area, Hops, and Index.
*/
{ // MaxExposure_CalculateHeuristic
  return(((double) pow(Area, GVEXPOSURESENSORNETWORK2D_AREAPRIORITY)) / (pow(Hops + 1, GVEXPOSURESENSORNETWORK2D_HOPSPRIORITY) * pow(Index + 1, GVEXPOSURESENSORNETWORK2D_INDEXPRIORITY)));
} // MaxExposure_CalculateHeuristic



int GVExposureSensorNetwork2D::MaxExposure_UpdateHeuristic(void)
/*
  MaxExposure_UpdateHeuristic() updates the network search using the heuristic
  method.

  INSERT HEURISTIC DESCRIPTION HERE.
*/
{ // MaxExposure_UpdateHeuristic
  int i = 0;                                                      // Index Variable
  int j = 0;                                                      // Index Variable
  int ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS; // Return Value (1 Indicates Keep Going)
  int NbrIndex = 0;                                               // Index of Neighboring Node
  int NewArea = 0;                                                // Area of New Node
  int NewHops = 0;                                                // Hops to New Node
  int NewIndex = 0;                                               // Index of New Node
  int NewHoleExists = 0;                                          // Hole Exists for New Node
  int BestArea = 0;                                               // Area of Best Node
  int BestHops = 0;                                               // Hops to Best Node
  int BestIndex = 0;                                              // Index of Best Node
  int BestHoleExists = 0;                                         // Hole Exists for Best Node
  int ForwardDone = 0;                                            // Forwarding Done
  int HopDelta = 0;                                               // Difference in Hops
  int NbrNum = 0;                                                 // Number of Neighbors
  double NewHeuristic = 0.0;                                      // Heuristic of New Node
  double BestHeuristic = 0.0;                                     // Heuristic of Best Node

  if (!MaxExposureInitialized) MaxExposure_Initialize();
  if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_CENTRAL) ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_DONE;
  else if ((CurrentIteration >= GVEXPOSURESENSORNETWORK2D_MAXTRIES) || (NextNode < 0) || (NextNode >= NumSensors)) ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_ERROR;
  if ((ReturnVal == GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS) && (Sensors != NULL) && (NumNeighbors != NULL) && (NeighborIndex != NULL))
  {
    BestHoleExists = Grid.GetSizeH() * Grid.GetSizeV();
    BestIndex = GVEXPOSUREGRIDATTRIBUTES_MAXLEVEL;
    BestArea = 0;
    BestHops = NumSensors;
    BestHeuristic = 0.0;
    ReturnVal = MaxExposure_UpdateCurrentNode(NextNode);
    NextNode = -1;
    if (ReturnVal)
    {
      MaxExposure_UpdateLevels();
      if (MessageMode == GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH)
      {
        for (i = 0; i < NumSensors; ++i)
        {
          if (Sensors[i].GetKnown() == GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN)
          {
            NewArea = MaxExposure_CalculatePotentialArea(i, GVEXPOSURESENSORNETWORK2D_GRID_POTENTIAL);
            if (NewArea > 0)
            {
              NewHoleExists = MaxExposure_CalculateHoleCreation(i);
              if (NewHoleExists < BestHoleExists)
              {
                CalculateBreadthFirstSearch(CurrentNode);
                BestHoleExists = NewHoleExists;
                BestArea = NewArea;
                BestHops = Sensors[i].GetCost();
                BestIndex = MaxExposure_CalculateIndex(i);
                BestHeuristic = MaxExposure_CalculateHeuristic(BestArea, BestHops, BestIndex);
                NextNode = i;
              }
              else if (NewHoleExists == BestHoleExists)
              {
                CalculateBreadthFirstSearch(CurrentNode);
                NewIndex = MaxExposure_CalculateIndex(i);
                NewHops = Sensors[i].GetCost();
                if (((NewIndex < BestIndex) && (NewHops <= BestHops)) || ((NewIndex == BestIndex) && (NewHops < BestHops)) || ((NewIndex == BestIndex) && (NewHops == BestHops) && (NewArea > BestArea)))
                {
                  BestHoleExists = NewHoleExists;
                  BestArea = NewArea;
                  BestHops = NewHops;
                  BestIndex = NewIndex;
                  BestHeuristic = MaxExposure_CalculateHeuristic(BestArea, BestHops, BestIndex);
                  NextNode = i;
                }
                else if (((NewIndex < BestIndex) && (NewHops > BestHops)) || ((NewIndex == BestIndex) && (NewHops > BestHops) && (NewArea > BestArea)) || ((NewIndex > BestIndex) && (NewHops < BestHops)))
                {
                  NewHeuristic = MaxExposure_CalculateHeuristic(BestArea, BestHops, BestIndex);
                  if (NewHeuristic > BestHeuristic)
                  {
                    BestHoleExists = NewHoleExists;
                    BestArea = NewArea;
                    BestHops = NewHops;
                    BestIndex = NewIndex;
                    BestHeuristic = NewHeuristic;
                    NextNode = i;
                  }
                }
              }
              MaxExposure_ErasePotentialArea(i, GVEXPOSURESENSORNETWORK2D_GRID_POTENTIAL);
            }
            else
            {
              Sensors[i].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_USELESS);
            }
          }
        }
        SourceNode = CurrentNode;
        DestinationNode = NextNode;
        if (BestHops > 1) MessageMode = GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_FORWARD;
        if (GVEXPOSURESENSORNETWORK2D_PROGRESSMESSAGE) cout << "Searching... (" << BestHoleExists << ", " << BestIndex << ", " << BestHops << ", " << BestArea << ")" << endl;
      }

      if (MessageMode == GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_FORWARD)
      {
        SourceNode = CurrentNode;
        CalculateBreadthFirstSearch(DestinationNode);
        BestHops = Sensors[CurrentNode].GetCost();
        BestIndex = GVEXPOSUREGRIDATTRIBUTES_MAXLEVEL;
        BestArea = 0;
        NextNode = -1;
        BestHoleExists = Grid.GetSizeH() * Grid.GetSizeV();
        for (i = 0; i < NumSensors; ++i)
        {
          if (Sensors[i].GetKnown() == GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN)
          {
            NewArea = MaxExposure_CalculatePotentialArea(i, GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED);
            if (NewArea <= 0) Sensors[i].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_USELESS);
          }
        }
        NbrNum = NumNeighbors[CurrentNode];
        for (ForwardDone = 0, i = 0; (!ForwardDone) && (i < NbrNum); ++i)
        {
          NbrIndex = NeighborIndex[CurrentNode][i];
          if (NbrIndex == DestinationNode)
          {
            ForwardDone = 1;
            NextNode = NbrIndex;
            MessageMode = GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH;
          }
          else
          {
            if ((Sensors[NbrIndex].GetKnown() == GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN) && (Sensors[NbrIndex].GetCost() <= BestHops))
            {
              NewHoleExists = MaxExposure_CalculateHoleCreation(NbrIndex);
              NewArea = MaxExposure_CalculatePotentialArea(NbrIndex, GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED);
              if (NewHoleExists < BestHoleExists)
              {
                BestIndex = MaxExposure_CalculateIndex(NbrIndex);
                BestArea = NewArea;
                BestHoleExists = NewHoleExists;
                NextNode = NbrIndex;
              }
              else if (NewHoleExists == BestHoleExists)
              {
                NewIndex = MaxExposure_CalculateIndex(NbrIndex);
                if (NewIndex < BestIndex)
                {
                  BestIndex = NewIndex;
                  BestArea = NewArea;
                  BestHoleExists = NewHoleExists;
                  NextNode = NbrIndex;
                }
                else if (NewIndex == BestIndex)
                {
                  if (NewArea > BestArea)
                  {
                    BestIndex = NewIndex;
                    BestArea = NewArea;
                    BestHoleExists = NewHoleExists;
                    NextNode = NbrIndex;
                  }
                }
              }
            }
            if (NextNode < 0)
            {
              NextNode = Sensors[CurrentNode].GetParent();
            }
          }
        }
        if (NextNode == DestinationNode) MessageMode = GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH;
        if (GVEXPOSURESENSORNETWORK2D_PROGRESSMESSAGE) cout << "Forwarding... " << endl;
      }
    }
  }
  if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL)
  {
    for (i = 0; i < NumSensors; ++i)
    {
      if (Sensors[i].GetKnown() == GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN)
      {
        NewArea = MaxExposure_CalculatePotentialArea(i, GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED);
        if (NewArea <= 0) Sensors[i].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_USELESS);
      }
    }
  }
  if (GVEXPOSURESENSORNETWORK2D_PROGRESSMESSAGE)
  {
    if (ReturnVal == GVEXPOSURESENSORNETWORK2D_RETURNVAL_DONE)
    {
      cout << "Algorithm Comlete!" << endl;
    }
    else if (ReturnVal == GVEXPOSURESENSORNETWORK2D_RETURNVAL_ERROR)
    {
      cout << "Algorithm Terminated With Errors!" << endl;
    }
  }
  return(ReturnVal);
} // MaxExposure_UpdateHeuristic


/////////////////////////////////////
//Lin's functions  Begin

int GVExposureSensorNetwork2D::GPSR_Run(void)
{
	int ReturnVal = 1;
	int ret;
	if (!GPSRInitialized) AlgorithmStage = GVEXPOSURESENSORNETWORK2D_MODE_INIT;
	switch(AlgorithmStage)
	{
	case GVEXPOSURESENSORNETWORK2D_MODE_GPSR_INPROGRESS:
		ret = GPSR_UpdateRouting();
		if(ret == GPSR_DONE)
		{
			GPSR_Path[NumCommunications] = DestinationNode;
			ReturnVal = 0;
			Reset();
		}
		else if(ret == GPSR_CONTINUE)
		{
			ReturnVal = 1;
			AlgorithmStage = GVEXPOSURESENSORNETWORK2D_MODE_GPSR_INPROGRESS;
		}
		else if(ret == GPSR_FAIL)
		{
			ReturnVal = 2;
		}
	break;
	case GVEXPOSURESENSORNETWORK2D_MODE_INIT:
    default:	
      GPSR_Initialize();
      AlgorithmStage = GVEXPOSURESENSORNETWORK2D_MODE_GPSR_INPROGRESS;
    break;
	}
	// 0=done; 1=keepgoing; 2=failed
	return ReturnVal;
} // A finite state machine to control routing

void GVExposureSensorNetwork2D::GPSR_Initialize(void)
/*
  MaxExposure_Initialize() initializes the class for the maximal exposure
  algorithm.  Separate initialization exists for localized and centralized
  versions.
*/
{ // MaxExposure_Initialize
  int i = 0;                                              // Index Variable
  int j = 0;                                              // Index Variable
  int GridH = Grid.GetSizeH();                            // Horizontal Grid Squares
  int GridV = Grid.GetSizeV();                            // Vertical Grid Squares
  double InitialX = 0.0;                                  // Ideal Initial Sensor X Coordinate
  double InitialY = 0.0;                                  // Ideal Initial Sensor Y Coordinate
  double StartX = Start.GetX();                           // Starting X Coordinate
  double StartY = Start.GetY();                           // Starting Y Coordinate
  double EndX = End.GetX();                               // Ending X Coordinate
  double EndY = End.GetY();                               // Ending Y Coordinate
  double D = 0.0;                                         // Temporary Distance
  double MinDistance = GVSENSORNETWORK_INFINITY;          // Current Minimum Distance

  if ((Sensors != NULL) && (NumNeighbors != NULL) && (NeighborIndex != NULL))
  {
	for (i = 0; i < NumSensors; ++i)
	{
        Sensors[i].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN);
	}
    
    SourceNode = CalculateNearestSensor(StartX, StartY);
    DestinationNode = CalculateNearestSensor(EndX,EndY);
	CurrentNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
	NextNode = SourceNode;
    GPSRInitialized = 1;
    NumCommunications = 0;
	MessageDeliver = 0;
	ShortestPath = 0;
	GPSR_ShortestPath();
	MaxLength = 1000;
	GPSR_Path = new int [MaxLength]; 
  }
} // GPSR_Initialize

int GVExposureSensorNetwork2D::GPSR_UpdateRouting(void)
{
  int i = 0;                                                      // Index Variable
  int j = 0;                                                      // Index Variable
  int ReturnVal = 0;											  // Return Value
  int NbrIndex = 0;                                               // Neighboring Index
  int NbrNum = 0;                                                 // Number of Neighbors
  int NewIndex = 0;                                               // Index From Neighbor Node
  int BestIndex = 0;                                              // Index From Best Node
  double CurrentX = 0.0;                                          // Current X Coordinate
  double CurrentY = 0.0;                                          // Current Y Coordinate
  double StartX = Start.GetX();									  // Starting X Coordinate
  double StartY = Start.GetY();									  // Starting Y Coordinate
  double EndX = End.GetX();										  // Ending X Coordinate
  double EndY = End.GetY();										  // Ending Y Coordinate
  double MinD;
  double D = 0.0;
  double AX = 0.0;                                                // X Coordinate of Vector From Previous Node
  double AY = 1.0;                                                // Y Coordinate of Vector From Previous Node
  double BX = 0.0;                                                // X Coordinate of Vector To Next Node
  double BY = 0.0;                                                // Y Coordinate of Vector To Next Node
  double ToDegrees = (180.0 / 3.14159); 
  double LargestAngle = 0.0;									  // Used to choose the next clockwise node
  double Angle = 0.0;

  if (!GPSRInitialized) GPSR_Initialize();
  if ((NextNode < 0) || (NextNode >= NumSensors)) ReturnVal = GPSR_FAIL;
  if ((Sensors != NULL) && (NumNeighbors != NULL) && (NeighborIndex != NULL))
  {
	  if(CurrentNode >= 0)
	  {
		  AX = Sensors[CurrentNode].GetX() - Sensors[NextNode].GetX();
		  AY = Sensors[CurrentNode].GetY() - Sensors[NextNode].GetY();
	  }
	  GPSR_UpdateCurrentNode(NextNode);
	  NextNode = -1;
	  CurrentX = Sensors[CurrentNode].GetX();
	  CurrentY = Sensors[CurrentNode].GetY();
	  MinD = Distance2D(EndX, EndY, CurrentX, CurrentY); 
	  NbrNum = NumNeighbors[CurrentNode];	

	  // Forward Starts
	  for (i = 0; i < NbrNum; ++i)
	  {
		  NbrIndex = NeighborIndex[CurrentNode][i];
		  // if a neighbor has been visited, skip this
		  if (Sensors[NbrIndex].GetKnown() == GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN)
		  {
			  D = Distance2D(EndX, EndY, Sensors[NbrIndex].GetX(), Sensors[NbrIndex].GetY());
			  if( D < MinD)
			  {
				  MinD = D;
				  NextNode = NbrIndex;
			  }
		  }
	  }

	  if(NextNode == DestinationNode)
	  {		  
		  ReturnVal = GPSR_DONE;
		  MessageDeliver = 1;
	  }
	  else if(NextNode != -1)
	  {
		  ReturnVal = GPSR_CONTINUE;
	  }
	  else // Perimeter Starts
	  {
		  LargestAngle = 0.0;
		  for (i = 0; i < NbrNum; ++i)
		  {
			  NbrIndex = NeighborIndex[CurrentNode][i];
			  // if a neighbor has been visited, skip this
			  if (Sensors[NbrIndex].GetKnown() == GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN)
			  {
				BX = Sensors[NbrIndex].GetX() - CurrentX;
                BY = Sensors[NbrIndex].GetY() - CurrentY;
				Angle = acos((AX * BX + AY * BY) / (Distance2D(0.0, 0.0, AX, AY) * Distance2D(0.0, 0.0, BX, BY))) * ToDegrees;
				// clockwise
				if(Angle > LargestAngle)
				{
					LargestAngle = Angle;
					NextNode = NbrIndex;
				}
			  }
		  }
		
		  if(NextNode == DestinationNode)
		  {
			  ReturnVal = GPSR_DONE;
			  MessageDeliver = 1;
		  }
		  else if(NextNode != -1)
			  ReturnVal = GPSR_CONTINUE;
		  else
			  ReturnVal = GPSR_FAIL;
	  }	 

  }
  else
  {
	  cout<<"something wrong in GPSR_Run?"<<endl;
  }
   return(ReturnVal);
}// GPSR routing

void GVExposureSensorNetwork2D::GPSR_UpdateCurrentNode(int Next)
{ 
  int i = 0;                                                                   // Index Variable
  int j = 0;                                                                   // Index Variable
  int ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS;              // Return Value
  int GridH = Grid.GetSizeH();                                                 // Number of Horizontal Grid Squares
  int GridV = Grid.GetSizeV();                                                 // Number of Vertical Grid Squares
  int NbrNum = NumNeighbors[Next];                                             // Number of Neighbors
  int NbrIndex = 0;                                                            // Index of Current Neighbor
  double D = 0.0;                                                              // Temporary Distance
  double X = 0.0;                                                              // Temporary X Coordinate
  double Y = 0.0;                                                              // Temporary Y Coordinate
  double SensorX = Sensors[Next].GetX();                                       // Sensor X Coordinate
  double SensorY = Sensors[Next].GetY();                                       // Sensor Y Coordinate
   
  // Set Next Sensor Attributes
  if (Sensors[Next].GetParent() < 0) Sensors[Next].SetParent(CurrentNode);

  // Set Algorithm Parameters
  CurrentNode = Next;
  GPSR_Path[NumCommunications] = Next;
  ++NumCommunications;
  // for debug
  if(NumCommunications > MaxLength)
	  cout<<"The path lengh exceeds the maximal!"<<endl;

  // Set Current Sensor Attributes
  Sensors[CurrentNode].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_VISITED);

  // Set Neighbors to Known
  for (i = 0; i < NbrNum; ++i)
  {
    NbrIndex = NeighborIndex[CurrentNode][i];
	// if a node is has been visited, the state will not be update here
    if (!Sensors[NbrIndex].GetKnown()) Sensors[NbrIndex].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN);
  }
}// GPSR_UpdateCurrentNode  

void GVExposureSensorNetwork2D::GPSR_ShortestPath(void)
{
	CalculateBreadthFirstSearch(DestinationNode);
	ShortestPath = Sensors[SourceNode].GetCost();
}

int GVExposureSensorNetwork2D::GetShortestPathLength(void)
{
	return ShortestPath;
}

int GVExposureSensorNetwork2D::GetGPSRPathLength(void)
{
	return NumCommunications;
}

void GVExposureSensorNetwork2D::GPSR_OutputResults(char* FileName)
/*
  OutputResults() calculates the required results and outputs them to the
  specified text file (FileName).
*/
{ // OutputResults
  int i = 0;                   // Index Variable
  int j = 0;                   // Index Variable
  int SensorsTotal = 0;        // Total Number of Sensors in Ellipse
  int SensorsUnknown = 0;      // Number of Sensors Unknown in Ellipse
  int SensorsKnown = 0;        // Number of Sensors Known (and Useless) in Ellipse
  int SensorsVisited = 0;      // Number of Sensors Visited in Ellipse
  double D = 0.0;              // Temporary Distance
  double X = 0.0;              // Temporary X Coordinate
  double Y = 0.0;              // Temporary Y Coordinate
  double SX = Start.GetX();    // Starting X Coordinate
  double SY = Start.GetY();    // Starting Y Coordinate
  double EX = End.GetX();      // End X Coordinate
  double EY = End.GetY();      // End Y Coordinate
  ofstream OutFile;            // Output File

  // Output Results

  //OutFile.open(FileName, ios::app);
  
  cout.width(6);
  cout << setiosflags(ios::fixed | ios::showpoint) << setprecision(2);
  
  cout << setw(4) << SourceNode << GVEXPOSURESENSORNETWORK2D_SPACE
	   << setw(4) << DestinationNode << GVEXPOSURESENSORNETWORK2D_SPACE
	   << Start.GetX() << GVEXPOSURESENSORNETWORK2D_SPACE
	   << Start.GetY() << GVEXPOSURESENSORNETWORK2D_SPACE
	   << End.GetX()   << GVEXPOSURESENSORNETWORK2D_SPACE
	   << End.GetY()   << GVEXPOSURESENSORNETWORK2D_SPACE
	   << setw(4) << NumCommunications  << GVEXPOSURESENSORNETWORK2D_SPACE
	   << setw(4) << ShortestPath << GVEXPOSURESENSORNETWORK2D_SPACE
	   << setw(4) << MessageDeliver << GVEXPOSURESENSORNETWORK2D_SPACE
	   << setw(4) << GPSR_Path[1] << GVEXPOSURESENSORNETWORK2D_SPACE
       << endl;
		  
/*		  
  for(i=0;i<=NumCommunications; i++)
	  cout << setw(4) << GPSR_Path[i] << GVEXPOSURESENSORNETWORK2D_SPACE;
  cout << endl;
  */
  //OutFile.close();
  
} // OutputResults

void GVExposureSensorNetwork2D::GenerateEvent(int NV)
{
	NumEvent = NV;
	Event = new int[NumEvent];
	for(int i=0; i< NumEvent; i++)
	{
		Event[i] = CoinToss( EVENTPROBABILITY );
	}
}

void GVExposureSensorNetwork2D::DetectionInitialize(void)
{
	double EnergyTransmit = 0.0;
	double EnergyReceive = 0.0;
	double EnergyMonitor = 0.0;
	double EnergyProcess = 0.0;
	double EnergyTotal  = 0.0;
}

void GVExposureSensorNetwork2D::DetectEvent(int NodeIndex)
{
	int i = 0;
	int DecisionMade = 0;
	int CountOnes = 0;
	int CountZeros = 0;
	int DetectionResult = 0;
	int NumSample = 0;
	int SentPacketSize = 0;;
	int DetectBit;
	int SentPacketSizeHybrid = 0;
	int SentPacketSizeCentral = 0;
	int SentPacketSizeDist = 0;
	
	double DistanceSquareSum = 0.0;	
	for(int j=0; j<NumCommunications; j++)
	{
		DistanceSquareSum += SQUARE(Distance2D(GetX(GPSR_Path[j]),GetY(GPSR_Path[j]),GetX(GPSR_Path[j+1]),GetY(GPSR_Path[j+1])));
	}

	SentPacketSizeDist = 1;
	SentPacketSizeCentral = ceil(LOGN(NumEvent+1,2));

	// simulate the hybrid detection scheme
	while(!DecisionMade && i<NumEvent)
	{
		// detected bit
		if(Event[i] == 1)
			DetectBit = CoinToss(P1);
		else
			DetectBit = CoinToss(P0);

		if(DetectBit == 1)
			CountOnes++;
		else
			CountZeros++;

		if(CountOnes >= N2)
		{
			DetectionResult = 1;
			DecisionMade = 1;
		}
		else if(CountZeros >= (NumEvent - N1))
		{
			DetectionResult = 0;
			DecisionMade = 1;
		}

		i++;
	}
	
	NumSample = CountOnes;

	if(DecisionMade)
		SentPacketSizeHybrid = 1;
	else
		SentPacketSizeHybrid = ceil(LOGN((N2-N1-1),2))>0 ? ceil(LOGN((N2-N1-1),2)): 1;

	// should consider processor rather than a hardware adder or counter
	// receive one bit, the processor has to use one cycle to process the bit information
	// before send out, the processor needs one cycle to encode the result/raw data
	switch (DetectionScheme)
	{
	case CENTRALIZED:
		EnergyTransmit = SentPacketSizeCentral * UNITENERGYTRANSMIT * DistanceSquareSum; //NumCommunications;
		EnergyReceive = SentPacketSizeCentral * UNITENERGYRECEIVE * DistanceSquareSum;//(NumCommunications-1);
		EnergyMonitor = NumEvent * UNITENERGYMONITOR;
		EnergyProcess = 1 * UNITENERGYPROCESS;
		break;
	case DISTRIBUTED:
		EnergyTransmit = SentPacketSizeDist * UNITENERGYTRANSMIT * DistanceSquareSum; //NumCommunications;
		EnergyReceive = SentPacketSizeDist * UNITENERGYRECEIVE * DistanceSquareSum; //(NumCommunications-1);
		EnergyMonitor = NumEvent * UNITENERGYMONITOR;
		EnergyProcess = (NumEvent+1) * UNITENERGYPROCESS;
		break;
	case HYBRID:
	default:
		EnergyTransmit = SentPacketSizeHybrid * UNITENERGYTRANSMIT * DistanceSquareSum; //NumCommunications;
		EnergyReceive = SentPacketSizeHybrid * UNITENERGYRECEIVE * DistanceSquareSum; //(NumCommunications-1);
		EnergyMonitor = NumSample * UNITENERGYMONITOR;
		EnergyProcess = (NumSample+1) * UNITENERGYPROCESS;
		break;
	}

	EnergyTotal = EnergyProcess + EnergyMonitor + EnergyTransmit + EnergyReceive;

//	Sensors[NodeIndex].SetEnergyTransmit( EnergyTransmit );
//	Sensors[NodeIndex].SetEnergyReceive( EnergyReceive );
//	Sensors[NodeIndex].SetEnergyMonitor( EnergyMonitor );
//	Sensors[NodeIndex].SetEnergyProcess( EnergyProcess );
}

void GVExposureSensorNetwork2D::SetDetectionScheme(int DS)
{
	DetectionScheme = DS;
}

double GVExposureSensorNetwork2D::GetTotalEnergy(void)
{
	return EnergyTotal;
}

double GVExposureSensorNetwork2D::GetTransmitEnergy(void)
{
	return EnergyTransmit;
}

double GVExposureSensorNetwork2D::GetReceiveEnergy(void)
{
	return EnergyReceive;
}

double GVExposureSensorNetwork2D::GetMonitorEnergy(void)
{
	return EnergyMonitor;
}

double GVExposureSensorNetwork2D::GetProcessEnergy(void)
{
	return EnergyProcess;
}

// Lin's Functions end
//////////////////////////////////////////////////////////////////////////

int GVExposureSensorNetwork2D::MaxExposure_UpdateGPSR(void)
/*
  MaxExposure_UpdateGPSR() updates the localized algorithm using the GPSR
  algorithm (counterclockwise around the edges).
*/
{ // MaxExposure_UpdateGPSR
  int i = 0;                                                      // Index Variable
  int j = 0;                                                      // Index Variable
  int ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS; // Return Value
  int NbrIndex = 0;                                               // Neighboring Index
  int NbrNum = 0;                                                 // Number of Neighbors
  int NewArea = 0;                                                // Area From Neighbor Node
  int NewIndex = 0;                                               // Index From Neighbor Node
  int NewHoleExists = 0;                                          // Holes From Neighbor Node
  int BestArea = 0;                                               // Area From Best Node
  int BestIndex = 0;                                              // Index From Best Node
  int BestHoleExists = 0;                                         // Holes From Best Node
  int KeepGoing = 1;                                              // Keep Searching?
  double NewHeuristic = 0.0;                                      // Heuristic From Neighbor Node
  double BestHeuristic = 0.0;                                     // Heuristic From Best Node
  double Angle = 0.0;                                             // Current Angle Formed
  double CurrentX = 0.0;                                          // Current X Coordinate
  double CurrentY = 0.0;                                          // Current Y Coordinate
  double AX = 0.0;                                                // X Coordinate of Vector From Previous Node
  double AY = 1.0;                                                // Y Coordinate of Vector From Previous Node
  double BX = 0.0;                                                // X Coordinate of Vector To Next Node
  double BY = 0.0;                                                // Y Coordinate of Vector To Next Node
  double ToDegrees = (180.0 / 3.14159);                           // Conversion To Degrees

  if (!MaxExposureInitialized) MaxExposure_Initialize();
  if ((CurrentIteration >= GVEXPOSURESENSORNETWORK2D_MAXTRIES) || (NextNode < 0) || (NextNode >= NumSensors)) ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_ERROR;
  if ((ReturnVal == GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS) && (Sensors != NULL) && (NumNeighbors != NULL) && (NeighborIndex != NULL))
  {
    if (CurrentNode >= 0)
    {
      AX = Sensors[CurrentNode].GetX() - Sensors[NextNode].GetX();
      AY = Sensors[CurrentNode].GetY() - Sensors[NextNode].GetY();
    }
    BestHoleExists = Grid.GetSizeH() * Grid.GetSizeV();
    BestIndex = GVEXPOSUREGRIDATTRIBUTES_MAXLEVEL;
    BestArea = 0;
    BestHeuristic = 0.0;
    ReturnVal = MaxExposure_UpdateCurrentNode(NextNode);
	if(ReturnVal == 0)
		//cout<<"here it is done"<<endl;
    if (ReturnVal)
    {
      if (MessageMode == GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH)
      {
        CurrentX = Sensors[CurrentNode].GetX();
        CurrentY = Sensors[CurrentNode].GetY();
        NextNode = -1;
        NbrNum = NumNeighbors[CurrentNode];
        for (i = 0; i < NbrNum; ++i)
        {
          NbrIndex = NeighborIndex[CurrentNode][i];
          if (Sensors[NbrIndex].GetKnown() == GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN)
          {
            NewArea = MaxExposure_CalculatePotentialArea(NbrIndex, GVEXPOSURESENSORNETWORK2D_GRID_POTENTIAL);
            if (NewArea > 0)
            {
              NewHoleExists = MaxExposure_CalculateHoleCreation(NbrIndex);
              if (NewHoleExists < BestHoleExists)
              {
                BX = Sensors[NbrIndex].GetX() - CurrentX;
                BY = Sensors[NbrIndex].GetY() - CurrentY;
                Angle = acos((AX * BX + AY * BY) / (Distance2D(0.0, 0.0, AX, AY) * Distance2D(0.0, 0.0, BX, BY))) * ToDegrees;
                BestHoleExists = NewHoleExists;
                BestIndex = MaxExposure_CalculateIndex(NbrIndex);
                BestArea = NewArea;
                BestHeuristic = (Angle / 180.0) * BestArea;
                NextNode = NbrIndex;
              }
              else if (NewHoleExists == BestHoleExists)
              {
                NewIndex = MaxExposure_CalculateIndex(NbrIndex);
                if (NewIndex < BestIndex)
                {
                  BX = Sensors[NbrIndex].GetX() - CurrentX;
                  BY = Sensors[NbrIndex].GetY() - CurrentY;
                  Angle = acos((AX * BX + AY * BY) / (Distance2D(0.0, 0.0, AX, AY) * Distance2D(0.0, 0.0, BX, BY))) * ToDegrees;
                  BestHoleExists = NewHoleExists;
                  BestIndex = NewIndex;
                  BestArea = NewArea;
                  BestHeuristic = (Angle / 180.0) * BestArea;
                  NextNode = NbrIndex;
                }
                else if (NewIndex == BestIndex)
                {
                  BX = Sensors[NbrIndex].GetX() - CurrentX;
                  BY = Sensors[NbrIndex].GetY() - CurrentY;
                  Angle = acos((AX * BX + AY * BY) / (Distance2D(0.0, 0.0, AX, AY) * Distance2D(0.0, 0.0, BX, BY))) * ToDegrees;
                  NewHeuristic = (Angle / 180.0) * NewArea;
                  if (NewHeuristic > BestHeuristic)
                  {
                    BestHoleExists = NewHoleExists;
                    BestIndex = NewIndex;
                    BestArea = NewArea;
                    BestHeuristic = NewHeuristic;
                    NextNode = NbrIndex;
                  }
                }
              }
            }
            else
            {
              Sensors[NbrIndex].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_USELESS);
            }
		  }
          MaxExposure_ErasePotentialArea(NbrIndex, GVEXPOSURESENSORNETWORK2D_GRID_POTENTIAL);
		}
        if (NextNode < 0)
        {
          BestHeuristic = 10000.0;
          for (i = 0; i < NumSensors; ++i)
          {
			//find the closest neighbor
            if (Sensors[i].GetKnown() == GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN)
            {
              NewHeuristic = Distance2D(CurrentX, CurrentY, Sensors[i].GetX(), Sensors[i].GetY());
              if (NewHeuristic < BestHeuristic)
              {
                BestHeuristic = NewHeuristic;
                NextNode = i;
              }
            }
          }
          if (NextNode >= 0)
          {
			// find a path to this neighbor using BFS
            DestinationNode = NextNode;
            CalculateBreadthFirstSearch(DestinationNode);
            NextNode = Sensors[CurrentNode].GetParent();
            MessageMode = GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_FORWARD;
            if (NextNode < 0) ReturnVal = 2; //message deliveray failed
          }
          else
          {
            ReturnVal = 0;//??success
			//ReturnVal = 2;//my new code
          }
        }
        if (GVEXPOSURESENSORNETWORK2D_PROGRESSMESSAGE) cout << "Searching... (" << BestHoleExists << ", " << BestIndex << ", " << BestArea << ")" << endl;
      }
      if (MessageMode == GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_FORWARD)
      {
        NextNode = Sensors[CurrentNode].GetParent();
        if (NextNode == DestinationNode) MessageMode = GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH;
        else if (NextNode < 0) ReturnVal = 2; //deliveray failed
        if (GVEXPOSURESENSORNETWORK2D_PROGRESSMESSAGE) cout << "Forwarding... " << endl;
      }
    }
  }
  if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL)
  {
    for (i = 0; i < NumSensors; ++i)
    {
      if (Sensors[i].GetKnown() == GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN)
      {
        NewArea = MaxExposure_CalculatePotentialArea(i, GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED);
        if (NewArea <= 0) Sensors[i].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_USELESS);
      }
    }
  }
  if (GVEXPOSURESENSORNETWORK2D_PROGRESSMESSAGE)
  {
    if (ReturnVal == GVEXPOSURESENSORNETWORK2D_RETURNVAL_DONE)
    {
      cout << "Algorithm Comlete!" << endl;
    }
    else if (ReturnVal == GVEXPOSURESENSORNETWORK2D_RETURNVAL_ERROR)
    {
      cout << "Algorithm Terminated With Errors!" << endl;
    }
  }
  cout<<"GPSR"<<endl;
  return(ReturnVal);
} // MaxExposure_UpdateGPSR


int GVExposureSensorNetwork2D::MaxExposure_Run(void)
/*
  MaxExposure_Run() runs the maximal exposure algorihtm from initialization
  to completion.
*/
{ // MaxExposure_Run
  int ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS;

  if (!MaxExposureInitialized) AlgorithmStage = GVEXPOSURESENSORNETWORK2D_MODE_INIT;

  switch(AlgorithmStage)
  {
    case GVEXPOSURESENSORNETWORK2D_MODE_UPDATE:
      if (!MaxExposure_Update()) 
	  {
		AlgorithmStage = GVEXPOSURESENSORNETWORK2D_MODE_CALCULATE;
		ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_DONE;
	  }
    break;

    case GVEXPOSURESENSORNETWORK2D_MODE_CALCULATE:
      MaxExposure_Calculate();
      Reset();
      ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_DONE;
    break;

    case GVEXPOSURESENSORNETWORK2D_MODE_INIT:
    default:
      MaxExposure_Initialize();
      AlgorithmStage = GVEXPOSURESENSORNETWORK2D_MODE_UPDATE;
    break;
  }

  return(ReturnVal);
} // MaxExposure_Run

int GVExposureSensorNetwork2D::MaxExposure_Update(void)
/*
  MaxExposure_Update() calls the correct updating function for the algorithm.
*/
{ // MaxExposure_Update
  int ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS; // Return Value

  switch(LocMaxExposureMode)
  {
    case GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_GPSR:
      ReturnVal = MaxExposure_UpdateGPSR();
    break;

    case GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_HEURISTIC:
    default:
      ReturnVal = MaxExposure_UpdateHeuristic();
    break;
  }

  return(ReturnVal);
} // MaxExposure_Update



void GVExposureSensorNetwork2D::MaxExposure_Initialize(void)
/*
  MaxExposure_Initialize() initializes the class for the maximal exposure
  algorithm.  Separate initialization exists for localized and centralized
  versions.
*/
{ // MaxExposure_Initialize
  int i = 0;                                              // Index Variable
  int j = 0;                                              // Index Variable
  int GridH = Grid.GetSizeH();                            // Horizontal Grid Squares
  int GridV = Grid.GetSizeV();                            // Vertical Grid Squares
  double InitialX = 0.0;                                  // Ideal Initial Sensor X Coordinate
  double InitialY = 0.0;                                  // Ideal Initial Sensor Y Coordinate
  double StartX = Start.GetX();                           // Starting X Coordinate
  double StartY = Start.GetY();                           // Starting Y Coordinate
  double EndX = End.GetX();                               // Ending X Coordinate
  double EndY = End.GetY();                               // Ending Y Coordinate
  double VX = EndX - StartX;                              // X Coordinate of Vector from Start to End
  double VY = EndY - StartY;                              // Y Coordinate of Vector from Start to End
  double VN = Distance2D(0.0, 0.0, VX, VY);               // Normalization Constant for Vector from Start to End
  double VScale = (MaxLength - VN) / (2.0 * VN);          // Scale Factor for Vector from Start to End (So it touches the edge of the ellipse)
  double D = 0.0;                                         // Temporary Distance
  double MinDistance = GVSENSORNETWORK_INFINITY;          // Current Minimum Distance

  if ((Sensors != NULL) && (NumNeighbors != NULL) && (NeighborIndex != NULL))
  {
    if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_CENTRAL)
    {
      for (i = 0; i < NumSensors; ++i)
      {
        Sensors[i].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN);
      }
      for (i = 0; i < GridH; ++i)
      {
        for (j = 0; j < GridV; ++j)
        {
          Grid.SetCovered(i, j, GVEXPOSURESENSORNETWORK2D_GRID_COVERED);
        }
      }
      CurrentNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
      NextNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
      SourceNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
      DestinationNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
      MaxExposureInitialized = 1;
      if (CenMaxPath != NULL) delete[] LocMaxPath;
      CenMaxPath = NULL;
      NumCenMaxPath = 0;
    }
    else if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL)
    {
      GridH = Grid.GetSizeH();
      GridV = Grid.GetSizeV();
      VX *= VScale;
      VY *= VScale;
      InitialX = StartX - VX;
      InitialY = StartY - VY;
      for (i = 0; i < GridH; ++i)
      {
        for (j = 0; j < GridV; ++j)
        {
          Grid.SetCovered(i, j, GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED);
        }
      }
      NextNode = CalculateNearestSensor(InitialX, InitialY);
      CurrentNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
      SourceNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
      DestinationNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
      CurrentIteration = 0;
      NumCommunications = 0;
      MaxExposureInitialized = 1;
      MessageMode = GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH;
      Grid.SetMinHorizontal(0);
      Grid.SetMaxHorizontal(GridH - 1);
      Grid.SetMinVertical(0);
      Grid.SetMaxVertical(GridV - 1);
      if (LocMaxPath != NULL) delete[] LocMaxPath;
      LocMaxPath = NULL;
      NumLocMaxPath = 0;
    }
    else
    {
      if (CenMaxPath != NULL) delete[] CenMaxPath;
      CenMaxPath = NULL;
      NumCenMaxPath = 0;
    }
  }
} // MaxExposure_Initialize


void GVExposureSensorNetwork2D::SetAlgorithmMode(int AM)
/*
  SetAlgorithmMode() sets the algorithm type to LOCALIZED or CENTRALIZED.
*/
{ // SetAlgorithmMode
  if ((AM >= GVEXPOSURESENSORNETWORK2D_ALGORITHM_CENTRAL) && (AM < GVEXPOSURESENSORNETWORK2D_ALGORITHM_NUM)) AlgorithmMode = AM;
  Reset();
} // SetAlgorithmMode



void GVExposureSensorNetwork2D::SetCenMinExposureMode(int MESM)
/*
  SetCenMinExposureMode() sets the centralized minimal exposure mode to
  DEFAULT.
*/
{ // SetCenMinExposureMode
  if ((MESM >= GVEXPOSURESENSORNETWORK2D_CENMINEXPOSUREMODE_DEFAULT) && (MESM < GVEXPOSURESENSORNETWORK2D_CENMINEXPOSUREMODE_NUM)) CenMinExposureMode = MESM;
} // SetCenMinExposureMode



void GVExposureSensorNetwork2D::SetCenMaxExposureMode(int MESM)
/*
  SetCenMaxExposureMode() sets the centralzied maximal exposure mode to
  RANDOM, SHORTEST PATH, BEST PATH, ADJUSTED BEST PATH.
*/
{ // SetCenMaxExposureMode
  if ((MESM >= GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_RANDOM) && (MESM < GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_NUM)) CenMaxExposureMode = MESM;
} // SetCenMaxExposureMode



void GVExposureSensorNetwork2D::SetLocMinExposureMode(int MESM)
/*
  SetLocMinExposureMode() sets the search mode for minimal exposure to
  DEFAULT.
*/
{ // SetLocMinExposureMode
  if ((MESM >= GVEXPOSURESENSORNETWORK2D_LOCMINEXPOSUREMODE_DEFAULT) && (MESM < GVEXPOSURESENSORNETWORK2D_LOCMINEXPOSUREMODE_NUM)) LocMinExposureMode = MESM;
  Reset();
} // SetLocMinExposureMode



void GVExposureSensorNetwork2D::SetLocMaxExposureMode(int MESM)
/*
  SetLocMaxExposureMode() sets the search mode for maximal exposure to
  HEURISTIC or GPSR.
*/
{ // SetLocMaxExposureMode
  if ((MESM >= GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_HEURISTIC) && (MESM < GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_NUM)) LocMaxExposureMode = MESM;
  Reset();
} // SetLocMaxExposureMode



void GVExposureSensorNetwork2D::SetGridParameters(int GH, int GV)
/*
  SetGridParameters() re-calculates a maximal or minimal exposure path grid
  depending on the existing maximum length.
*/
{ // SetGridParameters
  if (MaxLength < 0.0) MinExposure_CalculateGrid(GH, GV);
  else MaxExposure_CalculateGrid(GH, GV);
  Reset();
} // SetGridParameters



void GVExposureSensorNetwork2D::SetPathParameters(double SX, double SY, double EX, double EY)
/*
  SetPathParameters() sets the parameters for the minimal exposure path and
  calculates a minimal exposure path grid.
*/
{ // SetPathParameters
  Start.SetX(SX);
  Start.SetY(SY);
  End.SetX(EX);
  End.SetY(EY);
  MaxLength = 1000;
  SetGridParameters(Grid.GetSizeH(), Grid.GetSizeV());
} // SetPathParameters

void GVExposureSensorNetwork2D::SetPathParameters(double SX, double SY, double EX, double EY, double ML)
/*
  SetPathParameters() sets the parameters for the maximal exposure path and
  calculates a maximal exposure path grid.
*/
{ // SetPathParameters
  Start.SetX(SX);
  Start.SetY(SY);
  End.SetX(EX);
  End.SetY(EY);
  MaxLength = ML;
  SetGridParameters(Grid.GetSizeH(), Grid.GetSizeV());
} // SetPathParameters



int GVExposureSensorNetwork2D::MinExposure_Update(void)
/*
  
*/
{ // MinExposure_Update
  int ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_DONE; // Return Value

  switch(MessageMode)
  {
    case GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH:
      ++NumCommunications;
      ReturnVal = MinExposure_UpdateMessageSearch();
    break;

    case GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_FORWARD:
      ++NumCommunications;
      ReturnVal = MinExposure_UpdateMessageForward();
    break;

    case GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_CHECK:
      ReturnVal = MinExposure_UpdateMessageCheck();
    break;
  }
  return(ReturnVal);
} // MinExposure_Update



void GVExposureSensorNetwork2D::MinExposure_Initialize(void)
/*
  MinExposure_Initialize() initializes the minimal exposure algorithsm.
  For Centralized exposure, it sets all grid squares to COVERED and all
  sensors to KNOWN.
*/
{ // MinExposure_Initialize
  int i = 0;                                              // Index Variable
  int j = 0;                                              // Index Variable
  int GridH = Grid.GetSizeH();                            // Horizontal Grid Squares
  int GridV = Grid.GetSizeV();                            // Vertical Grid Squares
  double InitialX = 0.0;                                  // Ideal Initial Sensor X Coordinate
  double InitialY = 0.0;                                  // Ideal Initial Sensor Y Coordinate
  double StartX = Start.GetX();                           // Starting X Coordinate
  double StartY = Start.GetY();                           // Starting Y Coordinate
  double EndX = End.GetX();                               // Ending X Coordinate
  double EndY = End.GetY();                               // Ending Y Coordinate
  double VX = EndX - StartX;                              // X Coordinate of Vector from Start to End
  double VY = EndY - StartY;                              // Y Coordinate of Vector from Start to End
  double VN = Distance2D(0.0, 0.0, VX, VY);               // Normalization Constant for Vector from Start to End
  double VScale = (MaxLength - VN) / (2.0 * VN);          // Scale Factor for Vector from Start to End (So it touches the edge of the ellipse)
  double D = 0.0;                                         // Temporary Distance
  double MinDistance = GVSENSORNETWORK_INFINITY;          // Current Minimum Distance

  if ((Sensors != NULL) && (NumNeighbors != NULL) && (NeighborIndex != NULL))
  {
    if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_CENTRAL)
    {
      for (i = 0; i < NumSensors; ++i)
      {
        Sensors[i].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN);
      }
      for (i = 0; i < GridH; ++i)
      {
        for (j = 0; j < GridV; ++j)
        {
          Grid.SetCovered(i, j, GVEXPOSURESENSORNETWORK2D_GRID_COVERED);
        }
      }
      CurrentNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
      NextNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
      SourceNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
      DestinationNode = GVEXPOSURESENSORNETWORK2D_NOSENSOR;
      MinExposureInitialized = 1;
      if (CenMaxPath != NULL) delete[] LocMaxPath;
      CenMaxPath = NULL;
      NumCenMaxPath = 0;
    }
    else if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL)
    {
      for (i = 0; i < NumSensors; ++i)
      {
        Sensors[i].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_UNKNOWN);
        Sensors[i].SetParent(GVEXPOSURESENSORNETWORK2D_NOSENSOR);
      }
      for (i = 0; i < GridH; ++i)
      {
        for (j = 0; j < GridV; ++j)
        {
          Grid.SetCovered(i, j, GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED);
        }
      }
      CurrentNode = CalculateNearestSensor(StartX, StartY);
      NextNode = CurrentNode;
      SourceNode = CurrentNode;
      DestinationNode = CalculateNearestSensor(EndX, EndY);
      CurrentIteration = 0;
      NumCommunications = 0;
      MinExposureInitialized = 1;
      MessageMode = GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH;
      Grid.SetMinHorizontal(0);
      Grid.SetMaxHorizontal(GridH - 1);
      Grid.SetMinVertical(0);
      Grid.SetMaxVertical(GridV - 1);
      if (LocMaxPath != NULL) delete[] LocMaxPath;
      LocMaxPath = NULL;
      NumLocMaxPath = 0;
    }
    else
    {
      if (CenMaxPath != NULL) delete[] CenMaxPath;
      CenMaxPath = NULL;
      NumCenMaxPath = 0;
    }
  }
} // MinExposure_Initialize



void GVExposureSensorNetwork2D::MinExposure_Calculate(void)
/*
*/
{ // MinExposure_Calculate
  int i = 0;                               // Index Variable
  int j = 0;                               // Index Variable
  int GridH = Grid.GetSizeH();             // Grid Horizontal Size
  int GridV = Grid.GetSizeV();             // Grid Vertical Size
  int Total = GridH * GridV;               // Total Number of Grid Points
  int CurrentH = 0;                        // Current Horizontal Index
  int CurrentV = 0;                        // Current Vertical Index
  int StartH = 0;                          // Start Horizontal Index
  int StartV = 0;                          // Start Vertical Index
  int EndH = 0;                            // End Horizontal Index
  int EndV = 0;                            // End Vertical Index
  int NumPath = 0;                         // Number of Elements in Path
  int* Path = NULL;                        // Path to Store Points
  double StartX = Start.GetX();            // Starting X Coordinate
  double StartY = Start.GetY();            // Starting Y Coordinate
  double EndX = End.GetX();                // Ending X Coordinate
  double EndY = End.GetY();                // Ending Y Coordinate
  double X = 0.0;                          // Temporary X Coordinate
  double Y = 0.0;                          // Temporary Y Coordinate
  double D = 0.0;                          // Distance
  double MinSD = GVSENSORNETWORK_INFINITY; // Min Start Point Distance
  double MinED = GVSENSORNETWORK_INFINITY; // Min End Point Distance

  // Pick Nearest Grid Point to Starting Point
  Path = new int[Total];
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    { 
       X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
       Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
       D = Distance2D(StartX, StartY, X, Y);
       if (D < MinSD)
       {
         StartH = i;
         StartV = j;
         MinSD = D;
       }
       D = Distance2D(EndX, EndY, X, Y);
       if (D < MinED)
       {
         EndH = i;
         EndV = j;
         MinED = D;
       }
    }
  }
  NumPath = MinExposure_GridDijkstraSearch(StartH, StartV, EndH, EndV, Path);
  if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_CENTRAL)
  {
    // Allocate the Path
    if (CenMinPath != NULL) delete[] CenMinPath;
    NumCenMinPath = NumPath + 2;
    CenMinPath = new GVPoint2D[NumCenMinPath];
    // Store the Start and End Point
    CenMinPath[0].SetX(StartX);
    CenMinPath[0].SetY(StartY);
    CenMinPath[NumCenMinPath - 1].SetX(EndX);
    CenMinPath[NumCenMinPath - 1].SetY(EndY);
    // Copy the Elements
    for (i = 0; i < NumPath; ++i)
    {
      CurrentH = DecodeGridIndexH(Path[i]);
      CurrentV = DecodeGridIndexV(Path[i]);
      CenMinPath[i + 1].SetX(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
      CenMinPath[i + 1].SetY(Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
    }
  }
  else if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL)
  {
    // Allocate the Path
    if (LocMinPath != NULL) delete[] LocMinPath;
    NumLocMinPath = NumPath + 2;
    LocMinPath = new GVPoint2D[NumLocMinPath];
    // Store the Start and End Point
    LocMinPath[0].SetX(StartX);
    LocMinPath[0].SetY(StartY);
    LocMinPath[NumLocMinPath - 1].SetX(EndX);
    LocMinPath[NumLocMinPath - 1].SetY(EndY);
    // Copy the Elements
    for (i = 0; i < NumPath; ++i)
    {
      CurrentH = DecodeGridIndexH(Path[i]);
      CurrentV = DecodeGridIndexV(Path[i]);
      LocMinPath[i + 1].SetX(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
      LocMinPath[i + 1].SetY(Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
    }
  }
  if (Path != NULL) delete[] Path;
} // MinExposure_Calculate



int GVExposureSensorNetwork2D::MinExposure_GridDijkstraSearch(int SH, int SV, int EH, int EV, int* Path)
/*
  MinExposure_GridDijkstraSearch() calculates a Dijkstra search on the grid
  squares except that it uses cumulative Exposure as the metric as opposed to
  distance.
*/
{ // MinExposure_GridDijkstraSearch
  int i = 0;                   // Index Variable
  int j = 0;                   // Index Variable
  int CurrentH = 0;            // Current H Value
  int CurrentV = 0;            // Current V Value
  int CurrentIndex = 0;        // Current Encoded HV Value
  int NeighborH = 0;           // Current H Value
  int NeighborV = 0;           // Current V Value
  int NeighborIndex = 0;       // Neighbor Encoded HV Value
  int GridH = Grid.GetSizeH(); // Grid Horizontal Size
  int GridV = Grid.GetSizeV(); // Grid Vertical Size
  int Total = GridH * GridV;   // Total Grid Points
  int Finished = 0;            // Is Search Over
  int NumPath = 0;             // Number in Path
  int NextIndexStatus = 0;     // Status of Neighbor Node (DEQUEUED, ENQUEUED, FINISHED)
  int AddIndex = 0;            // Index Position to Add Element
  int NumQueue = 0;            // Number of Elements in Index Queue
  int NumNbr = 0;              // Number of Neighbors of Node
  int Covered = 0;             // Was the Grid Square Covered?
  int* Neighbors = NULL;       // Neighbors of Node
  int* IndexQueue = NULL;      // Index Queue for Nodes in Network
  double CurrentCost = 0.0;    // Current Cost
  double MinCost = 0.0;        // Minimum Cost

  IndexQueue = new int[Total];
  Neighbors = new int[2 * (GridH + GridV)];
  // Initialize Dijkstra Search (Set All Nodes to DEQUEUED, INFINITY, NOPARENT)
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    {
      Grid.SetCost(i, j, GVSENSORNETWORK_INFINITY);
      Grid.SetEnqueued(i, j, GVEXPOSURESENSORNETWORK2D_BFS_DEQUEUED);
      Grid.SetParent(i, j, GVEXPOSURESENSORNETWORK2D_NOPARENT, GVEXPOSURESENSORNETWORK2D_NOPARENT);
    }
  }
  // Add Start Node To Queue
  if (Grid.GetCovered(SH, SV))
  {
    CurrentIndex = EncodeGridIndex(SH, SV);
    Grid.SetEnqueued(SH, SV, GVEXPOSURESENSORNETWORK2D_BFS_ENQUEUED);
    Grid.SetCost(SH, SV, 0.0);
    IndexQueue[NumQueue] = CurrentIndex;
    ++NumQueue;
  }
  // While Not Finished
  while(!Finished)
  {
    // Pick Index From Index Queue With Min Cost
    CurrentH = -1;
    CurrentV = -1;
    CurrentIndex = -1;
    for (MinCost = GVSENSORNETWORK_INFINITY, i = 0; i < NumQueue; ++i)
    {
      j = IndexQueue[i];
      NeighborH = DecodeGridIndexH(j);
      NeighborV = DecodeGridIndexV(j);
      if (Grid.GetEnqueued(NeighborH, NeighborV) == GVEXPOSURESENSORNETWORK2D_BFS_ENQUEUED)
      {
        CurrentCost = Grid.GetCost(NeighborH, NeighborV);
        if (CurrentCost < MinCost)
        {
          AddIndex = i;
          CurrentIndex = j;
          MinCost = CurrentCost;
        }
      }
    }
    if (CurrentIndex >= 0)
    {
      CurrentH = DecodeGridIndexH(CurrentIndex);
      CurrentV = DecodeGridIndexV(CurrentIndex);
      // Mark Index As Dequeued
      Grid.SetEnqueued(CurrentH, CurrentV, GVEXPOSURESENSORNETWORK2D_BFS_FINISHED);
      if ((CurrentH == EH) && (CurrentV == EV))
      {
        Finished = 1;
      }
    }
    else
    {
      Finished = 1;
    }
    if (!Finished)
    {
      NumNbr = CalculateSharedNeighbors(CurrentH, CurrentV, CurrentH, CurrentV, Neighbors);
      for (i = 0; i < NumNbr; ++i)
      {
        j = Neighbors[i];
        NeighborH = DecodeGridIndexH(j);
        NeighborV = DecodeGridIndexV(j);
        NextIndexStatus = Grid.GetEnqueued(NeighborH, NeighborV);
        if (NextIndexStatus != GVEXPOSURESENSORNETWORK2D_BFS_FINISHED)
        {
          CurrentCost = Grid.GetCost(CurrentH, CurrentV) + CalculateLineExposure(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointX(NeighborH, NeighborV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(NeighborH, NeighborV, GVGRIDPOINT2D_CENTERPOINT));
          if (CurrentCost < Grid.GetCost(NeighborH, NeighborV))
          {
            Grid.SetCost(NeighborH, NeighborV, CurrentCost);
            Grid.SetParent(NeighborH, NeighborV, CurrentH, CurrentV);
          }
          if (NextIndexStatus == GVEXPOSURESENSORNETWORK2D_BFS_DEQUEUED)
          {
            if (Grid.GetCovered(NeighborH, NeighborV) == GVEXPOSURESENSORNETWORK2D_GRID_COVERED)
            {
              Grid.SetEnqueued(NeighborH, NeighborV, GVEXPOSURESENSORNETWORK2D_BFS_ENQUEUED);
              IndexQueue[AddIndex] = EncodeGridIndex(NeighborH, NeighborV);
              if (AddIndex >= NumQueue) NumQueue = AddIndex + 1;
              AddIndex = NumQueue;
            }
            else
            {
              Grid.SetCovered(NeighborH, NeighborV, GVEXPOSURESENSORNETWORK2D_GRID_POTENTIAL);
            }
          }
        }
      }
    }
  }
  // Now That Parent Information Has Been Created, Store the Path in IndexQueue (Because it's Backwards)
  while ((CurrentH >= 0) && (CurrentV >= 0))
  {
    IndexQueue[NumPath] = EncodeGridIndex(CurrentH, CurrentV);
    NeighborH = Grid.GetParentH(CurrentH, CurrentV);
    NeighborV = Grid.GetParentV(CurrentH, CurrentV);
    CurrentH = NeighborH;
    CurrentV = NeighborV;
    ++NumPath;
  }
  // Store the Path in the provided Array, putting it Forwards
  for (i = 0; i < NumPath; ++i)
  {
    Path[i] = IndexQueue[NumPath - i - 1];
  }
  if (IndexQueue != NULL) delete[] IndexQueue;
  if (Neighbors != NULL) delete[] Neighbors;
  return(NumPath);
} // MinExposure_GridDijkstraSearch





void GVExposureSensorNetwork2D::MaxExposure_Calculate(void)
/*
  MaxExposure_Calculate() calculates the maximal exposure path throughout
  the network using the heuristics.
*/
{ // MaxExposure_Calculate
  switch(CenMaxExposureMode)
  {
    case GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_RANDOM:
      MaxExposure_UpdateRandomPath();
    break;

    case GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_SHORTEST:
      MaxExposure_UpdateShortestPath();
    break;

    case GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_BEST:
      MaxExposure_UpdateBestPoint();
    break;

    case GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_ADJUSTED:
    default:
      MaxExposure_UpdateAdjustedBestPoint();
    break;
  }
} // MaxExposure_Calculate



int GVExposureSensorNetwork2D::GetMessageMode(void)
/*
  GetMessageMode() returns the message mode.
*/
{ // GetMessageMode
  return(MessageMode);
} // GetMessageMode



int GVExposureSensorNetwork2D::GetSensorKnown(int Index)
/*
  GetSensorKnown() returns the Known attribute of the specified sensor.
*/
{ // GetSensorKnown
  int ReturnVal = 0;

  if ((Index >= 0) && (Index < NumSensors)) ReturnVal = Sensors[Index].GetKnown();
  return(ReturnVal);
} // GetSensorKnown



int GVExposureSensorNetwork2D::GetSensorCurrent(void)
/*
  GetSensorCurrent() returns the current sensor.
*/
{ // GetSensorCurrent
  return(CurrentNode);
} // GetSensorCurrent



int GVExposureSensorNetwork2D::GetSensorNext(void)
/*
  GetSensorNext() returns the next sensor.
*/
{ // GetSensorNext
  return(NextNode);
} // GetSensorNext



int GVExposureSensorNetwork2D::GetGridSizeH(void)
/*
  GetGridSizeH() returns the horizontal grid size.
*/
{ // GetGridSizeH
  return(Grid.GetSizeH());
} // GetGridSizeH



int GVExposureSensorNetwork2D::GetGridSizeV(void)
/*
  GetGridSizeV() returns the vertical grid size.
*/
{ // GetGridSizeV
  return(Grid.GetSizeV());
} // GetGridSizeV



int GVExposureSensorNetwork2D::GetGridCovered(int i, int j)
/*
  GetGridCovered() returns the covered attribute for the grid square (i, j).
*/
{ // GetGridCovered
  return(Grid.GetCovered(i, j));
} // GetGridCovered



double GVExposureSensorNetwork2D::GetGridPointX(int i, int j, int PointNumber)
/*
  GetGridPointX() returns the specified X coordinate point of the grid square (i, j).
*/
{ // GetGridPointX
  return(Grid.GetPointX(i, j, PointNumber));
} // GetGridPointX



double GVExposureSensorNetwork2D::GetGridPointY(int i, int j, int PointNumber)
/*
  GetGridPointY() returns the specified Y coordinate point of the grid square (i, j).
*/
{ // GetGridPointY
  return(Grid.GetPointY(i, j, PointNumber));
} // GetGridPointY



double GVExposureSensorNetwork2D::GetStartX(void)
/*
  GetStartX() returns the X coordinate of the starting point.
*/
{ // GetStartX
  return(Start.GetX());
} // GetStartX



double GVExposureSensorNetwork2D::GetStartY(void)
/*
  GetStartY() returns the Y coordinate of the starting point.
*/
{ // GetStartY
  return(Start.GetY());
} // GetStartY



double GVExposureSensorNetwork2D::GetEndX(void)
/*
  GetEndX() returns the X coordinate of the ending point.
*/
{ // GetEndX
  return(End.GetX());
} // GetEndX



double GVExposureSensorNetwork2D::GetEndY(void)
/*
  GetEndY() returns the Y coordinate of the ending point.
*/
{ // GetEndY
  return(End.GetY());
} // GetEndY



double GVExposureSensorNetwork2D::GetGridNormalizedExposureValue(int i, int j)
/*
  GetGridNormalizedExposureValue() returns the Normalized Exposure Value of the
  grid square (i, j).
*/
{ // GetGridNormalizedExposureValue
  return(Grid.GetNormalizedExposureValue(i, j));
} // GetGridNormalizedExposureValue



int GVExposureSensorNetwork2D::MinExposure_Run(void)
/*
  MinExposure_Run() runs the minimal exposure algorihtm from initialization
  to completion.
*/
{ // MinExposure_Run
  int ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS;

  if (!MinExposureInitialized) AlgorithmStage = GVEXPOSURESENSORNETWORK2D_MODE_INIT;

  switch(AlgorithmStage)
  {
    case GVEXPOSURESENSORNETWORK2D_MODE_UPDATE:
      if (!MinExposure_Update()) AlgorithmStage = GVEXPOSURESENSORNETWORK2D_MODE_CALCULATE;
    break;

    case GVEXPOSURESENSORNETWORK2D_MODE_CALCULATE:
      MinExposure_Calculate();
      Reset();
      ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_DONE;
    break;

    case GVEXPOSURESENSORNETWORK2D_MODE_INIT:
    default:
      MinExposure_Initialize();
      AlgorithmStage = GVEXPOSURESENSORNETWORK2D_MODE_UPDATE;
    break;
  }

  return(ReturnVal);
} // MinExposure_Run



void GVExposureSensorNetwork2D::CalculateGridExposure(void)
/*
  CalculateGridExposure() calculate the grid exposure.  It is a separate
  function because it can take a lot of time.  Used for displaying
  only.  It has no significance towards the actual algorithm.
*/
{ // CalculateGridExposure
  int i = 0;                // Index Variable
  int j = 0;                // Index Variable
  int GH = Grid.GetSizeH(); // Size of Grid
  int GV = Grid.GetSizeV(); // Size of Grid
  double Exposure = 0.0;    // Exposure Value
  double MaxExposure = 0.0; // Maximum Exposure Value
  double X = 0.0;           // X Coordinate
  double Y = 0.0;           // Y Coordinate

  // Calculate the Exposure of Each Grid Point
  for (i = 0; i < GH; ++i)
  {
    for (j = 0; j < GV; ++j)
    {
      X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
      Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
      Exposure = CalculateExposure(X, Y);
      Grid.SetExposureValue(i, j, Exposure);
      Grid.SetNearestSensor(i, j, CalculateNearestSensor(X, Y));
      if (Exposure > MaxExposure) MaxExposure = Exposure;
    }
  }

  // Calculate the Normalized Exposure of Each Grid Point
  for (i = 0; i < GH; ++i)
  {
    for (j = 0; j < GV; ++j)
    {
      Grid.SetNormalizedExposureValue(i, j, Grid.GetExposureValue(i, j) / MaxExposure);
    }
  }
} // CalculateGridExposure



int GVExposureSensorNetwork2D::DecodeGridIndexH(int Index)
/*
  DecodeGridIndexH() decodes the Horizontal index of a grid.
*/
{ // DecodeGridIndexH
  return(Index / Grid.GetSizeV());
} // DecodeGridIndexH



int GVExposureSensorNetwork2D::DecodeGridIndexV(int Index)
/*
  DecodeGridIndexV() decodes the Vertical index of a grid.
*/
{ // DecodeGridIndexV
  return(Index % Grid.GetSizeV());
} // DecodeGridIndexV



int GVExposureSensorNetwork2D::EncodeGridIndex(int i, int j)
/*
  EndodeGridIndex() encodes the horizontal and vertical indices of a grid.
*/
{ // EndodeGridIndex
  int ReturnVal = -1;

  if ((i >= 0) && (i < Grid.GetSizeH()) && (j >= 0) && (j < Grid.GetSizeV())) ReturnVal = i * Grid.GetSizeV() + j;
  return(ReturnVal);
} // EndodeGridIndex



double GVExposureSensorNetwork2D::CalculateLineExposure(double X1, double Y1, double X2, double Y2)
/*
  CalculateLineExposure() calculates the exposure from a starting point to
  the ending point.
*/
{ // CalculateLineExposure
  int GridIndexH = 0;                                                             // Horizontal Grid Index
  int GridIndexV = 0;                                                             // Vertical Grid Index
  int Valid = 1;                                                                  // Did the Line Go Out of Bounds?
  double GridX = Grid.GetStartX();                                                // Grid Starting Coordinate
  double GridY = Grid.GetStartY();                                                // Grid Starting Coordinate
  double GridHX = Grid.GetHorizontalVectorX();                                    // X Coordinate Horizontal Vector
  double GridHY = Grid.GetHorizontalVectorY();                                    // Y Coordinate Horizontal Vector
  double GridVX = Grid.GetVerticalVectorX();                                      // X Coordinate Horizontal Vector
  double GridVY = Grid.GetVerticalVectorY();                                      // Y Coordinate Horizontal Vector
  double GridH = Distance2D(0.0, 0.0, GridHX, GridHY);                            // Length of Horizontal Vector
  double GridV = Distance2D(0.0, 0.0, GridVX, GridVY);                            // Length of Horizontal Vector
  double DX = X2 - X1;                                                            // Change in X
  double DY = Y2 - Y1;                                                            // Change in Y
  double t = 0.0;                                                                 // Parameter Across Line
  double Dt = GVEXPOSURESENSORNETWORK2D_MINDISTANCE / Distance2D(X1, Y1, X2, Y2); // Percentage to Change Distance By
  double Lt = 0.0;                                                                // Remaining Percentage
  double X = 0.0;                                                                 // Temporary X Coordinate
  double Y = 0.0;                                                                 // Temporary Y Coordinate
  double Exposure = 0.0;                                                          // Exposure Value

  GridH *= GridH;
  GridV *= GridV;
  while ((Valid) && (t + Dt < 1.0))
  {
    X = X1 + (t  + Dt / 2) * DX - GridX;
    Y = Y1 + (t  + Dt / 2) * DY - GridY;
    GridIndexH = ((int) ((GridHX * X + GridHY * Y) / GridH));
    GridIndexV = ((int) ((GridVX * X + GridVY * Y) / GridV));
    if (Grid.GetCovered(GridIndexH, GridIndexV) == GVEXPOSURESENSORNETWORK2D_GRID_COVERED)
    {
      Exposure += Grid.GetExposureValue(GridIndexH, GridIndexV);
      // Exposure += CalculateExposure(X, Y);
    }
    else
    {
      Valid = 0;
    }
    t += Dt;
  }
  if (Valid)
  {
    Lt = 1.0 - t;
    X = X1 + (t  + Lt / 2) * DX - GridX;
    Y = Y1 + (t  + Lt / 2) * DY - GridY;
    GridIndexH = ((int) ((GridHX * X + GridHY * Y) / GridH));
    GridIndexV = ((int) ((GridVX * X + GridVY * Y) / GridV));
    if (Grid.GetCovered(GridIndexH, GridIndexV) == GVEXPOSURESENSORNETWORK2D_GRID_COVERED)
    {
      Exposure += Grid.GetExposureValue(GridIndexH, GridIndexV) * (Lt / Dt);
      // Exposure += CalculateExposure(X, Y) * (Lt / Dt);
    }
    else
    {
      Valid = 0;
    }
  }
  if (!Valid) Exposure = GVSENSORNETWORK_INFINITY + 1.0;
  return(Exposure);
} // CalculateLineExposure



double GVExposureSensorNetwork2D::CalculatePathExposure(int* Path, int NumPath)
/*
  CalculatePathExposure() calculates the exposure along a path.
*/
{ // CalculatePathExposure
  int Index = 0;                                        // Index Variable
  int MaxIndex = NumPath - 1;                           // Max Index Variable Value
  int Current = 0;                                      // Current Index in Path
  int Next = 0;                                         // Next Index in Path
  int TotalIndices = Grid.GetSizeH() * Grid.GetSizeV(); // Total Indices in Grid
  int iCurrent = 0;                                     // Current i Value
  int jCurrent = 0;                                     // Current j Value
  int iNext = 0;                                        // Next i Value
  int jNext = 0;                                        // Next j Value
  double ReturnVal = 0.0;                               // Return Value
  double X1 = 0.0;                                      // X Coordinate
  double Y1 = 0.0;                                      // Y Coordinate
  double X2 = 0.0;                                      // X Coordinate
  double Y2 = 0.0;                                      // Y Coordinate

  for (Index = 0; Index < MaxIndex; ++Index)
  {
    Current = Path[Index];
    Next = Path[Index + 1];
    iCurrent = DecodeGridIndexH(Current);
    jCurrent = DecodeGridIndexV(Current);
    iNext = DecodeGridIndexH(Next);
    jNext = DecodeGridIndexV(Next);
    if (Current > TotalIndices)
    {
      X1 = Start.GetX();
      Y1 = Start.GetY();
    }
    else if (Current == TotalIndices)
    {
      X1 = End.GetX();
      Y1 = End.GetY();
    }
    else
    {
      X1 = Grid.GetPointX(iCurrent, jCurrent, GVGRIDPOINT2D_CENTERPOINT);
      Y1 = Grid.GetPointY(iCurrent, jCurrent, GVGRIDPOINT2D_CENTERPOINT);
    }
    if (Next > TotalIndices)
    {
      X2 = Start.GetX();
      Y2 = Start.GetY();
    }
    else if (Next == TotalIndices)
    {
      X2 = End.GetX();
      Y2 = End.GetY();
    }
    else
    {
      X2 = Grid.GetPointX(iNext, jNext, GVGRIDPOINT2D_CENTERPOINT);
      Y2 = Grid.GetPointY(iNext, jNext, GVGRIDPOINT2D_CENTERPOINT);
    }
    ReturnVal += CalculateLineExposure(X1, Y1, X2, Y2);
  }
  return(ReturnVal);
} // CalculatePathExposure



double GVExposureSensorNetwork2D::CalculatePathLength(int* Path, int NumPath)
/*
  CalculatePathLength() calculates the length of a path
*/
{ // CalculatePathLength
  int Index = 0;                                        // Index Variable
  int MaxIndex = NumPath - 1;                           // Max Index Variable Value
  int Current = 0;                                      // Current Index in Path
  int Next = 0;                                         // Next Index in Path
  int TotalIndices = Grid.GetSizeH() * Grid.GetSizeV(); // Total Indices in Grid
  int iCurrent = 0;                                     // Current i Value
  int jCurrent = 0;                                     // Current j Value
  int iNext = 0;                                        // Next i Value
  int jNext = 0;                                        // Next j Value
  double ReturnVal = 0.0;                               // Return Value
  double X1 = 0.0;                                      // X Coordinate
  double Y1 = 0.0;                                      // Y Coordinate
  double X2 = 0.0;                                      // X Coordinate
  double Y2 = 0.0;                                      // Y Coordinate

  for (Index = 0; Index < MaxIndex; ++Index)
  {
    Current = Path[Index];
    Next = Path[Index + 1];
    iCurrent = DecodeGridIndexH(Current);
    jCurrent = DecodeGridIndexV(Current);
    iNext = DecodeGridIndexH(Next);
    jNext = DecodeGridIndexV(Next);
    if (Current > TotalIndices)
    {
      X1 = Start.GetX();
      Y1 = Start.GetY();
    }
    else if (Current == TotalIndices)
    {
      X1 = End.GetX();
      Y1 = End.GetY();
    }
    else
    {
      X1 = Grid.GetPointX(iCurrent, jCurrent, GVGRIDPOINT2D_CENTERPOINT);
      Y1 = Grid.GetPointY(iCurrent, jCurrent, GVGRIDPOINT2D_CENTERPOINT);
    }
    if (Next > TotalIndices)
    {
      X2 = Start.GetX();
      Y2 = Start.GetY();
    }
    else if (Next == TotalIndices)
    {
      X2 = End.GetX();
      Y2 = End.GetY();
    }
    else
    {
      X2 = Grid.GetPointX(iNext, jNext, GVGRIDPOINT2D_CENTERPOINT);
      Y2 = Grid.GetPointY(iNext, jNext, GVGRIDPOINT2D_CENTERPOINT);
    }
    ReturnVal += Distance2D(X1, Y1, X2, Y2);
  }
  return(ReturnVal);
} // CalculatePathLength

int GVExposureSensorNetwork2D::MaxExposure_GridDijkstraSearch(int SH, int SV, int EH, int EV, int* Path)
/*
  MaxExposure_GridDijkstraSearch() performs a Dijkstra search on the
  grid from the starting point to the ending point.

  The Queue works as follows.  Normally, CurrentAddIndex is at the End of the
  Queue.  When a node is selected as minimum, the CurrentAddIndex is set to
  that index, and the node can be replaced.  The CurrentAddIndex is then set
  to the back of the Queue (Array) again.  This way, an array can be used and
  you don't have to constantly move elements.  Each node contains its own
  enqueued value as well.
*/
{ // MaxExposure_GridDijkstraSearch
  int i = 0;                   // Index Variable
  int j = 0;                   // Index Variable
  int CurrentH = 0;            // Current H Value
  int CurrentV = 0;            // Current V Value
  int CurrentIndex = 0;        // Current Encoded HV Value
  int NeighborH = 0;           // Current H Value
  int NeighborV = 0;           // Current V Value
  int NeighborIndex = 0;       // Neighbor Encoded HV Value
  int GridH = Grid.GetSizeH(); // Grid Horizontal Size
  int GridV = Grid.GetSizeV(); // Grid Vertical Size
  int Finished = 0;            // Is Search Over
  int NumPath = 0;             // Number in Path
  int NextIndexStatus = 0;     // Status of Neighbor Node (DEQUEUED, ENQUEUED, FINISHED)
  int AddIndex = 0;            // Index Position to Add Element
  int NumQueue = 0;            // Number of Elements in Index Queue
  int NumNbr = 0;              // Number of Neighbors
  int* Neighbors = NULL;       // Neighbors
  int* IndexQueue = NULL;      // Index Queue for Nodes in Network
  double CurrentCost = 0.0;    // Current Cost
  double MinCost = 0.0;        // Minimum Cost

  Neighbors = new int[2 * (GridH + GridV)];
  IndexQueue = new int[GridH * GridV];
  // Initialize Dijkstra Search (Set All Nodes to DEQUEUED, INFINITY, NOPARENT)
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    {
      Grid.SetCost(i, j, GVSENSORNETWORK_INFINITY);
      Grid.SetEnqueued(i, j, GVEXPOSURESENSORNETWORK2D_BFS_DEQUEUED);
      Grid.SetParent(i, j, GVEXPOSURESENSORNETWORK2D_NOPARENT, GVEXPOSURESENSORNETWORK2D_NOPARENT);
    }
  }
  // Add Start Node To Queue
  if (Grid.GetCovered(SH, SV))
  {
    CurrentIndex = EncodeGridIndex(SH, SV);
    Grid.SetEnqueued(SH, SV, GVEXPOSURESENSORNETWORK2D_BFS_ENQUEUED);
    Grid.SetCost(SH, SV, 0.0);
    IndexQueue[NumQueue] = CurrentIndex;
    ++NumQueue;
  }
  // While Not Finished
  while(!Finished)
  {
    // Pick Index From Index Queue With Min Cost
    CurrentH = -1;
    CurrentV = -1;
    CurrentIndex = -1;
    for (MinCost = GVSENSORNETWORK_INFINITY, i = 0; i < NumQueue; ++i)
    {
      j = IndexQueue[i];
      NeighborH = DecodeGridIndexH(j);
      NeighborV = DecodeGridIndexV(j);
      if (Grid.GetEnqueued(NeighborH, NeighborV) == GVEXPOSURESENSORNETWORK2D_BFS_ENQUEUED)
      {
        CurrentCost = Grid.GetCost(NeighborH, NeighborV);
        if (CurrentCost < MinCost)
        {
          AddIndex = i;
          CurrentIndex = j;
          MinCost = CurrentCost;
        }
      }
    }
    if (CurrentIndex >= 0)
    {
      CurrentH = DecodeGridIndexH(CurrentIndex);
      CurrentV = DecodeGridIndexV(CurrentIndex);
      // Mark Index As Dequeued
      Grid.SetEnqueued(CurrentH, CurrentV, GVEXPOSURESENSORNETWORK2D_BFS_FINISHED);
      if ((CurrentH == EH) && (CurrentV == EV))
      {
        Finished = 1;
      }
    }
    else
    {
      Finished = 1;
    }
    if (!Finished)
    {
      NumNbr = CalculateSharedNeighbors(CurrentH, CurrentV, CurrentH, CurrentV, Neighbors);
      for (i = 0; i < NumNbr; ++i)
      {
        j = Neighbors[i];
        NeighborH = DecodeGridIndexH(j);
        NeighborV = DecodeGridIndexV(j);
        if (Grid.GetCovered(NeighborH, NeighborV))
        {
          NextIndexStatus = Grid.GetEnqueued(NeighborH, NeighborV);
          if (NextIndexStatus == GVEXPOSURESENSORNETWORK2D_BFS_DEQUEUED)
          {
            // Add Neighbor to Queue
            CurrentCost = Grid.GetCost(CurrentH, CurrentV) + Distance2D(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointX(NeighborH, NeighborV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(NeighborH, NeighborV, GVGRIDPOINT2D_CENTERPOINT));
            Grid.SetEnqueued(NeighborH, NeighborV, GVEXPOSURESENSORNETWORK2D_BFS_ENQUEUED);
            Grid.SetCost(NeighborH, NeighborV, CurrentCost);
            Grid.SetParent(NeighborH, NeighborV, CurrentH, CurrentV);
            IndexQueue[AddIndex] = EncodeGridIndex(NeighborH, NeighborV);
            if (AddIndex >= NumQueue) NumQueue = AddIndex + 1;
            AddIndex = NumQueue;
          }
          else if (NextIndexStatus == GVEXPOSURESENSORNETWORK2D_BFS_ENQUEUED)
          {
            // Update Neighbor If Necessary
            CurrentCost = Grid.GetCost(CurrentH, CurrentV) + Distance2D(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointX(NeighborH, NeighborV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(NeighborH, NeighborV, GVGRIDPOINT2D_CENTERPOINT));
            if (CurrentCost < Grid.GetCost(NeighborH, NeighborV))
            {
              Grid.SetCost(NeighborH, NeighborV, CurrentCost);
              Grid.SetParent(NeighborH, NeighborV, CurrentH, CurrentV);
            }
          }
        }
      }
    }
  }
  // Now That Parent Information Has Been Created, Store the Path in IndexQueue (Because it's Backwards)
  while ((CurrentH >= 0) && (CurrentV >= 0))
  {
    IndexQueue[NumPath] = EncodeGridIndex(CurrentH, CurrentV);
    NeighborH = Grid.GetParentH(CurrentH, CurrentV);
    NeighborV = Grid.GetParentV(CurrentH, CurrentV);
    CurrentH = NeighborH;
    CurrentV = NeighborV;
    ++NumPath;
  }
  // Store the Path in the provided Array, putting it Forwards
  for (i = 0; i < NumPath; ++i)
  {
    Path[i] = IndexQueue[NumPath - i - 1];
  }
  if (IndexQueue != NULL) delete[] IndexQueue;
  if (Neighbors != NULL) delete[] Neighbors;
  return(NumPath);
} // MaxExposure_GridDijkstraSearch



double GVExposureSensorNetwork2D::MaxExposure_UpdateRandomPath(void)
/*
  
*/
{ // MaxExposure_UpdateRandomPath
  int i = 0;                                // Index Variable
  int j = 0;                                // Index Variable
  int StartH = 0;                           // Starting Horizontal Grid Index
  int StartV = 0;                           // Starting Vertical Grid Index
  int EndH = 0;                             // Starting Horizontal Grid Index
  int EndV = 0;                             // Starting Vertical Grid Index
  int CurrentH = 0;                         // Current Horizontal Index
  int CurrentV = 0;                         // Current Vertical Index
  int NextH = 0;                            // Current Horizontal Index
  int NextV = 0;                            // Current Vertical Index
  int ParentH = 0;                          // Parent Horizontal Index
  int ParentV = 0;                          // Parent Vertical Index
  int GridH = Grid.GetSizeH();              // Horizontal Grid Size
  int GridV = Grid.GetSizeV();              // Vertical Grid Size
  int TotalGrid = GridH * GridV;            // Total Grid Points
  int NumTempPath = 0;                      // Number of Points in Temporary Path
  int NumNeighbors = 0;                     // Number of Neighbors
  int MaxNeighbors = 2 * GridH + 2 * GridV; // Maximum Number of Neighbors
  int* NeighborListH = NULL;                // List of Neighbors
  int* NeighborListV = NULL;                // List of Neighbors
  int* DijkstraPath = NULL;                 // Dijkstra Path Temp
  int* TempPath = NULL;                     // Temporary Path
  double Length = 0.0;                      // Current Length
  double StartX = Start.GetX();             // Starting Point
  double StartY = Start.GetY();             // Starting Point
  double EndX = End.GetX();                 // Starting Point
  double EndY = End.GetY();                 // Starting Point
  double InitialLength = 0.0;               // Initial Length of Path
  double RandVal = 0.0;                     // Random Value
  double ReturnVal = 0.0;                   // Return Value
  double D = 0.0;                           // Distance Variable
  double MinSD = GVSENSORNETWORK_INFINITY;  // Minimum Starting Distance
  double MinED = GVSENSORNETWORK_INFINITY;  // Minimum Starting Distance
  double X = 0.0;                           // Temporary X Value
  double Y = 0.0;                           // Temporary Y Value
  double NX = 0.0;                          // Neighbor X
  double NY = 0.0;                          // Neighbor Y
  double PX = 0.0;                          // Parent X
  double PY = 0.0;                          // Parent Y

  // Allocate Temporary Path
  TempPath = new int[TotalGrid];
  DijkstraPath = new int[TotalGrid];
  NeighborListH = new int[MaxNeighbors];
  NeighborListV = new int[MaxNeighbors];
  // Pick Nearest Grid Point to Starting Point
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    { 
       X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
       Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
       D = Distance2D(StartX, StartY, X, Y);
       if (D < MinSD)
       {
         StartH = i;
         StartV = j;
         MinSD = D;
       }
       D = Distance2D(EndX, EndY, X, Y);
       if (D < MinED)
       {
         EndH = i;
         EndV = j;
         MinED = D;
       }
    }
  }
  // Store Initial Length
  InitialLength = Distance2D(StartX, StartY, Grid.GetPointX(StartH, StartV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(StartH, StartV, GVGRIDPOINT2D_CENTERPOINT)) + Distance2D(EndX, EndY, Grid.GetPointX(EndH, EndV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(EndH, EndV, GVGRIDPOINT2D_CENTERPOINT));
  // Calculate A Dijkstra From The End To All Points
  MaxExposure_GridDijkstraSearch(EndH, EndV, -1, -1, TempPath);
  // Initialize the Search and Continue Until End Node is Reached
  NumTempPath = 0;
  CurrentH = StartH;
  CurrentV = StartV;
  TempPath[NumTempPath] = EncodeGridIndex(CurrentH, CurrentV);
  ++NumTempPath;
  while ((CurrentH != EndH) || (CurrentV != EndV))
  {
    if (GenerateRandomDouble(0.0, 1.0) < GVEXPOSURESENSORNETWORK2D_RANDTHRESHOLD)
    {
      // Create the Neighbor List
      X = Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT);
      Y = Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT);
      NumNeighbors = 0;

      NextH = CurrentH - 1;
      if ((NextH >= 0) && (NextH < GridH))
      {
        for (NextV = 0; NextV < GridV; ++NextV)
        {
          NX = Grid.GetPointX(NextH, NextV, GVGRIDPOINT2D_CENTERPOINT);
          NY = Grid.GetPointY(NextH, NextV, GVGRIDPOINT2D_CENTERPOINT);
          Length = InitialLength + CalculatePathLength(TempPath, NumTempPath) + Distance2D(X, Y, NX, NY);
          // MaxExposure_GridDijkstraSearch(EndH, EndV, CurrentH, CurrentV, DijkstraPath);
          ParentH = Grid.GetParentH(NextH, NextV);
          ParentV = Grid.GetParentV(NextH, NextV);
          while((ParentH > 0) && (ParentV > 0))
          {
            PX = Grid.GetPointX(ParentH, ParentV, GVGRIDPOINT2D_CENTERPOINT);
            PY = Grid.GetPointY(ParentH, ParentV, GVGRIDPOINT2D_CENTERPOINT);
            Length += Distance2D(NX, NY, PX, PY);
            j = ParentH;
            ParentH = Grid.GetParentH(ParentH, ParentV);
            ParentV = Grid.GetParentV(j, ParentV);
            NX = PX;
            NY = PY;
          }
          if (Length < MaxLength)
          {
            NeighborListH[NumNeighbors] = NextH;
            NeighborListV[NumNeighbors] = NextV;
            ++NumNeighbors;
          }
        }
      }

      NextH = CurrentH + 1;
      if ((NextH >= 0) && (NextH < GridH))
      {
        for (NextV = 0; NextV < GridV; ++NextV)
        {
          NX = Grid.GetPointX(NextH, NextV, GVGRIDPOINT2D_CENTERPOINT);
          NY = Grid.GetPointY(NextH, NextV, GVGRIDPOINT2D_CENTERPOINT);
          Length = InitialLength + CalculatePathLength(TempPath, NumTempPath) + Distance2D(X, Y, NX, NY);
          // MaxExposure_GridDijkstraSearch(EndH, EndV, CurrentH, CurrentV, DijkstraPath);
          ParentH = Grid.GetParentH(NextH, NextV);
          ParentV = Grid.GetParentV(NextH, NextV);
          while((ParentH > 0) && (ParentV > 0))
          {
            PX = Grid.GetPointX(ParentH, ParentV, GVGRIDPOINT2D_CENTERPOINT);
            PY = Grid.GetPointY(ParentH, ParentV, GVGRIDPOINT2D_CENTERPOINT);
            Length += Distance2D(NX, NY, PX, PY);
            j = ParentH;
            ParentH = Grid.GetParentH(ParentH, ParentV);
            ParentV = Grid.GetParentV(j, ParentV);
            NX = PX;
            NY = PY;
          }
          if (Length < MaxLength)
          {
            NeighborListH[NumNeighbors] = NextH;
            NeighborListV[NumNeighbors] = NextV;
            ++NumNeighbors;
          }
        }
      }

      NextV = CurrentV - 1;
      if ((NextV >= 0) && (NextV < GridV))
      {
        for (NextH = 0; NextH < GridH; ++NextH)
        {
          NX = Grid.GetPointX(NextH, NextV, GVGRIDPOINT2D_CENTERPOINT);
          NY = Grid.GetPointY(NextH, NextV, GVGRIDPOINT2D_CENTERPOINT);
          Length = InitialLength + CalculatePathLength(TempPath, NumTempPath) + Distance2D(X, Y, NX, NY);
          // MaxExposure_GridDijkstraSearch(EndH, EndV, CurrentH, CurrentV, DijkstraPath);
          ParentH = Grid.GetParentH(NextH, NextV);
          ParentV = Grid.GetParentV(NextH, NextV);
          while((ParentH > 0) && (ParentV > 0))
          {
            PX = Grid.GetPointX(ParentH, ParentV, GVGRIDPOINT2D_CENTERPOINT);
            PY = Grid.GetPointY(ParentH, ParentV, GVGRIDPOINT2D_CENTERPOINT);
            Length += Distance2D(NX, NY, PX, PY);
            j = ParentH;
            ParentH = Grid.GetParentH(ParentH, ParentV);
            ParentV = Grid.GetParentV(j, ParentV);
            NX = PX;
            NY = PY;
          }
          if (Length < MaxLength)
          {
            NeighborListH[NumNeighbors] = NextH;
            NeighborListV[NumNeighbors] = NextV;
            ++NumNeighbors;
          }
        }
      }

      NextV = CurrentV + 1;
      if ((NextV >= 0) && (NextV < GridV))
      {
        for (NextH = 0; NextH < GridH; ++NextH)
        {
          NX = Grid.GetPointX(NextH, NextV, GVGRIDPOINT2D_CENTERPOINT);
          NY = Grid.GetPointY(NextH, NextV, GVGRIDPOINT2D_CENTERPOINT);
          Length = InitialLength + CalculatePathLength(TempPath, NumTempPath) + Distance2D(X, Y, NX, NY);
          // MaxExposure_GridDijkstraSearch(EndH, EndV, CurrentH, CurrentV, DijkstraPath);
          ParentH = Grid.GetParentH(NextH, NextV);
          ParentV = Grid.GetParentV(NextH, NextV);
          while((ParentH > 0) && (ParentV > 0))
          {
            PX = Grid.GetPointX(ParentH, ParentV, GVGRIDPOINT2D_CENTERPOINT);
            PY = Grid.GetPointY(ParentH, ParentV, GVGRIDPOINT2D_CENTERPOINT);
            Length += Distance2D(NX, NY, PX, PY);
            j = ParentH;
            ParentH = Grid.GetParentH(ParentH, ParentV);
            ParentV = Grid.GetParentV(j, ParentV);
            NX = PX;
            NY = PY;
          }
          if (Length < MaxLength)
          {
            NeighborListH[NumNeighbors] = NextH;
            NeighborListV[NumNeighbors] = NextV;
            ++NumNeighbors;
          }
        }
      }

      // Pick a Random Neighbor
      i = rand() % NumNeighbors;
      CurrentH = NeighborListH[i];
      CurrentV = NeighborListV[i];
      TempPath[NumTempPath] = EncodeGridIndex(CurrentH, CurrentV);
      ++NumTempPath;
    }
    else
    {
      // Go In Direction of Shortest Path
      NextH = Grid.GetParentH(CurrentH, CurrentV);
      NextV = Grid.GetParentV(CurrentH, CurrentV);
      if ((NextH < 0) || (NextV < 0))
      {
        MaxExposure_GridDijkstraSearch(EndH, EndV, CurrentH, CurrentV, DijkstraPath);
        NextH = Grid.GetParentH(CurrentH, CurrentV);
        NextV = Grid.GetParentV(CurrentH, CurrentV);
      }
      CurrentH = NextH;
      CurrentV = NextV;
      TempPath[NumTempPath] = EncodeGridIndex(CurrentH, CurrentV);
      ++NumTempPath;
    }
  }
  // Store the Path
  if (NumTempPath > 0)
  {
    if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL)
    {
      // Allocate the Path
      if (LocMaxPath != NULL) delete[] LocMaxPath;
      NumLocMaxPath = NumTempPath + 2;
      LocMaxPath = new GVPoint2D[NumLocMaxPath];
      // Store the Start and End Point
      LocMaxPath[0].SetX(StartX);
      LocMaxPath[0].SetY(StartY);
      LocMaxPath[NumCenMaxPath - 1].SetX(EndX);
      LocMaxPath[NumCenMaxPath - 1].SetY(EndY);
      // Copy the Elements
      for (i = 0; i < NumTempPath; ++i)
      {
        CurrentH = DecodeGridIndexH(TempPath[i]);
        CurrentV = DecodeGridIndexV(TempPath[i]);
        LocMaxPath[i + 1].SetX(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
        LocMaxPath[i + 1].SetY(Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
      }
    }
    else if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_CENTRAL)
    {
      // Allocate the Path
      if (CenMaxPath != NULL) delete[] CenMaxPath;
      NumCenMaxPath = NumTempPath + 2;
      CenMaxPath = new GVPoint2D[NumCenMaxPath];
      // Store the Start and End Point
      CenMaxPath[0].SetX(StartX);
      CenMaxPath[0].SetY(StartY);
      CenMaxPath[NumCenMaxPath - 1].SetX(EndX);
      CenMaxPath[NumCenMaxPath - 1].SetY(EndY);
      // Copy the Elements
      for (i = 0; i < NumTempPath; ++i)
      {
        CurrentH = DecodeGridIndexH(TempPath[i]);
        CurrentV = DecodeGridIndexV(TempPath[i]);
        CenMaxPath[i + 1].SetX(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
        CenMaxPath[i + 1].SetY(Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
      }
    }
  }

  if (NeighborListH != NULL) delete[] NeighborListH;
  if (NeighborListV != NULL) delete[] NeighborListV;
  if (TempPath != NULL) delete[] TempPath;
  if (DijkstraPath != NULL) delete[] DijkstraPath;
  return(ReturnVal);
} // MaxExposure_UpdateRandomPath



double GVExposureSensorNetwork2D::MaxExposure_UpdateShortestPath(void)
/*
  MaxExposure_UpdateShortestPath() uses the shortest path as the heuristic
  for the maximal exposure path.
*/
{ // MaxExposure_UpdateShortestPath
  int i = 0;                               // Index Variable
  int j = 0;                               // Index Variable
  int CurrentH = 0;                        // Horizontal Index
  int CurrentV = 0;                        // Vertical Index
  int GridH = Grid.GetSizeH();             // Grid Horizontal Size
  int GridV = Grid.GetSizeV();             // Grid Vertical Size
  int TotalGrid = GridH * GridV;           // Total Grid Points
  int StartH = 0;                          // Starting Horizontal Index
  int StartV = 0;                          // Starting Vertical Index
  int EndH = 0;                            // Ending Horizontal Index
  int EndV = 0;                            // Ending Vertical Index
  int NumTempPath = 0;                     // Number of Nodes in Temporary Path
  int* TempPath = NULL;                    // Temporary Path
  double D = 0.0;                          // Temporary Distance
  double MinSD = GVSENSORNETWORK_INFINITY; // Minimum Distance
  double MinED = GVSENSORNETWORK_INFINITY; // Minimum Distance
  double StartX = Start.GetX();            // Starting X Coordinate
  double StartY = Start.GetY();            // Starting Y Coordinate
  double EndX = End.GetX();                // Ending X Coordinate
  double EndY = End.GetY();                // Ending X Coordinate
  double X = 0.0;                          // Temporary X Coordinate
  double Y = 0.0;                          // Temporary X Coordinate
  double ReturnVal = 0.0;                  // Return value

  // Allocate Temporary Path
  TempPath = new int[TotalGrid];
  // Pick Nearest Grid Point to Starting Point
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    { 
       X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
       Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
       D = Distance2D(StartX, StartY, X, Y);
       if (D < MinSD)
       {
         StartH = i;
         StartV = j;
         MinSD = D;
       }
       D = Distance2D(EndX, EndY, X, Y);
       if (D < MinED)
       {
         EndH = i;
         EndV = j;
         MinED = D;
       }
    }
  }
  NumTempPath = MaxExposure_GridDijkstraSearch(StartH, StartV, EndH, EndV, TempPath);
  // Store the Path
  if (NumTempPath > 0)
  {
    if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL)
    {
      // Allocate the Path
      if (LocMaxPath != NULL) delete[] LocMaxPath;
      NumLocMaxPath = NumTempPath + 2;
      LocMaxPath = new GVPoint2D[NumLocMaxPath];
      // Store the Start and End Point
      LocMaxPath[0].SetX(StartX);
      LocMaxPath[0].SetY(StartY);
      LocMaxPath[NumCenMaxPath - 1].SetX(EndX);
      LocMaxPath[NumCenMaxPath - 1].SetY(EndY);
      // Copy the Elements
      for (i = 0; i < NumTempPath; ++i)
      {
        CurrentH = DecodeGridIndexH(TempPath[i]);
        CurrentV = DecodeGridIndexV(TempPath[i]);
        LocMaxPath[i + 1].SetX(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
        LocMaxPath[i + 1].SetY(Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
      }
    }
    else if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_CENTRAL)
    {
      // Allocate the Path
      if (CenMaxPath != NULL) delete[] CenMaxPath;
      NumCenMaxPath = NumTempPath + 2;
      CenMaxPath = new GVPoint2D[NumCenMaxPath];
      // Store the Start and End Point
      CenMaxPath[0].SetX(StartX);
      CenMaxPath[0].SetY(StartY);
      CenMaxPath[NumCenMaxPath - 1].SetX(EndX);
      CenMaxPath[NumCenMaxPath - 1].SetY(EndY);
      // Copy the Elements
      for (i = 0; i < NumTempPath; ++i)
      {
        CurrentH = DecodeGridIndexH(TempPath[i]);
        CurrentV = DecodeGridIndexV(TempPath[i]);
        CenMaxPath[i + 1].SetX(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
        CenMaxPath[i + 1].SetY(Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
      }
    }
  }
  if (TempPath != NULL) delete[] TempPath;
  return(ReturnVal);
} // MaxExposure_UpdateShortestPath



double GVExposureSensorNetwork2D::MaxExposure_UpdateBestPoint(void)
/*
  MaxExposure_UpdateBestPoint() calculates the maximal exposure path
  according to the best point heuristic (shortest path to a point,
  stay there as long as possible, shortest path to end).
*/
{ // MaxExposure_UpdateBestPoint
  int i = 0;                               // Index Variable
  int j = 0;                               // Index Variable
  int k = 0;                               // Index Variable
  int CurrentH = 0;                        // Horizontal Index
  int CurrentV = 0;                        // Vertical Index
  int GridH = Grid.GetSizeH();             // Grid Horizontal Size
  int GridV = Grid.GetSizeV();             // Grid Vertical Size
  int TotalGrid = GridH * GridV;           // Total Grid Points
  int StartH = 0;                          // Starting Horizontal Index
  int StartV = 0;                          // Starting Vertical Index
  int EndH = 0;                            // Ending Horizontal Index
  int EndV = 0;                            // Ending Vertical Index
  int NumTempPath = 0;                     // Number of Nodes in Temporary Path
  int NumHalfPath1 = 0;                    // Number of Nodes in Half the Path
  int NumHalfPath2 = 0;                    // Number of Nodes Half the Path
  int* HalfPath1 = NULL;                   // Half the Path
  int* HalfPath2 = NULL;                   // Half the Path
  int* TempPath = NULL;                    // Temporary Path
  double Exposure = 0.0;                   // Exposure of Path
  double MaxExposure = 0.0;                // Exposure of Best Path
  double Length = 0.0;                     // Length of Path
  double InitialLength = 0.0;              // Initial Length (From Start and End to Respective Grid Point)
  double D = 0.0;                          // Temporary Distance
  double MinSD = GVSENSORNETWORK_INFINITY; // Minimum Distance
  double MinED = GVSENSORNETWORK_INFINITY; // Minimum Distance
  double StartX = Start.GetX();            // Starting X Coordinate
  double StartY = Start.GetY();            // Starting Y Coordinate
  double EndX = End.GetX();                // Ending X Coordinate
  double EndY = End.GetY();                // Ending X Coordinate
  double X = 0.0;                          // Temporary X Coordinate
  double Y = 0.0;                          // Temporary X Coordinate
  double ReturnVal = 0.0;                  // Return value

  // Allocate Paths
  HalfPath1 = new int[TotalGrid];
  HalfPath2 = new int[TotalGrid];
  TempPath = new int[TotalGrid];
  // Pick Nearest Grid Point to Starting Point
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    { 
       X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
       Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
       D = Distance2D(StartX, StartY, X, Y);
       if (D < MinSD)
       {
         StartH = i;
         StartV = j;
         MinSD = D;
       }
       D = Distance2D(EndX, EndY, X, Y);
       if (D < MinED)
       {
         EndH = i;
         EndV = j;
         MinED = D;
       }
    }
  }
  // Store Initial Length
  InitialLength = Distance2D(StartX, StartY, Grid.GetPointX(StartH, StartV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(StartH, StartV, GVGRIDPOINT2D_CENTERPOINT)) + Distance2D(EndX, EndY, Grid.GetPointX(EndH, EndV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(EndH, EndV, GVGRIDPOINT2D_CENTERPOINT));
  // Check Each Grid Point
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    {
      if (Grid.GetCovered(i, j))
      {
        // Calcualte the Two Paths
        NumHalfPath1 = MaxExposure_GridDijkstraSearch(StartH, StartV, i, j, HalfPath1);
        NumHalfPath2 = MaxExposure_GridDijkstraSearch(i, j, EndH, EndV, HalfPath2);
        if ((NumHalfPath1 + NumHalfPath2) > 0)
        {
          Length = InitialLength + CalculatePathLength(HalfPath1, NumHalfPath1) + CalculatePathLength(HalfPath2, NumHalfPath2);
          if (Length <= MaxLength)
          {
            Exposure = CalculatePathExposure(HalfPath1, NumHalfPath1) + CalculatePathExposure(HalfPath2, NumHalfPath2) + ((MaxLength - Length) / GVEXPOSURESENSORNETWORK2D_MINDISTANCE) * Grid.GetExposureValue(i, j);
            if (Exposure > MaxExposure)
            {
              NumTempPath = 0;
              for (k = 0; k < NumHalfPath1; ++k)
              {
                TempPath[NumTempPath] = HalfPath1[k];
                ++NumTempPath;
              }
              for (k = 1; k < NumHalfPath2; ++k)
              {
                TempPath[NumTempPath] = HalfPath2[k];
                ++NumTempPath;
              }
              MaxExposure = Exposure;
            }
          }
        }
      }
    }
  }
  // Store in Final Path if Necessary;
  if (NumTempPath > 0)
  {
    if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL)
    {
      // Allocate the Path
      if (LocMaxPath != NULL) delete[] LocMaxPath;
      NumLocMaxPath = NumTempPath + 2;
      LocMaxPath = new GVPoint2D[NumLocMaxPath];
      // Store the Start and End Point
      LocMaxPath[0].SetX(StartX);
      LocMaxPath[0].SetY(StartY);
      LocMaxPath[NumCenMaxPath - 1].SetX(EndX);
      LocMaxPath[NumCenMaxPath - 1].SetY(EndY);
      // Copy the Elements
      for (i = 0; i < NumTempPath; ++i)
      {
        CurrentH = DecodeGridIndexH(TempPath[i]);
        CurrentV = DecodeGridIndexV(TempPath[i]);
        LocMaxPath[i + 1].SetX(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
        LocMaxPath[i + 1].SetY(Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
      }
    }
    else if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_CENTRAL)
    {
      // Allocate the Path
      if (CenMaxPath != NULL) delete[] CenMaxPath;
      NumCenMaxPath = NumTempPath + 2;
      CenMaxPath = new GVPoint2D[NumCenMaxPath];
      // Store the Start and End Point
      CenMaxPath[0].SetX(StartX);
      CenMaxPath[0].SetY(StartY);
      CenMaxPath[NumCenMaxPath - 1].SetX(EndX);
      CenMaxPath[NumCenMaxPath - 1].SetY(EndY);
      // Copy the Elements
      for (i = 0; i < NumTempPath; ++i)
      {
        CurrentH = DecodeGridIndexH(TempPath[i]);
        CurrentV = DecodeGridIndexV(TempPath[i]);
        CenMaxPath[i + 1].SetX(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
        CenMaxPath[i + 1].SetY(Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
      }
    }
  }
  if (HalfPath1 != NULL) delete[] HalfPath1;
  if (HalfPath2 != NULL) delete[] HalfPath2;
  if (TempPath != NULL) delete[] TempPath;
  return(ReturnVal);
} // MaxExposure_UpdateBestPoint



double GVExposureSensorNetwork2D::MaxExposure_UpdateAdjustedBestPoint(void)
/*
  MaxExposure_UpdateAdjustedBestPoint() is similar to
  MaxExposure_UpdateBestPoint() except that it tweaks the path after the
  initial BestPoint-Path is found.
*/
{ // MaxExposure_UpdateAdjustedBestPoint
  int i = 0;                               // Index Variable
  int j = 0;                               // Index Variable
  int k = 0;                               // Index Variable
  int CurrentH = 0;                        // Horizontal Index
  int CurrentV = 0;                        // Vertical Index
  int GridH = Grid.GetSizeH();             // Grid Horizontal Size
  int GridV = Grid.GetSizeV();             // Grid Vertical Size
  int TotalGrid = GridH * GridV;           // Total Grid Points
  int StartH = 0;                          // Starting Horizontal Index
  int StartV = 0;                          // Starting Vertical Index
  int EndH = 0;                            // Ending Horizontal Index
  int EndV = 0;                            // Ending Vertical Index
  int NumTempPath = 0;                     // Number of Nodes in Temporary Path
  int NumHalfPath1 = 0;                    // Number of Nodes in Half the Path
  int NumHalfPath2 = 0;                    // Number of Nodes in Half the Path
  int NumFullPath = 0;                     // Number of Nodes in the Full Path
  int* HalfPath1 = NULL;                   // Half the Path
  int* HalfPath2 = NULL;                   // Half the Path
  int* FullPath = NULL;                    // Full Path To Tweak
  int* TempPath = NULL;                    // Temporary Path
  double AdjustedMaxLength = 0.0;          // Adjusted Maximum Length
  double OldExposure = 0.0;                // Exposure of Path
  double Exposure = 0.0;                   // Exposure of Path
  double MaxExposure = 0.0;                // Exposure of Best Path
  double Length = 0.0;                     // Length of Path
  double InitialLength = 0.0;              // Initial Length (From Start and End to Respective Grid Point)
  double D = 0.0;                          // Temporary Distance
  double MinSD = GVSENSORNETWORK_INFINITY; // Minimum Distance
  double MinED = GVSENSORNETWORK_INFINITY; // Minimum Distance
  double StartX = Start.GetX();            // Starting X Coordinate
  double StartY = Start.GetY();            // Starting Y Coordinate
  double EndX = End.GetX();                // Ending X Coordinate
  double EndY = End.GetY();                // Ending X Coordinate
  double X = 0.0;                          // Temporary X Coordinate
  double Y = 0.0;                          // Temporary X Coordinate
  double ReturnVal = 0.0;                  // Return value

  // Allocate Paths
  HalfPath1 = new int[TotalGrid];
  HalfPath2 = new int[TotalGrid];
  FullPath = new int[TotalGrid];
  TempPath = new int[TotalGrid];
  // Pick Nearest Grid Point to Starting Point
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    { 
       X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
       Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
       D = Distance2D(StartX, StartY, X, Y);
       if (D < MinSD)
       {
         StartH = i;
         StartV = j;
         MinSD = D;
       }
       D = Distance2D(EndX, EndY, X, Y);
       if (D < MinED)
       {
         EndH = i;
         EndV = j;
         MinED = D;
       }
    }
  }
  // Store Initial Length
  InitialLength = Distance2D(StartX, StartY, Grid.GetPointX(StartH, StartV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(StartH, StartV, GVGRIDPOINT2D_CENTERPOINT)) + Distance2D(EndX, EndY, Grid.GetPointX(EndH, EndV, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(EndH, EndV, GVGRIDPOINT2D_CENTERPOINT));
  AdjustedMaxLength = MaxLength - InitialLength;
  // Check Each Grid Point
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    {
      if (Grid.GetCovered(i, j))
      {
        // Calcualte the Two Paths
        NumHalfPath1 = MaxExposure_GridDijkstraSearch(StartH, StartV, i, j, HalfPath1);
        NumHalfPath2 = MaxExposure_GridDijkstraSearch(i, j, EndH, EndV, HalfPath2);
        // Combine the Paths
        if ((NumHalfPath1 + NumHalfPath2) > 0)
        {
          Length = CalculatePathLength(HalfPath1, NumHalfPath1) + CalculatePathLength(HalfPath2, NumHalfPath2);
          if (Length <= AdjustedMaxLength)
          {
            NumFullPath = 0;
            for (k = 0; k < NumHalfPath1; ++k)
            {
              FullPath[NumFullPath] = HalfPath1[k];
              ++NumFullPath;
            }
            for (k = 1; k < NumHalfPath2; ++k)
            {
              FullPath[NumFullPath] = HalfPath2[k];
              ++NumFullPath;
            }
            // Tweak the Paths
            NumFullPath = MaxExposure_AdjustPath(FullPath, NumFullPath, EncodeGridIndex(i, j), AdjustedMaxLength);
            Length = CalculatePathLength(FullPath, NumFullPath);
            if (Length < AdjustedMaxLength)
            {
              Exposure = CalculatePathExposure(FullPath, NumFullPath) + ((AdjustedMaxLength - Length) / GVEXPOSURESENSORNETWORK2D_MINDISTANCE) * Grid.GetExposureValue(i, j);
              if (Exposure > MaxExposure)
              {
                MaxExposure = Exposure;
                memcpy(TempPath, FullPath, NumFullPath * sizeof(int));
                NumTempPath = NumFullPath;
              }
            }
          }
        }
      }
    }
  }
  // Store in Final Path if Necessary;
  if (NumTempPath > 0)
  {
    if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL)
    {
      // Allocate the Path
      if (LocMaxPath != NULL) delete[] LocMaxPath;
      NumLocMaxPath = NumTempPath + 2;
      LocMaxPath = new GVPoint2D[NumLocMaxPath];
      // Store the Start and End Point
      LocMaxPath[0].SetX(StartX);
      LocMaxPath[0].SetY(StartY);
      LocMaxPath[NumCenMaxPath - 1].SetX(EndX);
      LocMaxPath[NumCenMaxPath - 1].SetY(EndY);
      // Copy the Elements
      for (i = 0; i < NumTempPath; ++i)
      {
        CurrentH = DecodeGridIndexH(TempPath[i]);
        CurrentV = DecodeGridIndexV(TempPath[i]);
        LocMaxPath[i + 1].SetX(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
        LocMaxPath[i + 1].SetY(Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
      }
    }
    else if (AlgorithmMode == GVEXPOSURESENSORNETWORK2D_ALGORITHM_CENTRAL)
    {
      // MaxExposure_AdjustPath(TempPath, NumTempPath, EncodeGridIndex(i, j), AdjustedMaxLength);
      // Allocate the Path
      if (CenMaxPath != NULL) delete[] CenMaxPath;
      NumCenMaxPath = NumTempPath + 2;
      CenMaxPath = new GVPoint2D[NumCenMaxPath];
      // Store the Start and End Point
      CenMaxPath[0].SetX(StartX);
      CenMaxPath[0].SetY(StartY);
      CenMaxPath[NumCenMaxPath - 1].SetX(EndX);
      CenMaxPath[NumCenMaxPath - 1].SetY(EndY);
      // Copy the Elements
      for (i = 0; i < NumTempPath; ++i)
      {
        CurrentH = DecodeGridIndexH(TempPath[i]);
        CurrentV = DecodeGridIndexV(TempPath[i]);
        CenMaxPath[i + 1].SetX(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
        CenMaxPath[i + 1].SetY(Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
      }
    }
  }
  if (HalfPath1 != NULL) delete[] HalfPath1;
  if (HalfPath2 != NULL) delete[] HalfPath2;
  if (FullPath != NULL) delete[] FullPath;
  if (TempPath != NULL) delete[] TempPath;
  return(ReturnVal);
} // MaxExposure_UpdateAdjustedBestPoint



int GVExposureSensorNetwork2D::MaxExposure_AdjustPath(int* Path, int NumPath, int HighIndex, double AdjustedMaxLength)
/*
  MaxExposure_AdjustPath() adjusts a path by inserting, removing, and moving
  nodes in the path.  The loop terminates when none of the actions are
  performed or a certain limit is hit. The number of nodes in the new path
  is returned.
*/
{ // MaxExposure_AdjustPath
  int PreviousNum = NumPath;          // Next Number of Nodes in Path
  int NextNum = NumPath;              // Next Number of Nodes in Path
  int GridH = Grid.GetSizeH();        // Grid Horizontal Size
  int GridV = Grid.GetSizeV();        // Grid Vertical Size
  int Adjusts = 0;                    // Number of Tries
  int MaxAdjusts = GridH * GridV / 2; // Maximum Iterations
  int PathAdjusted = 1;               // Keep Adjusting Nodes

  while((PathAdjusted) && (Adjusts < MaxAdjusts))
  {
    PathAdjusted = 0;

    NextNum = MaxExposure_AdjustPath_Insert(Path, PreviousNum, HighIndex, AdjustedMaxLength);
    if (NextNum != PreviousNum) PathAdjusted = 1;
    PreviousNum = NextNum;

    NextNum = MaxExposure_AdjustPath_Remove(Path, PreviousNum, HighIndex, AdjustedMaxLength);
    if (NextNum != PreviousNum) PathAdjusted = 1;
    PreviousNum = NextNum;

    NextNum = MaxExposure_AdjustPath_Move(Path, PreviousNum, HighIndex, AdjustedMaxLength);
    if (NextNum) PathAdjusted = 1;

    ++Adjusts;
  }
  
  return(PreviousNum);
} // MaxExposure_AdjustPath



int GVExposureSensorNetwork2D::MaxExposure_AdjustPath_Insert(int* Path, int NumPath, int HighIndex, double AdjustedMaxLength)
/*
  MaxExposure_AdjustPath_Insert() inserts a node that has the largest
  increase in exposure.  It returns the number of nodes in the adjusted path.
*/
{ // MaxExposure_AdjustPath_Insert
  int i = 0;                                                 // Index Variable
  int j = 0;                                                 // Index Variable
  int k = 0;                                                 // Index Variable
  int PI = 0;                                                // Previous Index
  int PH = 0;                                                // Previous Horizontal Index
  int PV = 0;                                                // Previous Vertical Index
  int NI = 0;                                                // Next Index
  int NH = 0;                                                // Next Horizontal Index
  int NV = 0;                                                // Next Vertical Index
  int DH = 0;                                                // Difference Between Horizontal Positions
  int DV = 0;                                                // Difference Between Vertical Positions
  int GridH = Grid.GetSizeH();                               // Grid Horizontal Size
  int GridV = Grid.GetSizeV();                               // Grid Vertical Size
  int TotalGrid = GridH * GridV;                             // Total Grid Points
  int NumNewPath = NumPath + 1;                              // Number of Elements in New Path
  int NumFinalPath = NumPath;                                // Number in Final Path
  int NumSharedNeighbors = 0;                                // Number of Shared Neighbors
  int Nbr = 0;                                               // Shared Neighbor
  int* FinalPath = NULL;                                     // New Path
  int* NewPath = NULL;                                       // New Path
  int* InPath = NULL;                                        // Nodes Already In Path
  int* SharedNeighbors = NULL;                               // Shared Neighbors
  double Length = 0.0;                                       // Current Length
  double Exposure = 0.0;                                     // Current Exposure
  double MaxExposure = 0.0;                                  // Exposure To Beat
  double HighExposure = 0.0;                                 // Highest Exposure Point

  HighExposure = Grid.GetExposureValue(DecodeGridIndexH(HighIndex), DecodeGridIndexV(HighIndex));
  Length = CalculatePathLength(Path, NumPath);
  MaxExposure = CalculatePathExposure(Path, NumPath) + ((AdjustedMaxLength - Length) / GVEXPOSURESENSORNETWORK2D_MINDISTANCE) * HighExposure;
  NewPath = new int[TotalGrid];
  InPath = new int[TotalGrid];
  SharedNeighbors = new int[2 * (GridV + GridH)];
  FinalPath = new int[TotalGrid];
  for (i = 0; i < TotalGrid; ++i) InPath[i] = 0;
  for (i = 1; i < NumPath; ++i)
  {
    // Copy Path, But Leave Spot Blank For Inserting Neighbor.  Mark Those In path
    for (j = 0; j < i; ++j)
    {
      NewPath[j] = Path[j];
      InPath[Path[j]] = 1;
    }
    for (j = i; j < NumPath; ++j)
    {
      NewPath[j + 1] = Path[j];
      InPath[Path[j]] = 1;
    }
    PI = Path[i - 1];
    PH = DecodeGridIndexH(PI);
    PV = DecodeGridIndexV(PI);
    NI = Path[i];
    NH = DecodeGridIndexH(NI);
    NV = DecodeGridIndexV(NI);
    NumSharedNeighbors = CalculateSharedNeighbors(PH, PV, NH, NV, SharedNeighbors);
    for (k = 0; k < NumSharedNeighbors; ++k)
    {
      Nbr = SharedNeighbors[k];
      if (Nbr >= 0)
      {
        if ((!InPath[Nbr]) && Grid.GetCovered(DecodeGridIndexH(Nbr), DecodeGridIndexV(Nbr)))
        {
          NewPath[i] = Nbr;
          Length = CalculatePathLength(NewPath, NumNewPath);
          if (Length < AdjustedMaxLength)
          {
            Exposure = CalculatePathExposure(NewPath, NumNewPath) + ((AdjustedMaxLength - Length) / GVEXPOSURESENSORNETWORK2D_MINDISTANCE) * HighExposure;
            if (Exposure > MaxExposure)
            {
              memcpy(FinalPath, NewPath, NumNewPath * sizeof(int));
              NumFinalPath = NumNewPath;
              MaxExposure = Exposure;
            }
          }
        }
      }
    }
  }
  if (NumFinalPath != NumPath)
  {
    memcpy(Path, FinalPath, NumNewPath * sizeof(int));
  }
  if (NewPath != NULL) delete[] NewPath;
  if (InPath != NULL) delete[] InPath;
  if (FinalPath != NULL) delete[] FinalPath;
  if (SharedNeighbors != NULL) delete[] SharedNeighbors;
  return(NumFinalPath);
} // MaxExposure_AdjustPath_Insert



int GVExposureSensorNetwork2D::MaxExposure_AdjustPath_Remove(int* Path, int NumPath, int HighIndex, double AdjustedMaxLength)
/*
  MaxExposure_AdjustPath_Remove() removes the node that maximally increases
  the exposure.  If removing nodes does not increase exposure, none are
  removed.  It returns the number of nodes in the adjusted path.
*/
{ // MaxExposure_AdjustPath_Remove
  int i = 0;                                    // Index Variable
  int j = 0;                                    // Index Variable
  int PI = 0;                                   // Previous Index
  int PH = 0;                                   // Previous Horizontal Index
  int PV = 0;                                   // Previous Vertical Index
  int NI = 0;                                   // Next Index
  int NH = 0;                                   // Next Horizontal Index
  int NV = 0;                                   // Next Vertical Index
  int DH = 0;                                   // Difference Between Horizontal Positions
  int DV = 0;                                   // Difference Between Vertical Positions
  int NumNewPath = NumPath - 1;                 // Number of Nodes in New Path
  int NumFinalPath = NumPath;                   // Number of Nodes in Final Path
  int GridH = Grid.GetSizeH();                  // Grid Horizontal Size
  int GridV = Grid.GetSizeV();                  // Grid Vertical Size
  int TotalGrid = GridH * GridV;                // Total Grid Points
  int* FinalPath = NULL;                        // New Path
  int* NewPath = NULL;                          // New Path
  double Length = 0.0;                          // Length of Path
  double Exposure = 0.0;                        // Exposure of Path
  double MaxExposure = 0.0;                     // Exposure To Beat
  double HighExposure = 0.0;                    // Highest Exposure Point

  HighExposure = Grid.GetExposureValue(DecodeGridIndexH(HighIndex), DecodeGridIndexV(HighIndex));
  Length = CalculatePathLength(Path, NumPath);
  MaxExposure = CalculatePathExposure(Path, NumPath) + ((AdjustedMaxLength - Length) / GVEXPOSURESENSORNETWORK2D_MINDISTANCE) * HighExposure;
  NewPath = new int[TotalGrid];
  FinalPath = new int[TotalGrid];
  for (i = 1; i < NumNewPath; ++i)
  {
    if (Path[i] != HighIndex)
    {
      // Calculate the Grid Indices of Next and Previous
      PI = Path[i - 1];
      PH = DecodeGridIndexH(PI);
      PV = DecodeGridIndexV(PI);
      NI = Path[i + 1];
      NH = DecodeGridIndexH(NI);
      NV = DecodeGridIndexV(NI);
      PI = Path[i - 1];
      PH = DecodeGridIndexH(PI);
      PV = DecodeGridIndexV(PI);
      NI = Path[i];
      NH = DecodeGridIndexH(NI);
      NV = DecodeGridIndexV(NI);
      // Calculate If They Are Neighbors
      DH = NH - PH;
      DV = NV - PV;
      if (DH < 0) DH = -DH;
      if (DV < 0) DV = -DV;
      if ((DH <= 1) || (DV <= 1))
      {
        // Copy Path into New Path
        for (j = 0; j < i; ++j)
        {
          NewPath[j] = Path[j];
        }
        for (j = i + 1; j < NumPath; ++j)
        {
          NewPath[j - 1] = Path[j];
        }
        // Calculate the Length
        Length = CalculatePathLength(NewPath, NumNewPath);
        if (Length < AdjustedMaxLength)
        {
          // Calculate the Exposure
          Exposure = CalculatePathExposure(NewPath, NumNewPath) + ((AdjustedMaxLength - Length) / GVEXPOSURESENSORNETWORK2D_MINDISTANCE) * HighExposure;
          if (Exposure > MaxExposure)
          {
            memcpy(FinalPath, NewPath, NumNewPath * sizeof(int));
            NumFinalPath = NumNewPath;
            MaxExposure = Exposure;
          }
        }
      }
    }
  }
  if (NumFinalPath != NumPath)
  {
    memcpy(Path, FinalPath, NumNewPath * sizeof(int));
  }
  if (NewPath != NULL) delete[] NewPath;
  if (FinalPath != NULL) delete[] FinalPath;
  return(NumFinalPath);
  return(NumFinalPath);
} // MaxExposure_AdjustPath_Remove



int GVExposureSensorNetwork2D::MaxExposure_AdjustPath_Move(int* Path, int NumPath, int HighIndex, double AdjustedMaxLength)
/*
  MaxExposure_AdjustPath_Move() moves the node that has the maximal
  increase in exposure.  The node moved must still be valid neigbhors
  with previous and next nodes in the path.  It returns 1 if a node in
  the path has been moved.
*/
{ // MaxExposure_AdjustPath_Move
  int i = 0;                   // Index Variable
  int j = 0;                   // Index Variable
  int k = 0;                   // Index Variable
  int PI = 0;                  // Previous Index
  int PH = 0;                  // Previous Horizontal Value
  int PV = 0;                  // Previous Vertical Value
  int NI = 0;                  // Next Index
  int NH = 0;                  // Next Horizontal Value
  int NV = 0;                  // Next Vertical Value
  int CI = 0;                  // Current Index
  int CH = 0;                  // Current Horizontal Value
  int CV = 0;                  // Current Vertical Value
  int PrevNbr = 0;             // Previous Neighbor
  int NextNbr = 0;             // Next Neighbor
  int MaxIndex = NumPath - 1;  // Maximum Index To Move
  int ReturnVal = 0;           // Was Path Moved?
  int GridH = Grid.GetSizeH(); // Horizontal Grid Size
  int GridV = Grid.GetSizeV(); // Horizontal Grid Size
  int Total = GridH * GridV;   // Total Indices
  int NumPrevNeighbors = 0;    // Number of Previous/Current Neighbors
  int NumNextNeighbors = 0;    // Number of Next/Current Neighbors
  int NumMutualNeighbors = 0;  // Number of Next/Previous/Current Neighbors
  int* TempPath = NULL;        // Temporary Path
  int* FinalPath = NULL;       // Final Path
  int* PrevNeighbors = NULL;   // Neighbors of Previous Index and Current Index
  int* NextNeighbors = NULL;   // Neighbors of Next Index and Current Index
  int* MutualNeighbors = NULL; // Neighbors of both Next, Current, and Previous
  double Length = 0.0;         // Length of Path
  double Exposure = 0.0;       // Exposure of Path
  double MaxExposure = 0.0;    // Exposure To Beat
  double HighExposure = 0.0;   // High Exposure Value

  HighExposure = Grid.GetExposureValue(DecodeGridIndexH(HighIndex), DecodeGridIndexV(HighIndex));
  Length = CalculatePathLength(Path, NumPath);
  MaxExposure = CalculatePathExposure(Path, NumPath) + ((AdjustedMaxLength - Length) / GVEXPOSURESENSORNETWORK2D_MINDISTANCE) * HighExposure;
  FinalPath = new int[Total];
  TempPath = new int[Total];
  PrevNeighbors = new int[Total];
  NextNeighbors = new int[Total];
  MutualNeighbors = new int[Total];
  for (i = 1; i < NumPath - 1; ++i)
  {
    if (Path[i] != HighIndex)
    {
      memcpy(TempPath, Path, NumPath * sizeof(int));
      // Calculate Shared Neigbhors of Both Indices
      PI = Path[i - 1];
      PH = DecodeGridIndexH(PI);
      PV = DecodeGridIndexV(PI);
      CI = Path[i];
      CH = DecodeGridIndexH(CI);
      CV = DecodeGridIndexV(CI);
      NI = Path[i + 1];
      NH = DecodeGridIndexH(NI);
      NV = DecodeGridIndexV(NI);
      NumPrevNeighbors = CalculateSharedNeighbors(PH, PV, CH, CV, PrevNeighbors);
      NumNextNeighbors = CalculateSharedNeighbors(CH, CV, NH, NV, NextNeighbors);
      // Calculate Mutual Neighbors
      NumMutualNeighbors = 0;
      for (j = 0; j < NumPrevNeighbors; ++j)
      {
        PrevNbr = PrevNeighbors[j];
        for (k = 0; k < NumNextNeighbors; ++k)
        {
          NextNbr = NextNeighbors[k];
          if (PrevNbr == NextNbr)
          {
            MutualNeighbors[NumMutualNeighbors] = PrevNbr;
            ++NumMutualNeighbors;
          }
        }
      }
      // For Each Mutual Neighbor, See if it Makes Path Better
      for (j = 0; j < NumMutualNeighbors; ++j)
      {
        TempPath[i] = MutualNeighbors[j];
        Length = CalculatePathLength(TempPath, NumPath);
        if (Length < AdjustedMaxLength)
        {
          Exposure = CalculatePathExposure(TempPath, NumPath) + ((AdjustedMaxLength - Length) / GVEXPOSURESENSORNETWORK2D_MINDISTANCE) * HighExposure;
          if (Exposure > MaxExposure)
          {
            memcpy(FinalPath, TempPath, NumPath * sizeof(int));
            ReturnVal = 1;
          }
        }
      }
    }
  }
  if (ReturnVal)
  {
    memcpy(Path, FinalPath, NumPath * sizeof(int));
  }
  if (FinalPath != NULL) delete[] FinalPath;
  if (TempPath != NULL) delete[] TempPath;
  if (PrevNeighbors != NULL) delete[] PrevNeighbors;
  if (NextNeighbors != NULL) delete[] NextNeighbors;
  if (MutualNeighbors != NULL) delete[] MutualNeighbors;
  return(ReturnVal);
} // MaxExposure_AdjustPath_Move



int GVExposureSensorNetwork2D::CalculateSharedNeighbors(int PH, int PV, int NH, int NV, int* SharedNeighbors)
/*
  CalculateSharedNeighbors() puts all the shared neighbors of
  (PH, PV) and (NH, NV) in SharedNeighbors.
*/
{ // CalculateSharedNeighbors
  int k = 0;                   // Index Variable
  int Nbr = 0;                 // Neighbor
  int GridH = Grid.GetSizeH(); // Horizontal Grid Size
  int GridV = Grid.GetSizeV(); // Vertical Grid Size
  int DH = 0;                  // Difference Horizontally
  int DV = 0;                  // Difference Vertically
  int NbrH = 0;                // Neighbor H
  int NbrV = 0;                // Neighbor H
  int NumSharedNeighbors = 0;  // Number of Shared Neighbors

  // Calculate Previous and Next Indices and The Horizontal/Vertical Deltas
  DH = NH - PH;
  DV = NV - PV;
  if (DH < 0) DH = -DH;
  if (DV < 0) DV = -DV;
  // Calculate Neighbors
  NumSharedNeighbors = 0;
  if (DH == 0)
  {
    NbrH = PH - 1;
    if ((NbrH >= 0) && (NbrH < GridH))
    {
      for (k = 0; k < GridV; ++k)
      {
        Nbr = EncodeGridIndex(NbrH, k);
        SharedNeighbors[NumSharedNeighbors] = Nbr;
        ++NumSharedNeighbors;
      }
    }
    NbrH = PH + 1;
    if ((NbrH >= 0) && (NbrH < GridH))
    {
      for (k = 0; k < GridV; ++k)
      {
        Nbr = EncodeGridIndex(NbrH, k);
        SharedNeighbors[NumSharedNeighbors] = Nbr;
        ++NumSharedNeighbors;
      }
    }
  }
  if (DV == 0)
  {
    NbrV = PV - 1;
    if ((NbrV >= 0) && (NbrV < GridV))
    {
      for (k = 0; k < GridH; ++k)
      {
        Nbr = EncodeGridIndex(k, NbrV);
        SharedNeighbors[NumSharedNeighbors] = Nbr;
        ++NumSharedNeighbors;
      }
    }
    NbrV = PV + 1;
    if ((NbrV >= 0) && (NbrV < GridV))
    {
      for (k = 0; k < GridH; ++k)
      {
        Nbr = EncodeGridIndex(k, NbrV);
        SharedNeighbors[NumSharedNeighbors] = Nbr;
        ++NumSharedNeighbors;
      }
    }
  }
  if ((DH != 0) && (DV != 0))
  {
    SharedNeighbors[0] = EncodeGridIndex(NH - 1, PV + 1);
    SharedNeighbors[1] = EncodeGridIndex(NH - 1, PV - 1);
    SharedNeighbors[2] = EncodeGridIndex(NH + 1, PV + 1);
    SharedNeighbors[3] = EncodeGridIndex(NH + 1, PV - 1);
    SharedNeighbors[4] = EncodeGridIndex(PH - 1, NV + 1);
    SharedNeighbors[5] = EncodeGridIndex(PH - 1, NV - 1);
    SharedNeighbors[6] = EncodeGridIndex(PH + 1, NV + 1);
    SharedNeighbors[7] = EncodeGridIndex(PH + 1, NV - 1);
    NumSharedNeighbors = 8;
  }

  return(NumSharedNeighbors);
} // CalculateSharedNeighbors



int GVExposureSensorNetwork2D::GetNumPathPoints(int PathNum)
/*
  GetNumPathPoints() returns the number of points in the specified path.
  The path can be CENMIN, CENMAX, LOCMIN, LOCMAX.
*/
{ // GetNumPathPoints
  int ReturnVal = 0; // Return Value

  switch(PathNum)
  {
    case GVEXPOSURESENSORNETWORK2D_PATH_CENMIN:
      ReturnVal = NumCenMinPath;
    break;

    case GVEXPOSURESENSORNETWORK2D_PATH_CENMAX:
      ReturnVal = NumCenMaxPath;
    break;

    case GVEXPOSURESENSORNETWORK2D_PATH_LOCMIN:
      ReturnVal = NumLocMinPath;
    break;

    case GVEXPOSURESENSORNETWORK2D_PATH_LOCMAX:
      ReturnVal = NumLocMaxPath;
    break;
  }
  return(ReturnVal);
} // GetNumPathPoints



double GVExposureSensorNetwork2D::GetPathPointX(int PathNum, int PointNum)
/*
  GetPathPointX() returns the X Coordinate of the PointNumth point in the
  specified path.
*/
{ // GetPathPointX
  int PointIndex = -1;            // index of Point in Path
  int GridH = Grid.GetSizeH();    // Horizontal Grid Size
  int GridV = Grid.GetSizeH();    // Vertical Grid Size
  int StartIndex = GridH * GridV; // Index of Starting Point
  int EndIndex = StartIndex + 1;  // Index of End Point
  double ReturnVal = -1.0;        // Return Value

  switch(PathNum)
  {
    case GVEXPOSURESENSORNETWORK2D_PATH_CENMIN:
      if ((PointNum >= 0) && (PointNum < NumCenMinPath)) ReturnVal = CenMinPath[PointNum].GetX();
    break;

    case GVEXPOSURESENSORNETWORK2D_PATH_CENMAX:
      if ((PointNum >= 0) && (PointNum < NumCenMaxPath)) ReturnVal = CenMaxPath[PointNum].GetX();
    break;

    case GVEXPOSURESENSORNETWORK2D_PATH_LOCMIN:
      if ((PointNum >= 0) && (PointNum < NumLocMinPath)) ReturnVal = LocMinPath[PointNum].GetX();
    break;

    case GVEXPOSURESENSORNETWORK2D_PATH_LOCMAX:
      if ((PointNum >= 0) && (PointNum < NumLocMaxPath)) ReturnVal = LocMaxPath[PointNum].GetX();
    break;
  }
  return(ReturnVal);
} // GetPathPointX



double GVExposureSensorNetwork2D::GetPathPointY(int PathNum, int PointNum)
/*
  GetPathPointY() returns the Y Coordinate of the PointNumth point in the
  specified path.
*/
{ // GetPathPointY
  int PointIndex = -1;            // index of Point in Path
  int GridH = Grid.GetSizeH();    // Horizontal Grid Size
  int GridV = Grid.GetSizeH();    // Vertical Grid Size
  int StartIndex = GridH * GridV; // Index of Starting Point
  int EndIndex = StartIndex + 1;  // Index of End Point
  double ReturnVal = -1.0;        // Return Value

  switch(PathNum)
  {
    case GVEXPOSURESENSORNETWORK2D_PATH_CENMIN:
      if ((PointNum >= 0) && (PointNum < NumCenMinPath)) ReturnVal = CenMinPath[PointNum].GetY();
    break;

    case GVEXPOSURESENSORNETWORK2D_PATH_CENMAX:
      if ((PointNum >= 0) && (PointNum < NumCenMaxPath)) ReturnVal = CenMaxPath[PointNum].GetY();
    break;

    case GVEXPOSURESENSORNETWORK2D_PATH_LOCMIN:
      if ((PointNum >= 0) && (PointNum < NumLocMinPath)) ReturnVal = LocMinPath[PointNum].GetY();
    break;

    case GVEXPOSURESENSORNETWORK2D_PATH_LOCMAX:
      if ((PointNum >= 0) && (PointNum < NumLocMaxPath)) ReturnVal = LocMaxPath[PointNum].GetY();
    break;
  }
  return(ReturnVal);
} // GetPathPointY



int GVExposureSensorNetwork2D::MinExposure_UpdateMessageSearch(void)
/*
  MinExposure_UpdateMessageSearch() picks the best neighbor according to the
  minimal exposure search heuristic.
*/
{ // MinExposure_UpdateMessageSearch
  int ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS; // Return Value
  int i = 0;                                                      // Index Variable
  int j = 0;                                                      // Index Variable
  int GridH = Grid.GetSizeH();                                    // Horizontal Grid Squares
  int GridV = Grid.GetSizeV();                                    // Vertical Grid Squares
  int NumNbr = NumNeighbors[NextNode];                            // Number of Neighbors for Current Node
  int NbrIndex = 0;                                               // Neighbor Index for Current Neighbor
  int DestinationH = 0;                                           // Source Grid Square
  int DestinationV = 0;                                           // Source Grid Square
  double NeighborValue = 0.0;                                     // Heuristic Value for Neighbor
  double MaxNeighborValue = 0.0;                                  // Best Heuristic Value for Neighbor
  double A = 0.0;                                                 // Heuristic Parameter
  double D0 = 0.0;                                                // Distance from Current Node to Destination
  double D1 = 0.0;                                                // Distance from Current Node to Neighbor
  double D2 = 0.0;                                                // Distance from Neighbor to Destination
  double SensorX = Sensors[CurrentNode].GetX();                   // Sensor X Coordinate
  double SensorY = Sensors[CurrentNode].GetY();                   // Sensor Y Coordinate
  double DestinationX = Sensors[DestinationNode].GetX();          // Destination X Coordinate
  double DestinationY = Sensors[DestinationNode].GetY();          // Destination Y Coordinate
  double NeighborX = 0.0;                                         // Neighbor X Coordinate
  double NeighborY = 0.0;                                         // Neighbor Y Coordinate

  MinExposure_UpdateCurrentNode(NextNode);
  if (CurrentNode == DestinationNode)
  {
    D1 = GVSENSORNETWORK_INFINITY;
    for (i = 0; i < GridH; ++i)
    {
      for (j = 0; j < GridV; ++j)
      {
        NeighborX = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
        NeighborY = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
        D0 = Distance2D(DestinationX, DestinationY, NeighborX, NeighborY);
        if (D0 < D1)
        {
          D1 = D0;
          DestinationH = i;
          DestinationV = j;
        }
      }
    }
    CalculateBreadthFirstSearch(EncodeGridIndex(DestinationH, DestinationV));
    MessageMode = GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_CHECK;
    CurrentIteration = 0;
  }
  else if (NumNbr > 0)
  {
    A = 1.0 / pow(CurrentIteration + 1, GVEXPOSURESENSORNETWORK2D_WEIGHTFACTOR);
    D0 = Distance2D(SensorX, SensorY, DestinationX, DestinationY);
    NextNode = CurrentNode;
    for (i = 0; i < NumNbr; ++i)
    {
      NbrIndex = NeighborIndex[CurrentNode][i];
      NeighborX = Sensors[NbrIndex].GetX();
      NeighborY = Sensors[NbrIndex].GetY();
      D1 = Distance2D(SensorX, SensorY, NeighborX, NeighborY) / Radius;
      D2 = 1.0 - (Distance2D(NeighborX, NeighborY, DestinationX, DestinationY) / D0);
      /*
        if (D1 > 1.0) D1 = 1.0;
        else if (D1 < 0.0) D1 = 0.0;
        if (D2 > 1.0) D2 = 1.0;
        else if (D2 < 0.0) D2 = 0.0;
      */
      NeighborValue =  A * D1 + (1.0 - A) * D2;
      if (NeighborValue > MaxNeighborValue)
      {
        NextNode = NbrIndex;
        MaxNeighborValue = NeighborValue;
      }
    }
    ++CurrentIteration;
  }
  else
  {
    ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_ERROR;
  }
  return(ReturnVal);
} // MinExposure_UpdateMessageSearch



int GVExposureSensorNetwork2D::MinExposure_UpdateMessageForward(void)
/*
  MinExposure_UpdateMessageForward() goes to the Breadth-First-Search parent
  if it is not the destination node.
*/
{ // MinExposure_UpdateMessageForward
  int StartSensor = 0;                                            // Closest Sensor to Start
  int EndSensor = 0;                                              // Closest Sensor to Start
  int ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS; // Return Value
  double CurrentX = Sensors[CurrentNode].GetX();                  // Current X Coordinate
  double CurrentY = Sensors[CurrentNode].GetY();                  // Current X Coordinate

  MinExposure_UpdateCurrentNode(NextNode);
  if (CurrentNode != DestinationNode)
  {
    NextNode = Sensors[CurrentNode].GetParent();
  }
  else
  {
    StartSensor = CalculateNearestSensor(Start.GetX(), Start.GetY());
    EndSensor = CalculateNearestSensor(End.GetX(), End.GetY());
    if (Distance2D(CurrentX, CurrentY, Sensors[StartSensor].GetX(), Sensors[StartSensor].GetY()) < Distance2D(CurrentX, CurrentY, Sensors[EndSensor].GetX(), Sensors[EndSensor].GetY()))
    {
      DestinationNode = EndSensor;
    }
    else
    {
      DestinationNode = StartSensor;
    }
    SourceNode = CurrentNode;
    NextNode = CurrentNode;
    MessageMode = GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH;
  }

  return(ReturnVal);
} // MinExposure_UpdateMessageForward



int GVExposureSensorNetwork2D::MinExposure_UpdateMessageCheck(void)
/*
  
*/
{ // MinExposure_UpdateMessageCheck
  int i = 0;                                                      // Index Variable
  int j = 0;                                                      // Index Variable
  int ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS; // Return Value
  int GridH = Grid.GetSizeH();                                    // Horizontal Grid Squares
  int GridV = Grid.GetSizeV();                                    // Vertical Grid Squares
  int Total = GridH * GridV;                                      // Total Grid Points
  int PotentialNode = -1;                                         // Node to Send Message To
  int SourceH = 0;                                                // Source Grid Square
  int SourceV = 0;                                                // Source Grid Square
  int DestinationH = 0;                                           // Destination Grid Square
  int DestinationV = 0;                                           // Destination Grid Square
  int CurrentH = 0;                                               // Current Grid Square
  int CurrentV = 0;                                               // Current Grid Square
  int NumTempPath = 0;                                            // Number of Nodes in the Temporary Path
  int* TempPath = NULL;                                           // Temporary Path
  double X = 0.0;                                                 // X Coordinate
  double Y = 0.0;                                                 // X Coordinate
  double D = 0.0;                                                 // Distance
  double MinDS = GVSENSORNETWORK_INFINITY;                        // Minimum Source Distance
  double MinDD = GVSENSORNETWORK_INFINITY;                        // Minimum Source Distance
  double SourceX = Sensors[SourceNode].GetX();                    // Destination X Coordinate
  double SourceY = Sensors[SourceNode].GetY();                    // Destination Y Coordinate
  double DestinationX = Sensors[DestinationNode].GetX();          // Destination X Coordinate
  double DestinationY = Sensors[DestinationNode].GetY();          // Destination X Coordinate
  double StartX = Start.GetX();                                   // Ending X Coordinate
  double StartY = Start.GetY();                                   // Ending Y Coordinate
  double EndX = End.GetX();                                       // Ending X Coordinate
  double EndY = End.GetY();                                       // Ending Y Coordinate
  double Exposure = 0.0;                                          // Potential Exposure
  double MinExposure = 0.0;                                       // Minimum Exposure for Potential

  TempPath = new int[Total];
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    {
      X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
      Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
      D = Distance2D(X, Y, StartX, StartY);
      if (D < MinDS)
      {
        MinDS = D;
        SourceH = i;
        SourceV = j;
      }
      D = Distance2D(X, Y, EndX, EndY);
      if (D < MinDD)
      {
        MinDD = D;
        DestinationH = i;
        DestinationV = j;
      }
    }
  }
  NumTempPath = MinExposure_GridDijkstraSearch(SourceH, SourceV, DestinationH, DestinationV, TempPath);
  // Allocate the Path
  if (LocMinPath != NULL) delete[] LocMinPath;
  NumLocMinPath = NumTempPath + 2;
  LocMinPath = new GVPoint2D[NumLocMinPath];
  // Store the Start and End Point
  LocMinPath[0].SetX(Start.GetX());
  LocMinPath[0].SetY(Start.GetY());
  LocMinPath[NumLocMinPath - 1].SetX(End.GetX());
  LocMinPath[NumLocMinPath - 1].SetY(End.GetY());
  // Copy the Elements
  for (i = 0; i < NumTempPath; ++i)
  {
    CurrentH = DecodeGridIndexH(TempPath[i]);
    CurrentV = DecodeGridIndexV(TempPath[i]);
    LocMinPath[i + 1].SetX(Grid.GetPointX(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
    LocMinPath[i + 1].SetY(Grid.GetPointY(CurrentH, CurrentV, GVGRIDPOINT2D_CENTERPOINT));
  }
  // Check Potential Nodes
  MinExposure = CalculatePathExposure(TempPath, NumTempPath);
  DestinationX = Grid.GetPointX(DestinationH, DestinationV, GVGRIDPOINT2D_CENTERPOINT);
  DestinationY = Grid.GetPointY(DestinationH, DestinationV, GVGRIDPOINT2D_CENTERPOINT);
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    {
      if (Grid.GetCovered(i, j) == GVEXPOSURESENSORNETWORK2D_GRID_POTENTIAL)
      {
        Exposure = Grid.GetExposureValue(i, j) + MinExposure_CalculatePotentialExposure(Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT), Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT), DestinationX, DestinationY);
        if (Exposure < MinExposure)
        {
          MinExposure = Exposure;
          PotentialNode = Grid.GetNearestSensor(i, j);
        }
      }
    }
  }
  if (PotentialNode >= 0)
  {
    DestinationNode = PotentialNode;
    CalculateBreadthFirstSearch(DestinationNode);
    MessageMode = GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_FORWARD;
  }
  else
  {
    ReturnVal = GVEXPOSURESENSORNETWORK2D_RETURNVAL_DONE;
  }
  if (TempPath != NULL) delete[] TempPath;
  return(ReturnVal);
} // MinExposure_UpdateMessageCheck



void GVExposureSensorNetwork2D::MinExposure_UpdateCurrentNode(int Next)
/*
  MinExposure_UpdateCurrentNode() sets the current node to visited.
  It marks the neighbor nodes as known.  It marks its entire voronoi
  cell as covered.
*/
{ // MinExposure_UpdateCurrentNode
  int i = 0;                                                                   // Index Variable
  int j = 0;                                                                   // Index Variable
  int NbrNum = NumNeighbors[Next];                                             // Number of Neighbors
  int NbrIndex = 0;                                                            // Index of Neighbor Node
  int GridH = Grid.GetSizeH();                                                 // Horizontal Grid Size
  int GridV = Grid.GetSizeV();                                                 // Vertical Grid Size
  int iSensor = 0;                                                             // Horizontal Grid Square of Sensor
  int jSensor = 0;                                                             // Vertical Grid Square of Sensor
  int iMin = 0;                                                                // Minimum i Value
  int iMax = 0;                                                                // Minimum j Value
  int jMin = 0;                                                                // Maximum i Value
  int jMax = 0;                                                                // Maximum j Value
  int GridRadiusH = Grid.GetRadiusHorizontal();                                // Horizontal Radius Grid Squares
  int GridRadiusV = Grid.GetRadiusVertical();                                  // Vertical Radius Grid Squares
  double D = 0.0;                                                              // Distance
  double X = 0.0;                                                              // X Coordinate
  double Y = 0.0;                                                              // Y Coordinate
  double GridHX = Grid.GetHorizontalVectorX();                                 // Horizontal Grid Vector X Coordinate
  double GridHY = Grid.GetHorizontalVectorY();                                 // Horizontal Grid Vector Y Coordinate
  double GridVX = Grid.GetVerticalVectorX();                                   // Vertical Grid Vector X Coordinate
  double GridVY = Grid.GetVerticalVectorY();                                   // Vertical Grid Vector Y Coordinate
  double GridHLength = Distance2D(0.0, 0.0, GridHX, GridHY);                   // Length of Grid Horizontal Vector
  double GridVLength = Distance2D(0.0, 0.0, GridVX, GridVY);                   // Length of Grid Vertical Vector  
  double SensorX = Sensors[Next].GetX();                                       // Sensor X Coordinate
  double SensorY = Sensors[Next].GetY();                                       // Sensor Y Coordinate
  double GridSensorX = SensorX - Grid.GetPointX(0, 0, GVGRIDPOINT2D_TOPLEFT);  // Sensor X Coordinate With Respect to Grid Start
  double GridSensorY = SensorY - Grid.GetPointY(0, 0, GVGRIDPOINT2D_TOPLEFT);  // Sensor Y Coordinate With Respect to Grid Start
    
  // Calculate Relevant Grid Squares: iMin to iMax, jMin to jMax (Significantly faster on high-resolution grids)
  CurrentNode = NextNode;
  GridHLength = Distance2D(0.0, 0.0, GridHX, GridHY);
  GridVLength = Distance2D(0.0, 0.0, GridVX, GridVY);
  iSensor = ((int) ((GridHX * GridSensorX + GridHY * GridSensorY) / (GridHLength * GridHLength)));
  jSensor = ((int) ((GridVX * GridSensorX + GridVY * GridSensorY) / (GridVLength * GridVLength)));
  iMin = iSensor - GridRadiusH;
  iMax = iSensor + GridRadiusH;
  jMin = jSensor - GridRadiusV;
  jMax = jSensor + GridRadiusV;
  if (iMin < 0) iMin = 0;
  if (iMax >= GridH) iMax = GridH - 1;
  if (jMin < 0) jMin = 0;
  if (jMax >= GridV) jMax = GridV - 1;

  // Set Area to Covered (Known AND Voronoi)
  for (i = iMin; i <= iMax; ++i)
  {
    for (j = jMin; j <= jMax; ++j)
    {
      if (!Grid.GetCovered(i, j))
      {
        X = Grid.GetPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
        Y = Grid.GetPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
        D = Distance2D(X, Y,SensorX, SensorY);
        if (D <= Radius) Grid.SetCovered(i, j, GVEXPOSURESENSORNETWORK2D_GRID_COVERED);
      }
    }
  }
  Sensors[CurrentNode].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_VISITED);
  for (i = 0; i < GridH; ++i)
  {
    for (j = 0; j < GridV; ++j)
    {
      if (Grid.GetNearestSensor(i, j) == CurrentNode)
      {
        Grid.SetCovered(i, j, GVEXPOSURESENSORNETWORK2D_GRID_COVERED);
      }
    }
  }

  // Set Neighbors to Known
  for (i = 0; i < NbrNum; ++i)
  {
    NbrIndex = NeighborIndex[CurrentNode][i];
    if (!Sensors[NbrIndex].GetKnown()) Sensors[NbrIndex].SetKnown(GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN);
  }
} // MinExposure_UpdateCurrentNode



double GVExposureSensorNetwork2D::MinExposure_CalculatePotentialExposure(double X1, double Y1, double X2, double Y2)
/*
  MinExposure_CalculatePotentialExposure() calculates the potentially smallest exposure
  for a path.
*/
{ // MinExposure_CalculatePotentialExposure
  int i = 0;              // Index Variable
  double Potential = 0.0; // Potential Value
  double ReturnVal = 0.0; // Return Value

  for (i = 0; i < NumSensors; ++i)
  {
    if (Sensors[i].GetKnown())
    {
      Potential = MinExposure_CalculateMinimalSensorExposure(i, X1, Y1, X2, Y2, 0);
      if (Potential > ReturnVal) ReturnVal = Potential;
    }
  }
  return(ReturnVal);
} // MinExposure_CalculatePotentialExposure


double GVExposureSensorNetwork2D::MinExposure_CalculateMinimalSensorExposure(int Sensor, double X1, double Y1, double X2, double Y2, int ValidityCheck)
/*
  MinExposure_CalculateMinimalSensorExposure() calculates the minimal exposure value
  for a single sensor between two points.
*/
{ // MinExposure_CalculateMinimalSensorExposure
  int i = 0;
  int NearestSensor = -1;
  int NextSensor = -1;
  int Valid = 1;
  int Flip = 0;
  double ReturnVal = -1.0;
  double TransformAngleRadians = 0.0;
  double AngleRadians = 0.0;
  double AngleDegrees = 0.0;
  double PreviousAngleRadians = 0.0;
  double PreviousAngleDegrees = 0.0;
  double CurrentAngleRadians = 0.0;
  double CurrentAngleDegrees = 0.0;
  double DA = 0.0;
  double DB = 0.0;
  double PowerVal = Power - 1.0;
  double InversePowerVal = 1.0 / PowerVal;
  double Coefficient = 0.0;
  double PreviousRadius = 0.0;
  double CurrentRadius = 0.0;
  double DistanceFactor = 0.0;
  double TempVal = 0.0;
  double SensorX = Sensors[Sensor].GetX();
  double SensorY = Sensors[Sensor].GetY();
  GVPoint2D PreviousPoint;
  GVPoint2D CurrentPoint;
  GVPoint2D A;
  GVPoint2D B;
  GVPoint2D E;

  // INITIALIZATION PHASE: Translate to Origin, Put A-Vector on X-Axis, B-Vector in Quadrant I/II.
  E.New(0.0, 1.0);
  // Translate to Origin
  A.SetX(X1 - SensorX);
  A.SetY(Y1 - SensorY);
  B.SetX(X2 - SensorX);
  B.SetY(Y2 - SensorY);
  // Calculate Rotation Angle and Rotate Vertices
  TransformAngleRadians = acos(A.GetX() / Distance2D(0.0, 0.0, A.GetX(), A.GetY()));
  if (A.GetY() > 0.0) TransformAngleRadians = -TransformAngleRadians;
  E.SetX(A.GetX() * cos(TransformAngleRadians) - A.GetY() * sin(TransformAngleRadians));
  E.SetY(A.GetY() * sin(TransformAngleRadians) + A.GetY() * cos(TransformAngleRadians));
  A.SetX(E.GetX());
  A.SetY(E.GetY());
  E.SetX(B.GetX() * cos(TransformAngleRadians) - B.GetY() * sin(TransformAngleRadians));
  E.SetY(B.GetX() * sin(TransformAngleRadians) + B.GetY() * cos(TransformAngleRadians));
  B.SetX(E.GetX());
  B.SetY(E.GetY());
  // Transform the Other Direction Next Time
  TransformAngleRadians = -TransformAngleRadians;
  // Calculate Flip of B Vector
  if (B.GetY() < 0.0)
  {
    B.SetY(-B.GetY());
    Flip = 1;
  }
  // Coefficient Initialization Phase
  DA = Distance2D(0.0, 0.0, A.GetX(), A.GetY());
  DB = Distance2D(0.0, 0.0, B.GetX(), B.GetY());
  AngleRadians = acos((A.GetX() * B.GetX() + A.GetY() * B.GetY()) / (DA * DB));
  AngleDegrees = GVEXPOSURESENSORNETWORK2D_TODEGREES * AngleRadians;
  if (AngleDegrees <= 180.0 / (PowerVal))
  {
    ReturnVal = 0.0;
    Coefficient = (((pow(DB, PowerVal) / pow(DA, PowerVal)) - cos(PowerVal * AngleRadians)) / sin(PowerVal * AngleRadians));
    // For Each Degree Between the Angle, Calculate Path Points and Exposure
    for (PreviousAngleDegrees = 0.0, PreviousAngleRadians = 0.0, PreviousRadius = DA, Valid = 1, CurrentAngleDegrees = GVEXPOSURESENSORNETWORK2D_ANGLEDELTA; Valid && (CurrentAngleDegrees < AngleDegrees); CurrentAngleDegrees += GVEXPOSURESENSORNETWORK2D_ANGLEDELTA)
    {
      // Calculate the Current Radial Value
      CurrentAngleRadians = GVEXPOSURESENSORNETWORK2D_TORADIANS * CurrentAngleDegrees;
      CurrentRadius = DA * pow((cos(PowerVal * CurrentAngleRadians) + sin(PowerVal * CurrentAngleRadians) * Coefficient), InversePowerVal);
      // Calculate the Transformed Points
      PreviousPoint.SetX(PreviousRadius * cos(PreviousAngleRadians));
      PreviousPoint.SetY(PreviousRadius * sin(PreviousAngleRadians));
      CurrentPoint.SetX(CurrentRadius * cos(CurrentAngleRadians));
      CurrentPoint.SetY(CurrentRadius * sin(CurrentAngleRadians));
      // Transform the Points Back
      if (Flip == 1)
      {
        PreviousPoint.SetY(-PreviousPoint.GetY());
        CurrentPoint.SetY(-CurrentPoint.GetY());
      }
      E.SetX(PreviousPoint.GetX() * cos(TransformAngleRadians) - PreviousPoint.GetY() * sin(TransformAngleRadians));
      E.SetX(PreviousPoint.GetX() * sin(TransformAngleRadians) + PreviousPoint.GetY() * cos(TransformAngleRadians));
      PreviousPoint.SetX(E.GetX());
      PreviousPoint.SetY(E.GetY());
      E.SetX(CurrentPoint.GetX() * cos(TransformAngleRadians) - CurrentPoint.GetY() * sin(TransformAngleRadians));
      E.SetY(CurrentPoint.GetX() * sin(TransformAngleRadians) + CurrentPoint.GetY() * cos(TransformAngleRadians));
      CurrentPoint.SetX(E.GetX());
      CurrentPoint.SetY(E.GetY());
      PreviousPoint.SetX(PreviousPoint.GetX() + SensorX);
      PreviousPoint.SetY(PreviousPoint.GetY() + SensorY);
      CurrentPoint.SetX(CurrentPoint.GetX() + SensorX);
      CurrentPoint.SetY(CurrentPoint.GetY() + SensorY);
      // Calculate the Line Exposure
      ReturnVal += CalculateLineExposure(PreviousPoint.GetX(), PreviousPoint.GetY(), CurrentPoint.GetX(), CurrentPoint.GetY());
      // Store the Previous Angle and Radius
      PreviousAngleDegrees = CurrentAngleDegrees;
      PreviousAngleRadians = GVEXPOSURESENSORNETWORK2D_TORADIANS * PreviousAngleDegrees;
      PreviousRadius = CurrentRadius;
      // Check For Validity
      if (ValidityCheck)
      {
        NearestSensor = CalculateNearestSensor(PreviousPoint.GetX(), PreviousPoint.GetY());
        NextSensor = CalculateNearestSensor(CurrentPoint.GetX(), CurrentPoint.GetY());
        if (NearestSensor != NextSensor) Valid = 0;
      }
    }
    // Get Remaining Exposure
    CurrentAngleDegrees = AngleDegrees;
    // Calculate the Current Radial Value
    CurrentAngleRadians = GVEXPOSURESENSORNETWORK2D_TORADIANS * CurrentAngleDegrees;
    CurrentRadius = DA * pow((cos(PowerVal * CurrentAngleRadians) + sin(PowerVal * CurrentAngleRadians) * Coefficient), InversePowerVal);
    // Calculate the Transformed Points
    PreviousPoint.SetX(PreviousRadius * cos(PreviousAngleRadians));
    PreviousPoint.SetY(PreviousRadius * sin(PreviousAngleRadians));
    CurrentPoint.SetX(CurrentRadius * cos(CurrentAngleRadians));
    CurrentPoint.SetY(CurrentRadius * sin(CurrentAngleRadians));
    // Transform the Points Back
    if (Flip == 1)
    {
      PreviousPoint.SetY(-PreviousPoint.GetY());
      CurrentPoint.SetY(-CurrentPoint.GetY());
    }
    E.SetX(PreviousPoint.GetX() * cos(TransformAngleRadians) - PreviousPoint.GetY() * sin(TransformAngleRadians));
    E.SetX(PreviousPoint.GetX() * sin(TransformAngleRadians) + PreviousPoint.GetY() * cos(TransformAngleRadians));
    PreviousPoint.SetX(E.GetX());
    PreviousPoint.SetY(E.GetY());
    E.SetX(CurrentPoint.GetX() * cos(TransformAngleRadians) - CurrentPoint.GetY() * sin(TransformAngleRadians));
    E.SetY(CurrentPoint.GetX() * sin(TransformAngleRadians) + CurrentPoint.GetY() * cos(TransformAngleRadians));
    CurrentPoint.SetX(E.GetX());
    CurrentPoint.SetY(E.GetY());
    PreviousPoint.SetX(PreviousPoint.GetX() + SensorX);
    PreviousPoint.SetY(PreviousPoint.GetY() + SensorY);
    CurrentPoint.SetX(CurrentPoint.GetX() + SensorX);
    CurrentPoint.SetY(CurrentPoint.GetY() + SensorY);
    // Calculate the Line Exposure
    ReturnVal += CalculateLineExposure(PreviousPoint.GetX(), PreviousPoint.GetY(), CurrentPoint.GetX(), CurrentPoint.GetY());
    // Check For Validity
    if (ValidityCheck)
    {
      NearestSensor = CalculateNearestSensor(PreviousPoint.GetX(), PreviousPoint.GetY());
      NextSensor = CalculateNearestSensor(CurrentPoint.GetX(), CurrentPoint.GetY());
      if (NearestSensor != NextSensor) Valid = 0;
    }
  }
  else
  {
    Valid = 0;
  }
  if (!Valid)
  {
    ReturnVal = 0.0;
  }
  return(ReturnVal);
} // MinExposure_CalculateMinimalSensorExposure