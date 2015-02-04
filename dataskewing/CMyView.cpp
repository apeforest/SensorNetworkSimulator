/////////////////////////////////////////////////////////////////////////////
// CMyView.cpp : Implementation of the CMyView class (Derived From COpenGLView)
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "CMyView.h"



/////////////////////////////////////////////////////////////////////////////
// CMyView
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMyView, COpenGLView)



#ifdef _DEBUG // NOTE: Non-debug version is inline
CMainDoc* CMyView::GetDocument()
{ // GetDocument
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMainDoc)));
	return (CMainDoc*)m_pDocument;
} // GetDocument
#endif //_DEBUG


CMyView::CMyView()
{ // CMyView
  TS = NULL;
  SensorRadiusOffsets = NULL;
  GridXOffsets = NULL;
  GridYOffsets = NULL;
  GridSpeed = CMYVIEW_GRID_SPEED;
  SensorRadiusSpeed = CMYVIEW_SENSOR_RADIUS_SPEED;
  ZoomTime = CMYVIEW_ZOOM_TIME;
  State = CMYVIEW_SYSTEM_INIT;
} // CMyView



CMyView::~CMyView()
{ // ~CMyView
  TS = NULL;
} // ~CMyView



void CMyView::InitializeColors(void)
{ // InitializeColors
  Colors[CMYVIEW_COLOR_BLACK][CMYVIEW_INDEX_RED]         = 0.00;
  Colors[CMYVIEW_COLOR_BLACK][CMYVIEW_INDEX_GREEN]       = 0.00;
  Colors[CMYVIEW_COLOR_BLACK][CMYVIEW_INDEX_BLUE]        = 0.00;
  
  Colors[CMYVIEW_COLOR_DARKBLUE][CMYVIEW_INDEX_RED]      = 0.00;
  Colors[CMYVIEW_COLOR_DARKBLUE][CMYVIEW_INDEX_GREEN]    = 0.00;
  Colors[CMYVIEW_COLOR_DARKBLUE][CMYVIEW_INDEX_BLUE]     = 0.50;

  Colors[CMYVIEW_COLOR_DARKGREEN][CMYVIEW_INDEX_RED]     = 0.00;
  Colors[CMYVIEW_COLOR_DARKGREEN][CMYVIEW_INDEX_GREEN]   = 0.50;
  Colors[CMYVIEW_COLOR_DARKGREEN][CMYVIEW_INDEX_BLUE]    = 0.00;

  Colors[CMYVIEW_COLOR_DARKCYAN][CMYVIEW_INDEX_RED]      = 0.00;
  Colors[CMYVIEW_COLOR_DARKCYAN][CMYVIEW_INDEX_GREEN]    = 0.50;
  Colors[CMYVIEW_COLOR_DARKCYAN][CMYVIEW_INDEX_BLUE]     = 0.50;

  Colors[CMYVIEW_COLOR_DARKRED][CMYVIEW_INDEX_RED]       = 0.50;
  Colors[CMYVIEW_COLOR_DARKRED][CMYVIEW_INDEX_GREEN]     = 0.00;
  Colors[CMYVIEW_COLOR_DARKRED][CMYVIEW_INDEX_BLUE]      = 0.00;

  Colors[CMYVIEW_COLOR_DARKMAGENTA][CMYVIEW_INDEX_RED]   = 0.50;
  Colors[CMYVIEW_COLOR_DARKMAGENTA][CMYVIEW_INDEX_GREEN] = 0.00;
  Colors[CMYVIEW_COLOR_DARKMAGENTA][CMYVIEW_INDEX_BLUE]  = 0.50;

  Colors[CMYVIEW_COLOR_DARKYELLOW][CMYVIEW_INDEX_RED]    = 0.50;
  Colors[CMYVIEW_COLOR_DARKYELLOW][CMYVIEW_INDEX_GREEN]  = 0.50;
  Colors[CMYVIEW_COLOR_DARKYELLOW][CMYVIEW_INDEX_BLUE]   = 0.00;

  Colors[CMYVIEW_COLOR_GRAY][CMYVIEW_INDEX_RED]          = 0.75;
  Colors[CMYVIEW_COLOR_GRAY][CMYVIEW_INDEX_GREEN]        = 0.75;
  Colors[CMYVIEW_COLOR_GRAY][CMYVIEW_INDEX_BLUE]         = 0.75;

  Colors[CMYVIEW_COLOR_DARKGRAY][CMYVIEW_INDEX_RED]      = 0.25;
  Colors[CMYVIEW_COLOR_DARKGRAY][CMYVIEW_INDEX_GREEN]    = 0.25;
  Colors[CMYVIEW_COLOR_DARKGRAY][CMYVIEW_INDEX_BLUE]     = 0.25;

  Colors[CMYVIEW_COLOR_BLUE][CMYVIEW_INDEX_RED]          = 0.00;
  Colors[CMYVIEW_COLOR_BLUE][CMYVIEW_INDEX_GREEN]        = 0.00;
  Colors[CMYVIEW_COLOR_BLUE][CMYVIEW_INDEX_BLUE]         = 1.00;

  Colors[CMYVIEW_COLOR_GREEN][CMYVIEW_INDEX_RED]         = 0.00;
  Colors[CMYVIEW_COLOR_GREEN][CMYVIEW_INDEX_GREEN]       = 1.00;
  Colors[CMYVIEW_COLOR_GREEN][CMYVIEW_INDEX_BLUE]        = 0.00;

  Colors[CMYVIEW_COLOR_CYAN][CMYVIEW_INDEX_RED]          = 0.00;
  Colors[CMYVIEW_COLOR_CYAN][CMYVIEW_INDEX_GREEN]        = 1.00;
  Colors[CMYVIEW_COLOR_CYAN][CMYVIEW_INDEX_BLUE]         = 1.00;

  Colors[CMYVIEW_COLOR_RED][CMYVIEW_INDEX_RED]           = 1.00;
  Colors[CMYVIEW_COLOR_RED][CMYVIEW_INDEX_GREEN]         = 0.00;
  Colors[CMYVIEW_COLOR_RED][CMYVIEW_INDEX_BLUE]          = 0.00;
  
  Colors[CMYVIEW_COLOR_MAGENTA][CMYVIEW_INDEX_RED]       = 1.00;
  Colors[CMYVIEW_COLOR_MAGENTA][CMYVIEW_INDEX_GREEN]     = 0.00;
  Colors[CMYVIEW_COLOR_MAGENTA][CMYVIEW_INDEX_BLUE]      = 1.00;

  Colors[CMYVIEW_COLOR_YELLOW][CMYVIEW_INDEX_RED]        = 1.00;
  Colors[CMYVIEW_COLOR_YELLOW][CMYVIEW_INDEX_GREEN]      = 1.00;
  Colors[CMYVIEW_COLOR_YELLOW][CMYVIEW_INDEX_BLUE]       = 0.00;

  Colors[CMYVIEW_COLOR_WHITE][CMYVIEW_INDEX_RED]         = 1.00;
  Colors[CMYVIEW_COLOR_WHITE][CMYVIEW_INDEX_GREEN]       = 1.00;
  Colors[CMYVIEW_COLOR_WHITE][CMYVIEW_INDEX_BLUE]        = 1.00;
} // InitializeColors



