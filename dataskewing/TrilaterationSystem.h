////////////////////////////////////////////////////////////////////////////////
// TrilaterationSystem.h
// Header file for TrilaterationSystem class.
////////////////////////////////////////////////////////////////////////////////



#ifndef TRILATERATIONSYSTEM_H
#define TRILATERATIONSYSTEM_H



// Includes ////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <math.h>
#include "stdafx.h"
#include <mmsystem.h>
#include "Point.h"
#include "TrilaterationPoint.h"
#include "Grid.h"
#include "ErrorModules.h"



// Defines /////////////////////////////////////////////////////////////////////
#define TRILATERATIONSYSTEM_PI             3.1415926535897
#define TRILATERATIONSYSTEM_INFINITY       1000000000000000.0
#define TRILATERATIONSYSTEM_NUMITERATIONS  3
#define TRILATERATIONSYSTEM_NUMSENSORS     3
#define TRILATERATIONSYSTEM_NUMSYSTEMTIMES 1000
#define TRILATERATIONSYSTEM_LX             -1.0
#define TRILATERATIONSYSTEM_LY             -1.0
#define TRILATERATIONSYSTEM_LZ             0.0
#define TRILATERATIONSYSTEM_UX             1.0
#define TRILATERATIONSYSTEM_UY             1.0
#define TRILATERATIONSYSTEM_UZ             0.0
#define TRILATERATIONSYSTEM_DX             (TRILATERATIONSYSTEM_UX - TRILATERATIONSYSTEM_LX)
#define TRILATERATIONSYSTEM_DY             (TRILATERATIONSYSTEM_UY - TRILATERATIONSYSTEM_LY)
#define TRILATERATIONSYSTEM_DZ             (TRILATERATIONSYSTEM_UZ - TRILATERATIONSYSTEM_LZ)
#define TRILATERATIONSYSTEM_ACCURACY       10000
#define TRILATERATIONSYSTEM_MEAN           0.0
#define TRILATERATIONSYSTEM_STD            0.1
#define TRILATERATIONSYSTEM_EPSILON        0.000001
#define TRILATERATIONSYSTEM_GRIDX          25
#define TRILATERATIONSYSTEM_GRIDY          25
#define TRILATERATIONSYSTEM_STD_MULT       1.0

/*
enum TrilaterationSystemStates
{
  SYSTEM_INIT,
  SENSOR_INIT,
  SENSOR_UPDATE,
  ZOOM_TO_PROBLEM_INIT,
  ZOOM_TO_PROBLEM_UPDATE,
  ZOOM_TO_AREA_INIT,
  ZOOM_TO_AREA_UPDATE,
  GRID_INIT,
  GRID_UPDATE,
  SEARCH_INIT,
  SEARCH_UPDATE,
  ZOOM_INIT,
  ZOOM_UPDATE,
  SET_RESULTS,
  COMPLETE
};
*/
enum TrilaterationSystemStates
{
  SYSTEM_INIT,
  DRAW_SENSOR,
  SYSTEM_ZOOM_PROBLEM_INIT,
  DRAW_ZOOM_PROBLEM,
  SYSTEM_ZOOM_INITIAL_AREA_INIT,
  DRAW_ZOOM_INITIAL_AREA,
  SYSTEM_GRID_INIT,
  DRAW_GRID,
  SYSTEM_SEARCH_INIT,
  SYSTEM_SEARCH_UPDATE,
  SYSTEM_ZOOM_AREA_INIT,
  DRAW_ZOOM_AREA,
  SYSTEM_CHECK_DONE,
  SYSTEM_ZOOM_END_INIT,
  DRAW_ZOOM_END,
  SYSTEM_SET_RESULTS,
  SYSTEM_COMPLETE
};



// TrilaterationSystem /////////////////////////////////////////////////////////
class TrilaterationSystem
{ // TrilaterationSystem
  public:
    // Constructors And Destructors
    TrilaterationSystem();
    ~TrilaterationSystem();

