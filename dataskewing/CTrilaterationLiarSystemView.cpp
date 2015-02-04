/////////////////////////////////////////////////////////////////////////////
// CTrilaterationLiarSystemView.cpp : Implementation of the
// CTrilaterationLiarSystemView class (Derived From COpenGLView)
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "CTrilaterationLiarSystemView.h"



/////////////////////////////////////////////////////////////////////////////
// CTrilaterationLiarSystemView
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CTrilaterationLiarSystemView, COpenGLView)



#ifdef _DEBUG // NOTE: Non-debug version is inline
CMainDoc* CTrilaterationLiarSystemView::GetDocument()
{ // GetDocument
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMainDoc)));
	return (CMainDoc*)m_pDocument;
} // GetDocument
#endif //_DEBUG


CTrilaterationLiarSystemView::CTrilaterationLiarSystemView()
{ // CTrilaterationLiarSystemView
  CTrilaterationSystemView();
  TLS = NULL;
  LiarPointColor = CTRILATERATIONLIARSYSTEMVIEW_LIAR_POINT_COLOR;
} // CTrilaterationLiarSystemView



CTrilaterationLiarSystemView::~CTrilaterationLiarSystemView()
{ // ~CTrilaterationLiarSystemView
  TS = NULL;
} // ~CTrilaterationLiarSystemView



BOOL CTrilaterationLiarSystemView::RenderScene(void)
{ // RenderScene
  BOOL ReturnVal = TRUE;
  
  if ((TS != NULL) && (TLS != NULL))
  {
    ReturnVal = CTrilaterationSystemView::RenderScene();
    DrawPoints();
  }
  return(ReturnVal);
} // RenderScene



void CTrilaterationLiarSystemView::DrawPoints(void)
{ // DrawPoints
  double X = 0.0;
  double Y = 0.0;
  double PointZOffset = CTRILATERATIONSYSTEMVIEW_POINT_Z_OFFSET;

  // CTrilaterationSystemView::DrawPoints();
  // glEnable(GL_POINT_SMOOTH);
  // glPointSize(5.0);
  // glBegin(GL_POINTS);
  // glColor3f((GLfloat) Colors[LiarPointColor][CTRILATERATIONSYSTEMVIEW_INDEX_RED], (GLfloat) Colors[LiarPointColor][CTRILATERATIONSYSTEMVIEW_INDEX_GREEN], (GLfloat) Colors[LiarPointColor][CTRILATERATIONSYSTEMVIEW_INDEX_BLUE]);
  X = TLS->GetLiarReportedX();
  Y = TLS->GetLiarReportedY();
  // if ((X > -1000) && (Y > -1000)) glVertex3f((GLfloat) X, (GLfloat) Y, (GLfloat) PointZOffset * 3);
  DrawPoint(X, Y, LiarPointColor);
  // glEnd();
} // DrawPoints



void CTrilaterationLiarSystemView::Update(void)
{ // LiarUpdate
  unsigned long CurrentTime = timeGetTime();

  if (State == CTRILATERATIONSYSTEMVIEW_SYSTEM_UPDATE)
  {
    CTrilaterationSystemView::Update();
    if (State == CTRILATERATIONSYSTEMVIEW_SYSTEM_UPDATE)
    {
      if (TLS->GetState() == SYSTEM_COMPLETE)
      {
        TLS->Update();
        if (DrawUpdates)
        {
          switch(TLS->GetLiarState())
          {
            case LIAR_SYSTEM_TRILATERATION_LIAR_SYSTEM_INIT:
              PointZDistance = CTRILATERATIONSYSTEMVIEW_POINT_Z_OFFSET;
            break;

            case LIAR_DRAW_ZOOM_END:
              PointZDistance = CTRILATERATIONSYSTEMVIEW_POINT_Z_FINAL_OFFSET;
            case LIAR_DRAW_ZOOM_INITIAL_AREA:
            case LIAR_DRAW_ZOOM_AREA:
              InitializeZoom(CurrentTime);
              State = CTRILATERATIONSYSTEMVIEW_ANIMATE_ZOOM;
            break;
        
            case LIAR_DRAW_GRID:
              InitializeGrid();
              State = CTRILATERATIONSYSTEMVIEW_ANIMATE_GRID;
            break;
          }
        }
      }
    }
  }
  else
  {
    CTrilaterationSystemView::Update();
  }
} // LiarUpdate



void CTrilaterationLiarSystemView::TSSet(TrilaterationSystem* T)
{ // TSSet
  TS = T;
  TLS = (TrilaterationLiarSystem*) T;
  if (TS != NULL)
  {
    NumSensors = TS->GetNumSensors();
    InitializeSensorRadiusOffsets();
  }
} // TSSet



void CTrilaterationLiarSystemView::ResetTrilaterationSystem(void)
{ // ResetTrilaterationSystem
  CTrilaterationSystemView::ResetTrilaterationSystem();
  TLS->Reset();
} // ResetTrilaterationSystem



void CTrilaterationLiarSystemView::InitTrilaterationSystem(void)
{ // InitTrilaterationSystem
  TS->Init();
  TLS->Init();
} // InitTrilaterationSystem



double CTrilaterationLiarSystemView::GetTrilaterationSystemLiarResultX(void)
{ // GetTrilaterationSystemLiarResultX
  return(TLS->GetLiarReportedX());
} // GetTrilaterationSystemLiarResultX



double CTrilaterationLiarSystemView::GetTrilaterationSystemLiarResultY(void)
{ // GetTrilaterationSystemLiarResultY
  return(TLS->GetLiarReportedY());
} // GetTrilaterationSystemLiarResultY



double CTrilaterationLiarSystemView::GetTrilaterationSystemLiarResultError(void)
{ // GetTrilaterationSystemLiarResultError
  return(TLS->GetLiarError());
} // GetTrilaterationSystemLiarResultError



void CTrilaterationLiarSystemView::RunMonteCarloSimulation(int MonteCarloSimulations, double MonteCarloPercentage, int MonteCarloSteps)
{ // RunMonteCarloSimulation
  TLS->RunMonteCarloSimulation(MonteCarloSimulations, MonteCarloPercentage, MonteCarloSteps);
} // RunMonteCarloSimulation