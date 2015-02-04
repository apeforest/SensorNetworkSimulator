////////////////////////////////////////////////////////////////////////////////
// ErrorModule.cpp
// Implementation file for ErrorModule class
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "ErrorModule.h"



// ErrorModule /////////////////////////////////////////////////////////////////
ErrorModule::ErrorModule()
{ // ErrorModule
  Mean = 0;
  StandardDeviation = 0;
} // ErrorModule



ErrorModule::~ErrorModule()
{ // ErrorModule
  Deinit();
} // ErrorModule



void ErrorModule::Init(double M, double STD)
{ // Init
  Deinit();
  Mean = M;
  StandardDeviation = STD;
} // Init



void ErrorModule::Deinit(void)
{ // Deinit
  Mean = 0;
  StandardDeviation = 0;
} // Deinit



double ErrorModule::GetMean(void)
{ // GetMean
  return(Mean);
} // GetMean



double ErrorModule::GetStandardDeviation(void)
{ // GetStandardDeviation
  return(StandardDeviation);
} // GetStandardDeviation



double ErrorModule::GetProbability(double Value)
{ // GetStandardDeviation
  double ReturnProbability = 0;

  if (fabs(Value - Mean) < StandardDeviation)
  {
    ReturnProbability = 1.0 / (2.0 * StandardDeviation);
  }
  return ReturnProbability;
} // GetStandardDeviation



double ErrorModule::GenerateError(void)
{ // GenerateError
  double Error = ((double) ((rand() % (ERRORMODULE_ACCURACY << 1 + 1)) - ERRORMODULE_ACCURACY)) / ((double) ERRORMODULE_ACCURACY);
  return(Error * StandardDeviation + Mean);
} // GenerateError
