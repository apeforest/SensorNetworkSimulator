/////////////////////////////////////////////////////////////////////////////
// CTrilaterationSystemView.h : Interface of the CTrilaterationSystemView
// class (Derived From COpenGLView)
/////////////////////////////////////////////////////////////////////////////



#ifndef CTRILATERATIONSYSTEMVIEW_H
#define CTRILATERATIONSYSTEMVIEW_H



/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "COpenGLView.h"
#include "TrilaterationLiarSystem.h"
#include "MainDoc.h"



/////////////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////////////
#define CTRILATERATIONSYSTEMVIEW_PI                   3.14159
#define CTRILATERATIONSYSTEMVIEW_COLOR_NUM            16
#define CTRILATERATIONSYSTEMVIEW_SENSOR_COLOR_NUM     12
#define CTRILATERATIONSYSTEMVIEW_COLOR_INDICES        3
#define CTRILATERATIONSYSTEMVIEW_COLOR_BLACK          0
#define CTRILATERATIONSYSTEMVIEW_COLOR_DARKBLUE       1
#define CTRILATERATIONSYSTEMVIEW_COLOR_DARKGREEN      2
#define CTRILATERATIONSYSTEMVIEW_COLOR_DARKCYAN       3
#define CTRILATERATIONSYSTEMVIEW_COLOR_DARKRED        4
#define CTRILATERATIONSYSTEMVIEW_COLOR_DARKMAGENTA    5
#define CTRILATERATIONSYSTEMVIEW_COLOR_DARKYELLOW     6
#define CTRILATERATIONSYSTEMVIEW_COLOR_GRAY           7
#define CTRILATERATIONSYSTEMVIEW_COLOR_DARKGRAY       8
#define CTRILATERATIONSYSTEMVIEW_COLOR_BLUE           9
#define CTRILATERATIONSYSTEMVIEW_COLOR_GREEN          10
#define CTRILATERATIONSYSTEMVIEW_COLOR_CYAN           11
#define CTRILATERATIONSYSTEMVIEW_COLOR_RED            12
#define CTRILATERATIONSYSTEMVIEW_COLOR_MAGENTA        13
#define CTRILATERATIONSYSTEMVIEW_COLOR_YELLOW         14
#define CTRILATERATIONSYSTEMVIEW_COLOR_WHITE          15
#define CTRILATERATIONSYSTEMVIEW_INDEX_RED            0
#define CTRILATERATIONSYSTEMVIEW_INDEX_GREEN          1
#define CTRILATERATIONSYSTEMVIEW_INDEX_BLUE           2
#define CTRILATERATIONSYSTEMVIEW_STD_MULT             3.0
#define CTRILATERATIONSYSTEMVIEW_SENSOR_RADIUS_SPEED  4.0
#define CTRILATERATIONSYSTEMVIEW_ZOOM_TIME            1500
#define CTRILATERATIONSYSTEMVIEW_GRID_SPEED           4.0
#define CTRILATERATIONSYSTEMVIEW_GRID_COLOR           CTRILATERATIONSYSTEMVIEW_COLOR_RED
#define CTRILATERATIONSYSTEMVIEW_GRID_COLOR_HIGHLIGHT CTRILATERATIONSYSTEMVIEW_COLOR_YELLOW
#define CTRILATERATIONSYSTEMVIEW_MEASURED_COLOR       CTRILATERATIONSYSTEMVIEW_COLOR_CYAN
#define CTRILATERATIONSYSTEMVIEW_ACTUAL_COLOR         CTRILATERATIONSYSTEMVIEW_COLOR_WHITE
#define CTRILATERATIONSYSTEMVIEW_RADIUS_Z_OFFSET      0.10
#define CTRILATERATIONSYSTEMVIEW_GRID_Z_OFFSET        0.05
#define CTRILATERATIONSYSTEMVIEW_POINT_Z_OFFSET       0.00
#define CTRILATERATIONSYSTEMVIEW_POINT_Z_FINAL_OFFSET 0.00 /* 0.125 */
#define CTRILATERATIONSYSTEMVIEW_POINT_RADIUS         0.0075
#define CTRILATERATIONSYSTEMVIEW_ORTHO_LEFT           -1.0
#define CTRILATERATIONSYSTEMVIEW_ORTHO_RIGHT          1.0
#define CTRILATERATIONSYSTEMVIEW_ORTHO_BOTTOM         -1.0
#define CTRILATERATIONSYSTEMVIEW_ORTHO_TOP            1.0
#define CTRILATERATIONSYSTEMVIEW_ORTHO_NEAR           0.001
#define CTRILATERATIONSYSTEMVIEW_ORTHO_FAR            20.0



enum CTrilaterationSystemViewStates
{
  CTRILATERATIONSYSTEMVIEW_SYSTEM_INIT,
  CTRILATERATIONSYSTEMVIEW_SYSTEM_UPDATE,
  CTRILATERATIONSYSTEMVIEW_ANIMATE_SENSORS,
  CTRILATERATIONSYSTEMVIEW_ANIMATE_ZOOM,
  CTRILATERATIONSYSTEMVIEW_ANIMATE_GRID,
};



