////////////////////////////////////////////////////////////////////////////////
// TrilaterationPoint.cpp
// Implementation file for TrilaterationPoint class
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "TrilaterationPoint.h"
#include "GaussianUniformErrorModule.h"



// TrilaterationPoint //////////////////////////////////////////////////////////
TrilaterationPoint::TrilaterationPoint()
{ // TrilaterationPoint
  X = 0.0;
  Y = 0.0;
  Z = 0.0;
  PX = 0.0;
  PY = 0.0;
  PZ = 0.0;
  Red = 0.0;
  Green = 0.0;
  Blue = 0.0;
  ActualRadius = 0.0;
  ReportedRadius = 0.0;
  Offset = 0.0;
  InitialOffset = 0.0;
  MeasurementError = NULL;
} // TrilaterationPoint



TrilaterationPoint::~TrilaterationPoint()
{
  Deinit();
}



void TrilaterationPoint::Init(double CX, double CY, double CZ, ErrorModule* ME)
{ // Init
  Deinit();
  X = CX;
  Y = CY;
  Z = CZ;
  PX = 0.0;
  PY = 0.0;
  PZ = 0.0;
  Red = 0.0;
  Green = 0.0;
  Blue = 0.0;
  ActualRadius = 0.0;
  ReportedRadius = 0.0;
  Offset = 0.0;
  InitialOffset = 0.0;
  MeasurementError = ME;
} // Init



void TrilaterationPoint::Deinit(void)
{ // Deinit
  X = 0.0;
  Y = 0.0;
  Z = 0.0;
  PX = 0.0;
  PY = 0.0;
  PZ = 0.0;
  Red = 0.0;
  Green = 0.0;
  Blue = 0.0;
  ActualRadius = 0.0;
  ReportedRadius = 0.0;
  Offset = 0.0;
  InitialOffset = 0.0;
  MeasurementError = NULL;
} // Deinit



void TrilaterationPoint::SetMeasuredPoint(double MX, double MY, double MZ)
{ // SetMeasuredPoint
  PX = MX;
  PY = MY;
  PZ = MZ;
  ActualRadius = Distance(X, Y, Z, PX, PY, PZ);
  ReportedRadius = ActualRadius * (1.0 + MeasurementError->GenerateError());
} // SetMeasuredPoint



void TrilaterationPoint::SetOffset(double O)
{ // GetOffset
  Offset = O;
} // GetOffset



double TrilaterationPoint::GetOffset(void)
{ // GetOffset
  return(Offset);
} // GetOffset



double TrilaterationPoint::GetPointX(void)
{ // GetPointX
  return(X);
} // GetPointX



double TrilaterationPoint::GetPointY(void)
{ // GetPointY
  return(Y);
} // GetPointY



double TrilaterationPoint::GetPointZ(void)
{ // GetPointZ
  return(Z);
} // GetPointZ



double TrilaterationPoint::GetMeasuredRadius(void)
{ // GetMeasuredRadius
  return(ReportedRadius);
} // GetMeasuredRadius



double TrilaterationPoint::Distance(double X1, double Y1, double Z1, double X2, double Y2, double Z2)
{ // Distance
  double DX = X1 - X2;
  double DY = Y1 - Y2;
  double DZ = Z1 - Z2;

  return(sqrt(DX*DX + DY*DY + DZ*DZ));
} // Distance



void TrilaterationPoint::SetColor(double R, double G, double B)
{ // SetColor
  if (R > 1.0) Red = 1.0; else if (R < 0.0) Red = 0.0; else Red = R;
  if (G > 1.0) Green = 1.0; else if (G < 0.0) Green = 0.0; else Green = G;
  if (B > 1.0) Blue = 1.0; else if (B < 0.0) Blue = 0.0; else Blue = B;
} // SetColor



double TrilaterationPoint::GetRed(void)
{ // GetRed
  return(Red);
} // GetRed



double TrilaterationPoint::GetGreen(void)
{ // GetGreen
  return(Green);
} // GetGreen



double TrilaterationPoint::GetBlue(void)
{ // GetBlue
  return(Blue);
} // GetBlue



double TrilaterationPoint::GetProbability(double Value)
{ // GetProbability
  if (MeasurementError == NULL)
  {
    MeasurementError = new GaussianUniformErrorModule();
    MeasurementError->Init(0.0, 0.01);
  }
  return(MeasurementError->GetProbability(Value * (ActualRadius / ReportedRadius)));
} // GetProbability



double TrilaterationPoint::GetActualRadius(void)
{ // GetActualRadius
  return(ActualRadius);
} // GetActualRadius



void TrilaterationPoint::SetRadius(double AR, double MR)
{ // SetRadius
  ReportedRadius = MR;
  ActualRadius = AR;
} // SetRadius