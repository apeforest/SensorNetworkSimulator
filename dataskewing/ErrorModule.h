////////////////////////////////////////////////////////////////////////////////
// ErrorModule.h
// Header file for ErrorModule class
////////////////////////////////////////////////////////////////////////////////



#ifndef ERRORMODULE_H
#define ERRORMODULE_H



// Includes ////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <math.h>



// Defines /////////////////////////////////////////////////////////////////////
#define ERRORMODULE_ACCURACY 10000



// ErrorModule /////////////////////////////////////////////////////////////////
class ErrorModule
{ // ErrorModule
  public:
    // Constructor And Destructor
    ErrorModule();
    ~ErrorModule();

    // Member Functions
    void Init(double M, double STD);
    void Deinit(void);
    double GetMean(void);
    double GetStandardDeviation(void);
    double GetProbability(double Value);
    double GenerateError(void);

  protected:
    double Mean;
    double StandardDeviation;
}; // ErrorModule



#endif
