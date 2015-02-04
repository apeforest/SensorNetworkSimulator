/////////////////////////////////////////////////////////////////////////////
// COpenGLView.h : interface of the COpenGLView class
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "gl\gl.h"
#include "gl\glu.h"
#include "gl\glaux.h"



/////////////////////////////////////////////////////////////////////////////
// Microsoft Windowes StdAfx Functions
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_COPENGLVIEW_H__96E5EDC7_5831_4BBD_BE66_FE9A23889EE4__INCLUDED_)
#define AFX_COPENGLVIEW_H__96E5EDC7_5831_4BBD_BE66_FE9A23889EE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



/////////////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////////////
#define COPENGLVIEW_HALFANGLE   30.0
#define COPENGLVIEW_NEARPLANE   0.001
#define COPENGLVIEW_FARPLANE    20.0
#define COPENGLVIEW_OFFSETPLANE 1.0



/////////////////////////////////////////////////////////////////////////////
// COpenGLView
/////////////////////////////////////////////////////////////////////////////
class COpenGLView : public CView
{ // COpenGLView
  protected:
	  COpenGLView();
	  DECLARE_DYNCREATE(COpenGLView)

  // Attributes
  public:

  // Operations
  public:

  // Overrides
	  // ClassWizard generated virtual function overrides
	  //{{AFX_VIRTUAL(COpenGLView)
	  public:
	  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	  //}}AFX_VIRTUAL

  // Implementation
  public:
	  virtual ~COpenGLView();
#ifdef _DEBUG
  	virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

  protected:

  // Generated message map functions
  protected:
	  //{{AFX_MSG(COpenGLView)
	  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	  afx_msg void OnDestroy();
	  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	  afx_msg void OnSize(UINT nType, int cx, int cy);
	  //}}AFX_MSG
	  DECLARE_MESSAGE_MAP()

  // Added For COpenGLView Class
  public:
    virtual void SetHalfAngle(double HA);
    virtual void SetNearPlane(double NP);
    virtual void SetFarPlane(double FP);
    virtual double GetHalfAngle(void);
    virtual double GetNearPlane(void);
    virtual double GetFarPlane(void);

  protected:
    virtual BOOL SetupPixelFormat(void);
    virtual BOOL SetupViewport(int CX, int CY);
    virtual BOOL SetupViewingFrustum(GLdouble AspectRatio);
    virtual BOOL SetupViewingTransform(void);
    virtual BOOL PreRenderScene(void);
    virtual BOOL RenderScene(void);
    virtual BOOL RenderStockScene(void);

  protected:
    double NearPlane;                         // Distance To Near Plane
    double FarPlane;                          // Distance To Far Plane
    double HalfAngle;                         // Half Angle Of View (30 Degrees)

  private:
    const char* m_ErrorString;                // Current Error Message
    static const char* const _ErrorStrings[]; // Error Message To Display
    HGLRC m_hRC;                              // Resource Context
    CDC* m_pDC;                               // Device Context

    void SetError(int e);
    BOOL InitializeOpenGL(void);
}; // COpenGLView



/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_COPENGLVIEW_H__96E5EDC7_5831_4BBD_BE66_FE9A23889EE4__INCLUDED_)
