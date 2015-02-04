/////////////////////////////////////////////////////////////////////////////
// CTrilaterationLiarSystemView.h : Interface of the
// CTrilaterationLiarSystemView class (Derived From COpenGLView)
/////////////////////////////////////////////////////////////////////////////



#ifndef CTRILATERATIONLIARSYSTEMVIEW_H
#define CTRILATERATIONLIARSYSTEMVIEW_H



/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "CTrilaterationSystemView.h"
#include "TrilaterationLiarSystem.h"
#include "MainDoc.h"



/////////////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////////////
#define CTRILATERATIONLIARSYSTEMVIEW_LIAR_POINT_COLOR CTRILATERATIONSYSTEMVIEW_COLOR_MAGENTA



/////////////////////////////////////////////////////////////////////////////
// CTrilaterationLiarSystemView
/////////////////////////////////////////////////////////////////////////////
class CTrilaterationLiarSystemView : public CTrilaterationSystemView
{ // CTrilaterationLiarSystemView
  private:
    DECLARE_DYNCREATE(CTrilaterationLiarSystemView)

  public:
    //{{AFX_VIRTUAL(COpenGLView)
	  public:
	  //}}AFX_VIRTUAL

    CTrilaterationLiarSystemView();
    ~CTrilaterationLiarSystemView();

    CMainDoc* GetDocument();
    virtual BOOL RenderScene(void);
    virtual void RunMonteCarloSimulation(int MonteCarloSimulations, double MonteCarloPercentage, int MonteCarloSteps);
    virtual void Update(void);
    virtual void ResetTrilaterationSystem(void);
    virtual void TSSet(TrilaterationSystem* T);
    virtual void InitTrilaterationSystem(void);
    virtual void DrawPoints(void);
    virtual double GetTrilaterationSystemLiarResultX(void);
    virtual double GetTrilaterationSystemLiarResultY(void);
    virtual double GetTrilaterationSystemLiarResultError(void);

  protected:
    int LiarPointColor;
    TrilaterationLiarSystem* TLS;
}; // CTrilaterationLiarSystemView



#ifndef _DEBUG  // Debug version in COpenGLView.cpp
inline CMainDoc* CTrilaterationLiarSystemView::GetDocument()
{ // GetDocument
  return (CMainDoc*)m_pDocument;
} // GetDocument
#endif



#endif