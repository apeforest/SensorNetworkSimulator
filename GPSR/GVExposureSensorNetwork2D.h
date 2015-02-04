////////////////////////////////////////////////////////////////////////////////
// GVExposureSensorNetwork2D.h                                                //
//                                                                            //
// Header file for two-dimensional exposure sensor network.                   //
////////////////////////////////////////////////////////////////////////////////



#ifndef GVEXPOSURESENSORNETWORK2D_H
#define GVEXPOSURESENSORNETWORK2D_H



// Includes ////////////////////////////////////////////////////////////////////
#include <iomanip.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include "GVSensorNetwork2D.h"
#include "GVExposureGrid2D.h"



// Defines /////////////////////////////////////////////////////////////////////
#define GVEXPOSURESENSORNETWORK2D_TODEGREES            57.29582791
#define GVEXPOSURESENSORNETWORK2D_TORADIANS            0.017453278
#define GVEXPOSURESENSORNETWORK2D_PROGRESSMESSAGE      1
#define GVEXPOSURESENSORNETWORK2D_MAXADJUSTS           1000
#define GVEXPOSURESENSORNETWORK2D_RANDTHRESHOLD        0.75
#define GVEXPOSURESENSORNETWORK2D_MINDISTANCE          0.001
#define GVEXPOSURESENSORNETWORK2D_NOSENSOR             -1
#define GVEXPOSURESENSORNETWORK2D_NOPARENT             -1
#define GVEXPOSURESENSORNETWORK2D_MAXTRIES             10000
#define GVEXPOSURESENSORNETWORK2D_SPACE                "   "
#define GVEXPOSURESENSORNETWORK2D_AREAPRIORITY         1
#define GVEXPOSURESENSORNETWORK2D_HOPSPRIORITY         1
#define GVEXPOSURESENSORNETWORK2D_INDEXPRIORITY        1
#define GVEXPOSURESENSORNETWORK2D_WEIGHTFACTOR         2.0
#define GVEXPOSURESENSORNETWORK2D_ANGLEDELTA           10.0

#define GVEXPOSURESENSORNETWORK2D_RETURNVAL_DONE       0
#define GVEXPOSURESENSORNETWORK2D_RETURNVAL_INPROGRESS 1
#define GVEXPOSURESENSORNETWORK2D_RETURNVAL_ERROR      2

#define GVEXPOSURESENSORNETWORK2D_BFS_DEQUEUED         (0                                         )
#define GVEXPOSURESENSORNETWORK2D_BFS_ENQUEUED         (GVEXPOSURESENSORNETWORK2D_BFS_DEQUEUED + 1)
#define GVEXPOSURESENSORNETWORK2D_BFS_FINISHED         (GVEXPOSURESENSORNETWORK2D_BFS_ENQUEUED + 1)

#define GVEXPOSURESENSORNETWORK2D_PATH_CENMIN          (0                                        )
#define GVEXPOSURESENSORNETWORK2D_PATH_CENMAX          (GVEXPOSURESENSORNETWORK2D_PATH_CENMIN + 1)
#define GVEXPOSURESENSORNETWORK2D_PATH_LOCMIN          (GVEXPOSURESENSORNETWORK2D_PATH_CENMAX + 1)
#define GVEXPOSURESENSORNETWORK2D_PATH_LOCMAX          (GVEXPOSURESENSORNETWORK2D_PATH_LOCMIN + 1)
#define GVEXPOSURESENSORNETWORK2D_PATH_NUM             (GVEXPOSURESENSORNETWORK2D_PATH_LOCMAX + 1)

#define GVEXPOSURESENSORNETWORK2D_MODE_INIT            (0                                           )
#define GVEXPOSURESENSORNETWORK2D_MODE_UPDATE          (GVEXPOSURESENSORNETWORK2D_MODE_INIT      + 1)
#define GVEXPOSURESENSORNETWORK2D_MODE_CALCULATE       (GVEXPOSURESENSORNETWORK2D_MODE_UPDATE    + 1)
//Lin's definitions
#define GVEXPOSURESENSORNETWORK2D_MODE_GPSR_INPROGRESS (GVEXPOSURESENSORNETWORK2D_MODE_CALCULATE + 1)
#define GVEXPOSURESENSORNETWORK2D_MODE_NUM             (GVEXPOSURESENSORNETWORK2D_MODE_GPSR_INPROGRESS + 1)
#define GPSR_DONE										0
#define GPSR_FAIL										(GPSR_DONE+1)
#define GPSR_CONTINUE									(GPSR_FAIL+1)

