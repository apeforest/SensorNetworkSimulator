/////////////////////////////////////////////////////////////////////////////
// COpenGLView.cpp : Implementation of the COpenGLView class
/////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "COpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// Microsoft Windowes StdAfx Functions
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(COpenGLView, CView)



BEGIN_MESSAGE_MAP(COpenGLView, CView)
	//{{AFX_MSG_MAP(COpenGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



#ifdef _DEBUG
void COpenGLView::AssertValid() const
{ // AssertValid
	CView::AssertValid();
} // AssertValid



void COpenGLView::Dump(CDumpContext& dc) const
{ // Dump
	CView::Dump(dc);
} // Dump
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// Initialize The Error Strings
/////////////////////////////////////////////////////////////////////////////
const char* const COpenGLView::_ErrorStrings[] =
{
  {"No Error."},                       // 0
  {"Unable To Get A Device Context!"}, // 1
  {"ChoosePixelFormat Failed!"},       // 2
  {"SelectPixelFormat Failed!"},       // 3
  {"wglCreateContext Failed!"},        // 4
  {"wglMakeCurrent Failed!"},          // 5
  {"wglDeleteContext Failed!"},        // 6
  {"SwapBuffers Failed!"}              // 7
};



/////////////////////////////////////////////////////////////////////////////
// COpenGLView
/////////////////////////////////////////////////////////////////////////////
COpenGLView::COpenGLView() : m_hRC(0), m_pDC(0), m_ErrorString(_ErrorStrings[0])
{ // COpenGLView
  NearPlane = COPENGLVIEW_NEARPLANE;
  FarPlane = COPENGLVIEW_FARPLANE;
  HalfAngle = COPENGLVIEW_HALFANGLE;
} // COpenGLView



COpenGLView::~COpenGLView()
{ // ~COpenGLView
} // ~COpenGLView



BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{ // PreCreateWindow
	// Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs.
  // An OpenGL window must be created with the following
  // flags and must not include CS_PARENTDC for the
  // class style
  cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
} // PreCreateWindow



void COpenGLView::OnDraw(CDC* pDC)
{ // OnDraw
  // NEED TO DO THIS IN MYVIEW
	// COpenGLDoc* pDoc = GetDocument();
	// ASSERT_VALID(pDoc);
  ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  PreRenderScene();

  ::glPushMatrix();
  RenderStockScene();
  ::glPopMatrix();

  ::glPushMatrix();
  RenderScene();
  ::glPopMatrix();

  ::glFinish();

  if (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc()))
  {
    SetError(7);
  }
} // OnDraw



int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{ // OnCreate
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
  // Initialize The OpenGL Stuff (i.e. Pixel Formats And Such)
  InitializeOpenGL();
	return 0;
} // OnCreate



void COpenGLView::OnDestroy() 
{ // OnDestroy
	CView::OnDestroy();
  if (FALSE == ::wglMakeCurrent(0, 0))
  {
    SetError(2);
  }

  if (FALSE == ::wglDeleteContext(m_hRC))
  {
    SetError(6);
  }

  if (m_pDC)
  {
    delete m_pDC;
  }
} // OnDestroy



BOOL COpenGLView::OnEraseBkgnd(CDC* pDC) 
{ // OnEraseBkgnd
  // OpenGL erases its own window, so this function can be
  // disregarded.
  // return CView::OnEraseBkgnd(pDC);
  return TRUE;
} // OnEraseBkgnd



void COpenGLView::OnSize(UINT nType, int cx, int cy) 
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
  SetupViewingTransform();
} // OnSize



void COpenGLView::SetError(int e)
{ // SetError
  // If there was no previous error, then save this one.
  if (_ErrorStrings[0] == m_ErrorString)
  {
    m_ErrorString = _ErrorStrings[e];
  }
} // SetError



void COpenGLView::SetHalfAngle(double HA)
{ // SetHalfAngle
  if (HA > 0.0) HalfAngle = HA;
  else HalfAngle = COPENGLVIEW_HALFANGLE;
} // SetHalfAngle



void COpenGLView::SetNearPlane(double NP)
{ // SetNearPlane
  if (NP > 0.0) NearPlane = NP;
  else NearPlane = COPENGLVIEW_NEARPLANE;
  if (FarPlane <= NearPlane) FarPlane = NearPlane + COPENGLVIEW_OFFSETPLANE;
} // SetNearPlane



void COpenGLView::SetFarPlane(double FP)
{ // SetFarPlane
  if (FP > 0.0) FarPlane = FP;
  else FarPlane = COPENGLVIEW_FARPLANE;
  if (NearPlane <= FarPlane) SetNearPlane(FarPlane - COPENGLVIEW_OFFSETPLANE);
} // SetFarPlane



double COpenGLView::GetHalfAngle(void)
{ // GetHalfAngle
  return(HalfAngle);
} // GetHalfAngle



double COpenGLView::GetNearPlane(void)
{ // GetNearPlane
  return(NearPlane);
} // GetNearPlane



