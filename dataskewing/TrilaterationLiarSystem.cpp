////////////////////////////////////////////////////////////////////////////////
// TrilaterationLiarSystem.cpp
// Implementation file for TrilaterationLiarSystem class.
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "TrilaterationLiarSystem.h"



// TrilaterationLiarSystem /////////////////////////////////////////////////////
TrilaterationLiarSystem::TrilaterationLiarSystem()
{ // TrilaterationLairSystem
  TrilaterationSystem();
  Init();
} // TrilaterationLairSystem




TrilaterationLiarSystem::~TrilaterationLiarSystem()
{ // ~TrilaterationLiarSystem
  Deinit();
} // ~TrilaterationLiarSystem



void TrilaterationLiarSystem::Init(void)
{ // Init
  Deinit();
  TrilaterationSystem::Init();
  PushData = 1;
  TargetPointX = 0.0;
  TargetPointY = 0.0;
  TargetPointZ = 0.0;
  LiarResult_ReportedX = -TRILATERATIONSYSTEM_INFINITY;
  LiarResult_ReportedY = -TRILATERATIONSYSTEM_INFINITY;
  UseMonteCarlo = 0;
  MonteCarloNum = 10000;
  BestPotentialColumn = 0;
  BestPotentialRow = 0;
  BestPotentialValue = 0.0;
  LiarState = LIAR_SYSTEM_INIT;
} // Init



void TrilaterationLiarSystem::Init(int NS, int NI, int GX, int GY, double M, double STD)
{ // Init
  Deinit();
  TrilaterationSystem::Init(NS, NI, GX, GY, M, STD);
  PushData = 1;
  TargetPointX = 0.0;
  TargetPointY = 0.0;
  TargetPointZ = 0.0;
  LiarResult_ReportedX = -TRILATERATIONSYSTEM_INFINITY;
  LiarResult_ReportedY = -TRILATERATIONSYSTEM_INFINITY;
  UseMonteCarlo = 0;
  MonteCarloNum = 10000;
  BestPotentialColumn = 0;
  BestPotentialRow = 0;
  BestPotentialValue = 0.0;
  LiarState = LIAR_SYSTEM_INIT;
} // Init



void TrilaterationLiarSystem::Deinit(void)
{ // Deinit
  TrilaterationSystem::Deinit();
  PushData =0;
  UseMonteCarlo = 0;
  MonteCarloNum = 0;
  TargetPointX = 0.0;
  TargetPointY = 0.0;
  TargetPointZ = 0.0;
  LiarResult_ReportedX = 0.0;
  LiarResult_ReportedY = 0.0;
  BestPotentialColumn = 0;
  BestPotentialRow = 0;
  BestPotentialValue = 0.0;
  LiarState = LIAR_SYSTEM_INIT;
} // Deinit



void TrilaterationLiarSystem::Run(void)
{ // Run
  while(LiarState != LIAR_SYSTEM_COMPLETE)
  {
    Update();
  }
  LiarState = LIAR_SYSTEM_INIT;
} // Run



