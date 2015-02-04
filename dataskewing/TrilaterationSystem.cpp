////////////////////////////////////////////////////////////////////////////////
// TrilaterationSystem.h
// Header file for TrilaterationSystem class.
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "TrilaterationSystem.h"



// TrilaterationSystem /////////////////////////////////////////////////////////
TrilaterationSystem::TrilaterationSystem()
/*
  This is the main constructor for the trilateration system.
*/
{ // TrilaterationSystem
  TheGrid = NULL;
  ActualPoint = NULL;
  MeasurementError = NULL;
  Sensors = NULL;
  Init();
} // TrilaterationSystem



TrilaterationSystem::~TrilaterationSystem()
/*
  This is the main destructor for the trilateration system.
*/
{ // ~TrilaterationSystem
  Deinit();
} // ~TrilaterationSystem



void TrilaterationSystem::Init()
/*
  This function initializes the trilateration system with default values.
*/
{ // Init
  // Remove Any Previous Initializations
  Deinit();

  // Set System Properties
  NumIterations = TRILATERATIONSYSTEM_NUMITERATIONS;
  NumSensors = TRILATERATIONSYSTEM_NUMSENSORS;
  GridX = TRILATERATIONSYSTEM_GRIDX;
  GridY = TRILATERATIONSYSTEM_GRIDY;
  State = SYSTEM_INIT;

  // Set Dynamic Data
  TheGrid = new Grid();
  ActualPoint = new Point();
  Sensors = new TrilaterationPoint[NumSensors];
  MeasurementError = new GaussianUniformErrorModule();
  MeasurementError->Init(TRILATERATIONSYSTEM_MEAN, TRILATERATIONSYSTEM_STD);

  // Set Iteration Properties 
  CurrentIteration = 0;
  CurrentColumn = 0;
  CurrentRow = 0;
  BestColumn = 0;
  BestRow = 0;
  BestValue = 0.0;
  CurrentLX = 0.0;
  CurrentLY = 0.0;
  CurrentUX = 0.0;
  CurrentUY = 0.0;

  // Set Results
  Result_ActualX = 0.0;
  Result_ActualY = 0.0;
  Result_ReportedX = 0.0;
  Result_ReportedY = 0.0;
  Result_Error = 0.0;

  // Set Time
  LastTime = 0;
} // Init



void TrilaterationSystem::Init(int NS, int NI, int GX, int GY, double M, double STD)
/*
  This function initializes the trilateration system with specified values for the
  number of iterations, the number of sensors, the grid granularity in the x and y,
  and the mean and standard deviation for the error in measurement.
*/
{ // Init
  // Remove Any Previous Initializations
  Deinit();

  // Set System Properties
  SetNumIterations(NI);
  SetNumSensors(NS);
  SetGridX(GX);
  SetGridY(GY);
  State = SYSTEM_INIT;

  // Set Dynamic Data
  TheGrid = new Grid();
  ActualPoint = new Point();
  Sensors = new TrilaterationPoint[NumSensors];
  MeasurementError = new GaussianUniformErrorModule();
  MeasurementError->Init(M, STD);

  // Set Iteration Properties 
  CurrentIteration = 0;
  CurrentColumn = 0;
  CurrentRow = 0;
  BestColumn = 0;
  BestRow = 0;
  BestValue = 0.0;
  CurrentLX = 0.0;
  CurrentLY = 0.0;
  CurrentUX = 0.0;
  CurrentUY = 0.0;

  // Set Results
  Result_ActualX = 0.0;
  Result_ActualY = 0.0;
  Result_ReportedX = 0.0;
  Result_ReportedY = 0.0;
  Result_Error = 0.0;

  // Set Time
  LastTime = 0;
} // Init



void TrilaterationSystem::Deinit()
{ // Deinit
  // Delete Dynamic Data
  if (TheGrid != NULL) delete TheGrid;
  if (ActualPoint != NULL) delete ActualPoint;
  if (Sensors != NULL) delete[] Sensors;
  if (MeasurementError != NULL) delete MeasurementError;

  // Set System Properties
  NumIterations = 0;
  NumSensors = 0;
  GridX = 0;
  GridY = 0;
  State = SYSTEM_INIT;

  // Set Dynamic Data
  TheGrid = NULL;
  ActualPoint = NULL;
  Sensors = NULL;
  MeasurementError = NULL;

  // Set Iteration Properties 
  CurrentIteration = 0;
  CurrentColumn = 0;
  CurrentRow = 0;
  BestColumn = 0;
  BestRow = 0;
  BestValue = 0.0;
  CurrentLX = 0.0;
  CurrentLY = 0.0;
  CurrentUX = 0.0;
  CurrentUY = 0.0;

  // Set Results
  Result_ActualX = 0.0;
  Result_ActualY = 0.0;
  Result_ReportedX = 0.0;
  Result_ReportedY = 0.0;
  Result_Error = 0.0;

  // Set Time
  LastTime = 0;
} // Deinit



void TrilaterationSystem::MeasurementInit(void)
{ // MeasurementInit
  int i = 0; // Index Variable

  // Make Sensors Aware Of Specified Point
  for (i = 0; i < NumSensors; ++i)
  {
    Sensors[i].SetMeasuredPoint(ActualPoint->GetX(), ActualPoint->GetY(), ActualPoint->GetZ());
  }
} // MeasurementInit



void TrilaterationSystem::SystemInit(void)
{ // SystemInit
  int i = 0;
  double X = 0.0;
  double Y = 0.0;

  // Clear The Iteration Count
  CurrentIteration = 0;
  Result_ReportedX = -TRILATERATIONSYSTEM_INFINITY;
  Result_ReportedY = -TRILATERATIONSYSTEM_INFINITY;
  Result_ActualX = ActualPoint->GetX();
  Result_ActualY = ActualPoint->GetY();

  // Store Original Sensor
  memcpy(&InitialPoint, &(Sensors[0]), sizeof(TrilaterationPoint));

  // Initialize Coordinates
  X = 0.0;
  Y = 0.0;
  for (i = 0; i < MonteCarloNum; ++i)
  {
    X += GenerateRandomDouble(TRILATERATIONSYSTEM_LX, TRILATERATIONSYSTEM_UX);
    Y += GenerateRandomDouble(TRILATERATIONSYSTEM_LY, TRILATERATIONSYSTEM_UY);
  }
  RandomPoint.Init(X / ((double) MonteCarloNum), Y / ((double) MonteCarloNum), 0.0, MeasurementError);
  RandomPoint.SetMeasuredPoint(ActualPoint->GetX(), ActualPoint->GetY(), ActualPoint->GetZ());
} // SystemInit



void TrilaterationSystem::ActualPointInitRandom(void)
{ // ActualPointInitRandom
  ActualPoint->Init(GenerateRandomDouble(TRILATERATIONSYSTEM_LX, TRILATERATIONSYSTEM_UX), GenerateRandomDouble(TRILATERATIONSYSTEM_LY, TRILATERATIONSYSTEM_UY), 0.0);
} // ActualPointInitRandom



void TrilaterationSystem::SensorInitRandom(int SensorNum)
{ // SensorInitRandom
  Sensors[SensorNum].Init(GenerateRandomDouble(TRILATERATIONSYSTEM_LX, TRILATERATIONSYSTEM_UX), GenerateRandomDouble(TRILATERATIONSYSTEM_LY, TRILATERATIONSYSTEM_UY), 0.0, MeasurementError);
} // SensorInitRandom



void TrilaterationSystem::ActualPointInit(double X, double Y, double Z)
{ // ActualPointInit
  if (X < TRILATERATIONSYSTEM_LX) X = TRILATERATIONSYSTEM_LX;
  if (Y < TRILATERATIONSYSTEM_LY) Y = TRILATERATIONSYSTEM_LY;
  if (Z < TRILATERATIONSYSTEM_LZ) Z = TRILATERATIONSYSTEM_LZ;
  if (X > TRILATERATIONSYSTEM_UX) X = TRILATERATIONSYSTEM_UX;
  if (Y > TRILATERATIONSYSTEM_UY) Y = TRILATERATIONSYSTEM_UY;
  if (Z > TRILATERATIONSYSTEM_UZ) Z = TRILATERATIONSYSTEM_UZ;
  ActualPoint->Init(X, Y, Z);
} // ActualPointInit