void CMyView::InitializeCamera(void)
{ // InitializeCamera
  if (TS != NULL)
  {
    CameraInitialX = 0.0;
    CameraInitialY = 0.0;
    CameraInitialZ = ((TS->GetBoundsUX() - TS->GetBoundsLX()) / 2.0) / tan(HalfAngle * CMYVIEW_PI / 180.0);

    CameraFinalX = CameraInitialX;
    CameraFinalY = CameraInitialY;
    CameraFinalZ = CameraInitialZ;
    CameraX = CameraInitialX;
    CameraY = CameraInitialY;
    CameraZ = CameraInitialZ;
  }
} // InitializeCamera



void CMyView::InitializeSensorRadiusOffsets(void)
{ // InitializeSensorRadiusOffsets
  int i = 0; // Index Variable

  if (SensorRadiusOffsets != NULL) delete[] SensorRadiusOffsets;
  SensorRadiusOffsets = new double[NumSensors];
  for (i = 0; i < NumSensors; ++i)
  {
    SensorRadiusOffsets[i] = -TS->GetSensorRadius(i) -CMYVIEW_STD_MULT * TS->GetStandardDeviation();;
  }
} // InitializeSensorRadiusOffsets



void CMyView::OnDraw(CDC *pDC)
{ // OnDraw
  CMainDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
  COpenGLView::OnDraw(pDC);
} // OnDraw