double COpenGLView::GetFarPlane(void)
{ // GetFarPlane
  return(FarPlane);
} // GetFarPlane



BOOL COpenGLView::InitializeOpenGL(void)
{ // InitializeOpenGL
  m_pDC = new CClientDC(this);

  if (NULL == m_pDC) // Failure To Get DC
  {
    SetError(1);
    return FALSE;
  }

  if (!SetupPixelFormat())
  {
    return FALSE;
  }

  if (0 == (m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc())))
  {
    SetError(4);
    return FALSE;
  }

  if (FALSE == ::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
  {
    SetError(5);
    return FALSE;
  }

  // Specify Black As The Clear Color
  ::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  // Specify The Back Of The Buffer As Clear Depth
  ::glClearDepth(1.0f);
  // Enable Depth Testing
  ::glEnable(GL_DEPTH_TEST);

  return TRUE;
} // InitializeOpenGL



BOOL COpenGLView::SetupPixelFormat(void)
{ // SetupPixelFormat
  PIXELFORMATDESCRIPTOR pfd =
  {
    sizeof(PIXELFORMATDESCRIPTOR),                                              // Size Of This PFD
    1,                                                                          // Version Number
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA, // Support Window, Support OpenGL, Double Buffered, RGBA Type
    24,                                                                         // 24-Bit Color Depth
    0, 0, 0, 0, 0, 0,                                                           // Color Bits Ignored
    0,                                                                          // No Alpha Buffer
    0,                                                                          // Shift Bit Ignored
    0,                                                                          // No Accumulation Buffer
    0, 0, 0, 0,                                                                 // Accumulation Bits Ignored
    16,                                                                         // 16-Bit Z-Buffer
    0,                                                                          // No Stencil Buffer
    0,                                                                          // No Auxiliary Buffer
    PFD_MAIN_PLANE,                                                             // Main Layer
    0,                                                                          // Reserved
    0, 0, 0                                                                     // Layer Masks Ignored
  };
  int pixelformat;

  if (0 == (pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)))
  {
    SetError(2);
    return FALSE;
  }

  if (FALSE == ::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd))
  {
    SetError(3);
    return FALSE;
  }

  return TRUE;
} // SetupPixelFormat



BOOL COpenGLView::SetupViewport(int CX, int CY)
{ // SetupViewport
  // Select The Full Client Area
  glViewport(0, 0, CX, CY);
  return TRUE;
} // SetupViewport



BOOL COpenGLView::SetupViewingFrustum(GLdouble AspectRatio)
{ // SetupViewingFrustum
  // Select a Default Viewing Volume
  gluPerspective(HalfAngle * 2.0, AspectRatio, NearPlane, FarPlane);
  return TRUE;
} // SetupViewingFrustum



BOOL COpenGLView::SetupViewingTransform(void)
{ // SetupViewingTransform
  // Select A Default Viewing Transformation
  // of a 20 Degree Rotation About The X-Axis
  // then a -5 Unit Transformation Along Z
  // ::glTranslatef(0.0f, 0.0f, -5.0f);
  // ::glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
  return TRUE;
} // SetupViewingTransform



BOOL COpenGLView::PreRenderScene(void)
{ // PreRenderScene
  return(TRUE);
} // PreRenderScene



BOOL COpenGLView::RenderScene()
{ // RenderScene()
  // Draw A Red Wire Sphere Inside A Light Blue Cube
  // Rotate The Wire Sphere So It's Vertically Oriented
  ::glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  ::glColor3f(1.0f, 0.0f, 0.0f);
  ::auxWireSphere(0.5);
  ::glColor3f(0.5f, 0.5f, 1.0f);
  ::auxWireCube(1.0);
  return TRUE;
} // RenderScene()



BOOL COpenGLView::RenderStockScene()
{ // RenderStockScene
  // Defaine All Vertices X Y Z
  GLfloat v0[3], v1[3], v2[3], v3[3], delta;
  int color = 0;

  delta = 0.5;

  // Define The Two Colors
  GLfloat color1[3] = {0.9f, 0.9f, 0.9f};
  GLfloat color2[3] = {0.05f, 0.05f, 0.05f};

  v0[1] = v1[1] = v2[1] = v3[1] = 0.0f;

  ::glBegin(GL_QUADS);

  for(int x = -5; x <= 5; ++x)
  {
    for(int z = -5; z <= 5; ++z)
    {
      ::glColor3fv((color++)%2 ? color1 : color2);
      v0[0] = 0.0f + delta*z;
      v0[2] = 0.0f + delta*x;

      v1[0] = v0[0] + delta;
      v1[2] = v0[2];

      v2[0] = v0[0] + delta;
      v2[2] = v0[2] + delta;

      v3[0] = v0[0];
      v3[2] = v0[2] + delta;

      ::glVertex3fv(v0);
      ::glVertex3fv(v1);
      ::glVertex3fv(v2);
      ::glVertex3fv(v3);
    }
  }
  ::glEnd();
  return(TRUE);
} // RenderStockScene
