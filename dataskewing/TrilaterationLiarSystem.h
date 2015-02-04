////////////////////////////////////////////////////////////////////////////////
// TrilaterationLiarSystem.h
// Header file for TrilaterationLiarSystem class.
////////////////////////////////////////////////////////////////////////////////



#ifndef TRILATERATIONLIARSYSTEM_H
#define TRILATERATIONLIARSYSTEM_H



// Includes ////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <iostream.h>
#include <fstream.h>
#include <mmsystem.h>
#include "TrilaterationSystem.h"



// Defines /////////////////////////////////////////////////////////////////////
enum TrilaterationLiarSystemStates
{
  LIAR_SYSTEM_INIT,
  LIAR_SYSTEM_TRILATERATION_SYSTEM_UPDATE,
  LIAR_SYSTEM_TRILATERATION_LIAR_SYSTEM_INIT,
  LIAR_SYSTEM_ZOOM_INITIAL_AREA_INIT,
  LIAR_DRAW_ZOOM_INITIAL_AREA,
  LIAR_SYSTEM_GRID_INIT,
  LIAR_DRAW_GRID,
  LIAR_SYSTEM_SEARCH_INIT,
  LIAR_SYSTEM_SEARCH_UPDATE,
  LIAR_SYSTEM_ZOOM_AREA_INIT,
  LIAR_DRAW_ZOOM_AREA,
  LIAR_SYSTEM_CHECK_DONE,
  LIAR_SYSTEM_ZOOM_END_INIT,
  LIAR_DRAW_ZOOM_END,
  LIAR_SYSTEM_SET_RESULTS,
  LIAR_SYSTEM_COMPLETE
};


// TrilaterationSystem /////////////////////////////////////////////////////////
class TrilaterationLiarSystem : public TrilaterationSystem
{ // TrilaterationLiarSystem
  public:
    // Constructors And Destructors
    TrilaterationLiarSystem();
    ~TrilaterationLiarSystem();

    // Member Functions
    int IsFinished(void);
    int IsBreak(void);
    void Run(void);
    void RunMonteCarloSimulation(int MonteCarloSimulations, double MonteCarloPercentage, int MonteCarloSteps);
    void Update(void);
    void Init(void);
    void Init(int NS, int NI, int GX, int GY, double M, double STD);
    void Deinit(void);
    void Reset(void);
    void OutputResults(void);
    void SetSkewMode(int PD);
    double GetLiarReportedX(void);
    double GetLiarReportedY(void);
    double GetLiarError(void);
    TrilaterationLiarSystemStates GetLiarState(void);

  protected:
    int PushData;                            // Push Data Towards Other Solutions?
    int BestPotentialColumn;                 // Best Potential Grid Column (But Doesn't Satisfy Detection Condition)
    int BestPotentialRow;                    // Best Potential Grid Row (But Doesn't Satisfy Detection Condition)
    double TargetPointX;                     // X Coordinate Of Point To Stay Away From
    double TargetPointY;                     // Y Coordinate Of Point To Stay Away From
    double TargetPointZ;                     // Z Coordinate Of Point To Stay Away From
    double BestPotentialValue;               // Best Potential Value
    double LiarResult_ReportedX;             // Reported X Coordinate By Liar
    double LiarResult_ReportedY;             // Reported Y Coordinate By Liar
    double LiarResult_Error;                 // Error As A Result Of Lying
    TrilaterationPoint TempSensor;           // Temporary Sensor For Storing Lied Result
    TrilaterationLiarSystemStates LiarState; // Liar State
    void LiarSystemInit(void);
    void LiarSystemSearchInit(void);
    int  LiarSystemSearchUpdate(void);
    void LiarSystemSetResults(void);
}; // TrilaterationLiarSystem



#endif