    // Member Functions
    int  IsFinished(void);
    int  IsBreak(void);
    void Init(void);
    void Init(int NS, int NI, int GX, int GY, double M, double STD);
    void Deinit(void);
    void Run(void);
    void Update(void);
    void Reset(void);
    // void InitializeCamera(double HalfAngle);
    // void InitializeSensors(unsigned long CurrentTime);
    // void SetTime(unsigned long CurrentTime);
    // void SetUpdateMode(int UpdateMode);

    // To Do:
    void SetNumSensors(int NS);
    int GetNumSensors(void);

    void SetNumIterations(int NI);
    int GetNumIterations(void);

    void SetMean(double M);
    double GetMean(void);

    void SetStandardDeviation(double STD);
    double GetStandardDeviation(void);

    void SetGridX(int X);
    void SetGridY(int Y);

    void MeasurementInit(void);

    void SetMonteCarloMode(int MCMode);

    double GetBoundsLX(void);
    double GetBoundsLY(void);
    double GetBoundsUX(void);
    double GetBoundsUY(void);
    double GetCurrentLX(void);
    double GetCurrentLY(void);
    double GetCurrentUX(void);
    double GetCurrentUY(void);

    TrilaterationSystemStates GetState(void);
    /* int GetNumSensors(void); */
    int GetGridX(void);
    int GetGridY(void);
    int GetCurrentColumn(void);
    int GetCurrentRow(void);
    double GetGridSquareLX(int Column, int Row);
    double GetGridSquareLY(int Column, int Row);
    double GetGridSquareUX(int Column, int Row);
    double GetGridSquareUY(int Column, int Row);
    // double GetGridSquareOffsetX(int Column, int Row);
    // double GetGridSquareOffsetY(int Column, int Row);

    double GetMeasuredX(void);
    double GetMeasuredY(void);
    double GetMeasuredError(void);
    double GetActualX(void);
    double GetActualY(void);
    double GetError(void);
    double GetSensorRadius(int SensorNum);
    double GetSensorActualRadius(int SensorNum);
    double GetSensorX(int SensorNum);
    double GetSensorY(int SensorNum);
    double GetSensorZ(int SensorNum);
    // double GetSensorRed(int SensorNum);
    // double GetSensorGreen(int SensorNum);
    // double GetSensorBlue(int SensorNum);
    double GetSensorProbability(int SensorNum, double Value);
    // double GetCameraX(void);
    // double GetCameraY(void);
    // double GetCameraZ(void);
    // double GetStandardDeviation(void);
    // double GetGridRed(void);
    // double GetGridGreen(void);
    // double GetGridBlue(void);
    // double GetGridRedHighlight(void);
    // double GetGridGreenHighlight(void);
    // double GetGridBlueHighlight(void);
    // double GetOffsetX(void);
    // double GetOffsetY(void);
    // double GetScaleX(void);
    // double GetScaleY(void);
    // double GetDrawActualX(void);
    // double GetDrawActualY(void);
    // double GetDrawMeasuredX(void);
    // double GetDrawMeasuredY(void);
    void ActualPointInitRandom(void);
    void ActualPointInit(double X, double Y, double Z);
    void SensorInitRandom(int SensorNum);
    void SensorInit(int SensorNum, double X, double Y, double Z);

  protected:
    // Added - Sort Later
    int UseMonteCarlo;                       // Use Monte-Carlo Guess For First Sensor
    int MonteCarloNum;                       // Number Of Runs For A Monte Carlo Simulation
    int NumIterations;
    int NumSensors;
    double Mean;
    double StandardDeviation;
    int GridX;
    int GridY;
    char* OutputFileName;

