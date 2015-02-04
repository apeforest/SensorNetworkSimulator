////////////////////////////////////////////////////////////////////////////////
// Point.h
// Header file for Point class.
////////////////////////////////////////////////////////////////////////////////



#ifndef POINT_H
#define POINT_H



// Point ///////////////////////////////////////////////////////////////////////
class Point
{ // Point
  public:
    // Constructors And Destructors
    Point();
    ~Point();

    // Member Functions
    void Init(double PX, double PY, double PZ);
    void Deinit(void);
    void SetX(double PX);
    void SetY(double PY);
    void SetZ(double PZ);
    double GetX(void);
    double GetY(void);
    double GetZ(void);

  private:
    double X;
    double Y;
    double Z;
}; // Point



#endif