/////////////////////////////////////////////////////////////////////////////
// CMyView.h : Interface of the CMyView class (Derived From COpenGLView)
/////////////////////////////////////////////////////////////////////////////



#ifndef CDEFAULTVIEW_H
#define CDEFAULTVIEW_H



/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "COpenGLView.h"
#include "MainDoc.h"
#include "TrilaterationLiarSystem.h"



/////////////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////////////
#define CMYVIEW_PI                  3.14159
#define CMYVIEW_COLOR_NUM           16
#define CMYVIEW_COLOR_INDICES       3
#define CMYVIEW_COLOR_BLACK         0
#define CMYVIEW_COLOR_DARKBLUE      1
#define CMYVIEW_COLOR_DARKGREEN     2
#define CMYVIEW_COLOR_DARKCYAN      3
#define CMYVIEW_COLOR_DARKRED       4
#define CMYVIEW_COLOR_DARKMAGENTA   5
#define CMYVIEW_COLOR_DARKYELLOW    6
#define CMYVIEW_COLOR_GRAY          7
#define CMYVIEW_COLOR_DARKGRAY      8
#define CMYVIEW_COLOR_BLUE          9
#define CMYVIEW_COLOR_GREEN         10
#define CMYVIEW_COLOR_CYAN          11
#define CMYVIEW_COLOR_RED           12
#define CMYVIEW_COLOR_MAGENTA       13
#define CMYVIEW_COLOR_YELLOW        14
#define CMYVIEW_COLOR_WHITE         15
#define CMYVIEW_INDEX_RED           0
#define CMYVIEW_INDEX_GREEN         1
#define CMYVIEW_INDEX_BLUE          2
#define CMYVIEW_STD_MULT            3.0
#define CMYVIEW_SENSOR_RADIUS_SPEED 4.0
#define CMYVIEW_ZOOM_TIME           1500
#define CMYVIEW_GRID_SPEED          4.0
#define CMYVIEW_GRID_COLOR          CMYVIEW_COLOR_DARKRED
#define CMYVIEW_GRID_COLOR_HILITE   CMYVIEW_COLOR_RED

enum CMyViewStates
{
  CMYVIEW_SYSTEM_INIT,
  CMYVIEW_SYSTEM_UPDATE,
  CMYVIEW_ANIMATE_SENSORS,
  CMYVIEW_ANIMATE_ZOOM,
  CMYVIEW_ANIMATE_GRID,
};



/////////////////////////////////////////////////////////////////////////////
// CMyView
/////////////////////////////////////////////////////////////////////////////
class CMyView : public COpenGLView
{ // CMyView
  private:
    DECLARE_DYNCREATE(CMyView)

  protected:
    TrilaterationSystem* TS;

  public:
    //{{AFX_VIRTUAL(COpenGLView)
	  public:
	  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual void OnSize(UINT nType, int cx, int cy);
	  //}}AFX_VIRTUAL

    CMyView();
    ~CMyView();

    CMainDoc* GetDocument();
    virtual BOOL PreRenderScene(void);
    virtual BOOL RenderScene(void);
    virtual BOOL RenderStockScene(void);
    virtual void TSSet(TrilaterationSystem* T);
    virtual void TSSetUpdateMode(int UpdateMode);
    virtual void Update(void);

  protected:
    int NumSensors;
    int GridX;
    int GridY;
    int ZoomTime;
    unsigned long ZoomStart;
    unsigned long LastTime;
    double CameraInitialX;
    double CameraInitialY;
    double CameraInitialZ;
    double CameraFinalX;
    double CameraFinalY;
    double CameraFinalZ;
    double CameraX;
    double CameraY;
    double CameraZ;
    double Colors[CMYVIEW_COLOR_NUM][CMYVIEW_COLOR_INDICES];
    double SensorRadiusSpeed;
    double GridSpeed;
    double* SensorRadiusOffsets;
    double* GridXOffsets;
    double* GridYOffsets;

    CMyViewStates State;

    int UpdateSensorRadiusOffsets(unsigned long CurrentTime);
    int UpdateZoom(unsigned long CurrentTime);
    int UpdateGrid(unsigned long CurrentTime);
    int CalculateGridIndex(int X, int Y);
    void FinalizeZoom(void);
    void InitializeColors(void);
    void InitializeSensorRadiusOffsets(void);
    void InitializeCamera(void);
    void InitializeZoom(unsigned long CurrentTime);
    void InitializeGrid(void);
    void DrawSensorRadius(int SensorNum);
    void DrawGridSquare(int Column, int Row);
    void DrawPoints(void);
}; // CMyView



#ifndef _DEBUG  // Debug version in COpenGLView.cpp
inline CMainDoc* CMyView::GetDocument()
{ // GetDocument
  return (CMainDoc*)m_pDocument;
} // GetDocument
#endif



#endif