    int CurrentIteration;            // Current Iteration Through Grid Algorithm
    int CurrentColumn;               // Current Column (X) In Grid
    int CurrentRow;                  // Current Row (Y) In Grid
    int BestColumn;                  // Best Column To Pick For Next Iteration Of Algorithm
    int BestRow;                     // Best Row To Pick For Next Iteration Of Algorithm
    unsigned long LastTime;          // Last Updated Time
    double BestValue;                // Value For Best (Column, Row)
    /* Get Rid Of */ double ZoomAreaThreshold;        // What Should The Area Of The Zoomed Portion Be Before Stopping?
    double CurrentLX;                // Current Lower X-Coordinate Of Searching
    double CurrentLY;                // Current Lower Y-Coordinate Of Searching
    double CurrentUX;                // Current Upper X-Coordinate Of Searching
    double CurrentUY;                // Current Upper Y-Coordinate Of Searching
    double Result_ReportedX;         // Reported X-Value (in [-0.5, 0.5] Range)
    double Result_ReportedY;         // Reported Y-Value (in [-0.5, 0.5] Range)
    double Result_ActualX;           // Actual X-Value (in [-0.5, 0.5] Range)
    double Result_ActualY;           // Actual Y-Value (in [-0.5, 0.5] Range)
    double Result_Error;             // Distance Between Reported And Actual
    Grid* TheGrid;                   // Grid Used To Estimate Trilateration
    Point* ActualPoint;              // Actual Point To Trilaterate
    ErrorModule* MeasurementError;   // Error In Measurements
    TrilaterationPoint RandomPoint;  // Random Monte Carlo Point
    TrilaterationPoint InitialPoint; // Original Point (Non-Monte Carlo)
    TrilaterationPoint* Sensors;     // Sensors By Which To Trilaterate Position
    TrilaterationSystemStates State; // State In Which The System Is In

    // Display Variables
    int SkipUpdate;                  // Skip Updating Stages?
    int ZoomTime;                    // Number Of Milliseconds A Zoom Should Take
    unsigned long ZoomStart;         // Start Of Zoom
    double CameraX;                  // Camera X-Coordinate
    double CameraY;                  // Camera X-Coordinate
    double CameraZ;                  // Camera X-Coordinate
    double InitialX;                 // Initial X-Coordinate
    double InitialY;                 // Initial Y-Coordinate
    double InitialZ;                 // Initial Z-Coordinate
    double FinalX;                   // Final X-Coordinate (After Zoom)
    double FinalY;                   // Final Y-Coordinate (After Zoom)
    double FinalZ;                   // Final Z-Coordinate (After All Zooms)
    double SensorUnitsPerSecond;     // How Fast A Sensor Radius Expands
    double GridUnitsPerSecond;       // How Fast A Grid Square Moves
    double OffsetX;                  // How The Scene Is Offset To The Camera
    double OffsetY;                  // How The Scene Is Offset To The Camera
    double ScaleX;                   // How The Scene Is Scaled To The Camera
    double ScaleY;                   // How The Scene Is Scaled To The Camera
    double DrawActualX;              // Draw X Coordinate
    double DrawActualY;              // Draw Y Coordinate
    double DrawMeasuredX;            // Draw X Coordinate
    double DrawMeasuredY;            // Draw Y Coordinate

    void SystemInit(void);
    
    
    //void SensorUpdate(unsigned long CurrentTime);
    void SystemGridInit(void);
    //void GridUpdate(unsigned long CurrentTime);
    int  SystemCheckDone(void);
    void SystemSearchInit(void);
    int  SystemSearchUpdate(void);
    void SystemZoomProblemInit(void);
    //void ZoomInit(unsigned long CurrentTime);
    //int  ZoomUpdate(unsigned long CurrentTime);
    void SystemZoomInitialAreaOverspecified(void);
    void SystemZoomInitialAreaUnderspecified(int UseIncorrectIntersection);
    void SystemSetResults(void);
    void IntersectCircle(double X1, double Y1, double R1, double X2, double Y2, double R2, double Points[4]);
    void SystemZoomInitialAreaInit(int UseIncorrectIntersection);
    double GenerateRandomDouble(double LowVal, double HighVal);
    double SumOfSquaresSquareRoot(double* Values, int NumValues);
    double Distance(double X1, double Y1, double Z1, double X2, double Y2, double Z2);
}; // TrilaterationSystem



#endif