void TrilaterationLiarSystem::Update(void)
{ // Update
  int i = 0;                     // Index Variable
  double X = 0.0;                // X Coordinate For Monte Carlo Simulation
  double Y = 0.0;                // Y Coordinate For Monte Carlo Simulation

  switch(LiarState)
  {
    case LIAR_SYSTEM_INIT:
      LiarSystemInit();
      if (UseMonteCarlo)
      {
        memcpy(&(Sensors[0]), &RandomPoint, sizeof(TrilaterationPoint));
      }      
      LiarState = LIAR_SYSTEM_TRILATERATION_SYSTEM_UPDATE;
    break;

    case LIAR_SYSTEM_TRILATERATION_SYSTEM_UPDATE:
      TrilaterationSystem::Update();
      if (State == SYSTEM_COMPLETE)
      {
        LiarState = LIAR_SYSTEM_TRILATERATION_LIAR_SYSTEM_INIT;
      }
    break;
  
    case LIAR_SYSTEM_TRILATERATION_LIAR_SYSTEM_INIT:
      LiarState = LIAR_SYSTEM_ZOOM_INITIAL_AREA_INIT;
    break;

    case LIAR_SYSTEM_ZOOM_INITIAL_AREA_INIT:
      if (NumSensors <= 3) SystemZoomInitialAreaInit(PushData);
      else SystemZoomInitialAreaInit(PushData);

      LiarState = LIAR_DRAW_ZOOM_INITIAL_AREA;
    break;

    case LIAR_DRAW_ZOOM_INITIAL_AREA:
      LiarState = LIAR_SYSTEM_GRID_INIT;
    break;

    case LIAR_SYSTEM_GRID_INIT:
      SystemGridInit();
      LiarState = LIAR_DRAW_GRID;
    break;

    case LIAR_DRAW_GRID:
      LiarState = LIAR_SYSTEM_SEARCH_INIT;
    break;

    case LIAR_SYSTEM_SEARCH_INIT:
      LiarSystemSearchInit();
      LiarState = LIAR_SYSTEM_SEARCH_UPDATE;
    break;

    case LIAR_SYSTEM_SEARCH_UPDATE:
      if (LiarSystemSearchUpdate()) LiarState = LIAR_SYSTEM_ZOOM_AREA_INIT;
    break;

    case LIAR_SYSTEM_ZOOM_AREA_INIT:
      LiarState = LIAR_DRAW_ZOOM_AREA;
    break;

    case LIAR_DRAW_ZOOM_AREA:
      LiarState = LIAR_SYSTEM_CHECK_DONE;
    break;

    case LIAR_SYSTEM_CHECK_DONE:
      if (SystemCheckDone()) LiarState = LIAR_SYSTEM_SET_RESULTS;
      else LiarState = LIAR_SYSTEM_GRID_INIT;
    break;

    case LIAR_SYSTEM_SET_RESULTS:
      LiarSystemSetResults();
      TheGrid->Deinit();
      LiarState = LIAR_SYSTEM_ZOOM_END_INIT;
    break;
    
    case LIAR_SYSTEM_ZOOM_END_INIT:
      SystemZoomProblemInit();
      LiarState = LIAR_DRAW_ZOOM_END;
    break;

    case LIAR_DRAW_ZOOM_END:
      // Restore The Original Point
      memcpy(&(Sensors[0]), &InitialPoint, sizeof(TrilaterationPoint));

      LiarState = LIAR_SYSTEM_COMPLETE;
    break;

    case LIAR_SYSTEM_COMPLETE:
    break;
  }
} // Update



void TrilaterationLiarSystem::LiarSystemInit(void)
{ // LiarSystemInit
  CurrentIteration = 0;
  LiarResult_ReportedX = -TRILATERATIONSYSTEM_INFINITY;
  LiarResult_ReportedY = -TRILATERATIONSYSTEM_INFINITY;
} // LiarSystemInit



void TrilaterationLiarSystem::LiarSystemSearchInit(void)
{ // LiarSearchInit
  CurrentColumn = 0;
  CurrentRow = 0;
  BestColumn = 0;
  BestRow = 0;
  BestValue = -TRILATERATIONSYSTEM_INFINITY;
  BestPotentialColumn = 0;
  BestPotentialRow = 0;
  BestPotentialValue = -TRILATERATIONSYSTEM_INFINITY;
  TargetPointX = Result_ReportedX;
  TargetPointY = Result_ReportedY;
  TargetPointZ = 0.0;
} // LiarSearchInit



int TrilaterationLiarSystem::LiarSystemSearchUpdate(void)
{ // LiarSearchUpdate
  int ReturnVal = 0;                                               // Return Value
  int ValueGood = 1;                                               // Is This Square Valid (All Errors < Tolerance)
  int i = 0;                                                       // Index Variable
  double CX = TheGrid->GetCenterPointX(CurrentColumn, CurrentRow); // X Center Point of Grid Square
  double CY = TheGrid->GetCenterPointY(CurrentColumn, CurrentRow); // Y Center Point of Grid Square
  double SensorX = 0.0;                                            // X Center Point Of Sensor
  double SensorY = 0.0;                                            // Y Center Point Of Sensor
  double Displacement = -TRILATERATIONSYSTEM_INFINITY;             // Displacement From Actual Solution
  double SensorErrors[TRILATERATIONSYSTEM_NUMSENSORS];             // Distance To Sensors
  // double MaxError = -TRILATERATIONSYSTEM_INFINITY;              // Maximum Radial Error

  // Get All Distances To Sensors
  for(i = 0; i < TRILATERATIONSYSTEM_NUMSENSORS - 1; ++i)
  {
    SensorX = Sensors[i].GetPointX();
    SensorY = Sensors[i].GetPointY();
    SensorErrors[i] = Distance(SensorX, SensorY, 0.0, CX, CY, 0.0) - Sensors[i].GetMeasuredRadius();
    if (SensorErrors[i] < 0.0) SensorErrors[i] = -SensorErrors[i];
    if (SensorErrors[i] > Sensors[i].GetMeasuredRadius() * MeasurementError->GetStandardDeviation())
    {
      ValueGood = 0;
    }
  }

  // Get Distance To Calculated Result
  Displacement = Distance(CX, CY, 0.0, TargetPointX, TargetPointY, TargetPointZ);
  
  // Get Heuristic Value
  if ((ValueGood) && (Displacement > BestValue))
  {
    BestValue = Displacement;
    BestColumn = CurrentColumn;
    BestRow = CurrentRow;
  }

  if (BestValue > 0.0)
  {
    CurrentLX = TheGrid->GetLX(BestColumn, BestRow);
    CurrentLY = TheGrid->GetLY(BestColumn, BestRow);
    CurrentUX = TheGrid->GetUX(BestColumn, BestRow);
    CurrentUY = TheGrid->GetUY(BestColumn, BestRow);
  }
  ++CurrentColumn;
  if (CurrentColumn >= GridX)
  {
    if (CurrentRow >= GridY - 1)
    {
      ReturnVal = 1;
    }
    else
    {
      CurrentColumn = 0;
      ++CurrentRow;
    }
  }
  LiarResult_ReportedX = (CurrentLX + CurrentUX) / 2;
  LiarResult_ReportedY = (CurrentLY + CurrentUY) / 2;
  return(ReturnVal);
} // LiarSearchUpdate