void TrilaterationSystem::SensorInit(int SensorNum, double X, double Y, double Z)
{ // ActualPointInit
  if (SensorNum < NumSensors) 
  {
    if (X < TRILATERATIONSYSTEM_LX) X = TRILATERATIONSYSTEM_LX;
    if (Y < TRILATERATIONSYSTEM_LY) Y = TRILATERATIONSYSTEM_LY;
    if (Z < TRILATERATIONSYSTEM_LZ) Z = TRILATERATIONSYSTEM_LZ;
    if (X > TRILATERATIONSYSTEM_UX) X = TRILATERATIONSYSTEM_UX;
    if (Y > TRILATERATIONSYSTEM_UY) Y = TRILATERATIONSYSTEM_UY;
    if (Z > TRILATERATIONSYSTEM_UZ) Z = TRILATERATIONSYSTEM_UZ;
    Sensors[SensorNum].Init(X, Y, Z, MeasurementError);
  }
} // ActualPointInit



/*
void TrilaterationSystem::SensorUpdate(unsigned long CurrentTime)
{ // SensorUpdate
  double TimeOffset = ((double) (CurrentTime - LastTime)) / 1000.0;
  int i = 0;

  for (i = 0; i < TRILATERATIONSYSTEM_NUMSENSORS; ++i)
  {
    if (Sensors[i].GetOffset() < 0.0)
    {
      Sensors[i].SetOffset(Sensors[i].GetOffset() + TimeOffset * SensorUnitsPerSecond);
    }
    if (Sensors[i].GetOffset() > 0.0)
    {
      Sensors[i].SetOffset(0.0);
    }
  }
} // SensorUpdate
*/



void TrilaterationSystem::SystemGridInit(void)
{ // GridInit
  TheGrid->Init(GridX, GridY, CurrentLX, CurrentLY, CurrentUX, CurrentUY);
} // GridInit



/*
void TrilaterationSystem::GridUpdate(unsigned long CurrentTime)
{ // GridUpdate
  double TimeOffset = ((double) (CurrentTime - LastTime)) / 1000.0;
  double OffsetX = 0.0;
  double OffsetY = 0.0;
  double FinalOffsetX = (GridUnitsPerSecond / sqrt(ScaleX*ScaleX + ScaleY*ScaleY)) * TimeOffset;
  double FinalOffsetY = (GridUnitsPerSecond / sqrt(ScaleX*ScaleX + ScaleY*ScaleY)) * TimeOffset;
  int i = 0;
  int j = 0;

  for (i = 0; i < TRILATERATIONSYSTEM_GRIDX; ++i)
  {
    for (j = 0; j < TRILATERATIONSYSTEM_GRIDY; ++j)
    {
      OffsetX = TheGrid->GetOffsetX(i, j);
      OffsetY = TheGrid->GetOffsetY(i, j);
      TheGrid->SetOffset(i, j, OffsetX + FinalOffsetX, OffsetY + FinalOffsetY);
    }
  }
} // GridUpdate
*/


void TrilaterationSystem::SystemSearchInit(void)
{ // SearchInit
  CurrentColumn = 0;
  CurrentRow = 0;
  BestColumn = 0;
  BestRow = 0;
  BestValue = TRILATERATIONSYSTEM_INFINITY;
} // SearchInit



int TrilaterationSystem::SystemSearchUpdate(void)
{ // SearchUpdate
  int ReturnVal = 0;                                               // Return Value
  int ValueGood = 1;                                               // Is This Square Valid (All Errors < Tolerance)
  int i = 0;                                                       // Index Variable
  double CX = TheGrid->GetCenterPointX(CurrentColumn, CurrentRow); // X Center Point of Grid Square
  double CY = TheGrid->GetCenterPointY(CurrentColumn, CurrentRow); // Y Center Point of Grid Square
  double SensorX = 0.0;                                            // X Center Point Of Sensor
  double SensorY = 0.0;                                            // Y Center Point Of Sensor
  double* SensorErrors;                                            // Distance To Sensors
  double TestValue = TRILATERATIONSYSTEM_INFINITY;                 // Value To Check For Best

  SensorErrors = new double[NumSensors];

  // Get All Distances To Sensors
  for(i = 0; i < NumSensors; ++i)
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
  // Get Heuristic Value
  TestValue = SumOfSquaresSquareRoot(SensorErrors, NumSensors);
  if ((ValueGood) && (TestValue < BestValue))
  {
    BestColumn = CurrentColumn;
    BestRow = CurrentRow;
    BestValue = TestValue;
  }
  CurrentLX = TheGrid->GetLX(BestColumn, BestRow);
  CurrentLY = TheGrid->GetLY(BestColumn, BestRow);
  CurrentUX = TheGrid->GetUX(BestColumn, BestRow);
  CurrentUY = TheGrid->GetUY(BestColumn, BestRow);

  // Go To Next Grid Square (Or Next State If Done With Grid Squares)
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
  Result_ReportedX = (CurrentLX + CurrentUX) / 2;
  Result_ReportedY = (CurrentLY + CurrentUY) / 2;
  delete[] SensorErrors;
  return(ReturnVal);
} // SearchUpdate



void TrilaterationSystem::SystemZoomProblemInit(void)
{ // ZoomToProblemInit
  int i = 0;                 // Index Variable
  double Temp;               // Temp Variable
  double* X;                 // Possible Minimum and Maximum X Values
  double* Y;                 // Possible Minimum and Maximum Y Values
  double StandardDeviation = MeasurementError->GetStandardDeviation();

  X = new double[NumSensors * 2];
  Y = new double[NumSensors * 2];

  // Get Possible Min/Max Points
  for (i = 0; i < NumSensors; ++i)
  {
    X[i * 2] = Sensors[i].GetPointX() + Sensors[i].GetMeasuredRadius() + StandardDeviation * TRILATERATIONSYSTEM_STD_MULT;
    X[i * 2 + 1] = Sensors[i].GetPointX() - Sensors[i].GetMeasuredRadius() - StandardDeviation * TRILATERATIONSYSTEM_STD_MULT;
    Y[i * 2] = Sensors[i].GetPointY() + Sensors[i].GetMeasuredRadius() + StandardDeviation * TRILATERATIONSYSTEM_STD_MULT;
    Y[i * 2 + 1] = Sensors[i].GetPointY() - Sensors[i].GetMeasuredRadius() - StandardDeviation * TRILATERATIONSYSTEM_STD_MULT;
  }
  // Find Min/Max Points
  CurrentLX = X[0];
  CurrentLY = Y[0];
  CurrentUX = X[0];
  CurrentUY = Y[0];
  for (i = 1; i < NumSensors * 2; ++i)
  {
    if (X[i] < CurrentLX) CurrentLX = X[i];
    if (X[i] > CurrentUX) CurrentUX = X[i];
    if (Y[i] < CurrentLY) CurrentLY = Y[i];
    if (Y[i] > CurrentUY) CurrentUY = Y[i];
  }
  // Make Sure Zoomed Area Is Square
  if (CurrentUX - CurrentLX > CurrentUY - CurrentLY)
  {
    Temp = ((CurrentUX - CurrentLX) - (CurrentUY - CurrentLY)) / 2.0;
    CurrentLY -= Temp;
    CurrentUY += Temp;
  }
  else
  {
    Temp = ((CurrentUY - CurrentLY) - (CurrentUX - CurrentLX)) / 2.0;
    CurrentLX -= Temp;
    CurrentUX += Temp;
  }

  delete[] X;
  delete[] Y;
} // ZoomToProblemInit