// Lin's Definition for Error detection
#define EVENTPROBABILITY								0.5
#define P0												0.2
#define P1												0.7
#define N1												1
#define N2												3
#define UNITENERGYTRANSMIT								800	
#define UNITENERGYRECEIVE								500
#define UNITENERGYMONITOR								10
#define UNITENERGYPROCESS								40
#define CENTRALIZED										(0)
#define DISTRIBUTED										(CENTRALIZED + 1)	
#define HYBRID											(DISTRIBUTED + 1)

#define SQUARE(x)										x*x
// End Lin's definition


#define GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED       (0                                           )
#define GVEXPOSURESENSORNETWORK2D_GRID_COVERED         (GVEXPOSURESENSORNETWORK2D_GRID_UNCOVERED + 1)
#define GVEXPOSURESENSORNETWORK2D_GRID_POTENTIAL       (GVEXPOSURESENSORNETWORK2D_GRID_COVERED   + 1)
#define GVEXPOSURESENSORNETWORK2D_GRID_NUM             (GVEXPOSURESENSORNETWORK2D_GRID_POTENTIAL + 1)

#define GVEXPOSURESENSORNETWORK2D_SENSOR_UNKNOWN       (0                                           )
#define GVEXPOSURESENSORNETWORK2D_SENSOR_USELESS       (GVEXPOSURESENSORNETWORK2D_SENSOR_UNKNOWN + 1)
#define GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN         (GVEXPOSURESENSORNETWORK2D_SENSOR_USELESS + 1)
#define GVEXPOSURESENSORNETWORK2D_SENSOR_VISITED       (GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN   + 1)
#define GVEXPOSURESENSORNETWORK2D_SENSOR_NUM           (GVEXPOSURESENSORNETWORK2D_SENSOR_VISITED + 1)

#define GVEXPOSURESENSORNETWORK2D_ALGORITHM_CENTRAL (0                                              )
#define GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL   (GVEXPOSURESENSORNETWORK2D_ALGORITHM_CENTRAL + 1)
#define GVEXPOSURESENSORNETWORK2D_ALGORITHM_NUM     (GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL   + 1)


#define GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH   (0                                                 )
#define GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_FORWARD  (GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH   + 1)
#define GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_CHECK    (GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_FORWARD  + 1)
#define GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_NUM      (GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_CHECK    + 1)

#define GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_RANDOM    (0                                                         )
#define GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_SHORTEST  (GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_RANDOM    + 1)
#define GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_BEST      (GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_SHORTEST  + 1)
#define GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_ADJUSTED  (GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_BEST      + 1)
#define GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_NUM       (GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_ADJUSTED  + 1)

#define GVEXPOSURESENSORNETWORK2D_CENMINEXPOSUREMODE_DEFAULT   (0                                                         )
#define GVEXPOSURESENSORNETWORK2D_CENMINEXPOSUREMODE_NUM       (GVEXPOSURESENSORNETWORK2D_CENMINEXPOSUREMODE_DEFAULT   + 1)

#define GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_HEURISTIC (0                                                         )
#define GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_GPSR      (GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_HEURISTIC + 1)
#define GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_NUM       (GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_GPSR      + 1)

#define GVEXPOSURESENSORNETWORK2D_LOCMINEXPOSUREMODE_DEFAULT   (0                                                         )
#define GVEXPOSURESENSORNETWORK2D_LOCMINEXPOSUREMODE_NUM       (GVEXPOSURESENSORNETWORK2D_LOCMINEXPOSUREMODE_DEFAULT   + 1)