void CMyView::OnSize(UINT nType, int cx, int cy) 
{ // OnSize
	CView::OnSize(nType, cx, cy);
	GLdouble aspect_ratio; // Width/Height Ratio

  if (0 >= cx || 0 >= cy)
  {
    return;
  }

  SetupViewport(cx, cy);

  // Compute the aspect ratio.
  // This will keep all dimension scales equal
  aspect_ratio = (GLdouble) cx / (GLdouble) cy;

  // Select The Projection Matrix and Clear It
  ::glMatrixMode(GL_PROJECTION);
  ::glLoadIdentity();

  // Select The Viewing Volume
  SetupViewingFrustum(aspect_ratio);

  // Switch Back To The ModelView Matrix and Clear It
  ::glMatrixMode(GL_MODELVIEW);
  ::glLoadIdentity();

  // Now Perform Any Viewing Transformations
  // SetupViewingTransform();
} // OnSize



BOOL CMyView::PreRenderScene(void)
{ // PreRenderScene
  // Clear The Background
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return(TRUE);
} // PreRenderScene



BOOL CMyView::RenderScene(void)
{ // RenderScene
  int i = 0;
  int j = 0;

  // Draw The TLS If It Is Present
  if (TS != NULL)
  {
    // Set The Camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    InitializeColors();
    gluLookAt(CameraX, CameraY, CameraZ, CameraX, CameraY, 0.0, 0.0, 1.0, 0.0);
    // Draw The Sensors
    for (i = 0; i < TS->GetNumSensors(); ++i)
    {
      DrawSensorRadius(i);
    }
    // Draw The Grid
    for (i = 0; i < GridX; ++i)
    {
      for (j = 0; j < GridY; ++j)
      {
        // if ((i != TS->GetCurrentColumn()) || (j != TS->GetCurrentRow()))
        // {
        DrawGridSquare(i, j);
        // }
      }
    }
    // Draw The Points
    /*
    DrawPoints();
    */
  }
  return(TRUE);
} // RenderScene


/*
BOOL CMyView::RenderScene(void)
{ // RenderScene
  int i = 0;
  int j = 0;
  int Rows = 0;
  int Columns = 0;

  // Draw The TLS If It Is Present
  if (TS != NULL)
  {
    // Initialize The Drawing Variables
    Columns = TS->GetGridX();
    Rows = TS->GetGridY();
    // Set The Camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // SetupViewingFrustum(1.0);
    // ***** TS->InitializeCamera(20.0);
    //glPerspective();
    // ***** gluLookAt(TS->GetCameraX(), TS->GetCameraY(), TS->GetCameraZ(), TS->GetCameraX(), TS->GetCameraY(), 0.0, 0.0, 1.0, 0.0);
    // Draw The Sensors
    for (i = 0; i < TS->GetNumSensors(); ++i)
    {
      DrawSensorRadius(i);
    }
    // Draw The Grid
    for (i = 0; i < Columns; ++i)
    {
      for (j = 0; j < Rows; ++j)
      {
        // if ((i != TS->GetCurrentColumn()) || (j != TS->GetCurrentRow()))
        // {
        DrawGridSquare(i, j);
        // }
      }
    }
    // Draw The Points
    DrawPoints();
  }
  return(TRUE);
} // RenderScene
*/


BOOL CMyView::RenderStockScene(void)
{ // RenderStockScene
  return(TRUE);
} // RenderStockScene



void CMyView::TSSet(TrilaterationSystem* T)
{ // TSSet
  TS = T;
  NumSensors = T->GetNumSensors();
  InitializeSensorRadiusOffsets();
} // TSSet