/////////////////////////////////////////////////////////////////////////////
// CTrilaterationSystemView
/////////////////////////////////////////////////////////////////////////////
class CTrilaterationSystemView : public COpenGLView
{ // CTrilaterationSystemView
  private:
    DECLARE_DYNCREATE(CTrilaterationSystemView)

  public:
    //{{AFX_VIRTUAL(COpenGLView)
	  public:
	  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual void OnSize(UINT nType, int cx, int cy);
	  //}}AFX_VIRTUAL

    CTrilaterationSystemView();
    ~CTrilaterationSystemView();

    CMainDoc* GetDocument();
    virtual BOOL SetupViewingFrustum(GLdouble AspectRatio);
    virtual BOOL PreRenderScene(void);
    virtual BOOL RenderScene(void);
    virtual BOOL RenderStockScene(void);
    virtual void DisplayUpdates(int DU);
    virtual void TSSet(TrilaterationSystem* T);
    virtual void Update(void);
    virtual void UpdateTime(void);
    virtual void InitializeTime(void);
    virtual void ResetTrilaterationSystem(void);
    virtual void SensorInit(int SensorNum, double X, double Y, double Z);
    virtual void ActualPointInit(double X, double Y, double Z);
    virtual void InitTrilaterationSystem(void);
    virtual void SetTrilaterationSystemMeasurements(void);
    virtual double GetTrilaterationSystemActualX(void);
    virtual double GetTrilaterationSystemActualY(void);
    virtual double GetTrilaterationSystemMeasuredX(void);
    virtual double GetTrilaterationSystemMeasuredY(void);
    virtual double GetTrilaterationSystemMeasuredError(void);
    virtual double GetTrilaterationSystemSensorX(int SensorNum);
    virtual double GetTrilaterationSystemSensorY(int SensorNum);
    virtual double GetTrilaterationSystemSensorActualRadius(int SensorNum);
    virtual double GetTrilaterationSystemSensorMeasuredRadius(int SensorNum);

  protected:
    int DrawUpdates;
    int NumSensors;
    int GridX;
    int GridY;
    int ZoomTime;
    int SensorColors[CTRILATERATIONSYSTEMVIEW_SENSOR_COLOR_NUM];
    int GridColor;
    int GridHighlightColor;
    int MeasuredPointColor;
    int ActualPointColor;
    unsigned long ZoomStart;
    unsigned long LastTime;
    unsigned long CurrentTime;
    double OrthoInitialLeft;
    double OrthoInitialRight;
    double OrthoInitialBottom;
    double OrthoInitialTop;
    double OrthoFinalLeft;
    double OrthoFinalRight;
    double OrthoFinalBottom;
    double OrthoFinalTop;
    double OrthoLeft;
    double OrthoRight;
    double OrthoBottom;
    double OrthoTop;
    double OrthoNear;
    double OrthoFar;
    double CurrentWidth;
    double InitialWidth;
    double FinalWidth;
    double PointRadius;
    double PointZDistance;
    double CameraInitialX;
    double CameraInitialY;
    double CameraInitialZ;
    double CameraFinalX;
    double CameraFinalY;
    double CameraFinalZ;
    double CameraX;
    double CameraY;
    double CameraZ;
    double Colors[CTRILATERATIONSYSTEMVIEW_COLOR_NUM][CTRILATERATIONSYSTEMVIEW_COLOR_INDICES];
    double SensorRadiusSpeed;
    double GridSpeed;
    double* SensorRadiusOffsets;
    double* GridXOffsets;
    double* GridYOffsets;
    TrilaterationSystem* TS;

    CTrilaterationSystemViewStates State;

    int UpdateSensorRadiusOffsets(unsigned long CurrentTime);
    int UpdateZoom(unsigned long CurrentTime);
    int UpdateGrid(unsigned long CurrentTime);
    int CalculateGridIndex(int X, int Y);
    void ClearSensorRadiusOffsets(void);
    void FinalizeZoom(void);
    void InitializeColors(void);
    void InitializeSensorColors(void);
    void InitializeSensorRadiusOffsets(void);
    void InitializeCamera(void);
    void InitializeZoom(unsigned long CurrentTime);
    void InitializeGrid(void);
    void DrawSensorRadius(int SensorNum);
    void DrawGridSquare(int Column, int Row);
    void DrawGridSquareHighlight(int Column, int Row);
    void DrawPoints(void);
    void DrawPoint(double X, double Y, int ColorIndex);
    
}; // CTrilaterationSystemView



#ifndef _DEBUG  // Debug version in COpenGLView.cpp
inline CMainDoc* CTrilaterationSystemView::GetDocument()
{ // GetDocument
  return (CMainDoc*)m_pDocument;
} // GetDocument
#endif



#endif