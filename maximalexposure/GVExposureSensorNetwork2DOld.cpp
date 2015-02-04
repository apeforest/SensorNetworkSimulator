////////////////////////////////////////////////////////////////////////////////
// GVSensorNetwork.cpp                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdlib.h>
#include <fstream.h>
#include "GVEllipseCoverSensorNetwork.h"



// GVEllipseCoverSensorNetwork /////////////////////////////////////////////////
int __cdecl CompareHoleGridPoints(const void* Element1, const void* Element2)
{
	return((int) (((GVEllipseCoverSensorNetwork_Hole*) Element1)->Angle - ((GVEllipseCoverSensorNetwork_Hole*) Element2)->Angle));
}



GVEllipseCoverSensorNetwork::GVEllipseCoverSensorNetwork()
{ // GVEllipseCoverSensorNetwork
  Attr = NULL;
  BFSAttr = NULL;
  GridRadiusX = 0;
  GridRadiusY = 0;
  GridXMin = 0;
  GridXMax = 0;
  GridYMin = 0;
  GridYMax = 0;
  GridHX = 0.0;
  GridHY = 0.0;
  GridVX = 0.0;
  GridVY = 0.0;
  GVSensorNetwork();
  Delete();
} // GVEllipseCoverSensorNetwork



GVEllipseCoverSensorNetwork::~GVEllipseCoverSensorNetwork()
{ // ~GVEllipseCoverSensorNetwork
  Delete();
} // ~GVEllipseCoverSensorNetwork


void GVEllipseCoverSensorNetwork::New(int NS, double R, double SX, double SY, double EX, double EY, double ML, int GX, int GY)
{ // New
  int i = 0;

  Delete();
  SetEllipseParameters(SX, SY, EX, EY, ML, GX, GY);
  GVSensorNetwork::New(NS, R);
  DestinationNode = -1;
  CurrentNode = -1;
  Initialized = 0;
  Attr = new GVEllipseCoverSensorNetwork_Attributes[NumSensors];
  for (i = 0; i < NumSensors; ++i) Attr[i].Known = 0;
} // New



void GVEllipseCoverSensorNetwork::SetEllipseParameters(double SX, double SY, double EX, double EY, double ML, int GX, int GY)
{ // SetEllipseParameters
  StartX = SX;
  StartY = SY;
  EndX = EX;
  EndY = EY;
  GridX = GX;
  GridY = GY;
  MaxLength = ML;
  NewGrid();
} // SetEllipseParameters



void GVEllipseCoverSensorNetwork::NewGrid(void)
{ // NewGrid
  int i = 0;
  int j = 0;
  int KeepGoing = 1;
  int Neighbor[4] = { 0 };
  int NumPoints = GridX * GridY;
  int CurrentLevel = GVGRID2D_MAXLEVEL;
  int NextLevel = GVGRID2D_MAXLEVEL;
  int NewGridX = GridX - 2;
  int NewGridY = GridY - 2;
  double Length = Distance(StartX, StartY, 0.0, EndX, EndY, 0.0);
  double A = MaxLength / 2.0;
  double B = sqrt(A * A - Length * Length / 4.0);
  double VX = (EndX - StartX) / Length;
  double VY = (EndY - StartY) / Length;
  double AD = 2.0 * A / ((double) NewGridX);
  double BD = 2.0 * B / ((double) NewGridY);
  double CX = (StartX + EndX) / 2.0;
  double CY = (StartY + EndY) / 2.0;
  double X = 0.0;
  double Y = 0.0;

  GridHX = VX * AD;
  GridHY = VY * AD;
  GridVX = -VY * BD;
  GridVY = VX * BD;
  
  X = CX - GridHX / 2.0 * NewGridX - GridHX - GridVX / 2.0 * NewGridY - GridVX;
  Y = CY - GridHY / 2.0 * NewGridX - GridHY - GridVY / 2.0 * NewGridY - GridVY;
  Grid.New(GridX, GridY, X, Y, GridHX, GridHY, GridVX, GridVY);
  // Calculate the Grid Radius Values
  GridRadiusX = ((int) (Radius / AD)) + 3;
  GridRadiusY = ((int) (Radius / BD)) + 3;
  GridXMin = 0;
  GridXMax = GridX - 1;
  GridYMin = 0;
  GridYMax = GridY - 1;
  CalculateLevels();
} // NewGrid



void GVEllipseCoverSensorNetwork::Delete()
{ // Delete
  GVSensorNetwork::Delete();
  if (Attr != NULL) delete[] Attr;
  if (BFSAttr != NULL) delete[] BFSAttr;
  Attr = NULL;
  BFSAttr = NULL;
  StartX = 0.0;
  StartY = 0.0;
  EndX = 0.0;
  EndY = 0.0;
  GridX = 0;
  GridY = 0;
  MaxLength = 0.0;
  GridXMin = 0;
  GridXMax = 0;
  GridYMin = 0;
  GridYMax = 0;
  GridRadiusX = 0;
  GridRadiusY = 0;
  MaxLevel = 0;
  CurrentNode = -1;
  NextNode = -1;
  SourceNode = -1;
  DestinationNode = -1;
  GridHX = 0.0;
  GridHY = 0.0;
  GridVX = 0.0;
  GridVY = 0.0;
  Initialized = 0;
  NumCommunications = 0;
  CurrentIteration = 0;
  Grid.Delete();
} // Delete



double GVEllipseCoverSensorNetwork::GetGridPointX(int i, int j, int PointNumber)
{ // GetGridPointX
  double ReturnVal = Grid.GetGridPointX(i, j, PointNumber);
  return(ReturnVal);
} // GetGridPointX



double GVEllipseCoverSensorNetwork::GetGridPointY(int i, int j, int PointNumber)
{ // GetGridPointY
  double ReturnVal = Grid.GetGridPointY(i, j, PointNumber);
  return(ReturnVal);
} // GetGridPointY



int GVEllipseCoverSensorNetwork::GetGridSizeX(void)
{ // GetGridSizeX
  return(GridX);
} // GetGridSizeX



int GVEllipseCoverSensorNetwork::GetGridSizeY(void)
{ // GetGridSizeY
  return(GridY);
} // GetGridSizeY



int GVEllipseCoverSensorNetwork::GetMaxLevel(void)
{ // GetMaxLevel
  return(MaxLevel);
} // GetMaxLevel



int GVEllipseCoverSensorNetwork::GetLevel(int i, int j)
{ // GetLevel
  int ReturnVal = 0;
  if ((i >= GridXMin) && (i <= GridXMax) && (j >= GridYMin) && (j <= GridYMax))
  {
    ReturnVal = Grid.GetLevel(i, j);
  }
  return(ReturnVal);
} // GetLevel



int GVEllipseCoverSensorNetwork::GetKnown(int Index)
{ // GetKnown
  int ReturnVal = GVELLIPSECOVERSENSORNETWORK_UNKNOWN;

  if ((Index >= 0) && (Index < NumSensors)) ReturnVal = Attr[Index].Known;
  return(ReturnVal);
} // GetKnown



int GVEllipseCoverSensorNetwork::GetCovered(int i, int j)
{ // GetCovered
  int ReturnVal = Grid.GetCovered(i, j);
  return(ReturnVal);
} // GetCovered



int GVEllipseCoverSensorNetwork::GetCurrentSensor(void)
{ // GetCurrentSensor
  return(CurrentNode);
} // GetCurrentSensor



int GVEllipseCoverSensorNetwork::GetNextSensor(void)
{ // GetNextSensor
  return(NextNode);
} // GetNextSensor



int GVEllipseCoverSensorNetwork::GetMessageMode(void)
{ // GetMessageMode
  return(MessageMode);
} // GetMessageMode



double GVEllipseCoverSensorNetwork::CalculateHeuristic(int Area, int Hops, int Index)
{ // CalculateHeuristic
  return(((double) pow(Area, GVELLIPSECOVERSENSORNETWORK_AREAPRIORITY)) / (pow(Hops + 1, GVELLIPSECOVERSENSORNETWORK_HOPSPRIORITY) * pow(Index + 1, GVELLIPSECOVERSENSORNETWORK_INDEXPRIORITY)));
} // CalculateHeuristic