void CMyView::DrawSensorRadius(int SensorNum)
{ // DrawSensorRadius
  int i = 0;                 // Index Variable
  int j = 0;                 // Index Variable
  int ColorNum = 0;          // Color Number To Display
  double X1[4] = { 0.0 };    // X Coordinate To Display (4 Points Because Of Quad) - Inner To Middle
  double Y1[4] = { 0.0 };    // Y Coordinate To Display (4 Points Because Of Quad) - Inner To Middle
  double Z1[4] = { 0.0 };    // Z Coordinate To Display (4 Points Because Of Quad) - Inner To Middle
  double X2[4] = { 0.0 };    // X Coordinate To Display (4 Points Because Of Quad) - Middle To Outer
  double Y2[4] = { 0.0 };    // Y Coordinate To Display (4 Points Because Of Quad) - Middle To Outer
  double Z2[4] = { 0.0 };    // Z Coordinate To Display (4 Points Because Of Quad) - Middle To Outer
  double RadiusI = 0.0;      // Inner Radius
  double RadiusM = 0.0;      // Middle Radius (Measured)
  double RadiusO = 0.0;      // Outer Radius
  double ToRadians = 0.0;    // Conversion To Radians
  double SensorX = 0.0;      // X Coordinate Of Sensor
  double SensorY = 0.0;      // Y Coordinate Of Sensor
  double SensorZ = 0.0;      // Z Coordinate Of Sensor
  double LowProb = 0.0;      // Lower Probability Value (Inner, Outer);
  double HighProb = 0.0;     // Higher Probability Value (Middle)
  double Offset = 0.0;       // Offset From Radius

  ToRadians = CMYVIEW_PI / 180.0;
  ColorNum = ((SensorNum + 8) % (CMYVIEW_COLOR_NUM - 2)) + 1;
  SensorX = TS->GetSensorX(SensorNum);
  SensorY = TS->GetSensorY(SensorNum);
  SensorZ = TS->GetSensorZ(SensorNum);
  Offset = CMYVIEW_STD_MULT * TS->GetStandardDeviation();
  RadiusM = TS->GetSensorRadius(SensorNum) + SensorRadiusOffsets[SensorNum];
  RadiusI = RadiusM - Offset;
  RadiusO = RadiusM + Offset;
  LowProb = TS->GetSensorProbability(SensorNum, Offset);
  HighProb = TS->GetSensorProbability(SensorNum, 0.0);
  if (RadiusM < 0.0) RadiusM = 0.0;
  if (RadiusI < 0.0) RadiusI = 0.0;
  if (RadiusO < 0.0) RadiusO = 0.0;
  for (i = 0; i < 360; ++i)
  {
    // Calculate Inner-Radius Quad
    X1[0] = SensorX + RadiusI * cos(i * ToRadians);
    Y1[0] = SensorY + RadiusI * sin(i * ToRadians);
    X1[1] = SensorX + RadiusM * cos(i * ToRadians);
    Y1[1] = SensorY + RadiusM * sin(i * ToRadians);
    X1[2] = SensorX + RadiusM * cos((i + 1) * ToRadians);
    Y1[2] = SensorY + RadiusM * sin((i + 1) * ToRadians);
    X1[3] = SensorX + RadiusI * cos((i + 1) * ToRadians);
    Y1[3] = SensorY + RadiusI * sin((i + 1) * ToRadians);
    // Calculate Outer-Radius Quad
    X2[0] = SensorX + RadiusM * cos(i * ToRadians);
    Y2[0] = SensorY + RadiusM * sin(i * ToRadians);
    X2[1] = SensorX + RadiusO * cos(i * ToRadians);
    Y2[1] = SensorY + RadiusO * sin(i * ToRadians);
    X2[2] = SensorX + RadiusO * cos((i + 1) * ToRadians);
    Y2[2] = SensorY + RadiusO * sin((i + 1) * ToRadians);
    X2[3] = SensorX + RadiusM * cos((i + 1) * ToRadians);
    Y2[3] = SensorY + RadiusM * sin((i + 1) * ToRadians);
    // Draw The Quads
    glBegin(GL_QUADS);
      glColor4f((GLfloat) (LowProb * Colors[ColorNum][CMYVIEW_INDEX_RED]), (GLfloat) (LowProb * Colors[ColorNum][CMYVIEW_INDEX_GREEN]), (GLfloat) (LowProb * Colors[ColorNum][CMYVIEW_INDEX_BLUE]), 1.0);
      glVertex3f((GLfloat) (X1[0]), (GLfloat) (Y1[0]), (GLfloat) (Z1[0] + SensorZ));
      glColor4f((GLfloat) (HighProb * Colors[ColorNum][CMYVIEW_INDEX_RED]), (GLfloat) (HighProb * Colors[ColorNum][CMYVIEW_INDEX_GREEN]), (GLfloat) (HighProb * Colors[ColorNum][CMYVIEW_INDEX_BLUE]), 1.0);
      glVertex3f((GLfloat) (X1[1]), (GLfloat) (Y1[1]), (GLfloat) (Z1[1] + SensorZ));
      glVertex3f((GLfloat) (X1[2]), (GLfloat) (Y1[2]), (GLfloat) (Z1[2] + SensorZ));
      glColor4f((GLfloat) (LowProb * Colors[ColorNum][CMYVIEW_INDEX_RED]), (GLfloat) (LowProb * Colors[ColorNum][CMYVIEW_INDEX_GREEN]), (GLfloat) (LowProb * Colors[ColorNum][CMYVIEW_INDEX_BLUE]), 1.0);
      glVertex3f((GLfloat) (X1[3]), (GLfloat) (Y1[3]), (GLfloat) (Z1[3] + SensorZ));

      glColor4f((GLfloat) (HighProb * Colors[ColorNum][CMYVIEW_INDEX_RED]), (GLfloat) (HighProb * Colors[ColorNum][CMYVIEW_INDEX_GREEN]), (GLfloat) (HighProb * Colors[ColorNum][CMYVIEW_INDEX_BLUE]), 1.0);
      glVertex3f((GLfloat) (X2[0]), (GLfloat) (Y2[0]), (GLfloat) (Z2[0] + SensorZ));
      glColor4f((GLfloat) (LowProb * Colors[ColorNum][CMYVIEW_INDEX_RED]), (GLfloat) (LowProb * Colors[ColorNum][CMYVIEW_INDEX_GREEN]), (GLfloat) (LowProb * Colors[ColorNum][CMYVIEW_INDEX_BLUE]), 1.0);
      glVertex3f((GLfloat) (X2[1]), (GLfloat) (Y2[1]), (GLfloat) (Z2[1] + SensorZ));
      glVertex3f((GLfloat) (X2[2]), (GLfloat) (Y2[2]), (GLfloat) (Z2[2] + SensorZ));
      glColor4f((GLfloat) (HighProb * Colors[ColorNum][CMYVIEW_INDEX_RED]), (GLfloat) (HighProb * Colors[ColorNum][CMYVIEW_INDEX_GREEN]), (GLfloat) (HighProb * Colors[ColorNum][CMYVIEW_INDEX_BLUE]), 1.0);
      glVertex3f((GLfloat) (X2[3]), (GLfloat) (Y2[3]), (GLfloat) (Z2[3] + SensorZ));
    glEnd();
  }
} // DrawSensorRadius



