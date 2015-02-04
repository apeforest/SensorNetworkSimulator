// Functions ///////////////////////////////////////////////////////////////////
// Voronoi Interface //////////////////////////////////////////////////////////
// This class is meant to facilitate the creation and drawing of Voronoi
// Diagrams using the Sensor class.  The Sensor class is basically a glorified
// Point class (with extra features like error consideration and beacon
// status).  This can be used with any normalized 2-dimensional graph.  In
// other words - a graph contained in a 1x1 grid.

#ifndef VI_CPP
#define VI_CPP

#include "Sensor.h"
#include "Graph.h"
#include "Voronoi.h"
#include "Intersection.cpp"
#include <math.h>

const double AddAcc = 0.00001; // Adding Voronoi Intersection Accuracy
const int MaxPolyV = 50;       // Maximum Polygon Vertices

class VIPolyVertex
{  // VIPolyVertex
  public:
    VIPolyVertex();
    ~VIPolyVertex();
    double x;
    double y;
}; // VIPolyVertex

VIPolyVertex::VIPolyVertex()
{ // VIPolyVertex
  // Set the Coordinates to 0
  x = 0;
  y = 0;
} // VIPolyVertex

VIPolyVertex::~VIPolyVertex()
{ // ~VIPolyVertex
} // ~VIPolyVertex

class VIPolyEdge
{  // VIPolyEdge
  public:
    VIPolyEdge();
    ~VIPolyEdge();

    VIPolyVertex P[2]; // Array of two End Points
}; // VIPolyEdge

VIPolyEdge::VIPolyEdge()
{ // VIPolyEdge
} // VIPolyEdge

VIPolyEdge::~VIPolyEdge()
{ // ~VIPolyEdge
} // ~VIPolyEdge

class VIPoly
{  // VIPoly
  public:
    VIPoly(int n = 0);
    ~VIPoly();
    void AddEdge(double X1, double Y1, double X2, double Y2);

    int NumE;         // Number of Edges
    int Max;          // Maximum Number of Edges/Vertices
    VIPolyEdge *PE;   // Array of Polygon Edges
}; // VIPoly

VIPoly::VIPoly(int n)
{ // VIPoly
  // Clear Both Dynamic Arrays
  PE = NULL;
  NumE = 0;
  if (n > 0)
  {
    PE = new VIPolyEdge[n];
    Max = n;
  }
  else
  {
    PE = new VIPolyEdge[MaxPolyV];
    Max = MaxPolyV;
  }
} // VIPoly

VIPoly::~VIPoly()
{ // ~VIPoly
  if (PE != NULL)
  {
    delete[] PE;
    PE = NULL;
  }
} // ~VIPoly

void VIPoly::AddEdge(double X1, double Y1, double X2, double Y2)
{ // AddEdge
  if (NumE < Max)
  {
    // Store the Vertices of the Edge
    PE[NumE].P[0].x = X1;
    PE[NumE].P[0].y = Y1;
    PE[NumE].P[1].x = X2;
    PE[NumE].P[1].y = Y2;
    // Increase the Number of Edges
    ++NumE;
  }
} // AddEdge

class VI
{  // VI
  public:
    VI(int n = 2000);
    ~VI();

    int in_between(double a, double b, double c);
    int SameVEdge(int Sensor, double X1, double Y1, double X2, double Y2);
    int FindVSensor(Sensor *S, int N, double X1, double Y1, double X2, double Y2, int SI = -1);
    int PolyIntersect(int Sensor, double X1, double Y1, double X2, double Y2, double &X, double &Y, double SX, double SY);
    int PolyIntersect2(double X1, double Y1, double X2, double Y2);
    double GetPolygonEdgeLength(int Poly);
    double GetTotalEdgeLength(void);
    double get_distance(double x, double y, double x1, double y1, double x2, double y2);
    double distance(double X1, double Y1, double X2, double Y2);
    void CalcVoronoi(Sensor *S, int N, CString *Status = NULL, CProgressCtrl *Progress = NULL, CWnd* Window = NULL);
    void CalcVPoly(Sensor *S, int N, CProgressCtrl *Progress = NULL);
    void CalcPIntersections(GridVertex *P, int NumP, GridVertex *PI, int MaxPI, int &NumPI, double X1, double Y1, double X2, double Y2);
    void GetPolyEdge(int Sensor, double X, double Y, double &X1, double &Y1, double &X2, double &Y2);
    void DrawVoronoi(CWnd* Window, CPen* Pen, int BorderSize, CBrush *Bkg = NULL, int ClearScreen = 0);
    void DrawPath(CWnd* Window, CPen* Pen, CPen* CriticalPen, int BorderSize, CBrush *Bkg = NULL, int ClearScreen = 0);
    void DrawPolygon(int PNum, CWnd* Window, CPen* Pen, int BorderSize, CBrush *Bkg = NULL, int ClearScreen = 0);

    Graph GV;        // Graph that Calculates the Maximum Path
    Voronoi v;       // Voronoi Graph

    double Breach;   // Breach Weight
    double CX1;      // Critical Start X Coordinate
    double CY1;      // Critical Start Y Coordinate
    double CX2;      // Critical End X Coordinate
    double CY2;      // Critical End Y Coordinate
    int NumPoly;     // Number of Polygons

    VIPoly *Polygon; // Voronoi Polygons Array
}; // VI

VI::VI(int n)
: v(n)
{
  Polygon = NULL;
}

VI::~VI()
{
  if (Polygon != NULL)
  {
    delete[] Polygon;
    Polygon = NULL;
  }
}

int VI::in_between(double a, double b, double c)
{
  if (a<=b && a>=c) return 1;
  if (a>=b && a<=c) return 1;
  return 0;
}