int GVEllipseCoverSensorNetwork::CalculateDone(void)
{ // CalculateDone
  int i = 0;
  int ReturnVal = 1;

  for (i = 0; ReturnVal && (i < NumSensors); ++i)
  {
    if (Attr[i].Known == GVELLIPSECOVERSENSORNETWORK_KNOWN)
    {
      if (CalculatePotentialArea(i, GVELLIPSECOVERSENSORNETWORK_UNCOVERED) > 0)
      {
        ReturnVal = 0;
      }
    }
  }
  // if (ReturnVal) Initialized = 0;
  return(ReturnVal);
} // CalculateDone



void GVEllipseCoverSensorNetwork::CalculateLevels(void)
{ // CalculateLevels
  int i = 0;
  int j = 0;
  int GridX = Grid.GetGridSizeX();
  int GridY = Grid.GetGridSizeY();
  int KeepGoing = 1;
  int CurrentLevel = 0;
  int NextLevel = 0;
  double Length = 0.0;
  double X = 0.0;
  double Y = 0.0;

  MaxLevel = 0;
  for (i = GridXMin; i <= GridXMax; ++i)
  {
    for (j = GridYMin; j <= GridYMax; ++j)
    {
      X = Grid.GetGridPointX(i, j, GVGRID2D_CENTERPOINT);
      Y = Grid.GetGridPointY(i, j, GVGRID2D_CENTERPOINT);
      Length = Distance(X, Y, 0.0, StartX, StartY, 0.0) + Distance(X, Y, 0.0, EndX, EndY, 0.0);
      if ((Length > MaxLength) || (Grid.GetCovered(i, j) == GVELLIPSECOVERSENSORNETWORK_COVERED))
      {
        Grid.SetLevel(i, j, 0);
      }
      else
      {
        Grid.SetLevel(i, j, GVGRID2D_MAXLEVEL);
      }
    }
  }

  while(KeepGoing)
  {
    KeepGoing = 0;
    for (i = GridXMin; i <= GridXMax; ++i)
    {
      for (j = GridYMin; j <= GridYMax; ++j)
      {
        CurrentLevel = Grid.GetLevel(i, j);
        NextLevel = Grid.GetLevel(i + 1, j) + 1;
        if (NextLevel < CurrentLevel)
        {
          CurrentLevel = NextLevel;
          KeepGoing = 1;
        }
        NextLevel = Grid.GetLevel(i - 1, j) + 1;
        if (NextLevel< CurrentLevel)
        {
          CurrentLevel = NextLevel;
          KeepGoing = 1;
        }
        NextLevel = Grid.GetLevel(i, j + 1) + 1;
        if (NextLevel < CurrentLevel)
        {
          CurrentLevel = NextLevel;
          KeepGoing = 1;
        }
        NextLevel = Grid.GetLevel(i, j - 1) + 1;
        if (NextLevel < CurrentLevel)
        {
          CurrentLevel = NextLevel;
          KeepGoing = 1;
        }
        Grid.SetLevel(i, j, CurrentLevel);
        if (CurrentLevel > MaxLevel) MaxLevel = CurrentLevel;
      }
    }
  }
} // CalculateLevels



void GVEllipseCoverSensorNetwork::CalculateGridIndex(int Index, int* i, int* j)
{ // CalculateGridIndex
  double X = Sensors[Index].GetX();
  double Y = Sensors[Index].GetY();

  *i = ((int) (GridHX * X + GridHY * Y));
  *j = ((int) (GridVX * X + GridVY * Y));
} // CalculateGridIndex



int GVEllipseCoverSensorNetwork::CalculateIndex(int Index)
{ // CalculateIndex
  int i = 0;
  int j = 0;
  int GridX = Grid.GetGridSizeX();
  int GridY = Grid.GetGridSizeY();
  int ReturnVal = GVGRID2D_MAXLEVEL;
  int NextLevel = 0;
  int iSensor = 0;
  int jSensor = 0;
  int iMin = 0;
  int jMin = 0;
  int iMax = 0;
  int jMax = 0;
  double D = 0.0;
  double L = 0.0;
  double X = 0.0;
  double Y = 0.0;
  double GridHLength = 0.0;
  double GridVLength = 0.0;
  double SensorX = Sensors[Index].GetX();
  double SensorY = Sensors[Index].GetY();
  double SensorZ = Sensors[Index].GetZ();
  double GridSensorX = SensorX - Grid.GetGridPointX(0, 0, GVGRID2D_TOPLEFT);
  double GridSensorY = SensorY - Grid.GetGridPointY(0, 0, GVGRID2D_TOPLEFT);

  GridHLength = Distance(0.0, 0.0, 0.0, GridHX, GridHY, 0.0);
  GridVLength = Distance(0.0, 0.0, 0.0, GridVX, GridVY, 0.0);
  iSensor = ((int) ((GridHX * GridSensorX + GridHY * GridSensorY) / (GridHLength * GridHLength)));
  jSensor = ((int) ((GridVX * GridSensorX + GridVY * GridSensorY) / (GridVLength * GridVLength)));
  iMin = iSensor - GridRadiusX;
  iMax = iSensor + GridRadiusX;
  jMin = jSensor - GridRadiusY;
  jMax = jSensor + GridRadiusY;
  if (iMin < 0) iMin = 0;
  if (iMax >= GridX) iMax = GridX - 1;
  if (jMin < 0) jMin = 0;
  if (jMax >= GridY) jMax = GridY - 1;

  for (i = iMin; i <= iMax; ++i)
  {
    for (j = jMin; j <= jMax; ++j)
    {
      if (Grid.GetCovered(i, j) != GVELLIPSECOVERSENSORNETWORK_COVERED)
      {
        X = Grid.GetGridPointX(i, j, GVGRID2D_CENTERPOINT);
        Y = Grid.GetGridPointY(i, j, GVGRID2D_CENTERPOINT);
        L = Distance(X, Y, 0.0, StartX, StartY, 0.0) + Distance(X, Y, 0.0, EndX, EndY, 0.0);
        D = Distance(X, Y, 0.0, SensorX, SensorY, SensorZ);
        if ((D <= Radius) && (L <= MaxLength))
        {
          NextLevel = Grid.GetLevel(i, j);
          if (NextLevel < ReturnVal) ReturnVal = NextLevel;
        }
      }
    }
  }
  return(ReturnVal);
} // CalculateIndex



void GVEllipseCoverSensorNetwork::CalculateHops(int Index)
{ // CalculateHops
  int i = 0;
  int CurIndex = Index;
  int NbrIndex = 0;
  int CurQueue = 0;
  int NumQueue = 0;
  int TempIndex = 0;
  int TempHoles = 0;
  int NewArea = 0;
  int* IndexQueue = NULL;

  if ((Index >= 0) && (Index < NumSensors))
  {
    // Initialize Breadth-First Search
    if (BFSAttr != NULL) delete[] BFSAttr;
    IndexQueue = new int[NumSensors];
    BFSAttr = new GVEllipseCoverSensorNetwork_BFSAttributes[NumSensors];
    for (i = 0; i < NumSensors; ++i)
    {
      BFSAttr[i].Holes = 32767;
      BFSAttr[i].Hops = 0;
      BFSAttr[i].Visited = 0;
      BFSAttr[i].Parent = -1;
      BFSAttr[i].NewArea = 0;
    }
    IndexQueue[NumQueue++] = CurIndex;
    BFSAttr[CurIndex].Visited = 1;
    BFSAttr[CurIndex].NewArea = CalculatePotentialArea(CurIndex, GVELLIPSECOVERSENSORNETWORK_UNCOVERED);
    // Perform the Breadth-First Search
    while(CurQueue < NumQueue)
    {
      CurIndex = IndexQueue[CurQueue];
      BFSAttr[CurIndex].Visited = 2;
      for (i = 0; i < NumNeighbors[CurIndex]; ++i)
      {
        NbrIndex = NeighborIndex[CurIndex][i];
        if ((Attr[NbrIndex].Known) && (!BFSAttr[NbrIndex].Visited))
        {
          IndexQueue[NumQueue++] = NbrIndex;
          BFSAttr[NbrIndex].Hops = BFSAttr[CurIndex].Hops + 1;
          BFSAttr[NbrIndex].Parent = CurIndex;
          BFSAttr[NbrIndex].Visited = 1;
        }
      }
      ++CurQueue;
    }
  }
  if (IndexQueue != NULL) delete[] IndexQueue;
} // CalculateHops



