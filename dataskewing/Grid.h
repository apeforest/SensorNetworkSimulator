////////////////////////////////////////////////////////////////////////////////
// Grid.h
// Header file for Grid class
////////////////////////////////////////////////////////////////////////////////



#ifndef GRID_H
#define GRID_H



// Includes ////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include "GridSquare.h"



// Grid ////////////////////////////////////////////////////////////////////////
class Grid
{ // Grid
  public:
    // Constructors And Destructors
    Grid();
    ~Grid();

    // Member Functions
    void Init(int NumColumns, int NumRows, double X1, double Y1, double X2, double Y2);
    void Deinit(void);
    void SetOffset(int Column, int Row, double ValueX, double ValueY);
    int GetRows(void);
    int GetColumns(void);
    double GetOffsetX(int Column, int Row);
    double GetOffsetY(int Column, int Row);
    double GetCenterPointX(int Column, int Row);
    double GetCenterPointY(int Column, int Row);
    double GetLX(int Column, int Row);
    double GetLY(int Column, int Row);
    double GetUX(int Column, int Row);
    double GetUY(int Column, int Row);
    double GetRed(void);
    double GetGreen(void);
    double GetBlue(void);
    double GetHighlightRed(void);
    double GetHighlightGreen(void);
    double GetHighlightBlue(void);

  protected:
    int Columns;
    int Rows;
    double XL;
    double YL;
    double XU;
    double YU;
    double Red;
    double Green;
    double Blue;
    double HighlightRed;
    double HighlightGreen;
    double HighlightBlue;
    GridSquare* GridArray;

    int GetIndex(int Column, int Row);
}; // Grid



#endif