double VI::get_distance(double x, double y, double x1, double y1, double x2, double y2)
{
  double d1,d2, yp,xp,m;

  if (x2==x1)
  { // Vertical line
    xp=x1;
    yp=y;
  }
  else if (y2==y1)
  { // horizontal line
    xp=x;
    yp=y1;
  }
  else
  {
    m  = (y2-y1)/(x2-x1);
    xp = (y-y1+m*x1+x/m)/(m+1/m);
    yp = m*(xp-x1)+y1;
  }

  if (!in_between(xp,x1,x2) || !in_between(yp,y1,y2))
  {
    d1=sqrt((x1-x)*(x1-x)+(y1-y)*(y1-y)) * 10000;
    d2=sqrt((x2-x)*(x2-x)+(y2-y)*(y2-y)) * 10000;
    if (d1<d2) return d1;
    return d2;
  }
  return sqrt((xp-x)*(xp-x)+(yp-y)*(yp-y)) * 10000;
}

void VI::CalcVoronoi(Sensor *S, int N, CString *Status, CProgressCtrl *Progress, CWnd* Window)
{
  int i;
  int j;
  unsigned int l;
  double d;
  double weight;
  double x1;
  double y1;
  double x2;
  double y2;
  double minw;
  double maxw;
  double start_x;
  double start_y;
  double end_x;
  double end_y;

  v.initialize();
  GV.initialize();
	
  if (Status != NULL)
  {
    Status->Format("Initializing Voronoi Diagram...");
    if (Window != NULL)
    {
      Window->UpdateData(FALSE);
    }
  }
  if (Progress != NULL)
  {
    Progress->SetRange(0, N);
    Progress->SetStep(1);
    Progress->SetPos(0);
  }

  v.nsites=N;
  for (i=0; i <N; ++i)
  {
    v.assign_sensor(i, S[i].x, S[i].y);
    Progress->StepIt();
  }

  v.do_voronoi();

  start_x = v.graph_v[v.start].x;
  start_y = v.graph_v[v.start].y;
  end_x   = v.graph_v[v.end  ].x;
  end_y   = v.graph_v[v.end  ].y;

  minw=1000000;
  maxw=0;

  if (Status != NULL)
  {
    Status->Format("Calculating Breach Path...");
    if (Window != NULL)
    {
      Window->UpdateData(FALSE);
    }
  }
  if (Progress != NULL)
  {
    Progress->SetRange(0, v.num_edges);
    Progress->SetStep(1);
    Progress->SetPos(0);
  }

  for (l=0; l<v.num_edges; ++l)
  {
    x1=v.graph_v[(int)(v.graph_e[l].ep[0])].x;
    y1=v.graph_v[(int)(v.graph_e[l].ep[0])].y;
    x2=v.graph_v[(int)(v.graph_e[l].ep[1])].x;
    y2=v.graph_v[(int)(v.graph_e[l].ep[1])].y;
    
    weight=1000000;
    for (j=0; j<N; ++j)
    {
      d = get_distance(S[j].x,S[j].y,x1,y1,x2,y2);
      if (d<weight) weight=d;
    }

    v.graph_e[l].c=weight;
    GV.AddEdge((int)v.graph_e[l].ep[0],(int)v.graph_e[l].ep[1],weight);
    if (weight<minw) minw=weight;
    if (weight>maxw) maxw=weight;
    Progress->StepIt();
  }

  // Update Voronoi Info (Coordiantes, Breach Weight, etc.)
  Breach = GV.MaxPath(v.start,v.end,minw,maxw);
  if (GV.critical > 0)
  {
    CX1 = v.graph_v[GV.P[GV.critical - 1]].x;
    CY1 = v.graph_v[GV.P[GV.critical - 1]].y;
    CX2 = v.graph_v[GV.P[GV.critical]].x;
    CY2 = v.graph_v[GV.P[GV.critical]].y;
  }
  else
  {
    CX1 = 0;
    CY1 = 0;
    CX2 = 0;
    CY2 = 0;
  }
  Status->Format("");
  Progress->SetPos(0);
}

