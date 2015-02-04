/////////////////////////////////////////////////////////////////////////////
// CTrilaterationSystemView.cpp : Implementation of the
// CTrilaterationSystemView class (Derived From COpenGLView)
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "CTrilaterationSystemView.h"



/////////////////////////////////////////////////////////////////////////////
// CTrilaterationSystemView
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CTrilaterationSystemView, COpenGLView)



#ifdef _DEBUG // NOTE: Non-debug version is inline
CMainDoc* CTrilaterationSystemView::GetDocument()
{ // GetDocument
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMainDoc)));
	return (CMainDoc*)m_pDocument;
} // GetDocument
#endif //_DEBUG


CTrilaterationSystemView::CTrilaterationSystemView()
{ // CTrilaterationSystemView
  TS = NULL;
  SensorRadiusOffsets = NULL;
  GridXOffsets = NULL;
  GridYOffsets = NULL;
  PointRadius = CTRILATERATIONSYSTEMVIEW_POINT_RADIUS;
  GridSpeed = CTRILATERATIONSYSTEMVIEW_GRID_SPEED;
  SensorRadiusSpeed = CTRILATERATIONSYSTEMVIEW_SENSOR_RADIUS_SPEED;
  ZoomTime = CTRILATERATIONSYSTEMVIEW_ZOOM_TIME;
  State = CTRILATERATIONSYSTEMVIEW_SYSTEM_INIT;
  InitializeColors();
  InitializeSensorColors();
  GridColor = CTRILATERATIONSYSTEMVIEW_GRID_COLOR;
  GridHighlightColor = CTRILATERATIONSYSTEMVIEW_GRID_COLOR_HIGHLIGHT;
  MeasuredPointColor = CTRILATERATIONSYSTEMVIEW_MEASURED_COLOR;
  ActualPointColor = CTRILATERATIONSYSTEMVIEW_ACTUAL_COLOR;
  OrthoInitialLeft = CTRILATERATIONSYSTEMVIEW_ORTHO_LEFT;
  OrthoInitialRight = CTRILATERATIONSYSTEMVIEW_ORTHO_RIGHT;
  OrthoInitialBottom = CTRILATERATIONSYSTEMVIEW_ORTHO_BOTTOM;
  OrthoInitialTop = CTRILATERATIONSYSTEMVIEW_ORTHO_TOP;
  OrthoFinalLeft = CTRILATERATIONSYSTEMVIEW_ORTHO_LEFT;
  OrthoFinalRight = CTRILATERATIONSYSTEMVIEW_ORTHO_RIGHT;
  OrthoFinalBottom = CTRILATERATIONSYSTEMVIEW_ORTHO_BOTTOM;
  OrthoFinalTop = CTRILATERATIONSYSTEMVIEW_ORTHO_TOP;
  OrthoLeft = CTRILATERATIONSYSTEMVIEW_ORTHO_LEFT;
  OrthoRight = CTRILATERATIONSYSTEMVIEW_ORTHO_RIGHT;
  OrthoBottom = CTRILATERATIONSYSTEMVIEW_ORTHO_BOTTOM;
  OrthoTop = CTRILATERATIONSYSTEMVIEW_ORTHO_TOP;
  OrthoNear = CTRILATERATIONSYSTEMVIEW_ORTHO_NEAR;
  OrthoFar = CTRILATERATIONSYSTEMVIEW_ORTHO_FAR;
  DrawUpdates = 1;
} // CTrilaterationSystemView



CTrilaterationSystemView::~CTrilaterationSystemView()
{ // ~CTrilaterationSystemView
  TS = NULL;
} // ~CTrilaterationSystemView



BOOL CTrilaterationSystemView::SetupViewingFrustum(GLdouble AspectRatio)
{ // SetupViewingFrustum
  // Select a Default Viewing Volume
  // gluPerspective(HalfAngle * 2.0, AspectRatio, NearPlane, FarPlane);
  glOrtho(OrthoLeft, OrthoRight, OrthoBottom, OrthoTop, OrthoNear, OrthoFar);
  return TRUE;
} // SetupViewingFrustum