void CMyView::DrawGridSquare(int Column, int Row)
{ // DrawGridSquare
  int Index = CalculateGridIndex(Column, Row);

  double LX = TS->GetGridSquareLX(Column, Row) + GridXOffsets[Index];
  double LY = TS->GetGridSquareLY(Column, Row) + GridYOffsets[Index];
  double UX = TS->GetGridSquareUX(Column, Row) + GridXOffsets[Index];
  double UY = TS->GetGridSquareUY(Column, Row) + GridYOffsets[Index];
  double Z = 0.07;

  glEnable(GL_LINE_SMOOTH);
  glBegin(GL_LINE_LOOP);
    glLineWidth(2.5);
    if ((Column == TS->GetCurrentColumn()) && (Row == TS->GetCurrentRow()))
    {
      // ***** glColor4f((GLfloat) TS->GetGridRedHighlight(), (GLfloat) TS->GetGridGreenHighlight(), (GLfloat) TS->GetGridBlueHighlight(), 1.0);
      glColor4f(1.0, 1.0, 1.0, 1.0);
      LX -= 0.1 * (UX - LX);
      LY -= 0.1 * (UY - LY);
      UX += 0.1 * (UX - LX);
      UY += 0.1 * (UY - LY);
    }
    else
    {
      // ***** glColor4f((GLfloat) TS->GetGridRed(), (GLfloat) TS->GetGridGreen(), (GLfloat) TS->GetGridBlue(), 1.0);
      glColor4f(1.0, 1.0, 1.0, 1.0);
    }
    glVertex3f((GLfloat) LX, (GLfloat) LY, (GLfloat) Z);
    glVertex3f((GLfloat) UX, (GLfloat) LY, (GLfloat) Z);
    glVertex3f((GLfloat) UX, (GLfloat) UY, (GLfloat) Z);
    glVertex3f((GLfloat) LX, (GLfloat) UY, (GLfloat) Z);
  glEnd();
} // DrawGridSquare