void VI::DrawVoronoi(CWnd* Window, CPen* Pen, int BorderSize, CBrush *Bkg, int ClearScreen)
{
  unsigned int i; // Index Variable
  int x1;         // Starting X Coordinate
  int y1;         // Starting Y Coordinate
  int x2;         // Ending X Coordinate
  int y2;         // Ending Y Coordinate
  int P1 = 0;     // (x1, y1) Out of Bounds
  int P2 = 0;     // (x2, y2) Out of Bounds
  double xm;      // Clipping Coordinate
  double ym;      // Clipping Coordinate
  double xn;      // Clipping Coordinate
  double yn;      // Clipping Coordinate
  double m;       // Slope for Clipping
  int Width;      // Width of Picture Window
  int Height;     // Height of Picture Window

  CRect DrawWindow;          // Rectangle for Getting Width and Height
  CClientDC* PicWindow;      // Picture Window for Drawing
  // CBrush Bkg(RGB (0, 0, 0)); // Brush to Draw the Background Color
  // CPen Blue(PS_SOLID, 1, RGB(0, 0, 192));         // Blue Pen
  // CPen Red(PS_SOLID, 1, RGB(192, 0, 0));          // Red Pen
  // CPen PBlue(PS_SOLID, 1, RGB(128, 128, 255));    // Blue Path Line
  // CPen PRed(PS_SOLID, 1, RGB(255, 128, 128));     // Red Path Line
  // CPen PWhite(PS_SOLID, 1, RGB(255, 255, 255));   // White Path Line

  PicWindow = new CClientDC(Window);
  Window->GetClientRect(&DrawWindow);


  // Get the Width and Height of the Drawing Window
  Width = DrawWindow.BottomRight().x;
  Height = DrawWindow.BottomRight().y;

  // Clear the Drawing Window
  if ((ClearScreen != 0) && (Bkg != NULL))
  {
    PicWindow->SelectObject(Bkg);
    PicWindow->Rectangle(0, 0, Width, Height);
  }
  // Select the Pen
  PicWindow->SelectObject(Pen);

  // Adjust for the Border
  Width -= 2 * BorderSize;
  Height -= 2 * BorderSize;

  // Draw the Voronoi Diagrom
  for (i = 0; i < v.num_edges; ++i)
  {
    x1 = int (double (Width) * v.graph_v[(int)(v.graph_e[i].ep[0])].x) + BorderSize;
    y1 = int (double (Height) * v.graph_v[(int)(v.graph_e[i].ep[0])].y) + BorderSize;
    x2 = int (double (Width) * v.graph_v[(int)(v.graph_e[i].ep[1])].x) + BorderSize;
    y2 = int (double (Height) * v.graph_v[(int)(v.graph_e[i].ep[1])].y) + BorderSize;

    // Assume Both Points Don't Need to Be Clipped
    P1 = 0;
    P2 = 0;
    // Check for Clipping of First Point
    if ((x1 < BorderSize) || (x1 > Width + BorderSize) || (y1 < BorderSize) || (y1 > Height + BorderSize))
    {
      P1 = 1;
    }
    // Check for Clipping of Second Point
    if ((x2 < BorderSize) || (x2 > Width + BorderSize) || (y2 < BorderSize) || (y2 > Height + BorderSize))
    {
      P2 = 1;
    }

    // If Both Points Don't Have to Be Clipped
    if ((P1 == 0) || (P2 == 0))
    {
      // If the First Point Needs Clipping
      if (P1 != 0)
      {
        // Calculate Slope
        if (x2 - x1 == 0.0)
        {
          if (y1 < y2)
          {
            y1 = BorderSize;
          }
          else
          {
            y1 = Height + BorderSize;
          }
        }
        else
        {
          m = double (y2 - y1) / double (x2 - x1);
          // Determine the Boundaries
          if (x1 < x2)
          {
            xm = BorderSize;
          }
          else
          {
            xm = Width + BorderSize;
          }
          if (y1 < y2)
          {
            ym = BorderSize;
          }
          else
          { 
            ym = Height + BorderSize;
          }
          // Get Clipping Points
          xn = double (ym - y2) / m + double (x2);
          yn = double (xm - x2) * m + double (y2);
          // Set New Points
          if ((xn >= BorderSize) && (xn <= Width + BorderSize))
          {
            x1 = int (xn);
            y1 = int (ym);
          }
          else
          {
            x1 = int (xm);
            y1 = int (yn);
          }
        }
      }

      // If the Second Point Need Clipping
      if (P2 != 0)
      {
        // Calculate Slope
        if (x2 - x1 == 0.0)
        {
          if (y2 < y1)
          {
            y2 = BorderSize;
          }
          else
          {
            y2 = Height + BorderSize;
          }
        }
        else
        {
          m = double (y2 - y1) / double (x2 - x1);
          // Determine the Boundaries
          if (x2 < x1)
          {
            xm = BorderSize;
          }
          else
          {
            xm = Width + BorderSize;
          }
          if (y2 < y1)
          {
            ym = BorderSize;
          }
          else
          { 
            ym = Height + BorderSize;
          }
          // Get Clipping Points
          xn = double (ym - y1) / m + double (x1);
          yn = double (xm - x1) * m + double (y1);
          // Set New Points
          if ((xn >= BorderSize) && (xn <= Width + BorderSize))
          {
            x2 = int (xn);
            y2 = int (ym);
          }
          else
          {
            x2 = int (xm);
            y2 = int (yn);
          }
        }
      }
      PicWindow->MoveTo(x1, y1);
      PicWindow->LineTo(x2, y2);
    }
  }
  // Delete the Drawing Window Variable
  delete PicWindow;
}