/*
void TrilaterationSystem::ZoomInit(unsigned long CurrentTime)
{ // ZoomInit
  double Width = ((double) TRILATERATIONSYSTEM_DX) / ((double) TRILATERATIONSYSTEM_GRIDX);
  double Height = ((double) TRILATERATIONSYSTEM_DY) / ((double) TRILATERATIONSYSTEM_GRIDY);
  InitializeCamera(20.0);
  CurrentLX = TheGrid->GetLX(BestColumn, BestRow);
  CurrentLY = TheGrid->GetLY(BestColumn, BestRow);
  CurrentUX = TheGrid->GetUX(BestColumn, BestRow);
  CurrentUY = TheGrid->GetUY(BestColumn, BestRow);
  FinalX = TRILATERATIONSYSTEM_LX + Width * BestColumn + Width / 2;
  FinalY = TRILATERATIONSYSTEM_LY + Height * BestRow + Height / 2;

  // FinalX = ((CurrentLX + CurrentUX) / 2) / ScaleX - OffsetX;
  // FinalY = ((CurrentLY + CurrentUY) / 2) / ScaleY - OffsetY;

  // FinalX = ((CurrentLX + CurrentUX) / 2);
  // FinalY = ((CurrentLY + CurrentUY) / 2);
  ZoomStart = CurrentTime;
} // ZoomInit
*/

/*
int TrilaterationSystem::ZoomUpdate(unsigned long CurrentTime)
{ // ZoomUpdate
  int ReturnVal = 0;
  double u = (((double) CurrentTime) - ((double) ZoomStart)) / ((double) ZoomTime);
  double t = 1.0 - ((1.0 - u) * (1.0 - u));

  if (u > 1.0)
  {
    ReturnVal = 1;
    t = 1.0;
    u = 1.0;
  }
  
  CameraX = InitialX + t * (FinalX - InitialX);
  CameraY = InitialY + t * (FinalY - InitialY);
  CameraZ = InitialZ + t * (FinalZ - InitialZ);

  return(ReturnVal);
} // ZoomUpdate
*/


void TrilaterationSystem::SystemSetResults(void)
{ // SetResults
  Result_ReportedX = (CurrentLX + CurrentUX) / 2;
  Result_ReportedY = (CurrentLY + CurrentUY) / 2;
  Result_ActualX = ActualPoint->GetX();
  Result_ActualY = ActualPoint->GetY();
  Result_Error = Distance(Result_ReportedX, Result_ReportedY, 0.0, Result_ActualX, Result_ActualY, 0.0);
} // SetResults



void TrilaterationSystem::Run(void)
{ // Run
  while(State != SYSTEM_COMPLETE)
  {
    Update();
  }
  State = SYSTEM_INIT;
} // Run



/*
void TrilaterationSystem::Update(unsigned long CurrentTime)
{ // Update
  int i = 0;             // Index Variable
  int j = 0;             // Index Variable
  int NumColumns = 0;    // Number Of Columns in Grid
  int NumRows = 0;       // Number Of Rows In Grid
  int GoToNextState = 0; // Flag To Determine If Switching States (For Update States)
  double Offset = 0.0;   // Offset For Sensors And Grid Squares

  switch(State)
  {
    case SYSTEM_INIT:
      SystemInit();
      State = SENSOR_INIT;
      LastTime = CurrentTime;
    break;

    case SENSOR_INIT:
      SensorInit();
      State = SENSOR_UPDATE;
      LastTime = CurrentTime;
    break;

    case SENSOR_UPDATE:
      SensorUpdate(CurrentTime);
      // Check For Next State
      for (i = 0, GoToNextState = 1; (GoToNextState) && (i < TRILATERATIONSYSTEM_NUMSENSORS); ++i)
      {
        Offset = Sensors[i].GetOffset();
        // if ((Offset <= TRILATERATIONSYSTEM_EPSILON) && (Offset >= -TRILATERATIONSYSTEM_EPSILON))
        if (Offset >= 0.0)
        {
          Sensors[i].SetOffset(0.0);
        }
        else
        {
          GoToNextState = 0;
        }
      }
      // if (GoToNextState) State = GRID_INIT;
      if (SkipUpdate) State = GRID_INIT;
      else if (GoToNextState) State = ZOOM_TO_PROBLEM_INIT;
      LastTime = CurrentTime;
    break;

    case ZOOM_TO_PROBLEM_INIT:
      ZoomToProblemInit(CurrentTime);
      State = ZOOM_TO_PROBLEM_UPDATE;
      LastTime = CurrentTime;
    break;

    case ZOOM_TO_PROBLEM_UPDATE:
      ZoomToProblemInit(0);
      if (ZoomUpdate(CurrentTime))
      {
        OffsetX = -((CurrentUX + CurrentLX) / 2);
        OffsetY = -((CurrentUY + CurrentLY) / 2);
        ScaleX  = ((double) TRILATERATIONSYSTEM_DX) / (CurrentUX - CurrentLX);
        ScaleY  = ((double) TRILATERATIONSYSTEM_DY) / (CurrentUY - CurrentLY);
        CameraX = InitialX;
        CameraY = InitialY;
        CameraZ = InitialZ;
        State = ZOOM_TO_AREA_INIT;
      }
      LastTime = CurrentTime;
    break;

    case ZOOM_TO_AREA_INIT:
      ZoomToAreaInit(CurrentTime);
      State = ZOOM_TO_AREA_UPDATE;
      LastTime = CurrentTime;
    break;

    case ZOOM_TO_AREA_UPDATE:
      ZoomToAreaInit(0);
      if (ZoomUpdate(CurrentTime))
      {
        OffsetX = -((CurrentUX + CurrentLX) / 2);
        OffsetY = -((CurrentUY + CurrentLY) / 2);
        ScaleX  = ((double) TRILATERATIONSYSTEM_DX) / (CurrentUX - CurrentLX);
        ScaleY  = ((double) TRILATERATIONSYSTEM_DY) / (CurrentUY - CurrentLY);
        CameraX = InitialX;
        CameraY = InitialY;
        CameraZ = InitialZ;
        State = GRID_INIT;
      }
      LastTime = CurrentTime;
    break;

    case GRID_INIT:
      GridInit();
      if (SkipUpdate) State = SEARCH_INIT;
      else State = GRID_UPDATE;
      LastTime = CurrentTime;
    break;

    case GRID_UPDATE:
      GridUpdate(CurrentTime);
      // Check For Next State
      NumRows = TheGrid->GetRows();
      NumColumns = TheGrid->GetColumns();
      GoToNextState = 1;
      for (i = 0; (GoToNextState) && (i < NumColumns); ++i)
      {
        for (j = 0; (GoToNextState) && (j < NumRows); ++j)
        {
          Offset = TheGrid->GetOffsetX(i, j);
          if ((Offset <= TRILATERATIONSYSTEM_EPSILON) && (Offset >= -TRILATERATIONSYSTEM_EPSILON))
          {
            TheGrid->SetOffset(i, j, 0.0, TheGrid->GetOffsetY(i, j));
          }
          else
          {
            GoToNextState = 0;
          }
          Offset = TheGrid->GetOffsetY(i, j);
          if ((Offset <= TRILATERATIONSYSTEM_EPSILON) && (Offset >= -TRILATERATIONSYSTEM_EPSILON))
          {
            TheGrid->SetOffset(i, j, TheGrid->GetOffsetX(i, j), 0.0);
          }
          else
          {
            GoToNextState = 0;
          }
        }
      }
      if (GoToNextState) State = SEARCH_INIT;
      LastTime = CurrentTime;
    break;

    case SEARCH_INIT:
      SearchInit();
      State = SEARCH_UPDATE;
      LastTime = CurrentTime;
    break;

    case SEARCH_UPDATE:
      SearchUpdate();
      // Go To Next Grid Square (Or Next State If Done With Grid Squares)
      ++CurrentColumn;
      if (CurrentColumn >= TRILATERATIONSYSTEM_GRIDX)
      {
        if (CurrentRow >= TRILATERATIONSYSTEM_GRIDY - 1)
        {
          State = ZOOM_INIT;
        }
        else
        {
          CurrentColumn = 0;
          ++CurrentRow;
        }
      }
      DrawMeasuredX = ((CurrentUX + CurrentLX) / 2);
      DrawMeasuredY = ((CurrentUY + CurrentLY) / 2);
      LastTime = CurrentTime;
    break;

    case ZOOM_INIT:
      ZoomInit(CurrentTime);
      // DrawMeasuredX = FinalX;
      // DrawMeasuredY = FinalY;
      DrawMeasuredX = ((CurrentUX + CurrentLX) / 2);
      DrawMeasuredY = ((CurrentUY + CurrentLY) / 2);
      State = ZOOM_UPDATE;
      LastTime = CurrentTime;
    break;

    case ZOOM_UPDATE:
      if (SkipUpdate || ZoomUpdate(CurrentTime))
      {
        OffsetX = -((CurrentUX + CurrentLX) / 2);
        OffsetY = -((CurrentUY + CurrentLY) / 2);
        ScaleX *= TRILATERATIONSYSTEM_GRIDX;
        ScaleY *= TRILATERATIONSYSTEM_GRIDY;
        CameraX = InitialX;
        CameraY = InitialY;
        CameraZ = InitialZ;
        if (((CurrentUX - CurrentLX) * (CurrentUY - CurrentLY)) <= ZoomAreaThreshold)
        {
          State = SET_RESULTS;
        }
        else
        {
          State = GRID_INIT;
        }
      }
      LastTime = CurrentTime;
    break;

    case SET_RESULTS:
      SetResults();
      State = COMPLETE;
      LastTime = CurrentTime;
    break;

    case COMPLETE:
      LastTime = CurrentTime;
    break;
  }
} // Update
*/



