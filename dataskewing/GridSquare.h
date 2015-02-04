////////////////////////////////////////////////////////////////////////////////
// GridSquare.h
// Header file for GridSquare class
////////////////////////////////////////////////////////////////////////////////



#ifndef GRIDSQUARE_H
#define GRIDSQUARE_H



// GridSquare //////////////////////////////////////////////////////////////////
class GridSquare
{ // GridSquare
  public:
    // Constructor And Deconstructor
    GridSquare();
    ~GridSquare();

    void Init(double CX, double CY, double X1, double Y1, double X2, double Y2, double OX, double OY);
    void Deinit(void);
    void SetCenterPoint(double CX, double CY);
    void SetOffset(double OX, double OY);
    double GetCenterPointX(void);
    double GetCenterPointY(void);
    double GetLX(void);
    double GetLY(void);
    double GetUX(void);
    double GetUY(void);
    double GetOffsetX(void);
    double GetOffsetY(void);

  protected:
    double XL;            // Lower X-Coordinate
    double YL;            // Lower Y-Coordinate
    double XU;            // Upper X-Coordinate
    double YU;            // Upper Y-Coordinate
    double CenterPointX;  // X-Point About Which Square Is Drawn
    double CenterPointY;  // Y-Point About Which Square Is Drawn
    double OffsetX;       // Offset Of CenterPoint
    double OffsetY;       // Offset Of CenterPoint
    double InitialOffset; // Initial Offset Of CenterPoint
}; // GridSquare



#endif