typedef struct GVExposureSensorNetwork2D_Hole_s
{ // GVExposureSensorNetwork2D_Hole
  int Covered;
  double Angle;
} GVExposureSensorNetwork2D_Hole;



// GVExposureSensorNetwork2D ///////////////////////////////////////////////////
class GVExposureSensorNetwork2D : public GVSensorNetwork2D
{ // GVExposureSensorNetwork2D
  protected:
    int MinExposureInitialized; // Was Minimal Exposure Path Initialized
    int MaxExposureInitialized; // Was Maximal Exposure Path Initialized
	
	// Lin's definition
	int GPSRInitialized;			// Was GPSR Initialized
	int MessageDeliver;		// If GPSR is successful
	int ShortestPath;
	int NumEvent;
	int* Event;
	double EnergyTransmit;
	double EnergyReceive;
	double EnergyMonitor;
	double EnergyProcess;
	double EnergyTotal;
	int* GPSR_Path;
	int DetectionScheme;;


    int LocMinExposureMode;     // Search Mode for Localized Minimal Exposure (Default)
    int LocMaxExposureMode;     // Search Mode for Localized Minimal Exposure (Heuristic/GPSR)
    int CenMinExposureMode;     // Search Mode for Centralized Minimal Exposure (Default)
    int CenMaxExposureMode;     // Search Mode for Centralized Minimal Exposure (Random/Shortest/Best/Adjusted)
    int NumCommunications;      // Number of Communications for Algorithm
    int CurrentIteration;       // Current Iteration of Algorithm (Prevent Infinite Loops)
    int CurrentNode;            // Current Node Containing Message
    int NextNode;               // Next Node to Send To
    int SourceNode;             // Source Node of Message
    int DestinationNode;        // Destination Node of Message
    int MessageMode;            // Message Mode (Search/Forward)
    int AlgorithmStage;         // Type for Algorithms (Initialize/Update/Calculate)
    int AlgorithmMode;          // Mode for Algorithms (Centralized/Localized)
    int NumCenMinPath;          // Number of Nodes in Centralized Minimal Exposure Path
    int NumCenMaxPath;          // Number of Nodes in Centralized Maximal Exposure Path
    int NumLocMinPath;          // Number of Nodes in Localized Minimal Exposure Path
    int NumLocMaxPath;          // Number of Nodes in Localized Maximal Exposure Path
    int MaxLength;           // Maximum Length of Path
	
	

    GVPoint2D Start;            // Starting Point of Path
    GVPoint2D End;              // Ending Point of Path
    GVPoint2D* CenMinPath;      // Centralized Minimal Exposure Path
    GVPoint2D* CenMaxPath;      // Centralized Minimal Exposure Path
    GVPoint2D* LocMinPath;      // Centralized Minimal Exposure Path
    GVPoint2D* LocMaxPath;      // Centralized Minimal Exposure Path
    GVExposureGrid2D Grid;      // Exposure Grid

    // Miscellaneous Functions
    int DecodeGridIndexH(int Index);                                                    // Decodes the Horizontal Component of a Grid Index
    int DecodeGridIndexV(int Index);                                                    // Decodes the Vertical Component of a Grid Index
    int EncodeGridIndex(int i, int j);                                                  // Endcodes Horizontal and Vertical Grid Indices
    int CalculateSharedNeighbors(int PH, int PV, int NH, int NV, int* SharedNeighbors); // This function puts all the Shared Neighbors of (i1, j1) and (i2, j2) in SharedNeighbors
    void Reset(void);                                                                   // Starts Algorithms Over
    double CalculateLineExposure(double X1, double Y1, double X2, double Y2);           // Calculates the Exposure of a Line
    double CalculatePathExposure(int* Path, int NumPath);                               // Calculates the Exposure of a Path
    double CalculatePathLength(int* Path, int NumPath);                                 // Calculates the Length of a Path
	
	// Lin's GPSR Functions
	void GPSR_Initialize(void); 
	int GPSR_UpdateRouting(void);
	void GPSR_UpdateCurrentNode(int Next);
	void GPSR_ShortestPath(void);