void VI::DrawPath(CWnd* Window, CPen* Pen, CPen* CriticalPen, int BorderSize, CBrush *Bkg, int ClearScreen)
{
  int j = 0;      // Index Variable
  int x1;         // Starting X Coordinate
  int y1;         // Starting Y Coordinate
  int x2;         // Ending X Coordinate
  int y2;         // Ending Y Coordinate
  int P1 = 0;     // (x1, y1) Out of Bounds
  int P2 = 0;     // (x2, y2) Out of Bounds
  double xm;      // Clipping Coordinate
  double ym;      // Clipping Coordinate
  double xn;      // Clipping Coordinate
  double yn;      // Clipping Coordinate
  double m;       // Slope for Clipping
  int Width;      // Width of Picture Window
  int Height;     // Height of Picture Window

  CRect DrawWindow;          // Rectangle for Getting Width and Height
  CClientDC* PicWindow;      // Picture Window for Drawing
  // Get the Window Area
  PicWindow = new CClientDC(Window);
  Window->GetClientRect(&DrawWindow);

  // Get the Width and Height of the Drawing Window
  Width = DrawWindow.BottomRight().x;
  Height = DrawWindow.BottomRight().y;

  // Clear the Drawing Window
  if ((ClearScreen != 0) && (Bkg != NULL))
  {
    PicWindow->SelectObject(Bkg);
    PicWindow->Rectangle(0, 0, Width, Height);
  }
  // Select the Pen
  PicWindow->SelectObject(Pen);

  // Adjust for the Border
  Width -= BorderSize;
  Height -= BorderSize;

  // Display the Breach Path
  for (j = 0; j < GV.p_count - 1; ++j)
  {
    x1 = int (double (Width) * v.graph_v[GV.P[j]].x) + BorderSize;
    y1 = int (double (Height) * v.graph_v[GV.P[j]].y) + BorderSize;
    x2 = int (double (Width) * v.graph_v[GV.P[j + 1]].x) + BorderSize;
    y2 = int (double (Height) * v.graph_v[GV.P[j + 1]].y) + BorderSize;

    if (j == GV.critical - 1)
    {
      PicWindow->SelectObject(CriticalPen);
    }
    else
    {
      PicWindow->SelectObject(Pen);
    }

    // Assume Both Points Don't Need to Be Clipped
    P1 = 0;
    P2 = 0;
    // Check for Clipping of First Point
    if ((x1 < BorderSize) || (x1 > Width + BorderSize) || (y1 < BorderSize) || (y1 > Height + BorderSize))
    {
      P1 = 1;
    }
    // Check for Clipping of Second Point
    if ((x2 < BorderSize) || (x2 > Width + BorderSize) || (y2 < BorderSize) || (y2 > Height + BorderSize))
    {
      P2 = 1;
    }

    // If Both Points Don't Have to Be Clipped
    if ((P1 == 0) || (P2 == 0))
    {
      // If the First Point Needs Clipping
      if (P1 != 0)
      {
        // Calculate Slope
        if (x2 - x1 == 0.0)
        {
          if (y1 < y2)
          {
            y1 = BorderSize;
          }
          else
          {
            y1 = Height + BorderSize;
          }
        }
        else
        {
          m = double (y2 - y1) / double (x2 - x1);
          // Determine the Boundaries
          if (x1 < x2)
          {
            xm = BorderSize;
          }
          else
          {
            xm = Width + BorderSize;
          }
          if (y1 < y2)
          {
            ym = BorderSize;
          }
          else
          { 
            ym = Height + BorderSize;
          }
          // Get Clipping Points
          xn = double (ym - y2) / m + double (x2);
          yn = double (xm - x2) * m + double (y2);
          // Set New Points
          if ((xn >= BorderSize) && (xn <= Width + BorderSize))
          {
            x1 = int (xn);
            y1 = int (ym);
          }
          else
          {
            x1 = int (xm);
            y1 = int (yn);
          }
        }
      }

      // If the Second Point Need Clipping
      if (P2 != 0)
      {
        // Calculate Slope
        if (x2 - x1 == 0.0)
        {
          if (y2 < y1)
          {
            y2 = BorderSize;
          }
          else
          {
            y2 = Height + BorderSize;
          }
        }
        else
        {
          m = double (y2 - y1) / double (x2 - x1);
          // Determine the Boundaries
          if (x2 < x1)
          {
            xm = BorderSize;
          }
          else
          {
            xm = Width + BorderSize;
          }
          if (y2 < y1)
          {
            ym = BorderSize;
          }
          else
          { 
            ym = Height + BorderSize;
          }
          // Get Clipping Points
          xn = double (ym - y1) / m + double (x1);
          yn = double (xm - x1) * m + double (y1);
          // Set New Points
          if ((xn >= BorderSize) && (xn <= Width + BorderSize))
          {
            x2 = int (xn);
            y2 = int (ym);
          }
          else
          {
            x2 = int (xm);
            y2 = int (yn);
          }
        }
      }
      PicWindow->MoveTo(x1, y1);
      PicWindow->LineTo(x2, y2);
    }
  }
}

int VI::FindVSensor(Sensor *S, int N, double X1, double Y1, double X2, double Y2, int SI)
{ // FindVSensor
  int i = 0;     // Index Variable
  int Index = 0; // Sensor Index
  double D = 0;  // Distance to Sensor
  double ND = 0; // New Distance to Sensor

  // Start at the Second Vertex if N = 0
  if (SI == 0)
  {
    ++i;
  }
  // Calculate Initial Distance
  D = distance(S[i].x, S[i].y, (X1 + X2) / 2, (Y1 + Y2) / 2);
  // Check Remaining Sensors
  for (++i; i < N; ++i)
  {
    ND = distance(S[i].x, S[i].y, (X1 + X2) / 2, (Y1 + Y2) / 2);
    if ((ND < D) && (i != SI))
    {
      // Store New Shortest Distance
      D = ND;
      // Save Sensor Index
      Index = i;
    }
  }
  // return the Sensor Index
  return (Index);
} // FindVSensor

void VI::CalcVPoly(Sensor *S, int N, CProgressCtrl *Progress)
{ // CalcVPoly
  unsigned int l = 0;       // Index Variable
  double x1 = 0;            // Starting X Coordinate
  double y1 = 0;            // Starting Y Coordinate
  double x2 = 0;            // Ending X Coordinate
  double y2 = 0;            // Ending Y Coordinate
  int i = 0;                // Index Variable  
  int Sensor1 = 0;          // First Nearest Sensor
  int Sensor2 = 0;          // Second Nearest Sensor

  // Delete Old Polygon Array
  if (Polygon != NULL)
  {
    delete[] Polygon;
    Polygon = NULL;
  }
  // Initialize the Progress Bar
  if (Progress != NULL)
  {
    Progress->SetRange(0, v.num_edges);
    Progress->SetPos(0);
    Progress->SetStep(1);
  }
  // Create New Polygon Array
  Polygon = new VIPoly[N];
  NumPoly = N;
  // For Each Edge in the array, Find the two Closest Sensors
  for (l = 0; l < v.num_edges; ++l)
  {
    // Calculate the Starting and Ending points
    x1 = v.graph_v[(int)(v.graph_e[l].ep[0])].x;
    y1 = v.graph_v[(int)(v.graph_e[l].ep[0])].y;
    x2 = v.graph_v[(int)(v.graph_e[l].ep[1])].x;
    y2 = v.graph_v[(int)(v.graph_e[l].ep[1])].y;
    // Calculate the Two Nearest Sensors
    Sensor1 = FindVSensor(S, N, x1, y1, x2, y2);
    Sensor2 = FindVSensor(S, N, x1, y1, x2, y2, Sensor1);
    // Add the Edges to the Appropriate Polygons
    Polygon[Sensor1].AddEdge(x1, y1, x2, y2);
    Polygon[Sensor2].AddEdge(x1, y1, x2, y2);
    // Step through the Polygon
    if (Progress != NULL)
    {
      Progress->StepIt();
    }
  }
  // Clear the Progress Bar
  Progress->SetPos(0);
} // CalcVPoly