void GVEllipseCoverSensorNetwork::CalculateBestPath(int Index, int Destination)
{ // CalculateBestPath
  int i = 0;
  int CurIndex = Index;
  int BestIndex = 0;
  int MinHops = NumSensors;
  int NbrIndex = 0;
  int CurQueue = 0;
  int NumQueue = 0;
  int TempIndex = 0;
  int TempHoles = 0;
  int MaxHoles = 0;
  int NewArea = 0;
  int BestArea = 0;
  int BestHoles = 32767;
  int* IndexQueue = NULL;

  if ((Index >= 0) && (Index < NumSensors))
  {
    // Initialize Breadth-First Search
    if (BFSAttr != NULL) delete[] BFSAttr;
    IndexQueue = new int[NumSensors];
    BFSAttr = new GVEllipseCoverSensorNetwork_BFSAttributes[NumSensors];
    for (i = 0; i < NumSensors; ++i)
    {
      BFSAttr[i].Holes = 32767;
      BFSAttr[i].Hops = 0;
      BFSAttr[i].Visited = 0;
      BFSAttr[i].Parent = -1;
      BFSAttr[i].NewArea = 0;
    }
    IndexQueue[NumQueue++] = CurIndex;
    BFSAttr[CurIndex].Visited = 1;
    BFSAttr[CurIndex].NewArea = CalculatePotentialArea(CurIndex, GVELLIPSECOVERSENSORNETWORK_UNCOVERED);
    // Perform the Breadth-First Search
    while((BFSAttr[Destination].Visited < 2))
    {
      for (MinHops = NumSensors, i = 0; (BestIndex != Destination) && (i < NumQueue); ++i)
      {
        CurIndex = IndexQueue[i];
        if (BFSAttr[CurIndex].Visited == 1)
        {
          if ((BFSAttr[CurIndex].Hops <= MinHops))
          {
            BestIndex = CurIndex;
            MinHops = BFSAttr[CurIndex].Hops;
          }
        }
      }
      CurIndex = BestIndex;
      BFSAttr[CurIndex].Visited = 2;
      BestHoles = 32767;
      if (CurIndex != Destination)
      {
        for (i = 0; i < NumNeighbors[CurIndex]; ++i)
        {
          NbrIndex = NeighborIndex[CurIndex][i];
          if (Attr[NbrIndex].Known)
          {
            if (!BFSAttr[NbrIndex].Visited)
            {
              IndexQueue[NumQueue++] = NbrIndex;
              
              NewArea = 0;
              TempIndex = CurIndex;
              while(TempIndex >= 0)
              {
                NewArea += CalculatePotentialArea(TempIndex, GVELLIPSECOVERSENSORNETWORK_POTENTIAL);
                TempIndex = BFSAttr[TempIndex].Parent;
              }
              TempHoles = CalculateHoleCreation(NbrIndex);
              NewArea += CalculatePotentialArea(NbrIndex, GVELLIPSECOVERSENSORNETWORK_POTENTIAL);
              ErasePotentialArea(-1, GVELLIPSECOVERSENSORNETWORK_POTENTIAL);

              if (TempHoles > BFSAttr[CurIndex].Holes) MaxHoles = TempHoles;
              else MaxHoles = BFSAttr[CurIndex].Holes;

              BFSAttr[NbrIndex].Holes = MaxHoles;
              BFSAttr[NbrIndex].NewArea = NewArea;
              BFSAttr[NbrIndex].Hops = BFSAttr[CurIndex].Hops + 1;
              BFSAttr[NbrIndex].Parent = CurIndex;
              BFSAttr[NbrIndex].Visited = 1;
            }
            else if ((BFSAttr[NbrIndex].Visited == 1) && (BFSAttr[CurIndex].Hops <= BFSAttr[BFSAttr[NbrIndex].Parent].Hops))
            {
              NewArea = 0;
              TempIndex = CurIndex;
              while(TempIndex >= 0)
              {
                NewArea += CalculatePotentialArea(TempIndex, GVELLIPSECOVERSENSORNETWORK_POTENTIAL);
                TempIndex = BFSAttr[TempIndex].Parent;
              }
              TempHoles = CalculateHoleCreation(NbrIndex);
              NewArea += CalculatePotentialArea(NbrIndex, GVELLIPSECOVERSENSORNETWORK_POTENTIAL);
              ErasePotentialArea(-1, GVELLIPSECOVERSENSORNETWORK_POTENTIAL);

              if (TempHoles > BFSAttr[CurIndex].Holes) MaxHoles = TempHoles;
              else MaxHoles = BFSAttr[CurIndex].Holes;

              if ((MaxHoles < BFSAttr[NbrIndex].Holes) || ((MaxHoles == BFSAttr[NbrIndex].Holes) && (NewArea > BFSAttr[NbrIndex].NewArea)))
              {
                BFSAttr[NbrIndex].Holes = MaxHoles;
                BFSAttr[NbrIndex].NewArea = NewArea;
                BFSAttr[NbrIndex].Hops = BFSAttr[CurIndex].Hops + 1;
                BFSAttr[NbrIndex].Parent = CurIndex;
              }
            }
          }
        }
      }
    }
  }
  if (IndexQueue != NULL) delete[] IndexQueue;
} // CalculateBestPath



