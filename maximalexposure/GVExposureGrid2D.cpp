////////////////////////////////////////////////////////////////////////////////
// GVExposureGrid2D.cpp                                                       //
//                                                                            //
// Implementation file for two-dimensional grid with exposure attributes.     //
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "GVExposureGrid2D.h"


// GVExposureGrid2D ////////////////////////////////////////////////////////////
GVExposureGrid2D::GVExposureGrid2D()
/*
  GVExposure2D() creates an empty exposure grid.
*/
{ // GVExposure2D
  GVGrid2D::GVGrid2D();
  MinHorizontal = 0;
  MinVertical = 0;
  MaxHorizontal = 0;
  MaxVertical = 0;
  RadiusHorizontal = 0;
  RadiusVertical = 0;
  Attributes = NULL;

  Delete();
} // GVExposure2D



GVExposureGrid2D::~GVExposureGrid2D()
/*
  ~GVExposure2D() deletes an exposure grid.
*/
{ // ~GVExposureGrid2D
  Delete();
} // ~GVExposureGrid2D



void GVExposureGrid2D::New(int NH, int NV, double SX, double SY, double HX, double HY, double VX, double VY, double R)
/*
  New() creates a new two-dimensional grid and then sets the properties for
  the exposure grid, including allocating memory for the attributes.
*/
{ // New
  int i = 0;                               // Index Variable
  double H = Distance2D(0.0, 0.0, HX, HY); // Length of Horizontal Vector
  double V = Distance2D(0.0, 0.0, VX, VY); // Length of Vertical Vector

  Delete();
  GVGrid2D::New(NH, NV, SX, SY, HX, HY, VX, VY);
  MinHorizontal = 0;
  MinVertical = 0;
  MaxHorizontal = SizeH - 1;
  MaxVertical = SizeV - 1;
  RadiusHorizontal = ((int) (R / H)) + 3;
  RadiusVertical = ((int) (R / V)) + 3;
  Attributes = new GVExposureGridAttributes* [SizeH];
  for (i = 0; i < SizeH; ++i)
  {
    Attributes[i] = new GVExposureGridAttributes[SizeV];
  }
} // New



void GVExposureGrid2D::Delete()
/*
  Delete() deletes an exposure grid and the underlying two-dimensional grid.
*/
{ // Delete
  int i = 0; // Index Variable

  if (Attributes != NULL)
  {
    for (i = 0; i < SizeH; ++i)
    {
      delete[] Attributes[i];
    }
    delete[] Attributes;
  }
  MinHorizontal = 0;
  MinVertical = 0;
  MaxHorizontal = 0;
  MaxVertical = 0;
  RadiusHorizontal = 0;
  RadiusVertical = 0;
  Attributes = NULL;
  GVGrid2D::Delete();
} // Delete



int GVExposureGrid2D::GetMinHorizontal(void)
/*
  GetMinHorizontal() returns the minimum horizontal value.
*/
{ // GetMinHorizontal
  return(MinHorizontal);
} // GetMinHorizontal



int GVExposureGrid2D::GetMinVertical(void)
/*
  GetMinVertical() returns the minimum vertical value.
*/
{ // GetMinVertical
  return(MinVertical);
} // GetMinVertical



int GVExposureGrid2D::GetMaxHorizontal(void)
/*
  GetMaxHorizontal() returns the maximum horizontal value.
*/
{ // GetMaxHorizontal
  return(MaxHorizontal);
} // GetMaxHorizontal



int GVExposureGrid2D::GetMaxVertical(void)
/*
  GetMaxVertical() returns the maximum vertical value.
*/
{ // GetMaxVertical
  return(MaxVertical);
} // GetMaxVertical



int GVExposureGrid2D::GetRadiusHorizontal(void)
/*
  GetRadiusHorizontal() returns the horizontal radius value.
*/
{ // GetRadiusHorizontal
  return(RadiusHorizontal);
} // GetRadiusHorizontal



int GVExposureGrid2D::GetRadiusVertical(void)
/*
  GetRadiusVertical() returns the vertical radius value.
*/
{ // GetRadiusVertical
  return(RadiusVertical);
} // GetRadiusVertical



int GVExposureGrid2D::GetEnqueued(int i, int j)
/*
  GetEnqueued() returns the enqueued attribute for grid square (i, j).
*/
{ // GetEnqueued
  int ReturnVal = 0; // Return Value

  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeV)) ReturnVal = Attributes[i][j].GetEnqueued();
  return(ReturnVal);
} // GetEnqueued



int GVExposureGrid2D::GetParentH(int i, int j)
/*
  GetParentH() returns the horizontal index of the parent of the
  grid square (i, j).
*/
{ // GetParentH
  int ReturnVal = 0; // Return Value

  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeV)) ReturnVal = Attributes[i][j].GetParentH();
  return(ReturnVal);
} // GetParentH



int GVExposureGrid2D::GetParentV(int i, int j)
/*
  GetParentV() returns the horizontal index of the parent of the
  grid square (i, j).
*/
{ // GetParentV
  int ReturnVal = 0; // Return Value

  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeV)) ReturnVal = Attributes[i][j].GetParentV();
  return(ReturnVal);
} // GetParentV



int GVExposureGrid2D::GetNearestSensor(int i, int j)
/*
  GetNearestSensor() returns the nearest sensor for grid square (i, j).
*/
{ // GetNearestSensor
  int ReturnVal = 0; // Return Value

  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeV)) ReturnVal = Attributes[i][j].GetNearestSensor();
  return(ReturnVal);
} // GetNearestSensor