    // Overall Algorithm Functions
    int MinExposure_Update(void);                                         // Update Localized Algorithm
    void MinExposure_Initialize(void);                                    // Initialize Algorithm
    void MinExposure_Calculate(void);                                     // Calculate Minimal Exposure Path

    int MaxExposure_Update(void);                                         // Update Localized Algorithm
    void MaxExposure_Initialize(void);                                    // Initialize Algorithm
    void MaxExposure_Calculate(void);                                     // Calculate Maximal Exposure Path

    // Localized Maximal Exposure Path Functions
    int MaxExposure_UpdateCurrentNode(int Next);                          // Updates Current Node Information
    int MaxExposure_UpdateHeuristic(void);                                // Calculates Algorithm Using Heuristic Method
    int MaxExposure_UpdateGPSR(void);                                     // Updates Algorithm Using GPSR Method
    int MaxExposure_CalculateDone(void);                                  // Calculates if Algorithm Is Done
    int MaxExposure_CalculatePotentialArea(int Index, int TempValue);     // Calcualtes Potential Area
    int MaxExposure_CalculateHoleCreation(int SensorIndex);               // Calculates Whether or not a Hole was created
    int MaxExposure_CalculateIndex(int Index);                            // Calculates Index of Sensor
    void MaxExposure_CalculateLevels(void);                               // Calculates Border Levels
    void MaxExposure_ErasePotentialArea(int Index, int TempValue);        // Erases Potential Area (Marked By CalculatePotentialArea() ).
    void MaxExposure_UpdateLevels(void);                                  // Updates Levels of Grid
    void MaxExposure_CalculateGrid(int GH, int GV);                       // Calculates Ellipse-Based Grid
    double MaxExposure_CalculateHeuristic(int Area, int Hops, int Index); // Calculates Heuristic Value

    // Centralized Maximal Exposure Path Functions
    int MaxExposure_GridDijkstraSearch(int SH, int SV, int EH, int EV, int* Path);                      // Performs a Dijkstra Search from S to E on the Grid
    int MaxExposure_AdjustPath(int* Path, int NumPath, int HighIndex, double AdjustedMaxLength);        // This Function Adjusts the Path (INSERT, REMOVE, MOVE)
    int MaxExposure_AdjustPath_Insert(int* Path, int NumPath, int HighIndex, double AdjustedMaxLength); // This Function Adjusts the Path by Inserting a Node
    int MaxExposure_AdjustPath_Remove(int* Path, int NumPath, int HighIndex, double AdjustedMaxLength); // This Function Adjusts the Path by Removing a Node
    int MaxExposure_AdjustPath_Move(int* Path, int NumPath, int HighIndex, double AdjustedMaxLength);   // This Function Adjusts the Path by Moving a Node
    double MaxExposure_UpdateRandomPath(void);                                                          // Calcualtes Maximal Exposure Path using Random Path
    double MaxExposure_UpdateShortestPath(void);                                                        // Calcualtes Maximal Exposure Path using Shortest Path
    double MaxExposure_UpdateBestPoint(void);                                                           // Calcualtes Maximal Exposure Path using Best Point
    double MaxExposure_UpdateAdjustedBestPoint(void);                                                   // Calcualtes Maximal Exposure Path using Adjusted Best Path

    // Localized Minimal Exposure Path Funcionts
    int MinExposure_UpdateMessageSearch(void);                                                                                    // Searches for new areas
    int MinExposure_UpdateMessageForward(void);                                                                                   // Forwards a message to a better path
    int MinExposure_UpdateMessageCheck(void);                                                                                     // Checks to see if any potentially better paths exist
    void MinExposure_UpdateCurrentNode(int Next);                                                                                 // Sets All Nodes in Voronoi Cell to Covered and Updates Current Node
    void MinExposure_CalculateGrid(int GH, int GV);                                                                               // Calculates a Grid
    double MinExposure_CalculateMinimalSensorExposure(int Sensor, double X1, double Y1, double X2, double Y2, int ValidityCheck); // Calculates the Single-Sensor Minimal Exposure
    double MinExposure_CalculatePotentialExposure(double X1, double Y1, double X2, double Y2);                                    // Calculates the best possible exposure for a sensor

