#ifndef INTERSECTION_CPP
#define INTERSECTION_CPP

const double PIAcc = 0.00000001;

void Swap(double &a, double &b)
{
  double Temp;

  Temp = a;
  a = b;
  b = Temp;
}

int PointIntersect(double X0, double Y0, double X1, double Y1, double X, double Y)
{ // PointIntersect
  double t = 0;    // X Segment Parameter
  double u = 0;    // Y Segment Parameter
  double t1 = 0;   // Divisor of t
  double u1 = 0;   // Divisor of u
  int Success = 0; // Is the Point in the Line Segment: 1 = Yes, 0 = No

  // Calculate the Divisors
  t1 = (X0 - X1);
  u1 = (Y0 - Y1);
  if ((t1 >= 0 - PIAcc) && (t1 <= PIAcc))
  {
    if ((((Y >= Y0) && (Y <= Y1)) || ((Y >= Y1) && (Y <= Y0))) && (X >= X0 - PIAcc) && (X <= X0 + PIAcc))
    {
      Success = 1;
    }
  }
  else if ((u1 >= 0 - PIAcc) && (u1 <= PIAcc))
  {
    if ((((X >= X0) && (X <= X1)) || ((X >= X1) && (X <= X0))) && (Y >= Y0 - PIAcc) && (Y <= Y0 + PIAcc))
    {
      Success = 1;
    }
  }
  else
  {
    // Calculate the "Sub-Line" Parameters
    t = (X - X1) / t1;
    u = (Y - Y1) / u1;
    // If the Two Parameters Are Equal and Between 0 and 1, The Point is in the Line
    if (((t >= u - PIAcc) && (t <= u + PIAcc) && (t >= 0) && (t <= 1)) || ((X == X1) && (X == X0) && (u >= 0) && (u <= 1)) || ((Y == Y0) && (Y == Y1) && (t >= 0) && (t <= 1)))
    {
      Success = 1;
    }
  }
  // Return the Success Value
  return (Success);
} // PointIntersect

int Intersect(double X0, double Y0, double X1, double Y1, double X2, double Y2, double X3, double Y3, double *XI = NULL, double *YI = NULL)
{ // Intersect
  double M1a = 0;  // Slope of First Line
  double M1b = 0;  // Slope of First Line
  double M2a = 0;  // Slope of Second Line
  double M2b = 0;  // Slope of Second Line
  double MEq = 0;  // Slopes Equal?
  double t = 0;    // Line Segment 1 Parameter
  double u = 0;    // Line Segment 2 Parameter
  int Success = 1; // Do the Lines Intersect: 1 = Yes, 0 = No

  // Calculate the Slopes
  M1a = (Y1 - Y0);
  M1b = (X1 - X0);
  M2a = (Y3 - Y2);
  M2b = (X3 - X2);
  if ((M1b >= 0 - PIAcc) && (M1b <= PIAcc))
  {
    Swap(X0, X2);
    Swap(Y0, Y2);
    Swap(X1, X3);
    Swap(Y1, Y3);
    Swap(M1a, M2a);
    Swap(M1b, M2b);
  }
  if (((M1a == 0) && (M2a == 0)) || ((M1b == 0) && (M2b == 0)))
  {
    MEq = 1;
  }
  else if ((M1a / M1b) == (M2a / M2b))
  {
    MEq = 1;
  }
  // If the Slopes are Equal, Lines are Parallel; Do Not Intersect (Unless Overlapping)
  if (MEq)
  {
    // Check for Overlapping Line Segments
    if (PointIntersect(X0, Y0, X1, Y1, X2, Y2))
    {
      if (XI != NULL)
      {
        *XI = X2;
      }
      if (YI != NULL)
      {
        *YI = Y2;
      }
    }
    else if (PointIntersect(X0, Y0, X1, Y1, X3, Y3))
    {
      if (XI != NULL)
      {
        *XI = X3;
      }
      if (YI != NULL)
      {
        *YI = Y3;
      }
    }
    else if (PointIntersect(X2, Y2, X3, Y3, X0, Y0))
    {
      if (XI != NULL)
      {
        *XI = X0;
      }
      if (YI != NULL)
      {
        *YI = Y0;
      }
    }
    else if (PointIntersect(X2, Y2, X3, Y3, X1, Y1))
    {
      if (XI != NULL)
      {
        *XI = X1;
      }
      if (YI != NULL)
      {
        *YI = Y1;
      }
    }
    else
    {
      Success = 0;
    }
  }
  else
  {
    u = ((Y3 - Y1) * (X0 - X1) - (X3 - X1) * (Y0 - Y1)) / ((X2 - X3) * (Y0 - Y1) - (Y2 - Y3) * (X0 - X1));
    t = (u * (X2 - X3) + X3 - X1) / (X0 - X1);
    // If the Parameters are Between 0 and 1, There was a Success
    if ((u >= 0) && (u <= 1) && (t >= 0) && (t <= 1))
    {
      if (XI != NULL)
      {
        *XI = t * (X0 - X1) + X1;
      }
      if (YI != NULL)
      {
        *YI = t * (Y0 - Y1) + Y1;
      }
    }
    else
    {
      Success = 0;
    }
  }
  // Return if the Two Line Segments Intersect
  return (Success);
} // Intersect

#endif