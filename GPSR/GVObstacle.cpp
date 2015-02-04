

#include "GVObstacle.h"

GVObstacle::GVObstacle()
{
	New( GVOBSTACLE_DEFAULTX1, GVOBSTACLE_DEFAULTY1, GVOBSTACLE_DEFAULTX2, GVOBSTACLE_DEFAULTY2);
}

GVObstacle::~GVObstacle()
{
	Delete();
}

void GVObstacle::New(double X1, double Y1, double X2, double Y2)
/*
  New() creates a new point with the specified coordinates (X1, Y1).
*/
{ // New
  Delete();
  MinX = X1;
  MinY = Y1;
  MaxX = X2;
  MaxY = Y2;
} // New



void GVObstacle::Delete()
/*
  Delete() restores the point to its default value.
*/
{ // Delete
  MinX = 0.0;
  MinY = 0.0;
  MaxX = 0.0;
  MaxY = 0.0;
} // Delete

int GVObstacle::CoveredPosition(double X, double Y)
{
	int ReturnVal = 0;
	if( X >= MinX && Y >= MinY && X <= MaxX && Y <= MaxY)
	{
		ReturnVal = 1;
	}
	return ReturnVal;
}