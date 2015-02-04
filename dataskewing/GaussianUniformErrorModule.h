////////////////////////////////////////////////////////////////////////////////
// GaussianUniformErrorModule.h
// Header file for GaussianUniform ErrorModule class
////////////////////////////////////////////////////////////////////////////////



#ifndef GAUSSIANUNIFORMERRORMODULE_H
#define GAUSSIANUNIFORMERRORMODULE_H



// Includes ////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <math.h>
#include "ErrorModule.h"



// Includes ////////////////////////////////////////////////////////////////////
#define GAUSSIANUNIFORMERRORMODULE_TWOPI 6.28318530717958647692528
#define GAUSSIANUNIFORMERRORMODULE_E     2.7182818284590



// GaussianUniformErrorModule //////////////////////////////////////////////////
class GaussianUniformErrorModule : public ErrorModule
{ // GaussianUniformErrorModule
  public:
    // Constructor And Destructor
    GaussianUniformErrorModule();
    ~GaussianUniformErrorModule();

    // Member Functions (Polymorphic)
    double GetProbability(double Value);
}; // GaussianUniformErrorModule



#endif