void TrilaterationSystem::Update(void)
{ // Update
  switch(State)
  {
    case SYSTEM_INIT:
      SystemInit();
      if (UseMonteCarlo)
      {
        memcpy(&(Sensors[0]), &RandomPoint, sizeof(TrilaterationPoint));
      }
      State = DRAW_SENSOR;
    break;

    case DRAW_SENSOR:
      State = SYSTEM_ZOOM_PROBLEM_INIT;
    break;

    case SYSTEM_ZOOM_PROBLEM_INIT:
      SystemZoomProblemInit();
      State = DRAW_ZOOM_PROBLEM;
    break;

    case DRAW_ZOOM_PROBLEM:
      State = SYSTEM_ZOOM_INITIAL_AREA_INIT;
    break;

    case SYSTEM_ZOOM_INITIAL_AREA_INIT:
      SystemZoomInitialAreaInit(0);
      State = DRAW_ZOOM_INITIAL_AREA;
    break;

    case DRAW_ZOOM_INITIAL_AREA:
      State = SYSTEM_GRID_INIT;
    break;

    case SYSTEM_GRID_INIT:
      SystemGridInit();
      State = DRAW_GRID;
    break;

    case DRAW_GRID:
      State = SYSTEM_SEARCH_INIT;
    break;

    case SYSTEM_SEARCH_INIT:
      SystemSearchInit();
      State = SYSTEM_SEARCH_UPDATE;
    break;

    case SYSTEM_SEARCH_UPDATE:
      if (SystemSearchUpdate()) State = SYSTEM_ZOOM_AREA_INIT;
    break;

    case SYSTEM_ZOOM_AREA_INIT:
      State = DRAW_ZOOM_AREA;
    break;

    case DRAW_ZOOM_AREA:
      State = SYSTEM_CHECK_DONE;
    break;

    case SYSTEM_CHECK_DONE:
      if (SystemCheckDone()) State = SYSTEM_SET_RESULTS;
      else State = SYSTEM_GRID_INIT;
    break;

    case SYSTEM_SET_RESULTS:
      SystemSetResults();
      TheGrid->Deinit();
      State = SYSTEM_ZOOM_END_INIT;
    break;

    case SYSTEM_ZOOM_END_INIT:
      SystemZoomProblemInit();
      State = DRAW_ZOOM_END;
    break;

    case DRAW_ZOOM_END:
      memcpy(&(Sensors[0]), &InitialPoint, sizeof(TrilaterationPoint));
      State = SYSTEM_COMPLETE;
    break;

    case SYSTEM_COMPLETE:
    break;
  }
} // Update





double TrilaterationSystem::GenerateRandomDouble(double LowVal, double HighVal)
{ // GenerateRandomDouble
  double Percentage = ((double) (rand() % (TRILATERATIONSYSTEM_ACCURACY + 1))) / ((double) TRILATERATIONSYSTEM_ACCURACY);
  return(LowVal + Percentage * (HighVal - LowVal));
} // GenerateRandomDouble



double TrilaterationSystem::SumOfSquaresSquareRoot(double* Values, int NumValues)
{ // SumOfSquares
  int i = 0;        // Index Variable
  double Sum = 0.0; // Sum Of Squares Of Values

  for (i = 0; i < NumValues; ++i)
  {
    Sum += Values[i] * Values[i];
  }
  return(sqrt(Sum));
} // SumOfSquares



double TrilaterationSystem::Distance(double X1, double Y1, double Z1, double X2, double Y2, double Z2)
{ // Distance
  double DX = X1 - X2;
  double DY = Y1 - Y2;
  double DZ = Z1 - Z2;
  return(sqrt(DX*DX + DY*DY + DZ*DZ));
} // Distance



double TrilaterationSystem::GetMeasuredX(void)
{ // GetReportedX
  return(Result_ReportedX);
} // GetReportedX



double TrilaterationSystem::GetMeasuredY(void)
{ // GetReportedY
  return(Result_ReportedY);
} // GetReportedY



double TrilaterationSystem::GetMeasuredError(void)
{ // GetMeasuredError
  return(Result_Error);
} // GetMeasuredError



double TrilaterationSystem::GetActualX(void)
{ // GetActualX
  return(Result_ActualX);
} // GetActualX



double TrilaterationSystem::GetActualY(void)
{ // GetActualY
  return(Result_ActualY);
} // GetActualY


double TrilaterationSystem::GetError(void)
{ // GetError
  return(Result_Error);
} // GetError


/*
void TrilaterationSystem::InitializeCamera(double HalfAngle)
{ // InitializeCamera
  int i = 0;                 // Index Variable
  double InitialWidth = 0.0; // Initial Width Of Viewing Area
  double FinalWidth = 0.0;   // Final Width Of Viewing Area

  InitialX = 0.0;
  InitialY = 0.0;
  InitialZ = 0.0;
  // FinalX = 0.0;
  // FinalY = 0.0;
  // FinalZ = 0.0;

  // Select Initial Width To Calculate Position From
  if (TRILATERATIONSYSTEM_DX > TRILATERATIONSYSTEM_DY)
  {
    InitialWidth = TRILATERATIONSYSTEM_DX / 2.0;
    // Calculate Final Width Of Viewing Area
    for (i = 0, FinalWidth = InitialWidth; i < TRILATERATIONSYSTEM_NUMITERATIONS; ++i)
    {
      FinalWidth /= TRILATERATIONSYSTEM_GRIDX;
    }
  }
  else
  {
    InitialWidth = TRILATERATIONSYSTEM_DY / 2.0;
    // Calculate Final Width Of Viewing Area
    for (i = 0, FinalWidth = InitialWidth; i < TRILATERATIONSYSTEM_NUMITERATIONS; ++i)
    {
      FinalWidth /= TRILATERATIONSYSTEM_GRIDY;
    }
  }
  
  InitialZ = InitialWidth / tan(HalfAngle * TRILATERATIONSYSTEM_PI / 180.0);
  FinalZ = (((double) InitialWidth) / ((double) TRILATERATIONSYSTEM_GRIDX)) / tan(HalfAngle * TRILATERATIONSYSTEM_PI / 180.0);
  // CameraX = 0.0;
  // CameraY = 0.0;
  // CameraZ = InitialZ;
} // InitializeCamera
*/


/*
int TrilaterationSystem::GetNumSensors(void)
{ // GetNumSensors
  return(TRILATERATIONSYSTEM_NUMSENSORS);
} // GetNumSensors
*/