void CMyView::DrawPoints(void)
{ // DrawPoints
  int i = 0;
  double ZoomOffsetX = 0;
  double ZoomOffsetY = 0;
  double ZoomScaleX = 0;
  double ZoomScaleY = 0;
  double X = 0;
  double Y = 0;

  if (TS != NULL)
  {
    // ***** ZoomOffsetX = TS->GetOffsetX();
    // ***** ZoomOffsetY = TS->GetOffsetY();
    // ***** ZoomScaleX = TS->GetScaleX();
    // ***** ZoomScaleY = TS->GetScaleY();
    glEnable(GL_POINT_SMOOTH);
    glPointSize(5.0);
    glBegin(GL_POINTS);
      // Draw Sensor Points
      for (i = 0; i < TS->GetNumSensors(); ++i)
      {
        // ***** glColor3f((GLfloat) TS->GetSensorRed(i), (GLfloat) TS->GetSensorGreen(i), (GLfloat) TS->GetSensorBlue(i));
        X = (TS->GetSensorX(i) + ZoomOffsetX) * ZoomScaleX;
        Y = (TS->GetSensorY(i) + ZoomOffsetY) * ZoomScaleY;
        glVertex3f((GLfloat) X, (GLfloat) Y, (GLfloat) 0.055);
      }
      // Draw Measured Point
      glColor3f(1.0, 1.0, 0.0);
      // ***** X = (TS->GetDrawMeasuredX() + ZoomOffsetX) * ZoomScaleX;
      // ***** Y = (TS->GetDrawMeasuredY() + ZoomOffsetY) * ZoomScaleY;
      if ((X > -1000) && (Y > -1000)) glVertex3f((GLfloat) X, (GLfloat) Y, (GLfloat) 0.055);
      // Draw Lied Point
      glColor3f(0.75, 0.75, 0.75);
      X = (((TrilaterationLiarSystem*) TS)->LiarGetDrawLiedX() + ZoomOffsetX) * ZoomScaleX;
      Y = (((TrilaterationLiarSystem*) TS)->LiarGetDrawLiedY() + ZoomOffsetY) * ZoomScaleY;
      if ((X > -1000) && (Y > -1000)) glVertex3f((GLfloat) X, (GLfloat) Y, (GLfloat) 0.055);
      // Draw Actual Point
      glColor3f(1.0, 1.0, 1.0);
      // ***** X = (TS->GetDrawActualX() + ZoomOffsetX) * ZoomScaleX;
      // ***** Y = (TS->GetDrawActualY() + ZoomOffsetY) * ZoomScaleY;
    glEnd();
  }
} // DrawPoints


void CMyView::TSSetUpdateMode(int UpdateMode)
{ // TSSetUpdateMode
  // ***** TS->SetUpdateMode(UpdateMode);
} // TSSetUpdateMode



void CMyView::Update(void)
{ // Update
  unsigned long CurrentTime = timeGetTime();
  TrilaterationSystemStates SystemState = TS->GetState();

  switch(State)
  {
    case CMYVIEW_SYSTEM_INIT:
      InitializeCamera();
      LastTime = CurrentTime;
      State = CMYVIEW_SYSTEM_UPDATE;
    break;

    case CMYVIEW_SYSTEM_UPDATE:
      TS->Update();
      switch(TS->GetState())
      {
        case DRAW_SENSOR:
          InitializeSensorRadiusOffsets();
          State = CMYVIEW_ANIMATE_SENSORS;
        break;

        case DRAW_ZOOM_PROBLEM:
        case DRAW_ZOOM_INITIAL_AREA:
        case DRAW_ZOOM_AREA:
        case DRAW_ZOOM_END:
          InitializeZoom(CurrentTime);
          State = CMYVIEW_ANIMATE_ZOOM;
        break;

        case DRAW_GRID:
          InitializeGrid();
          State = CMYVIEW_ANIMATE_GRID;
        break;
      }
    break;

    case CMYVIEW_ANIMATE_SENSORS:
      if (UpdateSensorRadiusOffsets(CurrentTime))
      {
        State = CMYVIEW_SYSTEM_UPDATE;
      }
    break;

    case CMYVIEW_ANIMATE_ZOOM:
      if (UpdateZoom(CurrentTime))
      {
        FinalizeZoom();
        State = CMYVIEW_SYSTEM_UPDATE;
      }
    break;

    case CMYVIEW_ANIMATE_GRID:
      if (UpdateGrid(CurrentTime))
      {
        State = CMYVIEW_SYSTEM_UPDATE;
      }
    break;
  }
  LastTime = CurrentTime;
} // Update