void CTrilaterationSystemView::InitializeColors(void)
{ // InitializeColors
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_BLACK][CTRILATERATIONSYSTEMVIEW_INDEX_RED]         = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_BLACK][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]       = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_BLACK][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]        = 0.00;
  
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKBLUE][CTRILATERATIONSYSTEMVIEW_INDEX_RED]      = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKBLUE][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]    = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKBLUE][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]     = 0.50;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKGREEN][CTRILATERATIONSYSTEMVIEW_INDEX_RED]     = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKGREEN][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]   = 0.50;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKGREEN][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]    = 0.00;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKCYAN][CTRILATERATIONSYSTEMVIEW_INDEX_RED]      = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKCYAN][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]    = 0.50;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKCYAN][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]     = 0.50;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKRED][CTRILATERATIONSYSTEMVIEW_INDEX_RED]       = 0.50;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKRED][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]     = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKRED][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]      = 0.00;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKMAGENTA][CTRILATERATIONSYSTEMVIEW_INDEX_RED]   = 0.50;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKMAGENTA][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN] = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKMAGENTA][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]  = 0.50;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKYELLOW][CTRILATERATIONSYSTEMVIEW_INDEX_RED]    = 0.50;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKYELLOW][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]  = 0.50;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKYELLOW][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]   = 0.00;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_GRAY][CTRILATERATIONSYSTEMVIEW_INDEX_RED]          = 0.75;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_GRAY][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]        = 0.75;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_GRAY][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]         = 0.75;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKGRAY][CTRILATERATIONSYSTEMVIEW_INDEX_RED]      = 0.25;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKGRAY][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]    = 0.25;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_DARKGRAY][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]     = 0.25;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_BLUE][CTRILATERATIONSYSTEMVIEW_INDEX_RED]          = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_BLUE][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]        = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_BLUE][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]         = 1.00;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_GREEN][CTRILATERATIONSYSTEMVIEW_INDEX_RED]         = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_GREEN][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]       = 1.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_GREEN][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]        = 0.00;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_CYAN][CTRILATERATIONSYSTEMVIEW_INDEX_RED]          = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_CYAN][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]        = 1.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_CYAN][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]         = 1.00;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_RED][CTRILATERATIONSYSTEMVIEW_INDEX_RED]           = 1.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_RED][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]         = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_RED][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]          = 0.00;
  
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_MAGENTA][CTRILATERATIONSYSTEMVIEW_INDEX_RED]       = 1.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_MAGENTA][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]     = 0.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_MAGENTA][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]      = 1.00;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_YELLOW][CTRILATERATIONSYSTEMVIEW_INDEX_RED]        = 1.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_YELLOW][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]      = 1.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_YELLOW][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]       = 0.00;

  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_WHITE][CTRILATERATIONSYSTEMVIEW_INDEX_RED]         = 1.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_WHITE][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]       = 1.00;
  Colors[CTRILATERATIONSYSTEMVIEW_COLOR_WHITE][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]        = 1.00;
} // InitializeColors



void CTrilaterationSystemView::InitializeSensorColors(void)
{ // InitializeSensorColors
  SensorColors[0] = CTRILATERATIONSYSTEMVIEW_COLOR_BLUE;
  SensorColors[1] = CTRILATERATIONSYSTEMVIEW_COLOR_CYAN;
  SensorColors[2] = CTRILATERATIONSYSTEMVIEW_COLOR_GREEN;
  SensorColors[3] = CTRILATERATIONSYSTEMVIEW_COLOR_YELLOW;
  SensorColors[4] = CTRILATERATIONSYSTEMVIEW_COLOR_RED;
  SensorColors[5] = CTRILATERATIONSYSTEMVIEW_COLOR_MAGENTA;
  SensorColors[6] = CTRILATERATIONSYSTEMVIEW_COLOR_DARKMAGENTA;
  SensorColors[7] = CTRILATERATIONSYSTEMVIEW_COLOR_DARKRED;
  SensorColors[8] = CTRILATERATIONSYSTEMVIEW_COLOR_DARKYELLOW;
  SensorColors[9] = CTRILATERATIONSYSTEMVIEW_COLOR_DARKGREEN;
  SensorColors[10] = CTRILATERATIONSYSTEMVIEW_COLOR_DARKCYAN;
  SensorColors[11] = CTRILATERATIONSYSTEMVIEW_COLOR_DARKBLUE;
} // InitializeSensorColors