double TrilaterationSystem::GetSensorRadius(int SensorNum)
{ // GetSensorRadius
  return(Sensors[SensorNum].GetMeasuredRadius() /* + Sensors[SensorNum].GetOffset() */);
} // GetSensorRadius



double TrilaterationSystem::GetSensorActualRadius(int SensorNum)
{ // GetSensorRadius
  return(Sensors[SensorNum].GetActualRadius() /* + Sensors[SensorNum].GetOffset() */);
} // GetSensorRadius



double TrilaterationSystem::GetSensorX(int SensorNum)
{ // GetSensorX
  return(Sensors[SensorNum].GetPointX());
} // GetSensorX



double TrilaterationSystem::GetSensorY(int SensorNum)
{ // GetSensorY
  return(Sensors[SensorNum].GetPointY());
} // GetSensorY



double TrilaterationSystem::GetSensorZ(int SensorNum)
{ // GetSensorZ
  return(Sensors[SensorNum].GetPointZ());
} // GetSensorZ


/*
double TrilaterationSystem::GetCameraX(void)
{ // GetCameraX
  return(CameraX);
} // GetCameraX



double TrilaterationSystem::GetCameraY(void)
{ // GetCameraY
  return(CameraY);
} // GetCameraY



double TrilaterationSystem::GetCameraZ(void)
{ // GetCameraZ
  return(CameraZ);
} // GetCameraZ
*/



double TrilaterationSystem::GetStandardDeviation(void)
{ // GetStandardDeviation
  return(MeasurementError->GetStandardDeviation());
} // GetStandardDeviation


/*
double TrilaterationSystem::GetSensorRed(int SensorNum)
{ // GetSensorRed
  return(Sensors[SensorNum].GetRed());
} // GetSensorRed



double TrilaterationSystem::GetSensorGreen(int SensorNum)
{ // GetSensorGreen
  return(Sensors[SensorNum].GetGreen());
} // GetSensorGreen



double TrilaterationSystem::GetSensorBlue(int SensorNum)
{ // GetSensorBlue
  return(Sensors[SensorNum].GetBlue());
} // GetSensorBlue
*/


double TrilaterationSystem::GetSensorProbability(int SensorNum, double Value)
{ // GetSensorProbability
  return(Sensors[SensorNum].GetProbability(Value));
} // GetSensorProbability


/*
void TrilaterationSystem::SetTime(unsigned long CurrentTime)
{ // SetTime
  LastTime = CurrentTime;
} // SetTime
*/


int TrilaterationSystem::GetGridX(void)
{ // GetGridX
  int ReturnVal = 0;

  if (TheGrid != NULL) ReturnVal = GridX;
  return(ReturnVal);
} // GetGridX



int TrilaterationSystem::GetGridY(void)
{ // GetGridY
  int ReturnVal = 0;

  if (TheGrid != NULL) ReturnVal = GridY;
  return(ReturnVal);
} // GetGridY



double TrilaterationSystem::GetGridSquareLX(int Column, int Row)
{ // GetGridSquareLX
  double ReturnVal = 0.0;

  if (TheGrid != NULL) ReturnVal = TheGrid->GetLX(Column, Row);
  return(ReturnVal);
} // GetGridSquareLX



double TrilaterationSystem::GetGridSquareLY(int Column, int Row)
{ // GetGridSquareLY
  double ReturnVal = 0.0;

  if (TheGrid != NULL) ReturnVal = TheGrid->GetLY(Column, Row);
  return(ReturnVal);
} // GetGridSquareLY



double TrilaterationSystem::GetGridSquareUX(int Column, int Row)
{ // GetGridSquareUX
  double ReturnVal = 0.0;

  if (TheGrid != NULL) ReturnVal = TheGrid->GetUX(Column, Row);
  return(ReturnVal);
} // GetGridSquareUX



double TrilaterationSystem::GetGridSquareUY(int Column, int Row)
{ // GetGridSquareUY
  double ReturnVal = 0.0;

  if (TheGrid != NULL) ReturnVal = TheGrid->GetUY(Column, Row);
  return(ReturnVal);
} // GetGridSquareUY


/*
double TrilaterationSystem::GetGridSquareOffsetX(int Column, int Row)
{ // GetGridSquareOffset
  double ReturnVal = 0.0;

  if (TheGrid != NULL) ReturnVal = TheGrid->GetOffsetX(Column, Row);
  return(ReturnVal);
} // GetGridSquareOffset



double TrilaterationSystem::GetGridSquareOffsetY(int Column, int Row)
{ // GetGridSquareOffset
  double ReturnVal = 0.0;

  if (TheGrid != NULL) ReturnVal = TheGrid->GetOffsetY(Column, Row);
  return(ReturnVal);
} // GetGridSquareOffset



double TrilaterationSystem::GetGridRed(void)
{ // GetGridRed
  return(TheGrid->GetRed());
} // GetGridRed



double TrilaterationSystem::GetGridGreen(void)
{ // GetGridGreen
  return(TheGrid->GetGreen());
} // GetGridGreen



double TrilaterationSystem::GetGridBlue(void)
{ // GetGridBlue
  return(TheGrid->GetBlue());
} // GetGridBlue



double TrilaterationSystem::GetGridRedHighlight(void)
{ // GetGridRedHighlight
  return(TheGrid->GetHighlightRed());
} // GetGridRedHighlight



double TrilaterationSystem::GetGridGreenHighlight(void)
{ // GetGridGreenHighlight
  return(TheGrid->GetHighlightGreen());
} // GetGridGreenHighlight



double TrilaterationSystem::GetGridBlueHighlight(void)
{ // GetGridBlueHighlight
  return(TheGrid->GetHighlightBlue());
} // GetGridBlueHighlight
*/


int TrilaterationSystem::GetCurrentColumn(void)
{ // GetCurrentColumn
  return(CurrentColumn);
} // GetCurrentColumn



int TrilaterationSystem::GetCurrentRow(void)
{ // GetCurrentRow
  return(CurrentRow);
} // GetCurrentRow


/*
double TrilaterationSystem::GetOffsetX(void)
{ // GetCameraZ
  return(OffsetX);
} // GetCameraZ



double TrilaterationSystem::GetOffsetY(void)
{ // GetCameraZ
  return(OffsetY);
} // GetCameraZ



double TrilaterationSystem::GetScaleX(void)
{ // GetCameraZ
  return(ScaleX);
} // GetCameraZ



double TrilaterationSystem::GetScaleY(void)
{ // GetCameraZ
  return(ScaleY);
} // GetCameraZ



double TrilaterationSystem::GetDrawActualX(void)
{ // GetDrawActualX
  return(DrawActualX);
} // GetDrawActualX



double TrilaterationSystem::GetDrawActualY(void)
{ // GetDrawActualY
  return(DrawActualY);
} // GetDrawActualY



double TrilaterationSystem::GetDrawMeasuredX(void)
{ // GetDrawMeasuredX
  return(DrawMeasuredX);
} // GetDrawMeasuredX



double TrilaterationSystem::GetDrawMeasuredY(void)
{ // GetDrawMeasuredY
  return(DrawMeasuredY);
} // GetDrawMeasuredY
*/
void TrilaterationSystem::SystemZoomInitialAreaInit(int UseIncorrectIntersection)
{ // SystemZoomInitialAreaInit
  if (NumSensors > 3) SystemZoomInitialAreaOverspecified();
  else SystemZoomInitialAreaUnderspecified(UseIncorrectIntersection);
} // SystemZoomInitialAreaInit