void VI::DrawPolygon(int PNum, CWnd* Window, CPen* Pen, int BorderSize, CBrush *Bkg, int ClearScreen)
{
  int i = 0;      // Index Variable
  int x1;         // Starting X Coordinate
  int y1;         // Starting Y Coordinate
  int x2;         // Ending X Coordinate
  int y2;         // Ending Y Coordinate
  int P1 = 0;     // (x1, y1) Out of Bounds
  int P2 = 0;     // (x2, y2) Out of Bounds
  double xm;      // Clipping Coordinate
  double ym;      // Clipping Coordinate
  double xn;      // Clipping Coordinate
  double yn;      // Clipping Coordinate
  double m;       // Slope for Clipping
  int Width;      // Width of Picture Window
  int Height;     // Height of Picture Window

  CRect DrawWindow;          // Rectangle for Getting Width and Height
  CClientDC* PicWindow;      // Picture Window for Drawing
  // CBrush Bkg(RGB (0, 0, 0)); // Brush to Draw the Background Color
  // CPen Blue(PS_SOLID, 1, RGB(0, 0, 192));         // Blue Pen
  // CPen Red(PS_SOLID, 1, RGB(192, 0, 0));          // Red Pen
  // CPen PBlue(PS_SOLID, 1, RGB(128, 128, 255));    // Blue Path Line
  // CPen PRed(PS_SOLID, 1, RGB(255, 128, 128));     // Red Path Line
  // CPen PWhite(PS_SOLID, 1, RGB(255, 255, 255));   // White Path Line

  PicWindow = new CClientDC(Window);
  Window->GetClientRect(&DrawWindow);


  // Get the Width and Height of the Drawing Window
  Width = DrawWindow.BottomRight().x;
  Height = DrawWindow.BottomRight().y;

  // Clear the Drawing Window
  if ((ClearScreen != 0) && (Bkg != NULL))
  {
    PicWindow->SelectObject(Bkg);
    PicWindow->Rectangle(0, 0, Width, Height);
  }
  // Select the Pen
  PicWindow->SelectObject(Pen);

  // Adjust for the Border
  Width -= 2 * BorderSize;
  Height -= 2 * BorderSize;

  // Draw the Voronoi Diagrom
  for (i = 0; i < Polygon[PNum].NumE; ++i)
  {
    x1 = int (double (Width) * Polygon[PNum].PE[i].P[0].x) + BorderSize;
    y1 = int (double (Height) * Polygon[PNum].PE[i].P[0].y) + BorderSize;
    x2 = int (double (Width) * Polygon[PNum].PE[i].P[1].x) + BorderSize;
    y2 = int (double (Height) * Polygon[PNum].PE[i].P[1].y) + BorderSize;

    // Assume Both Points Don't Need to Be Clipped
    P1 = 0;
    P2 = 0;
    // Check for Clipping of First Point
    if ((x1 < BorderSize) || (x1 > Width + BorderSize) || (y1 < BorderSize) || (y1 > Height + BorderSize))
    {
      P1 = 1;
    }
    // Check for Clipping of Second Point
    if ((x2 < BorderSize) || (x2 > Width + BorderSize) || (y2 < BorderSize) || (y2 > Height + BorderSize))
    {
      P2 = 1;
    }

    // If Both Points Don't Have to Be Clipped
    if ((P1 == 0) || (P2 == 0))
    {
      // If the First Point Needs Clipping
      if (P1 != 0)
      {
        // Calculate Slope
        if (x2 - x1 == 0.0)
        {
          if (y1 < y2)
         {
            y1 = BorderSize;
          }
          else
          {
            y1 = Height + BorderSize;
          }
        }
        else
        {
          m = double (y2 - y1) / double (x2 - x1);
          // Determine the Boundaries
          if (x1 < x2)
          {
            xm = BorderSize;
          }
          else
          {
            xm = Width + BorderSize;
          }
          if (y1 < y2)
          {
            ym = BorderSize;
          }
          else
          { 
            ym = Height + BorderSize;
          }
          // Get Clipping Points
          xn = double (ym - y2) / m + double (x2);
          yn = double (xm - x2) * m + double (y2);
          // Set New Points
          if ((xn >= BorderSize) && (xn <= Width + BorderSize))
          {
            x1 = int (xn);
            y1 = int (ym);
          }
          else
          {
            x1 = int (xm);
            y1 = int (yn);
          }
        }
      }

      // If the Second Point Need Clipping
      if (P2 != 0)
      {        
        // Calculate Slope
        if (x2 - x1 == 0.0)
        {
          if (y2 < y1)
          {
            y2 = BorderSize;
          }
          else
          {
            y2 = Height + BorderSize;
          }
        }
        else
        {
          m = double (y2 - y1) / double (x2 - x1);
          // Determine the Boundaries
          if (x2 < x1)
          {
            xm = BorderSize;
          }
          else
          {
            xm = Width + BorderSize;
          }
          if (y2 < y1)
          {
            ym = BorderSize;
          }
          else
          { 
            ym = Height + BorderSize;
          }
          // Get Clipping Points
          xn = double (ym - y1) / m + double (x1);
          yn = double (xm - x1) * m + double (y1);
          // Set New Points
          if ((xn >= BorderSize) && (xn <= Width + BorderSize))
          {
            x2 = int (xn);
            y2 = int (ym);
          }
          else
          {
            x2 = int (xm);
            y2 = int (yn);
          }
        }
      }
      PicWindow->MoveTo(x1, y1);
      PicWindow->LineTo(x2, y2);
    }
  }
  // Delete the Drawing Window Variable
  delete PicWindow;
}