void CTrilaterationSystemView::InitializeCamera(void)
{ // InitializeCamera
  if (TS != NULL)
  {
    CameraInitialX = 0.0;
    CameraInitialY = 0.0;
    CameraInitialZ = ((TS->GetBoundsUX() - TS->GetBoundsLX()) / 2.0) / tan(HalfAngle * CTRILATERATIONSYSTEMVIEW_PI / 180.0);
    InitialWidth = TS->GetBoundsUX() - TS->GetBoundsLX();

    CameraFinalX = CameraInitialX;
    CameraFinalY = CameraInitialY;
    CameraFinalZ = CameraInitialZ;
    FinalWidth = InitialWidth;
    CameraX = CameraInitialX;
    CameraY = CameraInitialY;
    CameraZ = CameraInitialZ;
    CurrentWidth = InitialWidth;
    
    // Set Up Orthogonal Stuff
    OrthoLeft = TS->GetBoundsLX();
    OrthoRight = TS->GetBoundsUX();;
    OrthoBottom = TS->GetBoundsLY();;
    OrthoTop = TS->GetBoundsUY();;
    CurrentWidth = OrthoRight - OrthoLeft;
  }
} // InitializeCamera



void CTrilaterationSystemView::InitializeSensorRadiusOffsets(void)
{ // InitializeSensorRadiusOffsets
  int i = 0; // Index Variable

  if (SensorRadiusOffsets != NULL) delete[] SensorRadiusOffsets;
  SensorRadiusOffsets = new double[NumSensors];
  for (i = 0; i < NumSensors; ++i)
  {
    SensorRadiusOffsets[i] = -TS->GetSensorRadius(i) -CTRILATERATIONSYSTEMVIEW_STD_MULT * TS->GetStandardDeviation();
  }
} // InitializeSensorRadiusOffsets



void CTrilaterationSystemView::ClearSensorRadiusOffsets(void)
{ // InitializeSensorRadiusOffsets
  int i = 0; // Index Variable

  if (SensorRadiusOffsets != NULL) delete[] SensorRadiusOffsets;
  SensorRadiusOffsets = new double[NumSensors];
  for (i = 0; i < NumSensors; ++i)
  {
    SensorRadiusOffsets[i] = 0.0;
  }
} // InitializeSensorRadiusOffsets



void CTrilaterationSystemView::OnDraw(CDC *pDC)
{ // OnDraw
  CMainDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
  COpenGLView::OnDraw(pDC);
} // OnDraw



void CTrilaterationSystemView::OnSize(UINT nType, int cx, int cy) 
{ // OnSize
	CView::OnSize(nType, cx, cy);
  SetupViewport(cx, cy);
  SetupViewingFrustum(0.0);

	GLdouble aspect_ratio; // Width/Height Ratio

  if (0 >= cx || 0 >= cy)
  {
    return;
  }

  

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



BOOL CTrilaterationSystemView::PreRenderScene(void)
{ // PreRenderScene
  // Clear The Background
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return(TRUE);
} // PreRenderScene



BOOL CTrilaterationSystemView::RenderScene(void)
{ // RenderScene
  int i = 0;
  int j = 0;

  // Draw The TLS If It Is Present
  if (TS != NULL)
  {
    // Set The Camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // gluLookAt(CameraX, CameraY, CameraZ, CameraX, CameraY, 0.0, 0.0, 1.0, 0.0);
    SetupViewingFrustum(0.0);

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
        DrawGridSquare(i, j);
      }
    }
    DrawGridSquareHighlight(TS->GetCurrentColumn(), TS->GetCurrentRow());

    // Draw The Points
    DrawPoints();
  }
  return(TRUE);
} // RenderScene