void TrilaterationLiarSystem::LiarSystemSetResults(void)
{ // LairSetResults
  LiarResult_ReportedX = (CurrentLX + CurrentUX) / 2;
  LiarResult_ReportedY = (CurrentLY + CurrentUY) / 2;
  LiarResult_Error = Distance(LiarResult_ReportedX, LiarResult_ReportedY, 0.0, Result_ActualX, Result_ActualY, 0.0);
} // LairSetResults



double TrilaterationLiarSystem::GetLiarReportedX(void)
{ // LiarGetReportedX
  return(LiarResult_ReportedX);
} // LiarGetReportedX



double TrilaterationLiarSystem::GetLiarReportedY(void)
{ // LiarGetReportedY
  return(LiarResult_ReportedY);
} // LiarGetReportedY



double TrilaterationLiarSystem::GetLiarError(void)
{ // LiarGetError
  return(LiarResult_Error);
} // LiarGetError



int TrilaterationLiarSystem::IsFinished(void)
{ // IsFinished
  int Finished = 0;

  if (LiarState == LIAR_SYSTEM_COMPLETE)
  {
    Finished = 1;
  }
  return(Finished);
} // IsFinished



int TrilaterationLiarSystem::IsBreak(void)
{ // IsLiarBreak
  int ReturnVal = 0;

  if ((LiarState == LIAR_SYSTEM_INIT) || (LiarState == LIAR_SYSTEM_TRILATERATION_SYSTEM_UPDATE))
  {
    ReturnVal = TrilaterationSystem::IsBreak();
  }
  else if ((LiarState == LIAR_SYSTEM_ZOOM_INITIAL_AREA_INIT) || (LiarState == LIAR_SYSTEM_GRID_INIT) || (LiarState == LIAR_SYSTEM_SEARCH_INIT) || (LiarState == LIAR_SYSTEM_ZOOM_AREA_INIT) || (LiarState == LIAR_SYSTEM_ZOOM_END_INIT) || (LiarState == LIAR_SYSTEM_COMPLETE))
  {
    ReturnVal = 1;
  }

  return(ReturnVal);
} // IsLiarBreak



void TrilaterationLiarSystem::OutputResults(void)
{ // LiarOutputResults
  int i = 0;        // Index Variable
  ofstream OutFile; // Output File
  double DM;        // Measured Difference
  double DL;        // Liar Difference
 
  DM = Distance(Result_ActualX, Result_ActualY, 0.0, Result_ReportedX, Result_ReportedY, 0.0);
  DL = Distance(Result_ActualX, Result_ActualY, 0.0, LiarResult_ReportedX, LiarResult_ReportedY, 0.0);
  OutFile.open("Output.txt", ios::app);
  OutFile   << Result_ActualX                 << "     "
            << Result_ActualY                 << "     "
            <<                                   "     "
            << Result_ReportedX               << "     "
            << Result_ReportedY               << "     "
            << DM                             << "     "
            <<                                   "     "
            << LiarResult_ReportedX           << "     "
            << LiarResult_ReportedY           << "     "
            << DL                             << "     "
            <<                                   "     "
            << DL / DM                        << "     "
            <<                                   "     ";
  for (i = 0; i < TRILATERATIONSYSTEM_NUMSENSORS; ++i)
  {
    OutFile << Sensors[i].GetPointX()         << "     "
            << Sensors[i].GetPointY()         << "     "
            << Sensors[i].GetActualRadius()   << "     "
            << Sensors[i].GetMeasuredRadius() << "     "
            <<                                   "     ";
  }
  OutFile   << endl;
  OutFile.close();
} // LiarOutputResults