    // Centralized Minmal Exposure Path Functions
    int MinExposure_GridDijkstraSearch(int SH, int SV, int EH, int EV, int* Path); // Performs a Dijkstra Search from S to E on the Grid

  public:
    GVExposureSensorNetwork2D();                                                   // Creates Empty Sensor Network
    ~GVExposureSensorNetwork2D();                                                  // Deletes Exposure Sensor Network
    void New(int NS, double R);                                                    // Creates New Sensor Network
    void Delete();                                                                 // Deletes Exposure Sensor Network
    void OutputResults(char* FileName);                                            // Outputs Results

    int MinExposure_Run(void);                                                     // Runs Maximal Exposure Path
    int MaxExposure_Run(void);                                                     // Runs Minimal Exposure Path

    void CalculateGridExposure(void);                                              // Calculates Exposure For Grid Points

    void SetAlgorithmMode(int AM);                                                 // Sets Mode for Algorithm (Centralized/Localized)
    void SetCenMinExposureMode(int MESM);                                          // Sets Mode for Centralized Minimal Exposure Algorithm
    void SetCenMaxExposureMode(int MESM);                                          // Sets Mode for Centralized Maximal Exposure Algorithm
    void SetLocMinExposureMode(int MESM);                                          // Sets Mode for Localized Minimal Exposure Algorithm
    void SetLocMaxExposureMode(int MESM);                                          // Sets Mode for Localized Maximal Exposure Algorithm
    void SetGridParameters(int GH, int GV);                                        // Sets Grid Size
    void SetPathParameters(double SX, double SY, double EX, double EY);            // Sets Path Parameters For Minimal Exposure path
    void SetPathParameters(double SX, double SY, double EX, double EY, double ML); // Sets Path Parameters For Minimal Exposure path
	void SetDetectionScheme(int DS);

    int GetNumPathPoints(int PathNum);                   // Gets the Number of Points for a Path (CENMIN, CENMAX, LOCMIN, LOCMAX);
    int GetMessageMode(void);                            // Returns the Message Mode
    int GetSensorKnown(int Index);                       // Returns the Known Value of a Sensor
    int GetSensorCurrent(void);                          // Returns the Current Sensor
    int GetSensorNext(void);                             // Returns the Next Sensor
    int GetGridSizeH(void);                              // Returns the Horizontal Size of the Grid
    int GetGridSizeV(void);                              // Returns the Vertical Size of the Grid
    int GetGridCovered(int i, int j);                    // Returns if the Grid Square is Covered
    double GetPathPointX(int PathNum, int PointNum);     // Gets the X Coordinate of a Point in a Path
    double GetPathPointY(int PathNum, int PointNum);     // Gets the Y Coordinate of a Point in a Path
    double GetGridPointX(int i, int j, int PointNumber); // Returns the X Coordinate of a Grid Square Point
    double GetGridPointY(int i, int j, int PointNumber); // Returns the Y Coordinate of a Grid Square Point
    double GetStartX(void);                              // Returns the Start Point X Coordinate
    double GetStartY(void);                              // Returns the Start Point Y Coordinate
    double GetEndX(void);                                // Returns the End Point X Coordinate
    double GetEndY(void);                                // Returns the End Point Y Coordinate
    double GetGridNormalizedExposureValue(int i, int j); // Returns the Normalized Exposure Value of a Grid Point

	// Lin's GPSR definition
	int GPSR_Run(void);
	void GPSR_OutputResults(char* FileName);			// Output GPSR Results
	double GetTotalEnergy(void);
	double GetTransmitEnergy(void);
	double GetReceiveEnergy(void);
	double GetMonitorEnergy(void);
	double GetProcessEnergy(void);
	void DetectEvent(int NodeIndex);
	void GenerateEvent(int NV);
	void DetectionInitialize(void);
	int GetShortestPathLength(void);
	int GetGPSRPathLength(void);
}; // GVExposureSensorNetwork2D



#endif