void VI::CalcPIntersections(GridVertex *P, int NumP, GridVertex *PI, int MaxPI, int &NumPI, double X1, double Y1, double X2, double Y2)
{ // CalcPIntersections
  int i = 0;         // Index Variable
  int j = 0;         // Index Variable
  int k = 0;         // Index Variable
  int l = 0;         // Index Variable
  int z = 0;         // Temp Variable
  int Add = 0;       // OK to Add Vertex?
  double XI = 0;     // X Intersection
  double YI = 0;     // Y Intersection
  double TempX = 0;  // Temp X
  double TempY = 0;  // Temp Y
  double LinePI = 0; // Line Index
  double New = 0;    // New Vertices on This Line
  double ND = 0;     // New Distnace
  double CD = 0;     // Current Distance
  int Index = 0;     // Intersection Index


  // Clear the Number of Path Intersections
  NumPI = 0;
  // Store the Ending Point
  if (NumPI < MaxPI)
  {
    PI[NumPI].x = X2;
    PI[NumPI].y = Y2;
    ++NumPI;
  }
  for (i = 0; i < NumP - 1; ++i)
  {
    // Store the first Index for this Line Segment
    LinePI = NumPI;
    // Clear the Number of New Vertices for the Line
    for (j = 0; j < NumPoly; ++j)
    {
      for (k = 0; k < Polygon[j].NumE; ++k)
      {
        if (Intersect(P[i].x, P[i].y, P[i + 1].x, P[i + 1].y, Polygon[j].PE[k].P[0].x, Polygon[j].PE[k].P[0].y, Polygon[j].PE[k].P[1].x, Polygon[j].PE[k].P[1].y, &XI, &YI))
        {
          for (Add = 1, z = 0; (Add == 1) && (z < NumPI); ++z)
          {
            if ((PI[z].x >= XI - AddAcc) && (PI[z].x <= XI + AddAcc) && (PI[z].y >= YI - AddAcc) && (PI[z].y <= YI + AddAcc))
            {
              Add = 0;
            }
          }
          if ((NumPI < MaxPI) && (Add == 1))
          {
            // Store the Vertex
            if (XI < AddAcc)
            {
              XI = 0;
            }
            if (YI < AddAcc)
            {
              YI = 0;
            }
            if (XI > 1.0 - AddAcc)
            {
              XI = 1.0;
            }
            if (YI > 1.0 - AddAcc)
            {
              YI = 1.0;
            }
            // Store the Point
            PI[NumPI].x = XI;
            PI[NumPI].y = YI;
            // Increase the Number of Vertices
            ++NumPI;
            // Sort the Points
            for (l = NumPI - 1; l > LinePI; --l)
            {
              if (distance(P[i].x, P[i].y, PI[l].x, PI[l].y) < distance(P[i].x, P[i].y, PI[l - 1].x, PI[l - 1].y))
              {
                TempX = PI[l].x;
                TempY = PI[l].y;
                PI[l].x = PI[l - 1].x;
                PI[l].y = PI[l - 1].y;
                PI[l - 1].x = TempX;
                PI[l - 1].y = TempY;
              }
            }
          }
        }
      }
    }
  }
  // Store the Starting Point
  if (NumPI < MaxPI)
  {
    PI[NumPI].x = X1;
    PI[NumPI].y = Y1;
    ++NumPI;
  }
  // Sort the Path Intersections
  for (i = 0; i < int (double (NumPI) / 2.0); ++i)
  {
    TempX = PI[i].x;
    TempY = PI[i].y;
    PI[i].x = PI[NumPI - 1 - i].x;
    PI[i].y = PI[NumPI - 1 - i].y;
    PI[NumPI - 1 - i].x = TempX;
    PI[NumPI - 1 - i].y = TempY;
  }

  /*
  // Clear the Number of Path Intersections
  NumPI = 0;
  // Store the Starting Point
  if (NumPI < MaxPI)
  {
    PI[NumPI].x = X1;
    PI[NumPI].y = Y1;
    ++NumPI;
  }
  for (i = 0; i < NumP - 1; ++i)
  {
    for (j = 0; j < NumPoly; ++j)
    {
      for (k = 0; k < Polygon[j].NumE; ++k)
      {
        if (Intersect(P[i].x, P[i].y, P[i + 1].x, P[i + 1].y, Polygon[j].PE[k].P[0].x, Polygon[j].PE[k].P[0].y, Polygon[j].PE[k].P[1].x, Polygon[j].PE[k].P[1].y, &XI, &YI))
        {
          for (Add = 1, z = 0; (Add == 1) && (z < NumPI); ++z)
          {
            if ((PI[z].x >= XI - AddAcc) && (PI[z].x <= XI + AddAcc) && (PI[z].y >= YI - AddAcc) && (PI[z].y <= YI + AddAcc))
            {
              Add = 0;
            }
          }
          if ((NumPI < MaxPI) && (Add == 1))
          {
            // Store the Vertex
            if (XI < AddAcc)
            {
              XI = 0;
            }
            if (YI < AddAcc)
            {
              YI = 0;
            }
            if (XI > 1.0 - AddAcc)
            {
              XI = 1.0;
            }
            if (YI > 1.0 - AddAcc)
            {
              YI = 1.0;
            }
            PI[NumPI].x = XI;
            PI[NumPI].y = YI;
            // Increase the Number of Vertices
            ++NumPI;
          }
        }
      }
    }
  }
  // Store the Ending Point
  if (NumPI < MaxPI)
  {
    PI[NumPI].x = X2;
    PI[NumPI].y = Y2;
    ++NumPI;
  }
  // Sort the Path Intersections
  for (i = 0; i < NumPI - 1; ++i)
  {
    // Start at i + 1
    j = i + 1;
    // Calculate the Distance and Current Index
    CD = distance(PI[i].x, PI[i].y, PI[j].x, PI[j].y);
    Index = j;
    for (++j; j < NumPI; ++j)
    {
      ND = distance(PI[i].x, PI[i].y, PI[j].x, PI[j].y);
      if (ND < CD)
      {
        // Store the New Low Distance and Index
        CD = ND;
        Index = j;
      }
    }
    // Swap the Smallest Distance Path
    TempX = PI[Index].x;
    TempY = PI[Index].y;
    PI[Index].x = PI[i + 1].x;
    PI[Index].y = PI[i + 1].y;
    PI[i + 1].x = TempX;
    PI[i + 1].y = TempY;
  }
  */
} // CalcPIntersections

