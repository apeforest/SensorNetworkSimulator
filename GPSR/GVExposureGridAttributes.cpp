////////////////////////////////////////////////////////////////////////////////
// GVExposureGridAttributes.cpp                                               //
//                                                                            //
// Implementation file for the attributes an exposure grid.                   //
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "GVExposureGridAttributes.h"



// GVExposureGridAttributes ////////////////////////////////////////////////////
GVExposureGridAttributes::GVExposureGridAttributes()
/*
  Creates a new attribute (coincidentally like ~GVExposureGridAttributes()).
*/
{ // GVExposureGridAttributes
  NearestSensor = GVEXPOSUREGRIDATTRIBUTES_DEFAULT_NEARESTSENSOR;
  BorderLevel = GVEXPOSUREGRIDATTRIBUTES_DEFAULT_BORDERLEVEL;
  Covered = GVEXPOSUREGRIDATTRIBUTES_DEFAULT_COVERED;
  ExposureValue = GVEXPOSUREGRIDATTRIBUTES_DEFAULT_EXPOSUREVALUE;
  NormalizedExposureValue = GVEXPOSUREGRIDATTRIBUTES_DEFAULT_NORMALIZEDEXPOSUREVALUE;
} // GVExposureGridAttributes



GVExposureGridAttributes::~GVExposureGridAttributes()
/*
  Deletes an attribute.
*/
{ // ~GVExposureGridAttributes
  NearestSensor = GVEXPOSUREGRIDATTRIBUTES_DEFAULT_NEARESTSENSOR;
  BorderLevel = GVEXPOSUREGRIDATTRIBUTES_DEFAULT_BORDERLEVEL;
  Covered = GVEXPOSUREGRIDATTRIBUTES_DEFAULT_COVERED;
  ExposureValue = GVEXPOSUREGRIDATTRIBUTES_DEFAULT_EXPOSUREVALUE;
  NormalizedExposureValue = GVEXPOSUREGRIDATTRIBUTES_DEFAULT_NORMALIZEDEXPOSUREVALUE;
} // ~GVExposureGridAttributes



void GVExposureGridAttributes::SetEnqueued(int E)
/*
  SetEnqueued() sets the Enqueued value to E.
*/
{ // SetEnqueued
  Enqueued = E;
} // SetEnqueued



void GVExposureGridAttributes::SetNearestSensor(int NS)
/*
  SetNearestSensor() sets the nearest sensor to NS.
*/
{ // SetNearestSensor
  NearestSensor = NS;
} // SetNearestSensor



void GVExposureGridAttributes::SetBorderLevel(int BL)
/*
  SetBorderLevel() sets the border level to BL.
*/
{ // SetBorderLevel
  BorderLevel = BL;
} // SetBorderLevel



void GVExposureGridAttributes::SetCovered(int C)
/*
  SetCovered() sets the covered attribute to C.
*/
{ // SetCovered
  Covered = C;
} // SetCovered



void GVExposureGridAttributes::SetCost(double C)
/*
  SetCost() sets the cost attribute to C.
*/
{ // SetCost
  Cost = C;
} // SetCost



void GVExposureGridAttributes::SetExposureValue(double EV)
/*
  SetExposureValue() sets the exposure value to EV.
*/
{ // SetExposureValue
  ExposureValue = EV;
} // SetExposureValue



void GVExposureGridAttributes::SetNormalizedExposureValue(double NEV)
/*
  SetNormalizedExposureValue() sets the normalized exposure value to NEV.
*/
{ // SetNormalizedExposureValue
  NormalizedExposureValue = NEV;
} // SetNormalizedExposureValue



void GVExposureGridAttributes::SetParent(int i, int j)
/*
  SetParent() sets the parent of a grid square to (i, j)
*/
{ // SetParent
  Parent[GVEXPOSUREGRIDATTRIBUTES_DEFAULT_IPARENT] = i;
  Parent[GVEXPOSUREGRIDATTRIBUTES_DEFAULT_JPARENT] = j;
} // SetParent



int GVExposureGridAttributes::GetEnqueued(void)
/*
  GetEnqueued() returns the Enqueued attribute.
*/
{ // GetEnqueued
  return(Enqueued);
} // GetEnqueued



int GVExposureGridAttributes::GetParentH(void)
/*
  GetParentH() gets the horziontal parent index.
*/
{ // GetParentH
  return(Parent[GVEXPOSUREGRIDATTRIBUTES_DEFAULT_IPARENT]);
} // GetParentH



int GVExposureGridAttributes::GetParentV(void)
/*
  GetParentV() gets the horziontal parent index.
*/
{ // GetParentV
  return(Parent[GVEXPOSUREGRIDATTRIBUTES_DEFAULT_JPARENT]);
} // GetParentV



int GVExposureGridAttributes::GetNearestSensor(void)
/*
  GetNearestSensor() returns the nearest sensor.
*/
{ // GetNearestSensor
  return(NearestSensor);
} // GetNearestSensor



int GVExposureGridAttributes::GetBorderLevel(void)
/*
  GetBorderLevel() returns the border level.
*/
{ // GetBorderLevel
  return(BorderLevel);
} // GetBorderLevel



int GVExposureGridAttributes::GetCovered(void)
/*
  GetCovered() returns the covered attribute.
*/
{ // GetCovered
  return(Covered);
} // GetCovered



double GVExposureGridAttributes::GetCost(void)
/*
  GetCost() returns the cost attribute.
*/
{ // GetCost
  return(Cost);
} // GetCost



double GVExposureGridAttributes::GetExposureValue(void)
/*
  GetExposureValue() returns the exposure value.
*/
{ // GetExposureValue
  return(ExposureValue);
} // GetExposureValue



double GVExposureGridAttributes::GetNormalizedExposureValue(void)
/*
  GetNormalizeExposureValue() returns the normalized exposure value.
*/
{ // GetNormalizedExposureValue
  return(NormalizedExposureValue);
} // GetNormalizedExposureValue
