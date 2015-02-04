////////////////////////////////////////////////////////////////////////////////
// GVUtil.h                                                                   //
//                                                                            //
// Standard Utility Functions for Classes.                                    //
////////////////////////////////////////////////////////////////////////////////



#ifndef GVUTIL_H
#define GVUTIL_H



// Includes ////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <math.h>



// Defines /////////////////////////////////////////////////////////////////////
#define GVUTIL_ACCURACY 10000



// Prototypes //////////////////////////////////////////////////////////////////
double GenerateRandomDouble(double LowerBound, double UpperBound);
double Distance2D(double X1, double Y1, double X2, double Y2);
double Distance3D(double X1, double Y1, double Z1, double X2, double Y2, double Z2);
int CoinToss(double p);
double LOGN(double R, int N);


#endif