double VI::distance(double X1, double Y1, double X2, double Y2)
{ // distance
  return (sqrt((X1 - X2) * (X1 - X2) + (Y1 - Y2) * (Y1 - Y2)));
} // distance

void VI::GetPolyEdge(int Sensor, double X, double Y, double &X1, double &Y1, double &X2, double &Y2)
{ // GetPolyEdge
  unsigned int i = 0; // Index Variable
  int S = 0;          // Success
  double x1 = 0;      // Starting Vertex
  double y1 = 0;      // Starting Vertex
  double x2 = 0;      // Ending Vertex
  double y2 = 0;      // Ending Vertex

  /*
  for (i = 0, S = 0; (S == 0) && (i < unsigned int (Polygon[Sensor].NumE)); ++i)
  {
    if (PointIntersect(Polygon[Sensor].PE[i].P[0].x, Polygon[Sensor].PE[i].P[0].y, Polygon[Sensor].PE[i].P[1].x, Polygon[Sensor].PE[i].P[1].y, X, Y))
    {
      // Edge Found, Set Polygon Edge Vertices
      X1 = Polygon[Sensor].PE[i].P[0].x;
      Y1 = Polygon[Sensor].PE[i].P[0].y;
      X2 = Polygon[Sensor].PE[i].P[1].x;
      Y2 = Polygon[Sensor].PE[i].P[1].y;
      S = 1;
    }
  }
  */

  for (i = 0, S = 0; (S == 0) && (i < v.num_edges); ++i)
  {
    x1 = v.graph_v[(int)(v.graph_e[i].ep[0])].x;
    y1 = v.graph_v[(int)(v.graph_e[i].ep[0])].y;
    x2 = v.graph_v[(int)(v.graph_e[i].ep[1])].x;
    y2 = v.graph_v[(int)(v.graph_e[i].ep[1])].y;
    
    if (PointIntersect(x1, y1, x2, y2, X, Y))
    {
      S = 1;
      X1 = x1;
      Y1 = y1;
      X2 = x2;
      Y2 = y2;
    }
  }

  /*
  while (!PointIntersect(Polygon[Sensor].PE[i].P[0].x, Polygon[Sensor].PE[i].P[0].y, Polygon[Sensor].PE[i].P[1].x, Polygon[Sensor].PE[i].P[1].y, X, Y) && (i < Polygon[Sensor].NumE))
  {
    ++i;
  }
  // Check for Success
  if (i < Polygon[Sensor].NumE)
  {
    // Edge Found, Set Polygon Edge Vertices
    X1 = Polygon[Sensor].PE[i].P[0].x;
    Y1 = Polygon[Sensor].PE[i].P[0].y;
    X2 = Polygon[Sensor].PE[i].P[1].x;
    Y2 = Polygon[Sensor].PE[i].P[1].y;
  }
  else
  {
    for (Sensor = 0, S = 0; (S == 0) && (Sensor < NumPoly); ++Sensor)
    {
      for (i = 0; (S == 0) && (i < Polygon[i].NumE); ++i)
      {
        if (PointIntersect(Polygon[Sensor].PE[i].P[0].x, Polygon[Sensor].PE[i].P[0].y, Polygon[Sensor].PE[i].P[1].x, Polygon[Sensor].PE[i].P[1].y, X, Y))
        {
          // Edge Found, Set Polygon Edge Vertices
          X1 = Polygon[Sensor].PE[i].P[0].x;
          Y1 = Polygon[Sensor].PE[i].P[0].y;
          X2 = Polygon[Sensor].PE[i].P[1].x;
          Y2 = Polygon[Sensor].PE[i].P[1].y;
          S = 1;
        }
      }
    }
  }
  */
} // GetPolyEdge

int VI::PolyIntersect(int Sensor, double X1, double Y1, double X2, double Y2, double &X, double &Y, double SX, double SY)
{ // PolyIntersect
  unsigned int i = 0; // Index Variable
  int S = 0;          // The Line Intersected a Polygon Edge
  double U = 0;       // Intersection Point
  double V = 0;       // Intersection Point
  double x1 = 0;      // Starting Vertex
  double y1 = 0;      // Starting Vertex
  double x2 = 0;      // Ending Vertex
  double y2 = 0;      // Ending Vertex
  double sx = -1;     // Sensor Vertex
  double sy = -1;     // Sensor Vertex
  double ex = -1;     // Sensor Vertex
  double ey = -1;     // Sensor Vertex
  double SEdge = 0;   // Edge Being Checked has the Sensor on It?

  // Get the Edge that the Sensor Lies on (if Any)
  GetPolyEdge(Sensor, SX, SY, sx, sy, ex, ey);

  for (i = 0, S = 0; (S == 0) && (i < v.num_edges); ++i)
  {
    x1 = v.graph_v[(int)(v.graph_e[i].ep[0])].x;
    y1 = v.graph_v[(int)(v.graph_e[i].ep[0])].y;
    x2 = v.graph_v[(int)(v.graph_e[i].ep[1])].x;
    y2 = v.graph_v[(int)(v.graph_e[i].ep[1])].y;

    // Check to make sure the Sensor Edge is not the Current Edge
    if (((sx == x1) && (sy == y1) && (ex == x2) && (ey == y2)) || ((sx == x2) && (sy == y2) && (ex == x1) && (ey == y1)))
    {
      SEdge = 1;
    }

    if ((Intersect(x1, y1, x2, y2, X1, Y1, X2, Y2, &U, &V)) && (!SEdge))
    {
      S = 1;
      X = U;
      Y = V;
      if (X < AddAcc)
      {
        X = 0;
      }
      if (X > 1.0 - AddAcc)
      {
        X = 1;
      }
      if (Y < AddAcc)
      {
        Y = 0;
      }
      if (Y > 1.0 - AddAcc)
      {
        Y = 1;
      }
    }
  }

  /*
  while ((!Intersect(Polygon[Sensor].PE[i].P[0].x, Polygon[Sensor].PE[i].P[0].y, Polygon[Sensor].PE[i].P[1].x, Polygon[Sensor].PE[i].P[1].y, X1, Y1, X2, Y2, &U, &V)) && (i < Polygon[Sensor].NumE))
  {
    ++i;
  }
  if (i >= Polygon[Sensor].NumE)
  {
    for (Sensor = 0, S = 0; (S == 0) && (Sensor < NumPoly); ++Sensor)
    {
      for (i = 0; (S == 0) && (i < Polygon[i].NumE); ++i)
      {
        if (Intersect(Polygon[Sensor].PE[i].P[0].x, Polygon[Sensor].PE[i].P[0].y, Polygon[Sensor].PE[i].P[1].x, Polygon[Sensor].PE[i].P[1].y, X1, Y1, X2, Y2, &U, &V))
        {
          S = 1;
          X = U;
          Y = V;
        }
      }
    }
  }
  else
  {
    X = U;
    Y = V;
  }
  */
  // Return Success
  return (S);
} // PolyIntersect

