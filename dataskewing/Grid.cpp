////////////////////////////////////////////////////////////////////////////////
// Grid.cpp
// Implementation file for Grid class
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include "Grid.h"



// Grid ////////////////////////////////////////////////////////////////////////
Grid::Grid()
{ // Grid
  GridArray = NULL;
  Deinit();
} // Grid



Grid::~Grid()
{ // ~Grid
  Deinit();
} // ~Grid



void Grid::Init(int NumColumns, int NumRows, double X1, double Y1, double X2, double Y2)
{ // Init
  int i;     // Index Variable
  int j;     // Index Variable
  double DX; // Horizontal Size of Each Grid Square
  double DY; // Vertical Size Of Each Grid Square


  Deinit();
  Red = 0.0;
  Green = 0.0;
  Blue = 0.0;
  HighlightRed = 0.0;
  HighlightGreen = 0.0;
  HighlightBlue = 0.0;
  if ((NumColumns > 0) && (NumRows > 0) && (X2 > X1) && (Y2 > Y1))
  {
    Columns = NumColumns;
    Rows = NumRows;
    XL = X1;
    YL = Y1;
    XU = X2;
    YU = Y2;
    GridArray = new GridSquare[Columns * Rows];

    // Initialize Each Grid Square
    DX = ((double) (XU - XL)) / ((double) Columns);
    DY = ((double) (YU - YL)) / ((double) Rows);
    for (j = 0; j < Rows; ++j)
    {
      for (i = 0; i < Columns; ++i)
      {
        GridArray[GetIndex(i, j)].Init(XL + (i * DX) + (DX / 2.0), YL + (j * DY) + (DY / 2.0), XL + (i * DX), YL + (j * DY), XL + ((i + 1) * DX), YL + ((j + 1) * DY), 0.0, 0.0);
        GridArray[GetIndex(i, j)].SetOffset(-((i * 2 + 1) * DX), -((j * 2 + 1) * DY));
      }
    }
  }
} // Init



void Grid::Deinit(void)
{ // Deinit
  Columns = 0;
  Rows = 0;
  XL = 0.0;
  YL = 0.0;
  XU = 0.0;
  YU = 0.0;
  if (GridArray != NULL) delete[] GridArray;
  GridArray = NULL;
} // Deinit



int Grid::GetRows(void)
{ // GetRows
  int ReturnVal = 0;

  if (GridArray != NULL) ReturnVal = Rows;
  return(ReturnVal);
} // GetRows



int Grid::GetColumns(void)
{ // GetColumns
  int ReturnVal = 0;

  if (GridArray != NULL) ReturnVal = Columns;
  return(ReturnVal);
} // GetColumns



double Grid::GetOffsetX(int Column, int Row)
{ // GetColumns
  double ReturnVal = 0.0;

  if (GridArray != NULL) ReturnVal = GridArray[GetIndex(Column, Row)].GetOffsetX();
  return(ReturnVal);
} // GetColumns



double Grid::GetOffsetY(int Column, int Row)
{ // GetColumns
  double ReturnVal = 0.0;

  if (GridArray != NULL) ReturnVal = GridArray[GetIndex(Column, Row)].GetOffsetY();
  return(ReturnVal);
} // GetColumns



void Grid::SetOffset(int Column, int Row, double ValueX, double ValueY)
{ // GetColumns
  if (GridArray != NULL) GridArray[GetIndex(Column, Row)].SetOffset(ValueX, ValueY);
} // GetColumns



int Grid::GetIndex(int Column, int Row)
{ // GetIndex
  return(Row * Columns + Column);
} // GetIndex



double Grid::GetCenterPointX(int Column, int Row)
{ // GetCenterPointX
  double ReturnVal = 0.0;

  if (GridArray != NULL) ReturnVal = GridArray[GetIndex(Column, Row)].GetCenterPointX();
  return(ReturnVal);
} // GetCenterPointX



double Grid::GetLX(int Column, int Row)
{ // GetLX
  double ReturnVal = 0.0;

  if (GridArray != NULL) ReturnVal = GridArray[GetIndex(Column, Row)].GetLX();
  return(ReturnVal);
} // GetLX



double Grid::GetLY(int Column, int Row)
{ // GetLY
  double ReturnVal = 0.0;

  if (GridArray != NULL) ReturnVal = GridArray[GetIndex(Column, Row)].GetLY();
  return(ReturnVal);
} // GetLY



double Grid::GetUX(int Column, int Row)
{ // GetUX
  double ReturnVal = 0.0;

  if (GridArray != NULL) ReturnVal = GridArray[GetIndex(Column, Row)].GetUX();
  return(ReturnVal);
} // GetUX



double Grid::GetUY(int Column, int Row)
{ // GetUY
  double ReturnVal = 0.0;

  if (GridArray != NULL) ReturnVal = GridArray[GetIndex(Column, Row)].GetUY();
  return(ReturnVal);
} // GetUY



double Grid::GetCenterPointY(int Column, int Row)
{ // GetCenterPointY
  double ReturnVal = 0.0;

  if (GridArray != NULL) ReturnVal = GridArray[GetIndex(Column, Row)].GetCenterPointY();
  return(ReturnVal);
} // GetCenterPointY



double Grid::GetRed(void)
{ // Get Red
  return(Red);
} // Get Red



double Grid::GetGreen(void)
{ // GetGreen
  return(Green);
} // GetGreen



double Grid::GetBlue(void)
{ // GetBlue
  return(Blue);
} // GetBlue



double Grid::GetHighlightRed(void)
{ // GetHighlightRed
  return(HighlightRed);
} // GetHighlightRed



double Grid::GetHighlightGreen(void)
{ // GetHighlightGreen
  return(HighlightGreen);
} // GetHighlightGreen



double Grid::GetHighlightBlue(void)
{ // GetHighlightBlue
  return(HighlightBlue);
} // GetHighlightBlue