int GVExposureGrid2D::GetBorderLevel(int i, int j)
/*
  GetBorderLevel() returns the border level for grid square (i, j).
*/
{ // GetBorderLevel
  int ReturnVal = GVEXPOSUREGRIDATTRIBUTES_MAXLEVEL; // Return Value

  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeV)) ReturnVal = Attributes[i][j].GetBorderLevel();
  return(ReturnVal);
} // GetBorderLevel



int GVExposureGrid2D::GetCovered(int i, int j)
/*
  GetCovered() returns the covered attribute for the grid square (i, j).
*/
{ // GetCovered
  int ReturnVal = 0; // Return Value

  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeV)) ReturnVal = Attributes[i][j].GetCovered();
  return(ReturnVal);
} // GetCovered



double GVExposureGrid2D::GetCost(int i, int j)
/*
  GetCost() returns the cost attribute for the grid square (i, j).
*/
{ // GetCost
  double ReturnVal = 0.0;

  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeV)) ReturnVal = Attributes[i][j].GetCost();
  return(ReturnVal);
} // GetCost



double GVExposureGrid2D::GetExposureValue(int i, int j)
/*
  GetExposureValue() gets the exposure value for grid square (i, j).
*/
{ // GetExposureValue
  double ReturnVal = 0; // Return Value

  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeV)) ReturnVal = Attributes[i][j].GetExposureValue();
  return(ReturnVal);
} // GetExposureValue



double GVExposureGrid2D::GetNormalizedExposureValue(int i, int j)
/*
  GetNormalizedExposureValue() returns the normalized exposure value for the grid square (i, j).
*/
{ // GetNormalizedExposureValue
  double ReturnVal = 0.0; // Return Value

  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeV)) ReturnVal = Attributes[i][j].GetNormalizedExposureValue();
  return(ReturnVal);
} // GetNormalizedExposureValue



void GVExposureGrid2D::SetMinHorizontal(int MH)
/*
  SetMinHorizontal() sets the minimum horizontal value if valid.
*/
{ // SetMinHorizontal
  if ((MH >= 0) && (MH < SizeH)) MinHorizontal = MH;
} // SetMinHorizontal



void GVExposureGrid2D::SetMinVertical(int MV)
/*
  SetMinVertical() sets the minimum vertical value if valid.
*/
{ // SetMinVertical
  if ((MV >= 0) && (MV < SizeH)) MinVertical = MV;
} // SetMinVertical



void GVExposureGrid2D::SetMaxHorizontal(int MH)
/*
  SetMaxHorizontal() sets the maximum horizontal value if valid.
*/
{ // SetMaxHorizontal
  if ((MH >= 0) && (MH < SizeH)) MaxHorizontal = MH;
} // SetMaxHorizontal



void GVExposureGrid2D::SetMaxVertical(int MV)
/*
  SetMaxVertical() sets the maximum vertical value if valid.
*/
{ // SetMaxVertical
  if ((MV >= 0) && (MV < SizeH)) MaxVertical = MV;
} // SetMaxVertical



void GVExposureGrid2D::SetRadiusHorizontal(int RH)
/*
  SetRadiusHorizontal() sets the radius horizontal value if valid.
*/
{ // SetRadiusHorizontal
  if (RH >= 0) RadiusHorizontal = RH;
} // SetRadiusHorizontal



void GVExposureGrid2D::SetRadiusVertical(int RV)
/*
  SetRadiusVertical() sets the radius vertical value if valid.
*/
{ // SetRadiusVertical
  if (RV >= 0) RadiusHorizontal = RV;
} // SetRadiusVertical



void GVExposureGrid2D::SetParent(int i, int j, int H, int V)
/*
  SetParent() sets the parent of grid square (i, j) to (H, V).
*/
{ // SetParent
  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeH)) Attributes[i][j].SetParent(H, V);
} // SetParent



void GVExposureGrid2D::SetEnqueued(int i, int j, int E)
/*
  SetEnqueued() sets the enqueued attribute of grid square (i, j) to E.
*/
{ // SetEnqueued
  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeH)) Attributes[i][j].SetEnqueued(E);
} // SetEnqueued



void GVExposureGrid2D::SetNearestSensor(int i, int j, int NS)
/*
  GetNearestSensor() sets the nearest sensor of grid square (i, j) to NS.
*/
{ // GetNearestSensor
  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeH)) Attributes[i][j].SetNearestSensor(NS);
} // GetNearestSensor



void GVExposureGrid2D::SetCost(int i, int j, double C)
/*
  SetCost() sets the cost of the grid square to C
*/
{ // SetCost
  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeH)) Attributes[i][j].SetCost(C);
} // SetCost



void GVExposureGrid2D::SetBorderLevel(int i, int j, int BL)
/*
  GetBorderLevel() sets the border level of grid square (i, j) to BL.
*/
{ // GetBorderLevel
  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeH)) Attributes[i][j].SetBorderLevel(BL);
} // GetBorderLevel



void GVExposureGrid2D::SetCovered(int i, int j, int C)
/*
  GetCovered() sets the covered attribute of grid square (i, j) to C.
*/
{ // GetCovered
  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeH)) Attributes[i][j].SetCovered(C);
} // GetCovered



void GVExposureGrid2D::SetExposureValue(int i, int j, double EV)
/*
  GetExposureValue() sets the exposure value of grid square (i, j) to EV.
*/
{ // GetExposureValue
  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeH)) Attributes[i][j].SetExposureValue(EV);
} // GetExposureValue



void GVExposureGrid2D::SetNormalizedExposureValue(int i, int j, double NEV)
/*
  GetNormalizedExposureValue() sets the normalized exposure value of grid square (i, j) to NEV.
*/
{ // GetNormalizedExposureValue
  if ((i >= 0) && (i < SizeH) && (j >= 0) && (j < SizeH)) Attributes[i][j].SetNormalizedExposureValue(NEV);
} // GetNormalizedExposureValue