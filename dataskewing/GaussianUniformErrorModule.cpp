////////////////////////////////////////////////////////////////////////////////
// ErrorModule.cpp
// Implementation file for ErrorModule class
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "GaussianUniformErrorModule.h"



// ErrorModule /////////////////////////////////////////////////////////////////
GaussianUniformErrorModule::GaussianUniformErrorModule()
{ // ErrorModule
  Mean = 0;
  StandardDeviation = 0;
} // ErrorModule



GaussianUniformErrorModule::~GaussianUniformErrorModule()
{ // ErrorModule
  Deinit();
} // ErrorModule



double GaussianUniformErrorModule::GetProbability(double Value)
{ // GetStandardDeviation
  return(1.0 / sqrt(GAUSSIANUNIFORMERRORMODULE_TWOPI * StandardDeviation * StandardDeviation) * pow(GAUSSIANUNIFORMERRORMODULE_E, -(Value - Mean)*(Value - Mean) / (2 * StandardDeviation * StandardDeviation)));
} // GetStandardDeviation
