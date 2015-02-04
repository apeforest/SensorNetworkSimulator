////////////////////////////////////////////////////////////////////////////////
// TrilaterationPoint.h
// Header file for TrilaterationPoint class
////////////////////////////////////////////////////////////////////////////////



#ifndef TRILATERATIONPOINT_H
#define TRILATERATIONPOINT_H



// Includes ////////////////////////////////////////////////////////////////////
#include "ErrorModule.h"
#include <math.h>



// TrilaterationPoint //////////////////////////////////////////////////////////
class TrilaterationPoint
{ // TrilaterationPoint
  public:
    // Constructors and Deconstructors
    TrilaterationPoint();
    ~TrilaterationPoint();

    // Member Functions
    void Init(double CX, double CY, double CZ, ErrorModule* ME);
    void Deinit(void);
    void SetMeasuredPoint(double MX, double MY, double MZ);
    void SetOffset(double O);
    void SetRadius(double AR, double MR);
    void SetColor(double R, double G, double B);
    double GetOffset(void);
    double GetPointX(void);
    double GetPointY(void);
    double GetPointZ(void);
    double GetRed(void);
    double GetGreen(void);
    double GetBlue(void);
    double GetMeasuredRadius(void);
    double GetActualRadius(void);
    double GetProbability(double Value);

  protected:
    double X;                       // X Coordinate Of Trilateration Point
    double Y;                       // Y Coordinate Of Trilateration Point
    double Z;                       // Z Coordinate Of Trilateration Point
    double PX;                      // X Coordinate Of Measured Point
    double PY;                      // Y Coordinate Of Measured Point
    double PZ;                      // Z Coordinate Of Measured Point
    double Red;                     // Red Color
    double Green;                   // Green Color
    double Blue;                    // Blue Color
    double ActualRadius;            // Actual Radius To Measured Point
    double ReportedRadius;          // Radius To Measured Point With Errors Applied
    double Offset;                  // Offset For Radius (Used In Animation)
    double InitialOffset;           // Initial Offset For Radius (Used In Animation)
    ErrorModule* MeasurementError;  // Used To Generated Measurement Error And Probabilities

    double Distance(double X1, double Y1, double Z1, double X2, double Y2, double Z2);
}; // TrilaterationPoint



#endif