int GVEllipseCoverSensorNetwork::CalculateHoleCreation(int SensorIndex)
{ // CalculateHoleCreation
  int i = 0;
  int j = 0;
  int k = 0;
  int iSensor = 0;
  int jSensor = 0;
  int iMin = 0;
  int jMin = 0;
  int iMax = 0;
  int jMax = 0;
  int iNext = 0;
  int jNext = 0;
  int GridX = Grid.GetGridSizeX();
  int GridY = Grid.GetGridSizeY();
  int ReturnVal = 0;
  int NumElements = 0;
  int iOffset[4] = { 1, -1, 0, 0 };
  int jOffset[4] = { 0, 0, 1, -1 };
  int ijAdded = 0;
  int KeepGoing = 0;
  int NumGroups = 0;
  int StartCovered = 0;
  int CurrentCovered = 0;
  int StartIndex = 0;
  int CurrentIndex = 0;
  double D = 0.0;
  double L = 0.0;
  double X = 0.0;
  double Y = 0.0;
  double Z = 0.0;
  double GX = 0.0;
  double GY = 0.0;
  double GZ = 0.0;
  double VX = 0.0;
  double VY = 0.0;
  double GridHLength = 0.0;
  double GridVLength = 0.0;
  double SensorX = Sensors[SensorIndex].GetX();
  double SensorY = Sensors[SensorIndex].GetY();
  double SensorZ = Sensors[SensorIndex].GetZ();
  double DNext = 0.0;
  double LNext = 0.0;
  double LSensor = Distance(SensorX, SensorY, SensorZ, StartX, StartY, 0.0) + Distance(SensorX, SensorY, SensorZ, EndX, EndY, 0.0);
  double Angle = 0.0;
  double GridSensorX = SensorX - Grid.GetGridPointX(0, 0, GVGRID2D_TOPLEFT);
  double GridSensorY = SensorY - Grid.GetGridPointY(0, 0, GVGRID2D_TOPLEFT);
  GVEllipseCoverSensorNetwork_Hole* HoleArray = NULL;

  GridHLength = Distance(0.0, 0.0, 0.0, GridHX, GridHY, 0.0);
  GridVLength = Distance(0.0, 0.0, 0.0, GridVX, GridVY, 0.0);
  iSensor = ((int) ((GridHX * GridSensorX + GridHY * GridSensorY) / (GridHLength * GridHLength)));
  jSensor = ((int) ((GridVX * GridSensorX + GridVY * GridSensorY) / (GridVLength * GridVLength)));
  iMin = iSensor - GridRadiusX;
  iMax = iSensor + GridRadiusX;
  jMin = jSensor - GridRadiusY;
  jMax = jSensor + GridRadiusY;
  if (iMin < 0) iMin = 0;
  if (iMax >= GridX) iMax = GridX - 1;
  if (jMin < 0) jMin = 0;
  if (jMax >= GridY) jMax = GridY - 1;

  HoleArray = new GVEllipseCoverSensorNetwork_Hole[GridX * GridY];
  for (i = iMin; i <= iMax; ++i)
  {
    for (j = jMin; j <= jMax; ++j)
    {
      X = Grid.GetGridPointX(i, j, GVGRID2D_CENTERPOINT);
      Y = Grid.GetGridPointY(i, j, GVGRID2D_CENTERPOINT);
      L = Distance(X, Y, Z, StartX, StartY, 0.0) + Distance(X, Y, Z, EndX, EndY, 0.0);
      if (L <= MaxLength)
      {
        D = Distance(X, Y, Z, SensorX, SensorY, SensorZ);
        for (ijAdded = 0, k = 0; (!ijAdded) && (k < 4); ++k)
        {
          iNext = i + iOffset[k];
          jNext = j + jOffset[k];
          if ((iNext >= 0) && (iNext < GridX) && (jNext >= 0) && (jNext < GridY))
          {
            GX = Grid.GetGridPointX(iNext, jNext, GVGRID2D_CENTERPOINT);
            GY = Grid.GetGridPointY(iNext, jNext, GVGRID2D_CENTERPOINT);
            DNext = Distance(GX, GY, GZ, SensorX, SensorY, SensorZ);
            LNext = Distance(GX, GY, GZ, StartX, StartY, 0.0) + Distance(GX, GY, GZ, EndX, EndY, 0.0);
            if ((D > Radius) && (DNext <= Radius) && (L < MaxLength))
            {
              VX = X - SensorX;
              VY = Y - SensorY;
              Angle = (acos(VX / Distance(0.0, 0.0, 0.0, VX, VY, 0.0))) * 180 / 3.14159;
              if (VY < 0) Angle = 360.0 - Angle;
              HoleArray[NumElements].Angle = Angle;
              if (L <= MaxLength) HoleArray[NumElements].Covered = Grid.GetCovered(i, j);
              else HoleArray[NumElements].Covered = 1;
              ++NumElements;
              ijAdded = 1;
            }
            else if ((LSensor <= MaxLength) && (D <= Radius) && (LNext > MaxLength))
            {
              VX = X - SensorX;
              VY = Y - SensorY;
              Angle = (acos(VX / Distance(0.0, 0.0, 0.0, VX, VY, 0.0))) * 180 / 3.14159;
              if (VY < 0) Angle = 360.0 - Angle;
              HoleArray[NumElements].Angle = Angle;
              if (L <= MaxLength) HoleArray[NumElements].Covered = 1;
              else HoleArray[NumElements].Covered = 1;
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
    qsort(HoleArray, NumElements, sizeof(GVEllipseCoverSensorNetwork_Hole), CompareHoleGridPoints);

    StartIndex = 0;
    StartCovered = (HoleArray[StartIndex].Covered != 0);
    KeepGoing = 1;
    while(KeepGoing)
    {
      if (StartIndex >= NumElements) KeepGoing = 0;
      else if (StartCovered != HoleArray[StartIndex].Covered) KeepGoing = 0;
      else StartIndex = (StartIndex + 1) % NumElements;
      if (!StartIndex) KeepGoing = 0;
    }

    CurrentIndex = StartIndex;
    CurrentCovered = (HoleArray[CurrentIndex].Covered != 0);
    NumGroups = 1;
    ReturnVal = 0;
    do
    {
      if (HoleArray[CurrentIndex].Covered != CurrentCovered)
      {
        ++NumGroups;
        CurrentCovered = (HoleArray[CurrentIndex].Covered != 0);
        if (NumGroups >= 3) ReturnVal = 1;
      }
      CurrentIndex = (CurrentIndex + 1) % NumElements;
    } while ((!ReturnVal) && (CurrentIndex != StartIndex));
  }
  
  if (HoleArray != NULL) delete[] HoleArray;

  return(ReturnVal);
} // CalculateHoleCreation



int GVEllipseCoverSensorNetwork::CalculateHoles(void)
{ // CalculateHoles
  int i = 0;
  int j = 0;
  int KeepGoing = 1;
  int MaxCount = 0;
  int CurrentAreaNumber = 2;
  int ReturnVal = 0;
  int iUncovered = -1;
  int jUncovered = -1;
  int GridX = Grid.GetGridSizeX();
  int GridY = Grid.GetGridSizeY();
  int GridSize = GridX * GridY;
  int CurrentIndex = 0;
  int SizeIndex = 0;
  int iCurrent = 0;
  int jCurrent = 0;
  int iNext = 0;
  int jNext = 0;
  int ijIndex = 0;
  int iOffset[4] = { 1, -1, 0, 0 };
  int jOffset[4] = { 0, 0, 1, -1 };
  int* iIndex = new int[GridSize];
  int* jIndex = new int[GridSize];
  int* ijVisited = new int[GridSize];
  int* AreaCount = NULL;
  double X = 0.0;
  double Y = 0.0;
  double Z = 0.0;
  double L = 0.0;

  for (i = 0; i < GridSize; ++i)
  {
    iIndex[i] = 0;
    jIndex[j] = 0;
    ijVisited[i] = 0;
  }

  for (i = 0; i < GridX; ++i)
  {
    for (j = 0; j < GridY; ++j)
    {
      ijIndex = i + j * GridX;
      X = Grid.GetGridPointX(i, j, GVGRID2D_CENTERPOINT);
      Y = Grid.GetGridPointY(i, j, GVGRID2D_CENTERPOINT);
      L = Distance(X, Y, Z, StartX, StartY, 0.0) + Distance(X, Y, Z, EndX, EndY, 0.0);
      if (Grid.GetCovered(i, j)) ijVisited[ijIndex] = 1;
      else if (L > MaxLength) ijVisited[ijIndex] = 1;
    }
  }

  while(KeepGoing)
  {
    // Pick Uncovered Index
    iUncovered = -1;
    jUncovered = -1;
    KeepGoing = 0;
    for (i = 0; (iUncovered < 0) && (jUncovered < 0) && (i < GridX); ++i)
    {
      for (j = 0; (iUncovered < 0) && (jUncovered < 0) && (j < GridY); ++j)
      {
        ijIndex = i + j * GridX;
        if (!ijVisited[ijIndex])
        {
          iUncovered = i;
          jUncovered = j;
        }
      }
    }

    if ((iUncovered >= 0) && (jUncovered >= 0))
    {
      iIndex[SizeIndex] = iUncovered;
      jIndex[SizeIndex] = jUncovered;
      iCurrent = iIndex[SizeIndex];
      jCurrent = jIndex[SizeIndex];
      ijIndex = iCurrent + jCurrent * GridX;
      ijVisited[ijIndex] = CurrentAreaNumber;
      ++SizeIndex;
      while(CurrentIndex < SizeIndex)
      {
        iCurrent = iIndex[CurrentIndex];
        jCurrent = jIndex[CurrentIndex];
        for (i = 0; i < 4; ++i)
        {
          iNext = iCurrent + iOffset[i];
          jNext = jCurrent + jOffset[i];
          if ((iNext >= 0) && (iNext < GridX) && (jNext >= 0) && (jNext < GridY))
          {
            ijIndex = iNext + jNext * GridX;
            if (!ijVisited[ijIndex])
            {
              ijVisited[ijIndex] = CurrentAreaNumber;
              iIndex[SizeIndex] = iNext;
              jIndex[SizeIndex] = jNext;
              ++SizeIndex;
            }
          }
        }
        ++CurrentIndex;
      }
    }
    ++CurrentAreaNumber;
    for (i = 0; (!KeepGoing) && (i < GridSize); ++i)
    {
      if (!ijVisited[i]) KeepGoing = 1;
    }
  }

  AreaCount = new int[CurrentAreaNumber];
  for (i = 0; i < CurrentAreaNumber; ++i)
  {
    AreaCount[i] = 0;
  }

  for (i = 0; i < GridX; ++i)
  {
    for (j = 0; j < GridY; ++j)
    {
      ijIndex = i + j * GridX;
      ++AreaCount[ijVisited[ijIndex]];
    }
  }

  for (ReturnVal = 0, i = 2; i < CurrentAreaNumber; ++i)
  {
    ReturnVal += AreaCount[i];
    if (AreaCount[i] > MaxCount)
    {
      MaxCount = AreaCount[i];
    }
  }
  ReturnVal -= MaxCount;

  if (iIndex != NULL) delete[] iIndex;
  if (jIndex != NULL) delete[] jIndex;
  if (ijVisited != NULL) delete[] ijVisited;
  if (AreaCount != NULL) delete[] AreaCount;

  return(ReturnVal);
} // CalculateHoles



int GVEllipseCoverSensorNetwork::CalculatePotentialArea(int Index, int TempValue)
{ // CalculatePotentialArea
  int i = 0;
  int j = 0;
  int iSensor = 0;
  int jSensor = 0;
  int iMin = 0;
  int jMin = 0;
  int iMax = 0;
  int jMax = 0;
  int ReturnVal = 0;
  int GridX = Grid.GetGridSizeX();
  int GridY = Grid.GetGridSizeY();
  double GridHLength = 0.0;
  double GridVLength = 0.0;
  double D = 0.0;
  double L = 0.0;
  double X = 0.0;
  double Y = 0.0;
  double SensorX = Sensors[Index].GetX();
  double SensorY = Sensors[Index].GetY();
  double SensorZ = Sensors[Index].GetZ();
  double GridSensorX = SensorX - Grid.GetGridPointX(0, 0, GVGRID2D_TOPLEFT);
  double GridSensorY = SensorY - Grid.GetGridPointY(0, 0, GVGRID2D_TOPLEFT);

  GridHLength = Distance(0.0, 0.0, 0.0, GridHX, GridHY, 0.0);
  GridVLength = Distance(0.0, 0.0, 0.0, GridVX, GridVY, 0.0);
  iSensor = ((int) ((GridHX * GridSensorX + GridHY * GridSensorY) / (GridHLength * GridHLength)));
  jSensor = ((int) ((GridVX * GridSensorX + GridVY * GridSensorY) / (GridVLength * GridVLength)));
  iMin = iSensor - GridRadiusX;
  iMax = iSensor + GridRadiusX;
  jMin = jSensor - GridRadiusY;
  jMax = jSensor + GridRadiusY;
  if (iMin < 0) iMin = 0;
  if (iMax >= GridX) iMax = GridX - 1;
  if (jMin < 0) jMin = 0;
  if (jMax >= GridY) jMax = GridY - 1;

  for (i = iMin; i <= iMax; ++i)
  {
    for (j = jMin; j <= jMax; ++j)
    {
      if (Grid.GetCovered(i, j) == GVELLIPSECOVERSENSORNETWORK_UNCOVERED)
      {
        X = Grid.GetGridPointX(i, j, GVGRID2D_CENTERPOINT);
        Y = Grid.GetGridPointY(i, j, GVGRID2D_CENTERPOINT);
        D = Distance(SensorX, SensorY, SensorZ, X, Y, 0.0);
        L = Distance(X, Y, 0.0, StartX, StartY, 0.0) + Distance(X, Y, 0.0, EndX, EndY, 0.0);
        if ((L <= MaxLength) && (D <= Radius))
        {
          Grid.SetCovered(i, j, TempValue);
          ++ReturnVal;
        }
      }
    }
  }
  return(ReturnVal);
} // CalculatePotentialArea



void GVEllipseCoverSensorNetwork::ErasePotentialArea(int Index, int TempValue)
{ // ErasePotentialArea
  int i = 0;
  int j = 0;
  int GridX = Grid.GetGridSizeX();
  int GridY = Grid.GetGridSizeY();
  int iSensor = 0;
  int jSensor = 0;
  int iMin = 0;
  int jMin = 0;
  int iMax = GridX - 1;
  int jMax = GridY - 1;
  double GridHLength = 0.0;
  double GridVLength = 0.0;
  double SensorX = 0.0;
  double SensorY = 0.0;
  double GridSensorX = 0.0;
  double GridSensorY = 0.0;

  if ((Index >= 0) && (Index < NumSensors))
  {
    GridHLength = Distance(0.0, 0.0, 0.0, GridHX, GridHY, 0.0);
    GridVLength = Distance(0.0, 0.0, 0.0, GridVX, GridVY, 0.0);
    SensorX = Sensors[Index].GetX();
    SensorY = Sensors[Index].GetY();
    GridSensorX = SensorX - Grid.GetGridPointX(0, 0, GVGRID2D_TOPLEFT);
    GridSensorY = SensorY - Grid.GetGridPointY(0, 0, GVGRID2D_TOPLEFT);
    iSensor = ((int) ((GridHX * GridSensorX + GridHY * GridSensorY) / (GridHLength * GridHLength)));
    jSensor = ((int) ((GridVX * GridSensorX + GridVY * GridSensorY) / (GridVLength * GridVLength)));
    iMin = iSensor - GridRadiusX;
    iMax = iSensor + GridRadiusX;
    jMin = jSensor - GridRadiusY;
    jMax = jSensor + GridRadiusY;
    if (iMin < 0) iMin = 0;
    if (iMax >= GridX) iMax = GridX - 1;
    if (jMin < 0) jMin = 0;
    if (jMax >= GridY) jMax = GridY - 1;
  }


  for (i = iMin; i <= iMax; ++i)
  {
    for (j = jMin; j <= jMax; ++j)
    {
      if (Grid.GetCovered(i, j) == TempValue)
      {
        Grid.SetCovered(i, j, GVELLIPSECOVERSENSORNETWORK_UNCOVERED);
      }
    }
  }
} // ErasePotentialArea



void GVEllipseCoverSensorNetwork::InitializeSearch(void)
{ // InitializeSearch
  int i = 0;
  int j = 0;
  int GridX = 0;
  int GridY = 0;
  double InitialX = 0.0;
  double InitialY = 0.0;
  double VX = EndX - StartX;
  double VY = EndY - StartY;
  double VN = Distance(VX, VY, 0.0, 0.0, 0.0, 0.0);
  double VScale = (MaxLength - VN) / (2.0 * VN);
  double D = 0.0;
  double MinDistance = 10000.0;

  if ((Sensors != NULL) && (NumNeighbors != NULL) && (NeighborIndex != NULL) && (Attr != NULL))
  {
    GridX = Grid.GetGridSizeX();
    GridY = Grid.GetGridSizeY();
    VX *= VScale;
    VY *= VScale;
    InitialX = StartX - VX;
    InitialY = StartY - VY;
    for (i = 0; i < NumSensors; ++i)
    {
      Attr[i].Known = GVELLIPSECOVERSENSORNETWORK_UNKNOWN;
      Attr[i].Parent = -1;
      D = Distance(Sensors[i].GetX(), Sensors[i].GetY(), Sensors[i].GetZ(), InitialX, InitialY, 0.0);
      if (D < MinDistance)
      {
        MinDistance = D;
        NextNode = i;
      }
    }
    for (i = 0; i < GridX; ++i)
    {
      for (j = 0; j < GridY; ++j)
      {
        Grid.SetCovered(i, j, GVELLIPSECOVERSENSORNETWORK_UNCOVERED);
      }
    }
    CurrentNode = -1;
    SourceNode = -1;
    DestinationNode = -1;
    CurrentIteration = 0;
    NumCommunications = 0;
    Initialized = 1;
    MessageMode = GVELLIPSECOVERSENSORNETWORK_SEARCH;
  }
  GridXMin = 0;
  GridXMax = GridX - 1;
  GridYMin = 0;
  GridYMax = GridY - 1;
} // InitializeSearch



void GVEllipseCoverSensorNetwork::UpdateLevels(void)
{ // UpdateLevels
  int i = 0;
  int j = 0;
  int Level = 0;
  int MinVal = GVGRID2D_MAXLEVEL;
  int CurrentXMin = GridXMax;
  int CurrentXMax = GridXMin;
  int CurrentYMin = GridYMax;
  int CurrentYMax = GridYMin;
  double D = 0.0;
  double X = 0.0;
  double Y = 0.0;
  double Z = 0.0;

  for (i = GridXMin; i <= GridXMax; ++i)
  {
    for (j = GridYMin; j <= GridYMax; ++j)
    {
      X = Grid.GetGridPointX(i, j, GVGRID2D_CENTERPOINT);
      Y = Grid.GetGridPointY(i, j, GVGRID2D_CENTERPOINT);
      D = Distance(X, Y, Z, StartX, StartY, 0.0) + Distance(X, Y, Z, EndX, EndY, 0.0);
      if ((D <= MaxLength) && (!Grid.GetCovered(i, j)))
      {
        Level = Grid.GetLevel(i, j);
        if (Level < MinVal) MinVal = Level;
        if (i < CurrentXMin) CurrentXMin = i;
        if (i > CurrentXMax) CurrentXMax = i;
        if (j < CurrentYMin) CurrentYMin = j;
        if (j > CurrentYMax) CurrentYMax = j;
      }
    }
  }
  GridXMin = CurrentXMin;
  GridXMax = CurrentXMax;
  GridYMin = CurrentYMin;
  GridYMax = CurrentYMax;
  if (MinVal > 1)
  {
    CalculateLevels();
  }
} // UpdateLevels



int GVEllipseCoverSensorNetwork::UpdateCurrentNode(int Next)
{ // UpdateCurrentNode
  int i = 0;
  int j = 0;
  int ReturnVal = 1;
  int GridX = Grid.GetGridSizeX();
  int GridY = Grid.GetGridSizeY();
  int NbrNum = NumNeighbors[Next];
  int NbrIndex = 0;
  int iSensor = 0;
  int jSensor = 0;
  int iMin = 0;
  int iMax = 0;
  int jMin = 0;
  int jMax = 0;
  double GridHLength = 0.0;
  double GridVLength = 0.0;
  double D = 0.0;
  double X = 0.0;
  double Y = 0.0;
  double Z = 0.0;
  double SensorX = Sensors[Next].GetX();
  double SensorY = Sensors[Next].GetY();
  double SensorZ = Sensors[Next].GetZ();
  double GridSensorX = SensorX - Grid.GetGridPointX(0, 0, GVGRID2D_TOPLEFT);
  double GridSensorY = SensorY - Grid.GetGridPointY(0, 0, GVGRID2D_TOPLEFT);

  GridHLength = Distance(0.0, 0.0, 0.0, GridHX, GridHY, 0.0);
  GridVLength = Distance(0.0, 0.0, 0.0, GridVX, GridVY, 0.0);
  iSensor = ((int) ((GridHX * GridSensorX + GridHY * GridSensorY) / (GridHLength * GridHLength)));
  jSensor = ((int) ((GridVX * GridSensorX + GridVY * GridSensorY) / (GridVLength * GridVLength)));
  iMin = iSensor - GridRadiusX;
  iMax = iSensor + GridRadiusX;
  jMin = jSensor - GridRadiusY;
  jMax = jSensor + GridRadiusY;
  if (iMin < 0) iMin = 0;
  if (iMax >= GridX) iMax = GridX - 1;
  if (jMin < 0) jMin = 0;
  if (jMax >= GridY) jMax = GridY - 1;

  if (Attr[Next].Parent < 0) Attr[Next].Parent = CurrentNode;
  CurrentNode = Next;
  ++NumCommunications;
  Attr[CurrentNode].Known = GVELLIPSECOVERSENSORNETWORK_VISITED;
  for (i = 0; i < NbrNum; ++i)
  {
    NbrIndex = NeighborIndex[CurrentNode][i];
    if (!Attr[NbrIndex].Known) Attr[NbrIndex].Known = GVELLIPSECOVERSENSORNETWORK_KNOWN;
  }
  for (i = iMin; i <= iMax; ++i)
  {
    for (j = jMin; j <= jMax; ++j)
    {
      if (!Grid.GetCovered(i, j))
      {
        X = Grid.GetGridPointX(i, j, GVGRID2D_CENTERPOINT);
        Y = Grid.GetGridPointY(i, j, GVGRID2D_CENTERPOINT);
        D = Distance(X, Y, Z, SensorX, SensorY, SensorZ);
        if (D <= Radius) Grid.SetCovered(i, j, GVELLIPSECOVERSENSORNETWORK_COVERED);
      }
    }
  }
  if (CalculateDone()) ReturnVal = 0;
  return(ReturnVal);
} // UpdateCurrentNode



int GVEllipseCoverSensorNetwork::UpdateSearch_GPSR(void)
{ // UpdateSearch_GPSR
  int i = 0;
  int j = 0;
  int ReturnVal = 1;
  int NbrIndex = 0;
  int NbrNum = 0;
  int NewArea = 0;
  int NewIndex = 0;
  int NewHoleExists = 0;
  int BestArea = 0;
  int BestIndex = 0;
  int BestHoleExists = 0;
  int KeepGoing = 1;
  double NewHeuristic = 0.0;
  double BestHeuristic = 0.0;
  double Angle = 0.0;
  double CurrentX = 0.0;
  double CurrentY = 0.0;
  double AX = 0.0;
  double AY = 1.0;
  double BX = 0.0;
  double BY = 0.0;
  double ToDegrees = (180.0 / 3.14159);

  if (!Initialized) InitializeSearch();
  if ((CurrentIteration >= GVELLIPSECOVERSENSORNETWORK_MAXTRIES) || (NextNode < 0) || (NextNode >= NumSensors)) ReturnVal = 2;
  if ((ReturnVal == 1) && (Sensors != NULL) && (NumNeighbors != NULL) && (NeighborIndex != NULL) && (Attr != NULL))
  {
    if (CurrentNode >= 0)
    {
      AX = Sensors[CurrentNode].GetX() - Sensors[NextNode].GetX();
      AY = Sensors[CurrentNode].GetY() - Sensors[NextNode].GetY();
    }
    BestHoleExists = Grid.GetGridSizeX() * Grid.GetGridSizeY();
    BestIndex = GVGRID2D_MAXLEVEL;
    BestArea = 0;
    BestHeuristic = 0.0;
    ReturnVal = UpdateCurrentNode(NextNode);
    if (ReturnVal)
    {
      if (MessageMode == GVELLIPSECOVERSENSORNETWORK_SEARCH)
      {
        CurrentX = Sensors[CurrentNode].GetX();
        CurrentY = Sensors[CurrentNode].GetY();
        NextNode = -1;
        NbrNum = NumNeighbors[CurrentNode];
        for (i = 0; i < NbrNum; ++i)
        {
          NbrIndex = NeighborIndex[CurrentNode][i];
          if (Attr[NbrIndex].Known == GVELLIPSECOVERSENSORNETWORK_KNOWN)
          {
            NewArea = CalculatePotentialArea(NbrIndex, GVELLIPSECOVERSENSORNETWORK_POTENTIAL);
            if (NewArea > 0)
            {
              NewHoleExists = CalculateHoleCreation(NbrIndex);
              if (NewHoleExists < BestHoleExists)
              {
                BX = Sensors[NbrIndex].GetX() - CurrentX;
                BY = Sensors[NbrIndex].GetY() - CurrentY;
                Angle = acos((AX * BX + AY * BY) / (Distance(0.0, 0.0, 0.0, AX, AY, 0.0) * Distance(0.0, 0.0, 0.0, BX, BY, 0.0))) * ToDegrees;
                BestHoleExists = NewHoleExists;
                BestIndex = CalculateIndex(NbrIndex);
                BestArea = NewArea;
                BestHeuristic = (Angle / 180.0) * BestArea;
                NextNode = NbrIndex;
              }
              else if (NewHoleExists == BestHoleExists)
              {
                NewIndex = CalculateIndex(NbrIndex);
                if (NewIndex < BestIndex)
                {
                  BX = Sensors[NbrIndex].GetX() - CurrentX;
                  BY = Sensors[NbrIndex].GetY() - CurrentY;
                  Angle = acos((AX * BX + AY * BY) / (Distance(0.0, 0.0, 0.0, AX, AY, 0.0) * Distance(0.0, 0.0, 0.0, BX, BY, 0.0))) * ToDegrees;
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
                  Angle = acos((AX * BX + AY * BY) / (Distance(0.0, 0.0, 0.0, AX, AY, 0.0) * Distance(0.0, 0.0, 0.0, BX, BY, 0.0))) * ToDegrees;
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
              Attr[NbrIndex].Known = GVELLIPSECOVERSENSORNETWORK_USELESS;
            }
          }
          ErasePotentialArea(NbrIndex, GVELLIPSECOVERSENSORNETWORK_POTENTIAL);
        }
        if (NextNode < 0)
        {
          BestHeuristic = 10000.0;
          for (i = 0; i < NumSensors; ++i)
          {
            if (Attr[i].Known == GVELLIPSECOVERSENSORNETWORK_KNOWN)
            {
              NewHeuristic = Distance(CurrentX, CurrentY, 0.0, Sensors[i].GetX(), Sensors[i].GetY(), Sensors[i].GetZ());
              if (NewHeuristic < BestHeuristic)
              {
                BestHeuristic = NewHeuristic;
                NextNode = i;
              }
            }
          }
          if (NextNode >= 0)
          {
            DestinationNode = NextNode;
            CalculateHops(DestinationNode);
            NextNode = BFSAttr[CurrentNode].Parent;
            MessageMode = GVELLIPSECOVERSENSORNETWORK_FORWARD;
            if (NextNode < 0) ReturnVal = 2;
          }
          else
          {
            ReturnVal = 0;
          }
        }
        if (GVELLIPSECOVERSENSORNETWORK_PROGRESSMESSAGE) cout << "Searching... (" << BestHoleExists << ", " << BestIndex << ", " << BestArea << ")" << endl;
      }
      if (MessageMode == GVELLIPSECOVERSENSORNETWORK_FORWARD)
      {
        NextNode = BFSAttr[CurrentNode].Parent;
        if (NextNode == DestinationNode) MessageMode = GVELLIPSECOVERSENSORNETWORK_SEARCH;
        else if (NextNode < 0) ReturnVal = 2;
        if (GVELLIPSECOVERSENSORNETWORK_PROGRESSMESSAGE) cout << "Forwarding... " << endl;
      }
    }
  }
  for (i = 0; i < NumSensors; ++i)
  {
    if (Attr[i].Known == GVELLIPSECOVERSENSORNETWORK_KNOWN)
    {
      NewArea = CalculatePotentialArea(i, GVELLIPSECOVERSENSORNETWORK_UNCOVERED);
      if (NewArea <= 0) Attr[i].Known = GVELLIPSECOVERSENSORNETWORK_USELESS;
    }
  }
  if (ReturnVal != 1)
  {
    if (GVELLIPSECOVERSENSORNETWORK_PROGRESSMESSAGE) cout << "Algorithm Comlete!" << endl;
    Initialized = 0;
  }
  return(ReturnVal);
} // UpdateSearch_GPSR



int GVEllipseCoverSensorNetwork::UpdateSearch_Heuristic(void)
{ // UpdateSearch_Heuristic
  int i = 0;
  int j = 0;
  int ReturnVal = 1;
  int NbrIndex = 0;
  int NewArea = 0;
  int NewHops = 0;
  int NewIndex = 0;
  int NewHoleExists = 0;
  int BestArea = 0;
  int BestHops = 0;
  int BestIndex = 0;
  int BestHoleExists = 0;
  int ForwardDone = 0;
  int HopDelta = 0;
  double NewHeuristic = 0.0;
  double BestHeuristic = 0.0;

  if (!Initialized) InitializeSearch();
  if ((CurrentIteration >= GVELLIPSECOVERSENSORNETWORK_MAXTRIES) || (NextNode < 0) || (NextNode >= NumSensors)) ReturnVal = 2;
  if ((ReturnVal == 1) && (Sensors != NULL) && (NumNeighbors != NULL) && (NeighborIndex != NULL) && (Attr != NULL))
  {
    BestHoleExists = Grid.GetGridSizeX() * Grid.GetGridSizeY();
    BestIndex = GVGRID2D_MAXLEVEL;
    BestArea = 0;
    BestHops = NumSensors;
    BestHeuristic = 0.0;
    ReturnVal = UpdateCurrentNode(NextNode);
    NextNode = -1;
    if (ReturnVal)
    {
      UpdateLevels();
      if (MessageMode == GVELLIPSECOVERSENSORNETWORK_SEARCH)
      {
        for (i = 0; i < NumSensors; ++i)
        {
          if (Attr[i].Known == GVELLIPSECOVERSENSORNETWORK_KNOWN)
          {
            NewArea = CalculatePotentialArea(i, GVELLIPSECOVERSENSORNETWORK_POTENTIAL);
            if (NewArea > 0)
            {
              NewHoleExists = CalculateHoleCreation(i);
              if (NewHoleExists < BestHoleExists)
              {
                CalculateHops(CurrentNode);
                BestHoleExists = NewHoleExists;
                BestArea = NewArea;
                BestHops = BFSAttr[i].Hops;
                BestIndex = CalculateIndex(i);
                BestHeuristic = CalculateHeuristic(BestArea, BestHops, BestIndex);
                NextNode = i;
              }
              else if (NewHoleExists == BestHoleExists)
              {
                CalculateHops(CurrentNode);
                NewIndex = CalculateIndex(i);
                NewHops = BFSAttr[i].Hops;
                if (((NewIndex < BestIndex) && (NewHops <= BestHops)) || ((NewIndex == BestIndex) && (NewHops < BestHops)) || ((NewIndex == BestIndex) && (NewHops == BestHops) && (NewArea > BestArea)))
                {
                  BestHoleExists = NewHoleExists;
                  BestArea = NewArea;
                  BestHops = NewHops;
                  BestIndex = NewIndex;
                  BestHeuristic = CalculateHeuristic(BestArea, BestHops, BestIndex);
                  NextNode = i;
                }
                else if (((NewIndex < BestIndex) && (NewHops > BestHops)) || ((NewIndex == BestIndex) && (NewHops > BestHops) && (NewArea > BestArea)) || ((NewIndex > BestIndex) && (NewHops < BestHops)))
                {
                  NewHeuristic = CalculateHeuristic(BestArea, BestHops, BestIndex);
                  if (NewHeuristic > BestHeuristic)
                  {
                    BestHoleExists = NewHoleExists;
                    BestArea = NewArea;
                    BestHops = NewHops;
                    BestIndex = NewIndex;
                    BestHeuristic = CalculateHeuristic(BestArea, BestHops, BestIndex);
                    NextNode = i;
                  }
                }
              }
              ErasePotentialArea(i, GVELLIPSECOVERSENSORNETWORK_POTENTIAL);
            }
            else
            {
              Attr[i].Known = GVELLIPSECOVERSENSORNETWORK_USELESS;
            }
          }
        }
        DestinationNode = NextNode;
        if (BestHops > 1) MessageMode = GVELLIPSECOVERSENSORNETWORK_FORWARD;
        if (GVELLIPSECOVERSENSORNETWORK_PROGRESSMESSAGE) cout << "Searching... (" << BestHoleExists << ", " << BestIndex << ", " << BestHops << ", " << BestArea << ")" << endl;
      }

      if (MessageMode == GVELLIPSECOVERSENSORNETWORK_FORWARD)
      {
        CalculateHops(DestinationNode);
        BestHops = BFSAttr[CurrentNode].Hops;
        BestIndex = GVGRID2D_MAXLEVEL;
        BestArea = 0;
        BestHoleExists = Grid.GetGridSizeX() * Grid.GetGridSizeY();
        NextNode = -1;
        for (i = 0; i < NumSensors; ++i)
        {
          if (Attr[i].Known == GVELLIPSECOVERSENSORNETWORK_KNOWN)
          {
            NewArea = CalculatePotentialArea(i, GVELLIPSECOVERSENSORNETWORK_UNKNOWN);
            if (NewArea <= 0) Attr[i].Known = GVELLIPSECOVERSENSORNETWORK_USELESS;
          }
        }
        for (ForwardDone = 0, i = 0; (!ForwardDone) && (i < NumNeighbors[CurrentNode]); ++i)
        {
          NbrIndex = NeighborIndex[CurrentNode][i];
          if (NbrIndex == DestinationNode)
          {
            ForwardDone = 1;
            NextNode = NbrIndex;
            MessageMode = GVELLIPSECOVERSENSORNETWORK_SEARCH;
          }
          else
          {
            if ((Attr[NbrIndex].Known == GVELLIPSECOVERSENSORNETWORK_KNOWN) && (BFSAttr[NbrIndex].Hops <= BestHops))
            {
              NewHoleExists = CalculateHoleCreation(NbrIndex);
              NewArea = CalculatePotentialArea(NbrIndex, GVELLIPSECOVERSENSORNETWORK_UNKNOWN);
              if (NewHoleExists < BestHoleExists)
              {
                BestIndex = CalculateIndex(NbrIndex);
                BestArea = NewArea;
                BestHoleExists = NewHoleExists;
                NextNode = NbrIndex;
              }
              else if (NewHoleExists == BestHoleExists)
              {
                NewIndex = CalculateIndex(NbrIndex);
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
              NextNode = BFSAttr[CurrentNode].Parent;
            }
          }
        }
        if (NextNode == DestinationNode) MessageMode = GVELLIPSECOVERSENSORNETWORK_SEARCH;
        if (GVELLIPSECOVERSENSORNETWORK_PROGRESSMESSAGE) cout << "Forwarding... " << endl;
      }
    }
  }
  for (i = 0; i < NumSensors; ++i)
  {
    if (Attr[i].Known == GVELLIPSECOVERSENSORNETWORK_KNOWN)
    {
      NewArea = CalculatePotentialArea(i, GVELLIPSECOVERSENSORNETWORK_UNCOVERED);
      if (NewArea <= 0) Attr[i].Known = GVELLIPSECOVERSENSORNETWORK_USELESS;
    }
  }
  if (ReturnVal != 1)
  {
    if (GVELLIPSECOVERSENSORNETWORK_PROGRESSMESSAGE) cout << "Algorithm Comlete!" << endl;
    Initialized = 0;
  }
  return(ReturnVal);
} // UpdateSearch_Heuristic



int GVEllipseCoverSensorNetwork::UpdateSearch(int SearchMode)
{ // UpdateSearch
  int ReturnVal = 0;

  switch(SearchMode)
  {
    case GVELLIPSECOVERSENSORNETWORK_SEARCHMODE_GPSR:
      ReturnVal = UpdateSearch_GPSR();
    break;

    case GVELLIPSECOVERSENSORNETWORK_SEARCHMODE_HEURISTIC:
    default:
      ReturnVal = UpdateSearch_Heuristic();
    break;
  }
  return(ReturnVal);
} // UpdateSearch



void GVEllipseCoverSensorNetwork::OutputResults(char* FileName)
{ // OutputResults
  int i = 0;
  int j = 0;
  int SensorsTotal = 0;
  int SensorsUnknown = 0;
  int SensorsKnown = 0;
  int SensorsVisited = 0;
  int AreaTotal = 0;
  int AreaUncovered = 0;
  int AreaCovered = 0;
  double X = 0.0;
  double Y = 0.0;
  double Z = 0.0;
  double D = 0.0;
  ofstream OutFile;

  // Calculate Sensor Information
  for (i = 0; i < NumSensors; ++i)
  {
    X = Sensors[i].GetX();
    Y = Sensors[i].GetY();
    Z = Sensors[i].GetZ();
    D = Distance(X, Y, Z, StartX, StartY, 0.0) + Distance(X, Y, Z, EndX, EndY, 0.0);
    if (D < MaxLength)
    {
      ++SensorsTotal;
      switch(Attr[i].Known)
      {
        case GVELLIPSECOVERSENSORNETWORK_UNKNOWN:
          ++SensorsUnknown;
        break;

        case GVELLIPSECOVERSENSORNETWORK_KNOWN:
        case GVELLIPSECOVERSENSORNETWORK_USELESS:
          ++SensorsKnown;
        break;

        case GVELLIPSECOVERSENSORNETWORK_VISITED:
          ++SensorsVisited;
        break;
      }
    }
  }
  // Calculate Area Information
  for (i = 0; i < GridX; ++i)
  {
    for (j = 0; j < GridY; ++j)
    {
      X = Grid.GetGridPointX(i, j, GVGRID2D_CENTERPOINT);
      Y = Grid.GetGridPointY(i, j, GVGRID2D_CENTERPOINT);
      Z = 0.0;
      D = Distance(X, Y, Z, StartX, StartY, 0.0) + Distance(X, Y, Z, EndX, EndY, 0.0);
      if (D < MaxLength)
      {
        ++AreaTotal;
        switch(Grid.GetCovered(i, j))
        {
          case GVELLIPSECOVERSENSORNETWORK_UNCOVERED:
            ++AreaUncovered;
          break;

          case GVELLIPSECOVERSENSORNETWORK_COVERED:
            ++AreaCovered;
          break;
        }
      }
    }
  }
  // Output Results
  OutFile.open(FileName, ios::app);
  OutFile << NumSensors         << GVELLIPSECOVERSENSORNETWORK_SPACE
          << Radius             << GVELLIPSECOVERSENSORNETWORK_SPACE
          << GridX              << GVELLIPSECOVERSENSORNETWORK_SPACE
          << GridY              << GVELLIPSECOVERSENSORNETWORK_SPACE
          << StartX             << GVELLIPSECOVERSENSORNETWORK_SPACE
          << StartY             << GVELLIPSECOVERSENSORNETWORK_SPACE
          << EndX               << GVELLIPSECOVERSENSORNETWORK_SPACE
          << EndY               << GVELLIPSECOVERSENSORNETWORK_SPACE
          << MaxLength          << GVELLIPSECOVERSENSORNETWORK_SPACE
          << SensorsTotal       << GVELLIPSECOVERSENSORNETWORK_SPACE
          << SensorsUnknown     << GVELLIPSECOVERSENSORNETWORK_SPACE
          << SensorsKnown       << GVELLIPSECOVERSENSORNETWORK_SPACE
          << SensorsVisited     << GVELLIPSECOVERSENSORNETWORK_SPACE
          << AreaTotal          << GVELLIPSECOVERSENSORNETWORK_SPACE
          << AreaUncovered      << GVELLIPSECOVERSENSORNETWORK_SPACE
          << AreaCovered        << GVELLIPSECOVERSENSORNETWORK_SPACE
          << NumCommunications  << GVELLIPSECOVERSENSORNETWORK_SPACE
          << endl;
  OutFile.close();
} // OutputResults