void TrilaterationSystem::SystemZoomInitialAreaUnderspecified(int UseIncorrectIntersection)
{ // SystemZoomInitialAreaInit
  int FirstSensor = 0;
  int SecondSensor = 1;
  int LastSensor = NumSensors - 1;
  double TargetX;
  double TargetY;
  double R1;         // Distance To First Point
  double R2;         // Distance To Second Point
  double DR1;        // Difference In Radius
  double DR2;        // Difference In Radius
  double Points[4];  // Intersection Points
  double StandardDeviation = MeasurementError->GetStandardDeviation();
  // double HalfAngle = 20.0;

  IntersectCircle(Sensors[FirstSensor].GetPointX(), Sensors[FirstSensor].GetPointY(), Sensors[FirstSensor].GetMeasuredRadius(), Sensors[SecondSensor].GetPointX(), Sensors[SecondSensor].GetPointY(), Sensors[SecondSensor].GetMeasuredRadius(), Points);
  // Below Is Cheating - If The Modified Radii Don't Intersect, Use The Actual
  if ((Points[0] < TRILATERATIONSYSTEM_LX) && (Points[1] < TRILATERATIONSYSTEM_LY) && (Points[2] < TRILATERATIONSYSTEM_UX) && (Points[3] < TRILATERATIONSYSTEM_UY))
  {
    IntersectCircle(Sensors[FirstSensor].GetPointX(), Sensors[FirstSensor].GetPointY(), Sensors[FirstSensor].GetActualRadius(), Sensors[SecondSensor].GetPointX(), Sensors[SecondSensor].GetPointY(), Sensors[SecondSensor].GetActualRadius(), Points);
  }
  R1 = Distance(Sensors[LastSensor].GetPointX(), Sensors[LastSensor].GetPointY(), 0.0, Points[0], Points[1], 0.0);
  R2 = Distance(Sensors[LastSensor].GetPointX(), Sensors[LastSensor].GetPointY(), 0.0, Points[2], Points[3], 0.0);
  DR1 = R1 - Sensors[LastSensor].GetMeasuredRadius();
  DR2 = R2 - Sensors[LastSensor].GetMeasuredRadius();
  if (DR1 < 0.0) DR1 = -DR1;
  if (DR2 < 0.0) DR2 = -DR2;
  if (((!UseIncorrectIntersection) && (DR1 < DR2)) || ((UseIncorrectIntersection) && (DR1 > DR2)))
  {
    TargetX = Points[0];
    TargetY = Points[1];
  }
  else
  {
    TargetX = Points[2];
    TargetY = Points[3];
  }
  CurrentLX = TargetX - TRILATERATIONSYSTEM_STD_MULT * StandardDeviation;
  CurrentLY = TargetY - TRILATERATIONSYSTEM_STD_MULT * StandardDeviation;
  CurrentUX = TargetX + TRILATERATIONSYSTEM_STD_MULT * StandardDeviation;
  CurrentUY = TargetY + TRILATERATIONSYSTEM_STD_MULT * StandardDeviation;

  // FinalX = (FinalX + OffsetX) * ScaleX;
  // FinalY = (FinalY + OffsetY) * ScaleX;
  // (TS->GetSensorX(i) + ZoomOffsetX) * ZoomScaleX;
  // FinalZ = (((double) (CurrentUX - CurrentLX)) / 2.0) / tan(HalfAngle * TRILATERATIONSYSTEM_PI / 180.0);
  // FinalZ = FinalZ * ScaleX;
  // if (CurrentTime > 0) ZoomStart = CurrentTime;
} // SystemZoomInitialAreaInit



void TrilaterationSystem::SystemZoomInitialAreaOverspecified(void)
{ // SystemZoomInitialAreaOverspecified
  int CurrentIndex = 0;   // Current Point Index
  int MinIndex = 0;       // Index Of Best Point
  int i = 0;              // Index Variable
  int j = 0;              // Index Variable
  int PointsPerPair = 18; // Number Of Points Generated Per Pair Of Circles
  int NumPoints = 0;      // Number of Points
  double Value = 0.0;     // Value By Which To Sort
  double Min = 0.0;       // Value of Best Point
  double Points[4];       // Intersection of Two Circles in Two Spots
  double* X = NULL;       // X-Coordinate of Intersection Points
  double* Y = NULL;       // Y-Coordinate of Intersection Points
  double StandardDeviation = MeasurementError->GetStandardDeviation();

  NumPoints = (NumSensors * (NumSensors - 1)) * 9;
  X = new double[NumPoints];
  Y = new double[NumPoints];
  CurrentIndex = 0;

  for (i = 0; i < NumSensors - 1; ++i)
  {
    for (j = i + 1; j < NumSensors; ++j)
    {
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), (1.0 + StandardDeviation) * Sensors[i].GetMeasuredRadius(), Sensors[j].GetPointX(), Sensors[j].GetPointY(), (1.0 + StandardDeviation) * Sensors[j].GetMeasuredRadius(), Points);
      X[CurrentIndex + 0] = Points[0];
      Y[CurrentIndex + 0] = Points[1];
      X[CurrentIndex + 1] = Points[2];
      Y[CurrentIndex + 1] = Points[3];
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(),                             Sensors[i].GetMeasuredRadius(), Sensors[j].GetPointX(), Sensors[j].GetPointY(), (1.0 + StandardDeviation) * Sensors[j].GetMeasuredRadius(), Points);
      X[CurrentIndex + 2] = Points[0];
      Y[CurrentIndex + 2] = Points[1];
      X[CurrentIndex + 3] = Points[2];
      Y[CurrentIndex + 3] = Points[3];
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), (1.0 - StandardDeviation) * Sensors[i].GetMeasuredRadius(), Sensors[j].GetPointX(), Sensors[j].GetPointY(), (1.0 + StandardDeviation) * Sensors[j].GetMeasuredRadius(), Points);
      X[CurrentIndex + 4] = Points[0];
      Y[CurrentIndex + 4] = Points[1];
      X[CurrentIndex + 5] = Points[2];
      Y[CurrentIndex + 5] = Points[3];
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), (1.0 + StandardDeviation) * Sensors[i].GetMeasuredRadius(), Sensors[j].GetPointX(), Sensors[j].GetPointY(),                             Sensors[j].GetMeasuredRadius(), Points);
      X[CurrentIndex + 6] = Points[0];
      Y[CurrentIndex + 6] = Points[1];
      X[CurrentIndex + 7] = Points[2];
      Y[CurrentIndex + 7] = Points[3];
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(),                             Sensors[i].GetMeasuredRadius(), Sensors[j].GetPointX(), Sensors[j].GetPointY(),                             Sensors[j].GetMeasuredRadius(), Points);
      X[CurrentIndex + 8] = Points[0];
      Y[CurrentIndex + 8] = Points[1];
      X[CurrentIndex + 9] = Points[2];
      Y[CurrentIndex + 9] = Points[3];
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), (1.0 - StandardDeviation) * Sensors[i].GetMeasuredRadius(), Sensors[j].GetPointX(), Sensors[j].GetPointY(),                             Sensors[j].GetMeasuredRadius(), Points);
      X[CurrentIndex + 10] = Points[0];
      Y[CurrentIndex + 10] = Points[1];
      X[CurrentIndex + 11] = Points[2];
      Y[CurrentIndex + 11] = Points[3];
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), (1.0 + StandardDeviation) * Sensors[i].GetMeasuredRadius(), Sensors[j].GetPointX(), Sensors[j].GetPointY(), (1.0 - StandardDeviation) * Sensors[j].GetMeasuredRadius(), Points);
      X[CurrentIndex + 12] = Points[0];
      Y[CurrentIndex + 12] = Points[1];
      X[CurrentIndex + 13] = Points[2];
      Y[CurrentIndex + 13] = Points[3];
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(),                             Sensors[i].GetMeasuredRadius(), Sensors[j].GetPointX(), Sensors[j].GetPointY(), (1.0 - StandardDeviation) * Sensors[j].GetMeasuredRadius(), Points);
      X[CurrentIndex + 14] = Points[0];
      Y[CurrentIndex + 14] = Points[1];
      X[CurrentIndex + 15] = Points[2];
      Y[CurrentIndex + 15] = Points[3];
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), (1.0 - StandardDeviation) * Sensors[i].GetMeasuredRadius(), Sensors[j].GetPointX(), Sensors[j].GetPointY(), (1.0 - StandardDeviation) * Sensors[j].GetMeasuredRadius(), Points);
      X[CurrentIndex + 16] = Points[0];
      Y[CurrentIndex + 16] = Points[1];
      X[CurrentIndex + 17] = Points[2];
      Y[CurrentIndex + 17] = Points[3];
    }
    CurrentIndex += PointsPerPair;
  }

  MinIndex = -1;
  Min = TRILATERATIONSYSTEM_INFINITY;
  for (i = 0; i < NumPoints; ++i)
  {
    Value = 0.0;
    if ((X[i] > -1000) && (Y[i] > -1000))
    {
      // Calculate Least Sum Of Squares Distance Of Differences From Radius.  Best Point Is Intersection
      for (j = 0; j < NumSensors; ++j)
      {
        Value += ((Distance(X[i], Y[i], 0.0, Sensors[j].GetPointX(), Sensors[j].GetPointY(), 0.0) - Sensors[j].GetMeasuredRadius()) * (Distance(X[i], Y[i], 0.0, Sensors[j].GetPointX(), Sensors[j].GetPointY(), 0.0) - Sensors[j].GetMeasuredRadius()));
      }
      Value = sqrt(Value);
      if (Value < Min)
      {
        Min = Value;
        MinIndex = i;
      }
    }
  }
  // Error Checking - Make Sure Minimum Index Exists
  if (MinIndex < 0)
  {
    i = 0;
    while ((MinIndex < 0) && (i < NumPoints))
    {
      if ((X[i] > -1000) && (Y[i] > -1000))
      {
        MinIndex = i;
      }
      ++i;
    }
  }
  // Error Checking - Set To (0, 0)
  if (MinIndex < 0)
  {
    X[0] = 0.0;
    Y[0] = 0.0;
    MinIndex = 0;
  }
  CurrentLX = X[MinIndex] - TRILATERATIONSYSTEM_STD_MULT * StandardDeviation;
  CurrentLY = Y[MinIndex] - TRILATERATIONSYSTEM_STD_MULT * StandardDeviation;
  CurrentUX = X[MinIndex] + TRILATERATIONSYSTEM_STD_MULT * StandardDeviation;
  CurrentUY = Y[MinIndex] + TRILATERATIONSYSTEM_STD_MULT * StandardDeviation;
} // SystemZoomInitialAreaOverspecified