TrilaterationLiarSystemStates TrilaterationLiarSystem::GetLiarState(void)
{ // GetLiarState
  return(LiarState);
} // GetLiarState



void TrilaterationLiarSystem::Reset(void)
{ // Reset
  TrilaterationSystem::Reset();
  LiarSystemInit();
  LiarState = LIAR_SYSTEM_INIT;
} // Reset



void TrilaterationLiarSystem::SetSkewMode(int PD)
{ // SetSkewMode
  if (PD) PushData = 1;
  else PushData = 0;
} // SetSkewMode



void TrilaterationLiarSystem::RunMonteCarloSimulation(int MonteCarloSimulations, double MonteCarloPercentage, int MonteCarloSteps)
{ // RunMonteCarloSimulation
  int i = 0;
  int RandomPoint = 0;
  int MonteCarloValid = 0;
  int MonteCarloTotal = ((int) (MonteCarloSimulations * MonteCarloPercentage));
  double OriginalLastX = 0.0;
  double OriginalLastY = 0.0;
  double OriginalLastZ = 0.0;
  double OriginalMeasuredRadius1 = 0.0;
  double OriginalMeasuredRadius2 = 0.0;
  double OriginalActualRadius1 = 0.0;
  double OriginalActualRadius2 = 0.0;
  double OriginalActualX = 0.0;
  double OriginalActualY = 0.0;
  double OriginalSkewedX = 0.0;
  double OriginalSkewedY = 0.0;
  double OriginalX = 0.0;
  double OriginalY = 0.0;
  double MinRadius = 0.0;
  double MaxRadius = 0.0;
  double CurrentRadius = 0.0;
  double StepRadius = 0.0;
  double SensorDistance = 0.0;
  double IntersectionPoints[4] = { 0.0 };
  double NewIntersectionPoints[4] = { 0.0 };
  double IntersectionDistances[4] = { 0.0 };
  double SensorErrors[3] = { 0.0 };
  double SkewedRadius = 0.0;
  double SkewedDistance = 0.0;
  double MinIntersectionDistance = 0.0;

  // Monte Carlo Only Used In 3-Sensor Network - Initialize Network
  SetNumSensors(3);
  ActualPointInitRandom();
  SensorInitRandom(0);
  SensorInitRandom(1);
  SensorInitRandom(2);
  Sensors[0].SetMeasuredPoint(ActualPoint->GetX(), ActualPoint->GetY(), ActualPoint->GetZ());
  Sensors[1].SetMeasuredPoint(ActualPoint->GetX(), ActualPoint->GetY(), ActualPoint->GetZ());
  Sensors[2].SetMeasuredPoint(ActualPoint->GetX(), ActualPoint->GetY(), ActualPoint->GetZ());
  OriginalLastX = Sensors[1].GetPointX();
  OriginalLastY = Sensors[1].GetPointY();
  OriginalLastZ = Sensors[1].GetPointZ();
  OriginalMeasuredRadius1 = Sensors[1].GetMeasuredRadius();
  OriginalActualRadius1 = Sensors[1].GetActualRadius();
  OriginalMeasuredRadius2 = Sensors[2].GetMeasuredRadius();
  OriginalActualRadius2 = Sensors[1].GetActualRadius();
  SkewedRadius = OriginalMeasuredRadius2;

  // Calculate The Original Solution
  TrilaterationSystem::Run();
  OriginalX = Result_ReportedX;
  OriginalY = Result_ReportedY;
  OriginalActualX = Result_ActualX;
  OriginalActualY = Result_ActualY;

  // Calculate The Two Potential Solutions
  IntersectCircle(Sensors[0].GetPointX(), Sensors[0].GetPointY(), Sensors[0].GetActualRadius(), Sensors[2].GetPointX(), Sensors[2].GetPointY(), Sensors[2].GetActualRadius(), IntersectionPoints);

  // Calculate Bounds Of Reported Radius
  SensorDistance = Distance(Sensors[0].GetPointX(), Sensors[0].GetPointY(), Sensors[0].GetPointZ(), Sensors[2].GetPointX(), Sensors[2].GetPointY(), Sensors[2].GetPointZ());
  MinRadius = SensorDistance - Sensors[0].GetMeasuredRadius();
  MaxRadius = SensorDistance + Sensors[0].GetMeasuredRadius();
  if (MinRadius < 0.0) MinRadius = 0.0;
  if (MaxRadius < MinRadius) MaxRadius = MinRadius;
  StepRadius = (MaxRadius - MinRadius) / ((double) MonteCarloSteps);

  for (CurrentRadius = MinRadius; CurrentRadius <= MaxRadius; CurrentRadius += StepRadius)
  {
    for (i = 0; i < MonteCarloSimulations; ++i)
    {
      // Pick Random Intersection Point
      RandomPoint = rand() % 2;
      // Pick Random Point And Radius Through Intersection Point
      SensorInitRandom(1);
      Sensors[1].SetMeasuredPoint(IntersectionPoints[RandomPoint << 1], IntersectionPoints[(RandomPoint << 1) + 1], 0.0);
      // Set The Radius Of The Attacker
      Sensors[2].SetRadius(OriginalMeasuredRadius2, CurrentRadius);
      // Run The Algorithm
      TrilaterationSystem::Run();
      // Calculate Errors
      SensorErrors[0] = fabs(Sensors[0].GetMeasuredRadius() - Distance(Sensors[0].GetPointX(), Sensors[0].GetPointY(), Sensors[0].GetPointZ(), Result_ReportedX, Result_ReportedY, 0.0));
      SensorErrors[1] = fabs(Sensors[1].GetMeasuredRadius() - Distance(Sensors[1].GetPointX(), Sensors[1].GetPointY(), Sensors[1].GetPointZ(), Result_ReportedX, Result_ReportedY, 0.0));
      SensorErrors[2] = fabs(Sensors[2].GetMeasuredRadius() - Distance(Sensors[2].GetPointX(), Sensors[2].GetPointY(), Sensors[2].GetPointZ(), Result_ReportedX, Result_ReportedY, 0.0));
      if ((SensorErrors[2] <= SensorErrors[0]) || (SensorErrors[2] <= SensorErrors[1]))
      {
        ++MonteCarloValid;
      }
    }
    if (MonteCarloValid >= MonteCarloTotal)
    {
      // Get New Intersection Points
      IntersectCircle(Sensors[0].GetPointX(), Sensors[0].GetPointY(), Sensors[0].GetActualRadius(), Sensors[2].GetPointX(), Sensors[2].GetPointY(), Sensors[2].GetActualRadius(), NewIntersectionPoints);
      IntersectionDistances[0] = Distance(IntersectionPoints[0], IntersectionPoints[1], 0.0, NewIntersectionPoints[0], NewIntersectionPoints[1], 0.0);
      IntersectionDistances[1] = Distance(IntersectionPoints[0], IntersectionPoints[1], 0.0, NewIntersectionPoints[2], NewIntersectionPoints[3], 0.0);
      IntersectionDistances[2] = Distance(IntersectionPoints[2], IntersectionPoints[3], 0.0, NewIntersectionPoints[0], NewIntersectionPoints[1], 0.0);
      IntersectionDistances[3] = Distance(IntersectionPoints[2], IntersectionPoints[3], 0.0, NewIntersectionPoints[2], NewIntersectionPoints[3], 0.0);
      // Calculate Minimum Distance Between The Two Sets Of Points
      MinIntersectionDistance = IntersectionDistances[0];
      for (i = 1; i < 4; ++i)
      {
        if(IntersectionDistances[i] < MinIntersectionDistance) MinIntersectionDistance = IntersectionDistances[i];
      }
      // If Minimum Offset Is The Largest Found So Far, Store It
      if (MinIntersectionDistance > SkewedDistance)
      {
        SkewedDistance = MinIntersectionDistance;
        SkewedRadius = CurrentRadius;
      }
    }
  }
  // Calculate Final Solution
  Sensors[1].Init(OriginalLastX, OriginalLastY, OriginalLastZ, MeasurementError);
  Sensors[1].SetRadius(OriginalActualRadius1, OriginalMeasuredRadius1);
  Sensors[2].SetRadius(OriginalActualRadius2, SkewedRadius);
  TrilaterationSystem::Run();
  OriginalSkewedX = Result_ReportedX;
  OriginalSkewedY = Result_ReportedY;
  // Store the Results
  Result_ReportedX = OriginalX;
  Result_ReportedY = OriginalY;
  Result_ActualX = OriginalActualX;
  Result_ActualY = OriginalActualY;
  LiarResult_ReportedX = OriginalSkewedX;
  LiarResult_ReportedY = OriginalSkewedY;
  Result_Error = Distance(Result_ReportedX, Result_ReportedY, 0.0, Result_ActualX, Result_ActualY, 0.0);
  LiarResult_Error = Distance(LiarResult_ReportedX, LiarResult_ReportedY, 0.0, Result_ActualX, Result_ActualY, 0.0);
} // RunMonteCarloSimulation