int CMyView::UpdateSensorRadiusOffsets(unsigned long CurrentTime)
{ // UpdateSensorRadiusOffsets
  int i = 0;
  int ReturnVal = 1;
  double TimeOffset = ((double) (CurrentTime - LastTime)) / 1000.0;

  for (i = 0; i < NumSensors; ++i)
  {
    if (SensorRadiusOffsets[i] < 0.0)
    {
      SensorRadiusOffsets[i] += TimeOffset * SensorRadiusSpeed;
      ReturnVal = 0;
    }
    if (SensorRadiusOffsets[i] > 0.0)
    {
      SensorRadiusOffsets[i] = 0.0;
    }
  }
  return(ReturnVal);
} // UpdateSensorRadiusOffsets



void CMyView::InitializeZoom(unsigned long CurrentTime)
{ // InitializeZoom
  double LX = TS->GetCurrentLX();
  double LY = TS->GetCurrentLY();
  double UX = TS->GetCurrentUX();
  double UY = TS->GetCurrentUY();
  
  CameraFinalX = (LX + UX) / 2.0;
  CameraFinalY = (LY + UY) / 2.0;
  CameraFinalZ = ((UX - LX) / 2.0) / tan(HalfAngle * CMYVIEW_PI / 180.0);

  ZoomStart = CurrentTime;
} // InitializeZoom



int CMyView::UpdateZoom(unsigned long CurrentTime)
{ // UpdateZoom
  int ReturnVal = 0;
  double u = (((double) CurrentTime) - ((double) ZoomStart)) / ((double) ZoomTime);
  double t = 1.0 - ((1.0 - u) * (1.0 - u));

  if (u > 1.0)
  {
    ReturnVal = 1;
    t = 1.0;
    u = 1.0;
  }
  
  CameraX = CameraInitialX + t * (CameraFinalX - CameraInitialX);
  CameraY = CameraInitialY + t * (CameraFinalY - CameraInitialY);
  CameraZ = CameraInitialZ + t * (CameraFinalZ - CameraInitialZ);

  return(ReturnVal);
} // UpdateZoom



void CMyView::FinalizeZoom(void)
{ // FinalizeZoom
  CameraInitialX = CameraFinalX;
  CameraInitialY = CameraFinalY;
  CameraInitialZ = CameraFinalZ;
  CameraX = CameraFinalX;
  CameraY = CameraFinalY;
  CameraZ = CameraFinalZ;
} // FinalizeZoom



void CMyView::InitializeGrid(void)
{ // InitializeGrid
  int i = 0;
  int j = 0;
  double Width = 0.0;
  double Height = 0.0;

  if (GridXOffsets != NULL) delete[] GridXOffsets;
  if (GridYOffsets != NULL) delete[] GridYOffsets;
  GridX = TS->GetGridX();
  GridY = TS->GetGridY();
  Width = (TS->GetCurrentUX() - TS->GetCurrentLX()) / GridX;
  Height = (TS->GetCurrentUY() - TS->GetCurrentLY()) / GridY;
  GridXOffsets = new double[GridX * GridY];
  GridYOffsets = new double[GridX * GridY];
  for (i = 0; i < GridX; ++i)
  {
    for(j = 0; j < GridY; ++j)
    {
      GridXOffsets[CalculateGridIndex(i, j)] = -(Width * (2 * i + 1.5));
      GridYOffsets[CalculateGridIndex(i, j)] = -(Width * (2 * j + 1.5));
    }
  }
} // InitializeGrid



int CMyView::UpdateGrid(unsigned long CurrentTime)
{ // UpdateGrid
  int i = 0;
  int j = 0;
  int Index = 0;
  int ReturnVal = 1;
  double TimeOffset = ((double) (CurrentTime - LastTime)) / 1000.0;
  double Offset = TimeOffset * GridSpeed;

  for(i = 0; i < GridX; ++i)
  {
    for(j = 0; j < GridY; ++j)
    {
      Index = CalculateGridIndex(i, j);
      GridXOffsets[Index] += Offset;
      GridYOffsets[Index] += Offset;
      if (GridXOffsets[Index] >= 0.0) GridXOffsets[Index] = 0.0;
      else ReturnVal = 0;
      if (GridYOffsets[Index] >= 0.0) GridYOffsets[Index] = 0.0;
      else ReturnVal = 0;
    }
  }
  return(ReturnVal);
} // UpdateGrid


int CMyView::CalculateGridIndex(int X, int Y)
{ // CalculateGridIndex
  return(X + Y * GridX);
} // CalculateGridIndex