int VI::SameVEdge(int Sensor, double X1, double Y1, double X2, double Y2)
{ // SameEdge
  unsigned int i = 0; // Index Variable
  int S = 0;          // Not on the Same Edge
  double x1 = 0;      // Starting Vertex
  double y1 = 0;      // Starting Vertex
  double x2 = 0;      // Ending Vertex
  double y2 = 0;      // Ending Vertex

  for (i = 0, S = 0; (S == 0) && (i < v.num_edges); ++i)
  {
    x1 = v.graph_v[(int)(v.graph_e[i].ep[0])].x;
    y1 = v.graph_v[(int)(v.graph_e[i].ep[0])].y;
    x2 = v.graph_v[(int)(v.graph_e[i].ep[1])].x;
    y2 = v.graph_v[(int)(v.graph_e[i].ep[1])].y;
    
    if ((PointIntersect(x1, y1, x2, y2, X1, Y1)) && (PointIntersect(x1, y1, x2, y2, X2, Y2)))
    {
      S = 1;
    }
  }
  
  /*
  for (i = 0, S = 0; (S == 0) && (i < Polygon[Sensor].NumE); ++i)
  {
    if (PointIntersect(Polygon[Sensor].PE[i].P[0].x, Polygon[Sensor].PE[i].P[0].y, Polygon[Sensor].PE[i].P[1].x, Polygon[Sensor].PE[i].P[1].y, X1, Y1) && PointIntersect(Polygon[Sensor].PE[i].P[0].x, Polygon[Sensor].PE[i].P[0].y, Polygon[Sensor].PE[i].P[1].x, Polygon[Sensor].PE[i].P[1].y, X2, Y2))
    {
      S = 1;
    }
  }
  */
  // Return Success
  return (S);
} // SameEdge

double VI::GetTotalEdgeLength(void)
{ // GetTotalEdgeLength
  unsigned int i = 0; // Index Variable
  double l = 0;       // Length

  for (i = 0; i < v.num_edges; ++i)
  {
    l += distance(v.graph_v[(int)(v.graph_e[i].ep[0])].x, v.graph_v[(int)(v.graph_e[i].ep[0])].y, v.graph_v[(int)(v.graph_e[i].ep[1])].x, v.graph_v[(int)(v.graph_e[i].ep[1])].y);
  }
  // Return the Total Length
  return (l);
} // GetTotalEdgeLength

double VI::GetPolygonEdgeLength(int Poly)
{ // GetTotalEdgeLength
  int i = 0;     // Index Variable
  double l = 0;  // Length

  for (i = 0; i < Polygon[Poly].NumE; ++i)
  {
    l += distance(Polygon[Poly].PE[i].P[0].x, Polygon[Poly].PE[i].P[0].y, Polygon[Poly].PE[i].P[1].x, Polygon[Poly].PE[i].P[1].y);
  }
  // Return the Total Length
  return (l);
} // GetTotalEdgeLength

int VI::PolyIntersect2(double X1, double Y1, double X2, double Y2)
{ // PolyIntersect2
  unsigned int i = 0; // Index Variable
  int S = 0;          // The Line Intersected a Polygon Edge
  double U = 0;       // Intersection Point
  double V = 0;       // Intersection Point
  double x1 = 0;      // Starting Vertex
  double y1 = 0;      // Starting Vertex
  double x2 = 0;      // Ending Vertex
  double y2 = 0;      // Ending Vertex

  for (i = 0, S = 0; (S == 0) && (i < v.num_edges); ++i)
  {
    x1 = v.graph_v[(int)(v.graph_e[i].ep[0])].x;
    y1 = v.graph_v[(int)(v.graph_e[i].ep[0])].y;
    x2 = v.graph_v[(int)(v.graph_e[i].ep[1])].x;
    y2 = v.graph_v[(int)(v.graph_e[i].ep[1])].y;

    if (Intersect(x1, y1, x2, y2, X1, Y1, X2, Y2, &U, &V))
    {
      if (!((U >= X1 - AddAcc) && (U <= X1 + AddAcc) && (V >= Y1 - AddAcc) && (V <= Y1 + AddAcc)) && !((U >= X2 - AddAcc) && (U <= X2 + AddAcc) && (V >= Y2 - AddAcc) && (V <= Y2 + AddAcc)))
      {
        S = 1;
      }
    }
  }
  // Return the Success Value
  return(S);
} // PolyIntersect2

#endif