/*
BOOL CTrilaterationSystemView::RenderScene(void)
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


BOOL CTrilaterationSystemView::RenderStockScene(void)
{ // RenderStockScene
  return(TRUE);
} // RenderStockScene



void CTrilaterationSystemView::TSSet(TrilaterationSystem* T)
{ // TSSet
  TS = T;
  if (TS != NULL)
  {
    NumSensors = T->GetNumSensors();
    InitializeSensorRadiusOffsets();
  }
} // TSSet



void CTrilaterationSystemView::DrawSensorRadius(int SensorNum)
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

  ToRadians = CTRILATERATIONSYSTEMVIEW_PI / 180.0;
  ColorNum = SensorColors[(NumSensors - SensorNum - 1) % CTRILATERATIONSYSTEMVIEW_SENSOR_COLOR_NUM];
  SensorX = TS->GetSensorX(SensorNum);
  SensorY = TS->GetSensorY(SensorNum);
  SensorZ = TS->GetSensorZ(SensorNum) + CTRILATERATIONSYSTEMVIEW_RADIUS_Z_OFFSET; // + ((SensorNum + 1) * CTRILATERATIONSYSTEMVIEW_RADIUS_Z_OFFSET);
  Offset = CTRILATERATIONSYSTEMVIEW_STD_MULT * TS->GetStandardDeviation() * TS->GetSensorRadius(SensorNum);
  RadiusM = TS->GetSensorRadius(SensorNum) + SensorRadiusOffsets[SensorNum];
  RadiusI = RadiusM - Offset;
  RadiusO = RadiusM + Offset;
  LowProb = TS->GetSensorProbability(SensorNum, Offset);
  HighProb = TS->GetSensorProbability(SensorNum, 0.0);
  LowProb = 0.0;
  HighProb = 1.0;
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
      glColor4f((GLfloat) (LowProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_RED]), (GLfloat) (LowProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]), (GLfloat) (LowProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]), 1.0);
      glVertex3f((GLfloat) (X1[0]), (GLfloat) (Y1[0]), (GLfloat) (Z1[0] + SensorZ));
      glColor4f((GLfloat) (HighProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_RED]), (GLfloat) (HighProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]), (GLfloat) (HighProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]), 1.0);
      glVertex3f((GLfloat) (X1[1]), (GLfloat) (Y1[1]), (GLfloat) (Z1[1] + SensorZ));
      glVertex3f((GLfloat) (X1[2]), (GLfloat) (Y1[2]), (GLfloat) (Z1[2] + SensorZ));
      glColor4f((GLfloat) (LowProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_RED]), (GLfloat) (LowProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]), (GLfloat) (LowProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]), 1.0);
      glVertex3f((GLfloat) (X1[3]), (GLfloat) (Y1[3]), (GLfloat) (Z1[3] + SensorZ));

      glColor4f((GLfloat) (HighProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_RED]), (GLfloat) (HighProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]), (GLfloat) (HighProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]), 1.0);
      glVertex3f((GLfloat) (X2[0]), (GLfloat) (Y2[0]), (GLfloat) (Z2[0] + SensorZ));
      glColor4f((GLfloat) (LowProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_RED]), (GLfloat) (LowProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]), (GLfloat) (LowProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]), 1.0);
      glVertex3f((GLfloat) (X2[1]), (GLfloat) (Y2[1]), (GLfloat) (Z2[1] + SensorZ));
      glVertex3f((GLfloat) (X2[2]), (GLfloat) (Y2[2]), (GLfloat) (Z2[2] + SensorZ));
      glColor4f((GLfloat) (HighProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_RED]), (GLfloat) (HighProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN]), (GLfloat) (HighProb * Colors[ColorNum][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]), 1.0);
      glVertex3f((GLfloat) (X2[3]), (GLfloat) (Y2[3]), (GLfloat) (Z2[3] + SensorZ));
    glEnd();
  }
} // DrawSensorRadius



void CTrilaterationSystemView::DrawGridSquare(int Column, int Row)
{ // DrawGridSquare
  int Index = CalculateGridIndex(Column, Row);
  double OffsetX = 0.0;
  double OffsetY = 0.0;
  double LX = TS->GetGridSquareLX(Column, Row);
  double LY = TS->GetGridSquareLY(Column, Row);
  double UX = TS->GetGridSquareUX(Column, Row);
  double UY = TS->GetGridSquareUY(Column, Row);
  double Z = CTRILATERATIONSYSTEMVIEW_GRID_Z_OFFSET;

  if (GridXOffsets != NULL) OffsetX = GridXOffsets[Index];
  if (GridYOffsets != NULL) OffsetX = GridYOffsets[Index];

  LX += OffsetX;
  LY += OffsetY;
  UX += OffsetX;
  UY += OffsetY;

  glEnable(GL_LINE_SMOOTH);
  glBegin(GL_LINE_LOOP);
    glLineWidth(2.5);
    glColor3f((GLfloat) Colors[GridColor][CTRILATERATIONSYSTEMVIEW_INDEX_RED], (GLfloat) Colors[GridColor][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN], (GLfloat) Colors[GridColor][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]);
    glVertex3f((GLfloat) LX, (GLfloat) LY, (GLfloat) Z);
    glVertex3f((GLfloat) UX, (GLfloat) LY, (GLfloat) Z);
    glVertex3f((GLfloat) UX, (GLfloat) UY, (GLfloat) Z);
    glVertex3f((GLfloat) LX, (GLfloat) UY, (GLfloat) Z);
  glEnd();
} // DrawGridSquare



void CTrilaterationSystemView::DrawGridSquareHighlight(int Column, int Row)
{ // DrawGridSquare
  int Index = CalculateGridIndex(Column, Row);
  double OffsetX = 0.0;
  double OffsetY = 0.0;
  double LX = TS->GetGridSquareLX(Column, Row);
  double LY = TS->GetGridSquareLY(Column, Row);
  double UX = TS->GetGridSquareUX(Column, Row);
  double UY = TS->GetGridSquareUY(Column, Row);
  double Z = CTRILATERATIONSYSTEMVIEW_GRID_Z_OFFSET;

  if (GridXOffsets != NULL) OffsetX = GridXOffsets[Index];
  if (GridYOffsets != NULL) OffsetX = GridYOffsets[Index];

  LX += OffsetX;
  LY += OffsetY;
  UX += OffsetX;
  UY += OffsetY;

  glEnable(GL_LINE_SMOOTH);
  glBegin(GL_LINE_LOOP);
    glColor3f((GLfloat) Colors[GridHighlightColor][CTRILATERATIONSYSTEMVIEW_INDEX_RED], (GLfloat) Colors[GridHighlightColor][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN], (GLfloat) Colors[GridHighlightColor][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]);
    glLineWidth(2.5);
    LX -= 0.1 * (UX - LX);
    LY -= 0.1 * (UY - LY);
    UX += 0.1 * (UX - LX);
    UY += 0.1 * (UY - LY);
    glVertex3f((GLfloat) LX, (GLfloat) LY, (GLfloat) Z);
    glVertex3f((GLfloat) UX, (GLfloat) LY, (GLfloat) Z);
    glVertex3f((GLfloat) UX, (GLfloat) UY, (GLfloat) Z);
    glVertex3f((GLfloat) LX, (GLfloat) UY, (GLfloat) Z);
  glEnd();
} // DrawGridSquare



void CTrilaterationSystemView::DrawPoints(void)
{ // DrawPoints
  int i = 0;
  double X = 0;
  double Y = 0;
  double PointZOffset = CTRILATERATIONSYSTEMVIEW_POINT_Z_OFFSET;

  if (TS != NULL)
  {
    // glEnable(GL_POINT_SMOOTH);
    // glPointSize(5.0);
    // glBegin(GL_POINTS);
    // Draw Sensor Points
    for (i = 0; i < NumSensors; ++i)
    {
      // glColor3f((GLfloat) Colors[SensorColors[i]][CTRILATERATIONSYSTEMVIEW_INDEX_RED], (GLfloat) Colors[SensorColors[i]][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN], (GLfloat) Colors[SensorColors[i]][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]);
      // glVertex3f((GLfloat) TS->GetSensorX(i), (GLfloat) TS->GetSensorY(i), (GLfloat) (GLfloat) (PointZOffset * (i + 4)));
      DrawPoint(TS->GetSensorX(i), TS->GetSensorY(i), SensorColors[(NumSensors - i - 1) % CTRILATERATIONSYSTEMVIEW_SENSOR_COLOR_NUM]);
    }

    // Draw Actual Point
    // glColor3f((GLfloat) Colors[ActualPointColor][CTRILATERATIONSYSTEMVIEW_INDEX_RED], (GLfloat) Colors[ActualPointColor][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN], (GLfloat) Colors[ActualPointColor][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]);
    X = TS->GetActualX();
    Y = TS->GetActualY();
    // if ((X > -1000) && (Y > -1000)) glVertex3f((GLfloat) X, (GLfloat) Y, (GLfloat) (PointZOffset * 1));
    DrawPoint(X, Y, ActualPointColor);

    // Draw Measured Point
    // glColor3f((GLfloat) Colors[MeasuredPointColor][CTRILATERATIONSYSTEMVIEW_INDEX_RED], (GLfloat) Colors[MeasuredPointColor][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN], (GLfloat) Colors[MeasuredPointColor][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]);
    X = TS->GetMeasuredX();
    Y = TS->GetMeasuredY();
    // if ((X > -1000) && (Y > -1000)) glVertex3f((GLfloat) X, (GLfloat) Y, (GLfloat) (PointZOffset * 2));
    if ((X > -1000) && (Y > -1000)) DrawPoint(X, Y, MeasuredPointColor);
    // glEnd();
  }
} // DrawPoints



void CTrilaterationSystemView::DrawPoint(double X, double Y, int ColorIndex)
{ // DrawPoint
  int i = 0;
  double XR;
  double YR;
  double PointSize = PointRadius * CurrentWidth;

  glColor3f((GLfloat) Colors[ColorIndex][CTRILATERATIONSYSTEMVIEW_INDEX_RED], (GLfloat) Colors[ColorIndex][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN], (GLfloat) Colors[ColorIndex][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]);
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f((GLfloat) X, (GLfloat) Y, (GLfloat) PointZDistance);
    for (i = 0; i <= 360; ++i)
    {
      XR = X + PointSize * cos(i * CTRILATERATIONSYSTEMVIEW_PI / 180.0);
      YR = Y + PointSize * sin(i * CTRILATERATIONSYSTEMVIEW_PI / 180.0);
      glVertex3f((GLfloat) XR, (GLfloat) YR, (GLfloat) PointZDistance);
    }
  glEnd();
} // DrawPoint



void CTrilaterationSystemView::Update(void)
{ // Update
  CurrentTime = timeGetTime();

  switch(State)
  {
    case CTRILATERATIONSYSTEMVIEW_SYSTEM_INIT:
      InitializeCamera();
      ClearSensorRadiusOffsets();
      PointZDistance = CTRILATERATIONSYSTEMVIEW_POINT_Z_OFFSET;
      LastTime = CurrentTime;
      State = CTRILATERATIONSYSTEMVIEW_SYSTEM_UPDATE;
    break;

    case CTRILATERATIONSYSTEMVIEW_SYSTEM_UPDATE:
      TS->Update();
      if (DrawUpdates)
      {
        switch(TS->GetState())
        {
          case DRAW_SENSOR:
            InitializeSensorRadiusOffsets();
            State = CTRILATERATIONSYSTEMVIEW_ANIMATE_SENSORS;
          break;

          case DRAW_ZOOM_END:
            PointZDistance = CTRILATERATIONSYSTEMVIEW_POINT_Z_FINAL_OFFSET;
          case DRAW_ZOOM_PROBLEM:
          case DRAW_ZOOM_INITIAL_AREA:
          case DRAW_ZOOM_AREA:
        
            InitializeZoom(CurrentTime);
            State = CTRILATERATIONSYSTEMVIEW_ANIMATE_ZOOM;
          break;

          case DRAW_GRID:
            InitializeGrid();
            State = CTRILATERATIONSYSTEMVIEW_ANIMATE_GRID;
          break;
        }
      }
    break;

    case CTRILATERATIONSYSTEMVIEW_ANIMATE_SENSORS:
      if (UpdateSensorRadiusOffsets(CurrentTime))
      {
        State = CTRILATERATIONSYSTEMVIEW_SYSTEM_UPDATE;
      }
    break;

    case CTRILATERATIONSYSTEMVIEW_ANIMATE_ZOOM:
      if (UpdateZoom(CurrentTime))
      {
        FinalizeZoom();
        State = CTRILATERATIONSYSTEMVIEW_SYSTEM_UPDATE;
      }
    break;

    case CTRILATERATIONSYSTEMVIEW_ANIMATE_GRID:
      if (UpdateGrid(CurrentTime))
      {
        State = CTRILATERATIONSYSTEMVIEW_SYSTEM_UPDATE;
      }
    break;
  }
} // Update



int CTrilaterationSystemView::UpdateSensorRadiusOffsets(unsigned long CurrentTime)
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



void CTrilaterationSystemView::InitializeZoom(unsigned long CurrentTime)
{ // InitializeZoom
  double LX = TS->GetCurrentLX();
  double LY = TS->GetCurrentLY();
  double UX = TS->GetCurrentUX();
  double UY = TS->GetCurrentUY();
  
  CameraFinalX = (LX + UX) / 2.0;
  CameraFinalY = (LY + UY) / 2.0;
  CameraFinalZ = ((UX - LX) / 2.0) / tan(HalfAngle * CTRILATERATIONSYSTEMVIEW_PI / 180.0);
  FinalWidth = UX - LX;

  OrthoFinalLeft = LX;
  OrthoFinalRight = UX;
  OrthoFinalBottom = LY;
  OrthoFinalTop = UY;

  ZoomStart = CurrentTime;
} // InitializeZoom



int CTrilaterationSystemView::UpdateZoom(unsigned long CurrentTime)
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
  CurrentWidth = InitialWidth + t * (FinalWidth - InitialWidth);

  OrthoLeft   = OrthoInitialLeft   + u * (OrthoFinalLeft   - OrthoInitialLeft);
  OrthoRight  = OrthoInitialRight  + u * (OrthoFinalRight  - OrthoInitialRight);
  OrthoBottom = OrthoInitialBottom + u * (OrthoFinalBottom - OrthoInitialBottom);
  OrthoTop    = OrthoInitialTop    + u * (OrthoFinalTop    - OrthoInitialTop);
  CurrentWidth = OrthoRight - OrthoLeft;

  return(ReturnVal);
} // UpdateZoom



void CTrilaterationSystemView::FinalizeZoom(void)
{ // FinalizeZoom
  CameraInitialX = CameraFinalX;
  CameraInitialY = CameraFinalY;
  CameraInitialZ = CameraFinalZ;
  InitialWidth = FinalWidth;
  CameraX = CameraFinalX;
  CameraY = CameraFinalY;
  CameraZ = CameraFinalZ;
  CurrentWidth = FinalWidth;

  OrthoInitialLeft = OrthoFinalLeft;
  OrthoInitialRight = OrthoFinalRight;
  OrthoInitialTop = OrthoFinalTop;
  OrthoInitialBottom = OrthoFinalBottom;
  OrthoLeft = OrthoFinalLeft;
  OrthoRight = OrthoFinalRight;
  OrthoTop = OrthoFinalTop;
  OrthoBottom = OrthoFinalBottom;
  CurrentWidth = OrthoRight - OrthoLeft;
} // FinalizeZoom



void CTrilaterationSystemView::InitializeGrid(void)
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



int CTrilaterationSystemView::UpdateGrid(unsigned long CurrentTime)
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


int CTrilaterationSystemView::CalculateGridIndex(int X, int Y)
{ // CalculateGridIndex
  return(X + Y * GridX);
} // CalculateGridIndex



void CTrilaterationSystemView::UpdateTime(void)
{ // InitializeTime
  LastTime = CurrentTime;
} // InitializeTime



void CTrilaterationSystemView::InitializeTime(void)
{ // InitializeTime
  CurrentTime = timeGetTime();
  UpdateTime();
} // InitializeTime



void CTrilaterationSystemView::ResetTrilaterationSystem(void)
{ // ResetTrilaterationSystem
  State = CTRILATERATIONSYSTEMVIEW_SYSTEM_INIT;
  TS->Reset();
} // ResetTrilaterationSystem



void CTrilaterationSystemView::SensorInit(int SensorNum, double X, double Y, double Z)
{ // SensorInit
  TS->SensorInit(SensorNum, X, Y, Z);
} // SensorInit



void CTrilaterationSystemView::ActualPointInit(double X, double Y, double Z)
{ // ActualPointInit
  TS->ActualPointInit(X, Y, Z);
} // ActualPointInit



void CTrilaterationSystemView::InitTrilaterationSystem(void)
{ // InitTrilaterationSystem
  TS->Init();
} // InitTrilaterationSystem



void CTrilaterationSystemView::SetTrilaterationSystemMeasurements(void)
{ // InitTrilaterationSystem
  TS->MeasurementInit();
} // InitTrilaterationSystem



double CTrilaterationSystemView::GetTrilaterationSystemActualX(void)
{ // GetTrilaterationSystemActualX
  return(TS->GetActualX());
} // GetTrilaterationSystemActualX



double CTrilaterationSystemView::GetTrilaterationSystemActualY(void)
{ // GetTrilaterationSystemActualY
  return(TS->GetActualY());
} // GetTrilaterationSystemActualY



double CTrilaterationSystemView::GetTrilaterationSystemMeasuredX(void)
{ // GetTrilaterationSystemMeasuredX
  return(TS->GetMeasuredX());
} // GetTrilaterationSystemMeasuredX



double CTrilaterationSystemView::GetTrilaterationSystemMeasuredY(void)
{ // GetTrilaterationSystemMeasuredY
  return(TS->GetMeasuredY());
} // GetTrilaterationSystemMeasuredY



double CTrilaterationSystemView::GetTrilaterationSystemMeasuredError(void)
{ // GetTrilaterationSystemMeasuredError
  return(TS->GetMeasuredError());
} // GetTrilaterationSystemMeasuredError



double CTrilaterationSystemView::GetTrilaterationSystemSensorX(int SensorNum)
{ // GetTrilaterationSystemSensorX
  return(TS->GetSensorX(SensorNum));
} // GetTrilaterationSystemSensorX



double CTrilaterationSystemView::GetTrilaterationSystemSensorY(int SensorNum)
{ // GetTrilaterationSystemSensorY
  return(TS->GetSensorY(SensorNum));
} // GetTrilaterationSystemSensorY



double CTrilaterationSystemView::GetTrilaterationSystemSensorActualRadius(int SensorNum)
{ // GetTrilaterationSystemSensorActualRadius
  return(TS->GetSensorActualRadius(SensorNum));
} // GetTrilaterationSystemSensorActualRadius



double CTrilaterationSystemView::GetTrilaterationSystemSensorMeasuredRadius(int SensorNum)
{ // GetTrilaterationSystemSensorMeasuredRadius
  return(TS->GetSensorRadius(SensorNum));
} // GetTrilaterationSystemSensorMeasuredRadius



void CTrilaterationSystemView::DisplayUpdates(int DU)
{ // DisplayUpdates
  if (DU) DrawUpdates = 1;
  else DrawUpdates = 0;
} // DisplayUpdates