/*
void TrilaterationSystem::ZoomToAreaInit(unsigned long CurrentTime)
{ // CalculateSearchBounds
  int i = 0;               // Index Variable
  int j = 0;               // Index Variable
  int k = 0;               // Index Variable
  int l = 0;               // Index Variable
  int Base = 0;            // Index To Store Point
  double Temp;             // Temp For Swapping
  double Points[27][4];    // Points Of Intersection
  double HalfAngle = 20.0; // Half Angle Of View
  double R1;               // Distance To First Point
  double R2;               // Distance To Second Point
  double DR1;              // Difference In Radius
  double DR2;              // Difference In Radius

  for (i = 0; i < TRILATERATIONSYSTEM_NUMSENSORS - 1; ++i)
  {
    Base = 0;
    for (j = i + 1; j < TRILATERATIONSYSTEM_NUMSENSORS; ++j)
    {
      if (i > 0) Base += 9;
      if (j > 1) Base += 9;
      // IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), Sensors[i].GetActualRadius() + 0.0 * TRILATERATIONSYSTEM_STD, Sensors[j].GetPointX(), Sensors[j].GetPointY(), Sensors[j].GetActualRadius() + 0.0 * TRILATERATIONSYSTEM_STD, Points[Base + 4]);
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), Sensors[i].GetMeasuredRadius() - 3.0 * TRILATERATIONSYSTEM_STD, Sensors[j].GetPointX(), Sensors[j].GetPointY(), Sensors[j].GetMeasuredRadius() - 3.0 * TRILATERATIONSYSTEM_STD, Points[Base + 0]);
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), Sensors[i].GetMeasuredRadius() + 0.0 * TRILATERATIONSYSTEM_STD, Sensors[j].GetPointX(), Sensors[j].GetPointY(), Sensors[j].GetMeasuredRadius() - 3.0 * TRILATERATIONSYSTEM_STD, Points[Base + 1]);
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), Sensors[i].GetMeasuredRadius() + 3.0 * TRILATERATIONSYSTEM_STD, Sensors[j].GetPointX(), Sensors[j].GetPointY(), Sensors[j].GetMeasuredRadius() - 3.0 * TRILATERATIONSYSTEM_STD, Points[Base + 2]);
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), Sensors[i].GetMeasuredRadius() - 3.0 * TRILATERATIONSYSTEM_STD, Sensors[j].GetPointX(), Sensors[j].GetPointY(), Sensors[j].GetMeasuredRadius() + 0.0 * TRILATERATIONSYSTEM_STD, Points[Base + 3]);
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), Sensors[i].GetMeasuredRadius() + 0.0 * TRILATERATIONSYSTEM_STD, Sensors[j].GetPointX(), Sensors[j].GetPointY(), Sensors[j].GetMeasuredRadius() + 0.0 * TRILATERATIONSYSTEM_STD, Points[Base + 4]);
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), Sensors[i].GetMeasuredRadius() + 3.0 * TRILATERATIONSYSTEM_STD, Sensors[j].GetPointX(), Sensors[j].GetPointY(), Sensors[j].GetMeasuredRadius() + 0.0 * TRILATERATIONSYSTEM_STD, Points[Base + 5]);
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), Sensors[i].GetMeasuredRadius() - 3.0 * TRILATERATIONSYSTEM_STD, Sensors[j].GetPointX(), Sensors[j].GetPointY(), Sensors[j].GetMeasuredRadius() + 3.0 * TRILATERATIONSYSTEM_STD, Points[Base + 6]);
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), Sensors[i].GetMeasuredRadius() + 0.0 * TRILATERATIONSYSTEM_STD, Sensors[j].GetPointX(), Sensors[j].GetPointY(), Sensors[j].GetMeasuredRadius() + 3.0 * TRILATERATIONSYSTEM_STD, Points[Base + 7]);
      IntersectCircle(Sensors[i].GetPointX(), Sensors[i].GetPointY(), Sensors[i].GetMeasuredRadius() + 3.0 * TRILATERATIONSYSTEM_STD, Sensors[j].GetPointX(), Sensors[j].GetPointY(), Sensors[j].GetMeasuredRadius() + 3.0 * TRILATERATIONSYSTEM_STD, Points[Base + 8]);
      // Erase Bad Values (The Ones That Don't Correspond To Th Third Radius
      for (l = 0; l < 9; ++l)
      {
        k = TRILATERATIONSYSTEM_NUMSENSORS - i - j;
        R1 = Distance(Sensors[k].GetPointX(), Sensors[k].GetPointY(), 0.0, Points[Base + l][0], Points[Base + l][1], 0.0);
        R2 = Distance(Sensors[k].GetPointX(), Sensors[k].GetPointY(), 0.0, Points[Base + l][2], Points[Base + l][3], 0.0);
        DR1 = R1 - Sensors[k].GetMeasuredRadius();
        DR2 = R2 - Sensors[k].GetMeasuredRadius();
        if (DR1 < 0.0) DR1 = -DR1;
        if (DR2 < 0.0) DR1 = -DR2;
        if (DR1 < DR2)
        {
          Points[Base + l][2] = -TRILATERATIONSYSTEM_INFINITY;
          Points[Base + l][3] = -TRILATERATIONSYSTEM_INFINITY;
        }
        else
        {
          Points[Base + l][0] = -TRILATERATIONSYSTEM_INFINITY;
          Points[Base + l][1] = -TRILATERATIONSYSTEM_INFINITY;
        }
      }
    }
  }
  // Calculate Bounds
  CurrentLX = 1;
  CurrentLY = 1;
  CurrentUX = -1;
  CurrentUY = -1;
  // Find Min And Max
  for (i = 0; i < 27; ++i)
  {
    if ((Points[i][0] < CurrentLX) && (Points[i][0] > -1)) CurrentLX = Points[i][0];
    if ((Points[i][1] < CurrentLY) && (Points[i][1] > -1)) CurrentLY = Points[i][1];
    if ((Points[i][2] < CurrentLX) && (Points[i][2] > -1)) CurrentLX = Points[i][2];
    if ((Points[i][3] < CurrentLY) && (Points[i][3] > -1)) CurrentLY = Points[i][3];
    if ((Points[i][0] > CurrentUX) && (Points[i][0] <  1)) CurrentUX = Points[i][0];
    if ((Points[i][1] > CurrentUY) && (Points[i][1] <  1)) CurrentUY = Points[i][1];
    if ((Points[i][2] > CurrentUX) && (Points[i][2] <  1)) CurrentUX = Points[i][2];
    if ((Points[i][3] > CurrentUY) && (Points[i][3] <  1)) CurrentUY = Points[i][3];
  }
  if (CurrentLX > CurrentUX)
  {
    Temp = CurrentLX;
    CurrentLX = CurrentUX;
    CurrentUX = Temp;
  }
  if (CurrentLY > CurrentUY)
  {
    Temp = CurrentLY;
    CurrentLY = CurrentUY;
    CurrentUY = Temp;
  }
  // Make Sure Bounds Are Square
  if (CurrentUX - CurrentLX > CurrentUY - CurrentLY)
  {
    Temp = (CurrentUX - CurrentLX) - (CurrentUY - CurrentLY);
    CurrentLY -= Temp / 2;
    CurrentUY += Temp / 2;
  }
  else
  {
    Temp = (CurrentUY - CurrentLY) - (CurrentUX - CurrentLX);
    CurrentLX -= Temp / 2;
    CurrentUX += Temp / 2;
  }
  FinalX = (CurrentLX + CurrentUX) / 2.0;
  FinalY = (CurrentLY + CurrentUY) / 2.0;
  FinalZ = (((double) (CurrentUX - CurrentLX)) / 2.0) / tan(HalfAngle * TRILATERATIONSYSTEM_PI / 180.0);
  if (CurrentTime > 0) ZoomStart = CurrentTime;
} // CalculateSearchBounds
*/


void TrilaterationSystem::IntersectCircle(double X1, double Y1, double R1, double X2, double Y2, double R2, double* Points)
{ // IntersectCircle
  // See: http://astronomy.swin.edu.au/~pbourke/geometry/2circle/
  int i = 0;                                     // Index Variable
  double D = Distance(X1, Y1, 0.0, X2, Y2, 0.0); // Distance Between Circle Center Points
  double DR = R1 - R2;                           // Difference In Radius
  double A;                                      // Distance To Midpoint Between Circles
  double H;                                      // Height Of Created Triangle
  double X3;                                     // Intermediate Point
  double Y3;                                     // Intermediate Point

  // Make Sure Radial Different Is Positive
  if (DR < 0) DR = -DR;
  // Set All Return Values To -Infinity
  for (i = 0; i < 4; ++i)
  {
    Points[i] = -TRILATERATIONSYSTEM_INFINITY;
  }
  // Make Sure Circles Intersect
  if (!((D > R1 + R2) || (D < DR)))
  {
    A = (R1*R1 - R2*R2 + D*D) / (2*D);
    X3 = X1 + A * (X2 - X1) / D;
    Y3 = Y1 + A * (Y2 - Y1) / D;
    H = sqrt(R1*R1 - A*A);
    Points[0] = X3 + H * (Y2 - Y1) / D;
    Points[1] = Y3 - H * (X2 - X1) / D;
    Points[2] = X3 - H * (Y2 - Y1) / D;
    Points[3] = Y3 + H * (X2 - X1) / D;
  }
} // IntersectCircle


/*
void TrilaterationSystem::SetUpdateMode(int UpdateMode)
{ // UpdateMode
  if (UpdateMode)
  {
    SkipUpdate = 0;
  }
  else
  {
    SkipUpdate = 1;
  }
} // UpdateMode
*/



void TrilaterationSystem::SetNumSensors(int NS)
{ // SetNumSensors
  if (NS >= 3) NumSensors = NS;
  else NumSensors = 3;
} // SetNumSensors



int TrilaterationSystem::GetNumSensors(void)
{ // GetNumSensors
  return(NumSensors);
} // GetNumSensors



void TrilaterationSystem::SetNumIterations(int NI)
{ // SetNumIterations
  if (NI > 0) NumIterations = NI;
} // SetNumIterations



int TrilaterationSystem::GetNumIterations(void)
{ // GetNumIterations
  return(NumIterations);
} // GetNumIterations



void TrilaterationSystem::SetMean(double M)
{ // SetMean
  Mean = M;
} // SetMean



double TrilaterationSystem::GetMean(void)
{ // GetMean
  return(Mean);
} // GetMean



void TrilaterationSystem::SetStandardDeviation(double STD)
{ // SetStandardDeviation
  if (STD >= 0.0) StandardDeviation = STD;
  else StandardDeviation = 0.0;
} // SetStandardDeviation


/*
double TrilaterationSystem::GetStandardDeviation(void)
{ // GetStandardDeviation
  return(StandardDeviation);
} // GetStandardDeviation
*/


void TrilaterationSystem::SetGridX(int X)
{ // SetGridX
  if (X > 0) GridX = X;
  else GridX = 1;
} // SetGridX



void TrilaterationSystem::SetGridY(int Y)
{ // SetGridY
  if (Y > 0) GridY = Y;
  else GridY = 1;
} // SetGridY



int TrilaterationSystem::SystemCheckDone(void)
{ // SystemCheckDone
  int ReturnVal = 0;

  ++CurrentIteration;
  if (CurrentIteration >= NumIterations) ReturnVal = 1;
  return(ReturnVal);
} // SystemCheckDone



double TrilaterationSystem::GetBoundsLX(void)
{ // GetBoundsLX
  return(TRILATERATIONSYSTEM_LX);
} // GetBoundsLX



double TrilaterationSystem::GetBoundsLY(void)
{ // GetBoundsLY
  return(TRILATERATIONSYSTEM_LY);
} // GetBoundsLY



double TrilaterationSystem::GetBoundsUX(void)
{ // GetBoundsUX
  return(TRILATERATIONSYSTEM_UX);
} // GetBoundsUX



double TrilaterationSystem::GetBoundsUY(void)
{ // GetBoundsUY
  return(TRILATERATIONSYSTEM_UY);
} // GetBoundsUY



TrilaterationSystemStates TrilaterationSystem::GetState(void)
{ // GetState
  return(State);
} // GetState



double TrilaterationSystem::GetCurrentLX(void)
{ // GetCurrentLX
  return(CurrentLX);
} // GetCurrentLX



double TrilaterationSystem::GetCurrentLY(void)
{ // GetCurrentLY
  return(CurrentLY);
} // GetCurrentLY



double TrilaterationSystem::GetCurrentUX(void)
{ // GetCurrentUX
  return(CurrentUX);
} // GetCurrentUX



double TrilaterationSystem::GetCurrentUY(void)
{ // GetCurrentUY
  return(CurrentUY);
} // GetCurrentUY



int TrilaterationSystem::IsFinished(void)
{ // IsFinished
  int Finished = 0;

  if (State == SYSTEM_COMPLETE)
  {
    Finished = 1;
  }
  return(Finished);
} // IsFinished



int TrilaterationSystem::IsBreak(void)
{ // IsBreak
  int Break = 0;

  if ((State == SYSTEM_COMPLETE) || (State == SYSTEM_ZOOM_PROBLEM_INIT) || (State == SYSTEM_ZOOM_INITIAL_AREA_INIT) || (State == SYSTEM_GRID_INIT) || (State == SYSTEM_SEARCH_INIT) || (State == SYSTEM_ZOOM_AREA_INIT) || (State == SYSTEM_ZOOM_END_INIT) || (State == SYSTEM_COMPLETE))
  {
    Break = 1;
  }
  return(Break);
} // IsBreak



void TrilaterationSystem::Reset(void)
{ // Reset
  SystemInit();
  State = SYSTEM_INIT;
} // Reset



void TrilaterationSystem::SetMonteCarloMode(int MCMode)
{ // SetMonteCarloMode
  if (MCMode) UseMonteCarlo = 1;
  else UseMonteCarlo = 0;
} // SetMonteCarloMode