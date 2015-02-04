// ExposureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Exposure.h"
#include "ExposureDlg.h"
#include "Sensor.cpp"
#include "VI.cpp"
#include <math.h>

const int Accuracy = 1000;        // Accuracy of Random Numbers
const double Precision = 2.00;    // Precision in Finding Vertices
const double GridMult = 10000.0;  // Grid Scaling Factor
CPen VoronoiPen(PS_SOLID, 1, RGB(0, 0, 192)); // Gray Pen
VI* VoronoiGraph;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Code from Seapahn ///////////////////////////////////////////////////////////
int in_between(double a, double b, double c)
{
    if (a<=b && a>=c) return 1;
    if (a>=b && a<=c) return 1;
    return 0;
}

double SDistance(double x, double y, double x1, double y1, double x2, double y2)
{
	double d1;
  double d2;
  double yp;
  double xp;
  double m;

	if (x2==x1)
  {	// Vertical line
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

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExposureDlg dialog

CExposureDlg::CExposureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExposureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExposureDlg)
	EX = 1.0;
	EY = 1.0;
	GridSize = 2;
	SideLength = 0.10;
	N = 200;
	OutFile = _T("Output.txt");
	P = 1;
	GenerateMode = 0;
	Status = _T("");
	SX = 0.0;
	SY = 0.0;
	u = 1.0;
	GridAccuracy = 5;
	ShowGrid = TRUE;
	ShowRefZone = TRUE;
	ShowPathV = FALSE;
	InFile = _T("Input.txt");
	PCost = _T("");
	ShowVoronoi = FALSE;
	PLen = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  RefineLevel = 0;
  Sensors = NULL;
  Vertices = NULL;
  PicWindow = NULL;
  Path = NULL;
  VoronoiDone = FALSE;
  VoronoiGraph = NULL;
}

CExposureDlg::~CExposureDlg()
{
  // Delete Vertex Array
  if (Vertices != NULL)
  {
    delete[] Vertices;
  }
  // Delete Sensor Array
  if (Sensors != NULL)
  {
    delete[] Sensors;
  }
  // Delete the Picture Window
  if (PicWindow != NULL)
  {
    delete PicWindow;
  }
  // Delete the Voronoi Graph
  if (VoronoiGraph != NULL)
  {
    delete VoronoiGraph;
  }
}

void CExposureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExposureDlg)
	DDX_Control(pDX, IDC_Progress, Progress);
	DDX_Text(pDX, IDC_EX, EX);
	DDX_Text(pDX, IDC_EY, EY);
	DDX_Text(pDX, IDC_GridSize, GridSize);
	DDX_Text(pDX, IDC_Length, SideLength);
	DDX_Text(pDX, IDC_N, N);
	DDX_Text(pDX, IDC_OutFile, OutFile);
	DDX_Text(pDX, IDC_P, P);
	DDX_Radio(pDX, IDC_Rand, GenerateMode);
	DDX_Text(pDX, IDC_Status, Status);
	DDX_Text(pDX, IDC_SX, SX);
	DDX_Text(pDX, IDC_SY, SY);
	DDX_Text(pDX, IDC_u, u);
	DDX_Text(pDX, IDC_GridAccuracy, GridAccuracy);
	DDX_Check(pDX, IDC_Grid, ShowGrid);
	DDX_Check(pDX, IDC_RefZone, ShowRefZone);
	DDX_Check(pDX, IDC_PathV, ShowPathV);
	DDX_Text(pDX, IDC_InputFile, InFile);
	DDX_Text(pDX, IDC_PathCost, PCost);
	DDX_Check(pDX, IDC_ShowVoronoi, ShowVoronoi);
	DDX_Text(pDX, IDC_PLen, PLen);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExposureDlg, CDialog)
	//{{AFX_MSG_MAP(CExposureDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Generate, OnGenerate)
	ON_BN_CLICKED(IDC_Refresh, OnRefresh)
	ON_BN_CLICKED(IDC_Refine, OnRefine)
	ON_BN_CLICKED(IDC_Input, OnInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExposureDlg message handlers

BOOL CExposureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CExposureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExposureDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExposureDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CExposureDlg::Initialize(void)
{
  int Success = 1; // Was the Initialization a Success? 1: Yes, 0: No
  CRect DrawWindow;          // Rectangle for Getting Width and Height
  CBrush Bkg(RGB (0, 0, 0)); // Brush to Draw the Background Color

  // Set the SUVI to the 0th Vertex Index
  SUVI = 0;
  // Set the Voronoi Path to Not Done
  VoronoiDone = FALSE;
  // Update the Associated Varaibles
  UpdateData();
  // Set the Refine Level
  RefineLevel = 0;
  // Erase the Path Array if it Exists
  if (Path != NULL)
  {
    delete[] Path;
    Path = NULL;
  }
  // Erase the Number of Path Nodes
  NumPath = 0;
  // Erase the Picture Window
  if (PicWindow != NULL)
  {
    delete PicWindow;
    PicWindow = NULL;
  }
  // Erase the Vertex Array
  if (Vertices != NULL)
  {
    delete[] Vertices;
    Vertices = NULL;
  }
  // Erase the Number of Vertices
  NumVertices = 0;
  // Erase the Sensor Array
  if (Sensors != NULL)
  {
    delete[] Sensors;
    Sensors = NULL;
  }
  // Set Maximum Vertices
  MaxVertices = 2 * (GridSize + 1) * (GridSize) * (GridAccuracy + 1);
  // Allocate a New Vertex Array
  Vertices = new GridVertex[MaxVertices];
  // Allocate a New Sensor Array
  switch (GenerateMode)
  {
    case 1:  // Triangle Mode
      N = int (4.0 / (SideLength * SideLength * 0.8660254038)) + 10;
      break;
    case 2:  // Rectangle Mode
      N = int (1.0 / (SideLength * SideLength)) + 10;
      break;
    case 3:  // Hexagon Mode
      N = int (1.0 / (SideLength * SideLength * 1.5 * 0.8660254038)) + 10;
      break;
    case 0:  // 0: Random Mode
    default: // Default: Random Mode
      // Leave N Alone
      break;
  }
  Sensors = new Sensor[N];
  // Create A New Picture Window
  PicWindow = new CClientDC(GetDlgItem(IDC_Graph));
  GetDlgItem(IDC_Graph)->GetClientRect(&DrawWindow);
  // Get the Width and Height
  Width = DrawWindow.BottomRight().x - 4;
  Height = DrawWindow.BottomRight().y - 4;
  // Clear the Screen
  PicWindow->SelectObject(&Bkg);
  PicWindow->Rectangle(0, 0, Width + 4, Height + 4);
  // Make Sure Enough Memory
  if ((Vertices == NULL) || (Sensors == NULL))
  {
    // Display Error Message
    MessageBox("Not Enough Memory!");
    // Initialization Failure
    Success = 0;
  }
  // Update the Data on the Screen
  UpdateData(FALSE);
  // Return Success Value
  return (Success);
}

void CExposureDlg::CalcPath(void)
{
  int CI;       // Current Coordinate Index;
  int DI;       // Destination Coordinate Index
  double CX;    // Current X Coordinate
  double CY;    // Current Y Coordinate
  double FX;    // Final X Coordinate
  double FY;    // Final Y Coordinate
  double DX;    // Destination X Coordinate
  double DY;    // Destination Y Coordinate
  double RX;    // Reference X Coordinate
  double RY;    // Reference Y Coordinate
  double UX;    // Upper X Coordinate
  double UY;    // Upper Y Coordinate
  double LX;    // Lower X Coordinate
  double LY;    // Lower Y Coordinate
  double W;     // Additional Weight Added to Path
  double Step;  // Step Value
  double CCost; // Current Cost to Get to Location

  // Set the Progress
  Status.Format("Calculating Path...");
  Progress.SetRange(0, MaxVertices);
  Progress.SetPos(0);
  Progress.SetStep(1);
  UpdateData(FALSE);
  // Get Starting Points
  CX = SX * GridMult;
  CY = SY * GridMult;
  FX = EX * GridMult;
  FY = EY * GridMult;
  SnapToGrid(FX, FY);
  // Set the Step and Size
  Step = GridMult / double (GridSize * GridAccuracy);
  // Find First Vertex
  CI = FindVertex(CX, CY, Step / Precision);
  // Set That Vertex's Cost to 0
  Vertices[CI].Cost = 0;
  // Process Vertices until End-Point is Hit
  while (!((CX >= FX - (Step / Precision)) && (CX <= FX + (Step / Precision)) && (CY >= FY - (Step / Precision)) && (CY <= FY + (Step / Precision))) && (Vertices[CI].Visited != 1))
  {
    // Set the Current Vertex to Visited
    Vertices[CI].Visited = 1;
    // Set the Current Cost
    CCost = Vertices[CI].Cost;
    // Calculate Reference Coordinates
    RX = double (int (CX * GridSize / GridMult + Step / Precision / GridMult)) / double (GridSize) * GridMult;
    RY = double (int (CY * GridSize / GridMult + Step / Precision / GridMult)) / double (GridSize) * GridMult;
    // Calculate Border Points
    LX = double (int (CX * GridSize / GridMult - 1 + Step / Precision / GridMult)) / double (GridSize) * GridMult;
    LY = double (int (CY * GridSize / GridMult - 1 + Step / Precision / GridMult)) / double (GridSize) * GridMult;
    UX = double (int (CX * GridSize / GridMult + 1 + Step / Precision / GridMult)) / double (GridSize) * GridMult;
    UY = double (int (CY * GridSize / GridMult + 1 + Step / Precision / GridMult)) / double (GridSize) * GridMult;
    
    for (DX = LX, DY = LY; (DX < RX) && (DY >= 0) && (DY <= GridMult); DX += Step)
    {
      if ((DX >= 0) && (DX <= GridMult))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    for (DX = RX, DY = LY; (DX < UX) && (DY >= 0) && (DY <= GridMult); DX += Step)
    {
      if ((DX >= 0) && (DX <= GridMult))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    for (DX = LX, DY = UY; (DX < RX) && (DY >= 0) && (DY <= GridMult); DX += Step)
    {
      if ((DX >= 0) && (DX <= GridMult))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    for (DX = RX, DY = UY; (DX < UX) && (DY >= 0) && (DY <= GridMult); DX += Step)
    {
      if ((DX >= 0) && (DX <= GridMult))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    for (DX = LX, DY = LY + Step; (DX >= 0) && (DX <= GridMult) && (DY < RY); DY += Step)
    {
      if ((DY >= 0) && (DY <= GridMult))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    for (DX = LX, DY = RY; (DX >= 0) && (DX <= GridMult) && (DY < UY); DY += Step)
    {
      if ((DY >= 0) && (DY <= GridMult))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    for (DX = UX, DY = LY; (DX >= 0) && (DX <= GridMult) && (DY < RY); DY += Step)
    {
      if ((DY >= 0) && (DY <= GridMult))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    for (DX = UX, DY = RY; (DX >= 0) && (DX <= GridMult) && (DY < UY); DY += Step)
    {
      if ((DY >= 0) && (DY <= GridMult))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    if ((UX >= 0) && (UX <= GridMult) && (UY >= 0) && (UY <= GridMult))
    {
      if (ShowRefZone == TRUE)
      {
        DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
      }
      // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
      // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
      W = CalcWeight(CX, CY, UX, UY, Step) + CCost;
      // Set the Destination Vertex Index
      DI = FindVertex(UX, UY, Step / Precision);
      // Check the New Distance
      if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
      {
        // Set the Cost to the Next Vertex (Current Cost + New Weight)
        Vertices[DI].Cost = W;
        // Set the Previous Vertex to the Current Vertex
        Vertices[DI].Prev = CI;
      }
    }
    // Check Point to Left
    if ((CX - Step >= 0) && (CY >= RY - Step / 2) && (CY <= RY + Step / 2))
    {
      if (ShowRefZone == TRUE)
      {
        DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
      }
      // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
      // W = WeightFunction(FindNearestSensor(CX, CY, CX - Step, CY), Distance(CX, CY, CX - Step, CY)) + CCost;
      W = CalcWeight(CX, CY, CX - Step, CY, Step) + CCost;
      // Set the Destination Vertex Index
      DI = FindVertex(CX - Step, CY, Step / Precision);
      // Check the New Distance
      if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
      {
        // Set the Cost to the Next Vertex (Current Cost + New Weight)
        Vertices[DI].Cost = W;
        // Set the Previous Vertex to the Current Vertex
        Vertices[DI].Prev = CI;
      }
    }
    // Check Point to Right
    if ((CX + Step <= GridMult) && (CY >= RY - Step / 2) && (CY <= RY + Step / 2))
    {
      if (ShowRefZone == TRUE)
      {
        DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
      }
      // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
      // W = WeightFunction(FindNearestSensor(CX, CY, CX + Step, CY), Distance(CX, CY, CX - Step, CY)) + CCost;
      W = CalcWeight(CX, CY, CX + Step, CY, Step) + CCost;
      // Set the Destination Vertex Index
      DI = FindVertex(CX + Step, CY, Step / Precision);
      // Check the New Distance
      if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
      {
        // Set the Cost to the Next Vertex (Current Cost + New Weight)
        Vertices[DI].Cost = W;
        // Set the Previous Vertex to the Current Vertex
        Vertices[DI].Prev = CI;
      }
    }
    // Check Point Above
    if ((CX >= RX - Step / 2) && (CX <= RX + Step / 2) && (CY - Step >= 0))
    {
      if (ShowRefZone == TRUE)
      {
        DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
      }
      // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
      // W = WeightFunction(FindNearestSensor(CX, CY, CX, CY - Step), Distance(CX, CY, CX, CY - Step)) + CCost;
      W = CalcWeight(CX, CY, CX, CY - Step, Step) + CCost;
      // Set the Destination Vertex Index
      DI = FindVertex(CX, CY - Step, Step / Precision);
      // Check the New Distance
      if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
      {
        // Set the Cost to the Next Vertex (Current Cost + New Weight)
        Vertices[DI].Cost = W;
        // Set the Previous Vertex to the Current Vertex
        Vertices[DI].Prev = CI;
      }
    }
    // Check Point Below
    if ((CX >= RX - Step / 2) && (CX <= RX + Step / 2) && (CY + Step <= GridMult))
    {
      if (ShowRefZone == TRUE)
      {
        DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
      }
      // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
      // W = WeightFunction(FindNearestSensor(CX, CY, CX, CY + Step), Distance(CX, CY, CX, CY + Step)) + CCost;
      W = CalcWeight(CX, CY, CX, CY + Step, Step) + CCost;
      // Set the Destination Vertex Index
      DI = FindVertex(CX, CY + Step, Step / Precision);
      // Check the New Distance
      if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
      {
        // Set the Cost to the Next Vertex (Current Cost + New Weight)
        Vertices[DI].Cost = W;
        // Set the Previous Vertex to the Current Vertex
        Vertices[DI].Prev = CI;
      }
    }
    // Find the Next Vertex
    CI = FindSmallestVertex(CX, CY);
    // Step the Progress Bar
    Progress.StepIt();
    // Status.Format("Calculating Path...          %d / %d", NumVertices, MaxVertices);
    // UpdateData(FALSE);
  }
  // Set the Ending Index Value
  EI = CI;
  // Clear the Progress Bar
  Progress.SetPos(0);
  // Display the Final Message
  if (Vertices[CI].Visited == 1)
  {
    Status.Format("Error! Vertex %d Already Visited!", CI);
    MessageBox(Status, "No More Vertices!");
  }
  PCost.Format("%0.3f", Vertices[CI].Cost);
  Status.Format("");
  UpdateData(FALSE);
}

double CExposureDlg::FindNearestSensor(double X, double Y)
{
  int i = 0; // Index Variable
  double CD; // Current Distance to Sensor
  double ND; // Distance to Next Sensor

  // Find Initial Distance
  CD = Distance(Sensors[0].x * GridMult, Sensors[0].y * GridMult, X, Y);
  // Check All Other Distances
  for (i = 1; i < N; ++i)
  {
    // Get New Distance
    ND = Distance(Sensors[i].x * GridMult, Sensors[i].y * GridMult, X, Y);
    if (ND < CD)
    {
      // Set New Distance to Current Distance
      CD = ND;
    }
  }
  // Return the Current Distance
  return (CD);
}

double CExposureDlg::Distance(double X1, double Y1, double X2, double Y2)
{
  return (sqrt((X1 - X2) * (X1 - X2) + (Y1 - Y2) * (Y1 - Y2)));
}

double CExposureDlg::WeightFunction(double D, double L)
{
  double SensorPower; // Power of Sensor

  // Calculate Power of Sensor
  SensorPower = 1 / pow(D, P);
  // Multiply by Modifiers
  SensorPower *= (u * L * 10000);
  // Return the Value
  return (SensorPower);
}

int CExposureDlg::FindVertex(double X, double Y, double Acc)
{
  int i = NumVertices - 1; // Index Variable
  int VIndex;              // Vertex Index
  int Pos;                 // Progress Bar Position
  GridVertex* Temp;        // Temporary Array for Doubling

  // Check Each Vertex to See if it is in List
  while((i >= 0) && !((X >= Vertices[i].x - Acc) && (X <= Vertices[i].x + Acc) && (Y >= Vertices[i].y - Acc) && (Y <= Vertices[i].y + Acc)))
  {
    --i;
  }
  // If the Vertex Was Not Found
  if (i < 0)
  {
    if (NumVertices == MaxVertices)
    {
      // Double the Maximum Number of Vertices
      MaxVertices *= 2;
      // Allocate a New Array
      Temp = new GridVertex[MaxVertices];
      // Copy the Old Array into the New Array
      for (i = 0; i < NumVertices; ++i)
      {
        Temp[i].x = Vertices[i].x;
        Temp[i].y = Vertices[i].y;
        Temp[i].Prev = Vertices[i].Prev;
        Temp[i].Visited = Vertices[i].Visited;
        Temp[i].Cost = Vertices[i].Cost;
      }
      // Delete the Old Array
      delete[] Vertices;
      // Restore the Array
      Vertices = Temp;
      // Set Progress Bar
      Pos = Progress.GetPos();
      Progress.SetRange(0, MaxVertices);
      Progress.SetStep(1);
      Progress.SetPos(Pos);
    }
    // Add the Vertex at the End of the List
    Vertices[NumVertices].x = X;
    Vertices[NumVertices].y = Y;
    VIndex = NumVertices;
    ++NumVertices;
  }
  else
  {
    // Set the Vertex Index to the found index
    VIndex = i;
  }
  // Return Index of Vertex
  return(VIndex);
}

int CExposureDlg::FindSmallestVertex(double &X, double &Y)
{
  int i = 0;        // Index Variable
  int VI = 0;       // Smallest Cost Vertex Index
  double PCost = 0; // Current Path Cost

  // Start with the First Vertex
  PCost = LONG_MAX;
  // PCost = Vertices[SUVI].Cost;
  /*
  // Make Sure the Cost is Infinity if it is Less Than 0
  if (PCost <= 0)
  {
    PCost = LONG_MAX;
  }
  X = Vertices[SUVI].x;
  Y = Vertices[SUVI].y;
  // Check Each Remaining Vertex
  */
  for (i = SUVI; i < NumVertices; ++i)
  {
    // Check Next Vertex for Lower Cost
    if (((Vertices[i].Cost > 0) && (Vertices[i].Cost < PCost)) && (Vertices[i].Visited == 0))
    {
      PCost = Vertices[i].Cost;
      X = Vertices[i].x;
      Y = Vertices[i].y;
      VI = i;
    }
  }
  // Update SUVI (Smallest Unvisited Vertex Index)
  while (Vertices[SUVI].Visited == 1)
  {
    ++SUVI;
  }
  // Return the Vertex Index)
  return (VI);
}

void CExposureDlg::InitSensors()
{
  double Height;         // Length of the Height of a Triangle
  double Offset[2];      // Offset for Trirangle
  double X;              // X Coordinate
  double Y;              // Y Coordinate
  int i = 0;             // Index Variable
  int j = 0;             // Index Variable
  int CurrentSensor = 0; // Current Sensor

  switch(GenerateMode)
  {
    case 1:  // Triangle Mode
      // Calculate the Offsets
      Offset[0] = 0.2886751346 * SideLength;
      Offset[1] = 2 * Offset[0];
      // Calculate the Height
      Height = SideLength * 0.8660254038;
      // Set the Indices
      i = 0;
      j = 0;
      while (j * Height <= 1)
      {
        // Start at the First Sensor in the Row
        i = 0;
        while ((i * SideLength / 2) <= 1)
        {
          X = (i + 1) * SideLength / 2;
          Y = j * Height + Offset[(i + j) % 2];
          if ((X <= 1) && (Y <= 1))
          {
            Sensors[CurrentSensor].x = X;
            Sensors[CurrentSensor].y = Y;
            ++CurrentSensor;
          }
          ++i;
        }
        ++j;
      }
      N = CurrentSensor;
      break;
    case 2:  // Rectangle Mode
      // Set the Indices
      i = 0;
      j = 0;
      while (j * SideLength <= 1)
      {
        // Start at the First Sensor in the Row
        i = 0;
        while ((i * SideLength) <= 1)
        {
          X = (i + 0.5) * SideLength;
          Y = (j + 0.5) * SideLength;
          if ((X <= 1) && (Y <= 1))
          {
            Sensors[CurrentSensor].x = X;
            Sensors[CurrentSensor].y = Y;
            ++CurrentSensor;
          }
          ++i;
        }
        ++j;
      }
      N = CurrentSensor;
      break;
    case 3:  // Hexagon Mode
      // Calculate the Height
      Height = SideLength * 0.8660254038 * 2;
      // Set the Indices
      i = 0;
      j = 0;
      while (j * Height <= 1)
      {
        // Start at the First Sensor in the Row
        i = 0;
        while ((i * SideLength / 2) <= 1)
        {
          X = (i * 1.5 + 1) * SideLength;
          Y = (j + 0.5) * Height + (i % 2) * (Height / 2);
          if ((X <= 1) && (Y <= 1))
          {
            Sensors[CurrentSensor].x = X;
            Sensors[CurrentSensor].y = Y;
            ++CurrentSensor;
          }
          ++i;
        }
        ++j;
      }
      N = CurrentSensor;
      break;
    case 0:  // Just Random Numbers
    default: // Just Random Numbers
      // Initialize the Random Seed
      srand(time(NULL));
      // Set Each Sensor to a Random Point
      for (i = 0; i < N; ++i)
      {
        Sensors[i].x = double (rand() % Accuracy) / double (Accuracy);
        Sensors[i].y = double (rand() % Accuracy) / double (Accuracy);
      }
      break;
  }
  UpdateData(FALSE);
}

void CExposureDlg::OnGenerate() 
{

  // If Initialization is OK, Process Path
  if (Initialize())
  {
    // If the Grid Should Be Shown, Show It
    if (ShowGrid == TRUE)
    {
      DrawGrid();
    }
    // Initialize the Sensors
    InitSensors();
    // Draw Voronoi Stuff
    if (ShowVoronoi == TRUE)
    {
      if (VoronoiGraph != NULL)
      {
        delete VoronoiGraph;
        VoronoiGraph = NULL;
      }
      Status.Format("Calculating Voronoi Diagram...");
      UpdateData(FALSE);
      VoronoiGraph = new VI;
      VoronoiGraph->CalcVoronoi(Sensors, N, &Status, &Progress);
      VoronoiGraph->DrawVoronoi(GetDlgItem(IDC_Graph), &VoronoiPen, 2);
      VoronoiDone = TRUE;
      Status.Format("");
      UpdateData(FALSE);
    }
    // Draw the Sensors
    DrawSensors();
    // Calculate the Path
    CalcPath();
    // Draw the Path
    DrawPath();
  }
}

void CExposureDlg::DrawSensor(double X, double Y, int Type)
{
  int R; // Red Color Value
  int G; // Green Color Value
  int B; // Blue Color Value

  // Determine Color Values
  switch(Type)
  {
    default: // (255, 0, 0)
    case 1:  // (255, 0, 0)
      R = 255;
      G = 0;
      B = 0;
      break;
    case 2:  // (255, 255, 0)
      R = 255;
      G = 255;
      B = 0;
      break;
    case 3:  // (0, 192, 0)
      R = 0;
      G = 192;
      B = 0;
      break;
    case 4:  // (0, 0, 255);
      R = 0;
      G = 0;
      B = 255;
      break;
    case 5: // (255, 128, 128)
      R = 255;
      G = 128;
      B = 128;
      break;
    case 6: // (128, 128, 255)
      R = 128;
      G = 128;
      B = 255;
      break;
  }

  if (!((int (Width * X + 2) < 2) || (int (Width * X + 2) > Width + 2) || (int (Height * Y + 2) < 2) || (int (Height * Y + 2) > Height + 2)))
  {
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 2, RGB(R, G, B));
    PicWindow->SetPixel(int (Width * X) + 3, int (Height * Y) + 2, RGB(R, G, B));
    PicWindow->SetPixel(int (Width * X) + 1, int (Height * Y) + 2, RGB(R, G, B));
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 3, RGB(R, G, B));
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 1, RGB(R, G, B));
  }
}

void CExposureDlg::DrawSensors()
{
  int i = 0;  // Index Variable

  // Draw Each Sensor
  for (i = 0; i < N; ++i)
  {
    DrawSensor(Sensors[i].x, Sensors[i].y);
  }
}

void CExposureDlg::DrawPath()
{
  int CI;    // Current Index
  double D;  // Distance
  double X1; // Coordinate
  double Y1; // Coordinate
  double X2; // Coordinate
  double Y2; // Coordinate

  CPen Wht(PS_SOLID, 1, RGB(255, 255, 255)); // Blue Pen

  // Start at the End of the Path
  CI = EI;
  // No Path Distance
  D = 0;
  // Select the Blue Pen
  PicWindow->SelectObject(&Wht);
  // Move to the Ending Value
  PicWindow->MoveTo(int (Width * Vertices[CI].x / GridMult) + 2, int (Height * Vertices[CI].y / GridMult) + 2);
  if (ShowPathV == TRUE)
  {
    DrawSensor(Vertices[CI].x / GridMult, Vertices[CI].y / GridMult, 2);
  }
  // Store Coordinates
  X1 = Vertices[CI].x;
  Y1 = Vertices[CI].y;
  // Get the Previous Point
  CI = Vertices[EI].Prev;
  // Draw a Line to Each Previous Point
  while (CI != -1)
  {
    // Store Coordinates
    X2 = Vertices[CI].x;
    Y2 = Vertices[CI].y;
    // Add the Distance
    D += Distance(X1, Y1, X2, Y2);
    // Draw the Line
    PicWindow->LineTo(int (Width * Vertices[CI].x / GridMult) + 2, int (Height * Vertices[CI].y / GridMult) + 2);
    if (ShowPathV == TRUE)
    {
      DrawSensor(Vertices[CI].x / GridMult, Vertices[CI].y / GridMult, 2);
    }
    CI = Vertices[CI].Prev;
    // Update the Previous Points
    X1 = X2;
    Y1 = Y2;
  }
  // Update the Path Length
  PLen.Format("%0.3f", D);
  UpdateData(FALSE);
}

void CExposureDlg::DrawGrid()
{
  double X;     // X Coordinate
  double Y;     // Y Coordinate
  double XStep; // X Step Distance
  double YStep; // Y Step Distance
  CPen Gray(PS_SOLID, 1, RGB(64, 64, 64));

  // Set the Stepping Distance
  XStep = double (Width) / double (pow(GridSize, RefineLevel + 1));
  YStep = double (Height) / double (pow(GridSize, RefineLevel + 1));
  // Get the Gray Pen
  PicWindow->SelectObject(&Gray);
  // Draw the Vertical Lines
  for (X = 0; X <= Width; X += XStep)
  {
    PicWindow->MoveTo(int (X) + 2, 2);
    PicWindow->LineTo(int (X) + 2, Height + 2);
  }
  // Draw the Horizontal Lines
  for (Y = 0; Y <= Height; Y += YStep)
  {
    PicWindow->MoveTo(2, int (Y) + 2);
    PicWindow->LineTo(Width + 2, int (Y) + 2);
  }
}

void CExposureDlg::OnRefresh() 
{
  CRect DrawWindow;                      // Rectangle for Getting Width and Height
  CPen Black(PS_SOLID, 1, RGB(0, 0, 0)); // Black Path Line
  CBrush Bkg(RGB (0, 0, 0));             // Brush to Draw the Background Color

  UpdateData();
  if (PicWindow != NULL)
  {
    PicWindow->SelectObject(&Black);
    PicWindow->SelectObject(&Bkg);
    PicWindow->Rectangle(0, 0, Width + 4, Height + 4);
    if (ShowGrid == TRUE)
    {
      DrawGrid();
    }
    if (ShowVoronoi == TRUE)
    {
      if (VoronoiDone == FALSE)
      {
        if (VoronoiGraph != NULL)
        {
          delete VoronoiGraph;
        }
        Status.Format("Calculating Voronoi Diagram...");
        UpdateData(FALSE);
        VoronoiGraph = new VI;
        VoronoiGraph->CalcVoronoi(Sensors, N, &Status, &Progress);
        VoronoiDone = TRUE;
        Status.Format("");
        UpdateData(FALSE);
      }
      VoronoiGraph->DrawVoronoi(GetDlgItem(IDC_Graph), &VoronoiPen, 2);
    }
    if (Sensors != NULL)
    {
      DrawSensors();
    }
    if (ShowRefZone == TRUE)
    {
      ShowRefineZone();
    }
    if (Vertices != NULL)
    {
      DrawPath();
    }
  }
}

double CExposureDlg::FindPathDistance(int Index)
{
  int CurrentIndex;       // Current Path Index
  double CurrentDistance; // Current Distance from Path
  double NewDistance;     // New Distance from Path

  // Set the Current Index to the End Point
  CurrentIndex = 0;
  // Get the Distance to the End Point
  CurrentDistance = Distance(Path[CurrentIndex].x, Path[CurrentIndex].y, Vertices[Index].x, Vertices[Index].y);
  // Go to the Previous Point
  for (CurrentIndex = 1; CurrentIndex < NumPath; ++CurrentIndex)
  {
    // Calculate the Next Distance
    NewDistance = Distance(Path[CurrentIndex].x, Path[CurrentIndex].y, Vertices[Index].x, Vertices[Index].y);
    // If it is smaller, set it
    if (NewDistance < CurrentDistance)
    {
      CurrentDistance = NewDistance;
    }
  }
  // Return the Final Distance
  return (CurrentDistance);
}

BOOL CExposureDlg::InBounds(int Index)
{
  // If the Distance is Less than the Appropriate Distance
  if (FindPathDistance(Index) < RefineBound)
  {
    // Return True
    return (TRUE);
  }
  else
  {
    // Mark the Vertex as Visited and Return False
    Vertices[Index].Visited = 1;
    if (ShowRefZone == TRUE)
    {
      DrawSensor(Vertices[Index].x / GridMult, Vertices[Index].y / GridMult, 4);
    }
    return (FALSE);
  }
}

void CExposureDlg::RefinePath()
{
  int CI;       // Current Coordinate Index;
  int DI;       // Destination Coordinate Index
  double CX;    // Current X Coordinate
  double CY;    // Current Y Coordinate
  double FX;    // Final X Coordinate
  double FY;    // Final Y Coordinate
  double DX;    // Destination X Coordinate
  double DY;    // Destination Y Coordinate
  double RX;    // Reference X Coordinate
  double RY;    // Reference Y Coordinate
  double UX;    // Upper X Coordinate
  double UY;    // Upper Y Coordinate
  double LX;    // Lower X Coordinate
  double LY;    // Lower Y Coordinate
  double W;     // Additional Weight Added to Path
  double Step;  // Step Value
  double CCost; // Current Cost to Get to Location
  double NGridSize; // New Grid Size (After Refine Calculation);
  int i = 0;        // Index Variable

  // Set the Progress
  Status.Format("Calculating Path...");
  Progress.SetRange(0, MaxVertices);
  Progress.SetPos(0);
  Progress.SetStep(1);
  UpdateData(FALSE);
  // Get Starting Points
  CX = SX * GridMult;
  CY = SY * GridMult;
  FX = EX * GridMult;
  FY = EY * GridMult;
  SnapToGrid(FX, FY);
  // Set the Step and Size
  NGridSize = 1;
  for (i = 0; i < RefineLevel + 1; ++i)
  {
    NGridSize *= GridSize;
  }
  Step = GridMult / double (NGridSize * GridAccuracy);
  // Find First Vertex
  CI = FindVertex(CX, CY, Step / Precision);
  // Set That Vertex's Cost to 0
  Vertices[CI].Cost = 0;
  // Process Vertices until End-Point is Hit
  while (!((CX >= FX - (Step / Precision)) && (CX <= FX + (Step / Precision)) && (CY >= FY - (Step / Precision)) && (CY <= FY + (Step / Precision))) && (Vertices[CI].Visited != 1))
  {
    // Set the Current Vertex to Visited
    Vertices[CI].Visited = 1;
    // Set the Current Cost
    CCost = Vertices[CI].Cost;
    // Calculate Reference Coordinates
    RX = double (int (CX * pow(GridSize, RefineLevel + 1) / GridMult + Step / Precision / GridMult)) / double (pow(GridSize, RefineLevel + 1)) * GridMult;
    RY = double (int (CY * pow(GridSize, RefineLevel + 1) / GridMult + Step / Precision / GridMult)) / double (pow(GridSize, RefineLevel + 1)) * GridMult;
    // Calculate Border Points
    LX = double (int (CX * pow(GridSize, RefineLevel + 1) / GridMult - 1 + (Step / Precision / GridMult))) / double (pow(GridSize, RefineLevel + 1)) * GridMult;
    LY = double (int (CY * pow(GridSize, RefineLevel + 1) / GridMult - 1 + (Step / Precision / GridMult))) / double (pow(GridSize, RefineLevel + 1)) * GridMult;
    UX = double (int (CX * pow(GridSize, RefineLevel + 1) / GridMult + 1 + (Step / Precision / GridMult))) / double (pow(GridSize, RefineLevel + 1)) * GridMult;
    UY = double (int (CY * pow(GridSize, RefineLevel + 1) / GridMult + 1 + (Step / Precision / GridMult))) / double (pow(GridSize, RefineLevel + 1)) * GridMult;

    for (DX = LX, DY = LY; (DX < RX) && (DY >= 0 - Step) && (DY <= GridMult); DX += Step)
    {
      if ((DX >= 0) && (DX <= GridMult))
      {
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if (InBounds(DI))
        {
          if (ShowRefZone == TRUE)
          {
            DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
          }
          if (((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost)))
          {
            // Set the Cost to the Next Vertex (Current Cost + New Weight)
            Vertices[DI].Cost = W;
            // Set the Previous Vertex to the Current Vertex
            Vertices[DI].Prev = CI;
          }
        }
      }
    }
    for (DX = RX, DY = LY; (DX < UX) && (DY >= 0) && (DY <= GridMult); DX += Step)
    {
      if ((DX >= 0) && (DX <= GridMult))
      {
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if (InBounds(DI))
        {
          if (ShowRefZone == TRUE)
          {
            DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
          }
          if (((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost)))
          {
            // Set the Cost to the Next Vertex (Current Cost + New Weight)
            Vertices[DI].Cost = W;
            // Set the Previous Vertex to the Current Vertex
            Vertices[DI].Prev = CI;
          }
        }
      }
    }
    for (DX = LX, DY = UY; (DX < RX) && (DY >= 0) && (DY <= GridMult); DX += Step)
    {
      if ((DX >= 0) && (DX <= GridMult))
      {
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if (InBounds(DI))
        {
          if (ShowRefZone == TRUE)
          {
            DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
          }
          if (((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost)))
          {
            // Set the Cost to the Next Vertex (Current Cost + New Weight)
            Vertices[DI].Cost = W;
            // Set the Previous Vertex to the Current Vertex
            Vertices[DI].Prev = CI;
          }
        }
      }
    }
    for (DX = RX, DY = UY; (DX < UX) && (DY >= 0) && (DY <= GridMult); DX += Step)
    {
      if ((DX >= 0) && (DX <= GridMult))
      {
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if (InBounds(DI))
        {
          if (ShowRefZone == TRUE)
          {
            DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
          }
          if (((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost)))
          {
            // Set the Cost to the Next Vertex (Current Cost + New Weight)
            Vertices[DI].Cost = W;
            // Set the Previous Vertex to the Current Vertex
            Vertices[DI].Prev = CI;
          }
        }
      }
    }
    for (DX = LX, DY = LY + Step; (DX >= 0) && (DX <= GridMult) && (DY < RY); DY += Step)
    {
      if ((DY >= 0) && (DY <= GridMult))
      {
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if (InBounds(DI))
        {
          if (ShowRefZone == TRUE)
          {
            DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
          }
          if (((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost)))
          {
            // Set the Cost to the Next Vertex (Current Cost + New Weight)
            Vertices[DI].Cost = W;
            // Set the Previous Vertex to the Current Vertex
            Vertices[DI].Prev = CI;
          }
        }
      }
    }
    for (DX = LX, DY = RY; (DY < UY) && (DX >= 0) && (DX <= GridMult); DY += Step)
    {
      if ((DY >= 0) && (DY <= GridMult))
      {
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if (InBounds(DI))
        {
          if (ShowRefZone == TRUE)
          {
            DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
          }
          if (((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost)))
          {
            // Set the Cost to the Next Vertex (Current Cost + New Weight)
            Vertices[DI].Cost = W;
            // Set the Previous Vertex to the Current Vertex
            Vertices[DI].Prev = CI;
          }
        }
      }
    }
    for (DX = UX, DY = LY; (DY < RY) && (DX >= 0) && (DX <= GridMult); DY += Step)
    {
      if ((DY >= 0) && (DY <= GridMult))
      {
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if (InBounds(DI))
        {
          if (ShowRefZone == TRUE)
          {
            DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
          }
          if (((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost)))
          {
            // Set the Cost to the Next Vertex (Current Cost + New Weight)
            Vertices[DI].Cost = W;
            // Set the Previous Vertex to the Current Vertex
            Vertices[DI].Prev = CI;
          }
        }
      }
    }
    for (DX = UX, DY = RY; (DY < UY) && (DX >= 0) && (DX <= GridMult); DY += Step)
    {
      if ((DY >= 0) && (DY <= GridMult))
      {
        // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
        // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
        W = CalcWeight(CX, CY, DX, DY, Step) + CCost;
        // Set the Destination Vertex Index
        DI = FindVertex(DX, DY, Step / Precision);
        // Check the New Distance
        if (InBounds(DI))
        {
          if (ShowRefZone == TRUE)
          {
            DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
          }
          if (((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost)))
          {
            // Set the Cost to the Next Vertex (Current Cost + New Weight)
            Vertices[DI].Cost = W;
            // Set the Previous Vertex to the Current Vertex
            Vertices[DI].Prev = CI;
          }
        }
      }
    }
    if ((UX >= 0) && (UX <= GridMult) && (UY >= 0) && (UY <= GridMult))
    {
      // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
      // W = WeightFunction(FindNearestSensor(CX, CY, DX, DY), Distance(CX, CY, DX, DY)) + CCost;
      W = CalcWeight(CX, CY, UX, UY, Step) + CCost;
      // Set the Destination Vertex Index
      DI = FindVertex(UX, UY, Step / Precision);
      // Check the New Distance
      if (InBounds(DI))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        if (((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost)))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    // Check Point to Left
    if ((CX - Step >= 0) && (CY >= RY - Step / 2) && (CY <= RY + Step / 2))
    {
      // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
      // W = WeightFunction(FindNearestSensor(CX, CY, CX - Step, CY), Distance(CX, CY, CX - Step, CY)) + CCost;
      W = CalcWeight(CX, CY, CX - Step, CY, Step) + CCost;
      // Set the Destination Vertex Index
      DI = FindVertex(CX - Step, CY, Step / Precision);
      if (InBounds(DI))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        // Check the New Distance
        if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    // Check Point to Right
    if ((CX + Step <= GridMult) && (CY >= RY - Step / 2) && (CY <= RY + Step / 2))
    {
      // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
      // W = WeightFunction(FindNearestSensor(CX, CY, CX + Step, CY), Distance(CX, CY, CX + Step, CY)) + CCost;
      W = CalcWeight(CX, CY, CX + Step, CY, Step) + CCost;
      // Set the Destination Vertex Index
      DI = FindVertex(CX + Step, CY, Step / Precision);
      if (InBounds(DI))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        // Check the New Distance
        if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    // Check Point Above
    if ((CX >= RX - Step) && (CX <= RX + Step / 2) && (CY - Step >= 0))
    {
      // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
      // W = WeightFunction(FindNearestSensor(CX, CY, CX, CY - Step), Distance(CX, CY, CX, CY - Step)) + CCost;
      W = CalcWeight(CX, CY, CX, CY - Step, Step) + CCost;
      // Set the Destination Vertex Index
      DI = FindVertex(CX, CY - Step, Step / Precision);
      if (InBounds(DI))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        // Check the New Distance
        if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    // Check Point Below
    if ((CX >= RX - Step) && (CX <= RX + Step / 2) && (CY + Step <= GridMult))
    {
      // Calculate the Distance to the Midpoint of the Edge and add the Additional Weight
      // W = WeightFunction(FindNearestSensor(CX, CY, CX, CY + Step), Distance(CX, CY, CX, CY + Step)) + CCost;
      W = CalcWeight(CX, CY, CX, CY + Step, Step) + CCost;
      // Set the Destination Vertex Index
      DI = FindVertex(CX, CY + Step, Step / Precision);
      if (InBounds(DI))
      {
        if (ShowRefZone == TRUE)
        {
          DrawSensor(Vertices[DI].x / GridMult, Vertices[DI].y / GridMult, 6);
        }
        // Check the New Distance
        if ((Vertices[DI].Cost < 0) || (W < Vertices[DI].Cost))
        {
          // Set the Cost to the Next Vertex (Current Cost + New Weight)
          Vertices[DI].Cost = W;
          // Set the Previous Vertex to the Current Vertex
          Vertices[DI].Prev = CI;
        }
      }
    }
    // Find the Next Vertex
    CI = FindSmallestVertex(CX, CY);
    // Get Current Vertex Index
    // CI = FindVertex(CX, CY, Step / Precision);
    // Step the Progress Bar
    Progress.StepIt();
    // Status.Format("Calculating Path...          %d / %d", Progress.GetPos(), MaxVertices);
    // UpdateData(FALSE);
  }
  // Set the Ending Index Value
  EI = CI;
  // Clear the Progress Bar
  Progress.SetPos(0);
  // Display the Final Message
  if (Vertices[CI].Visited == 1)
  {
    Status.Format("Error! Vertex %d Already Visited!", CI);
    MessageBox(Status, "No More Vertices!");
  }
  PCost.Format("%0.3f", Vertices[CI].Cost);
  Status.Format("");
  UpdateData(FALSE);
}

void CExposureDlg::OnRefine() 
{
  // If Already Generated
  if ((Sensors != NULL) && (Vertices != NULL) && (PicWindow != NULL))
  {
    InitRefine();
    // If the Grid Should Be Shown, Show It
    if (ShowGrid == TRUE)
    {
      DrawGrid();
    }
    // Do the Voronoi Stuff
    if (ShowVoronoi == TRUE)
    {
      if (VoronoiGraph == NULL)
      {
        VoronoiGraph = new VI;
      }
      if (VoronoiDone == FALSE)
      {
        Status.Format("Calculating Voronoi Diagram...");
        UpdateData(FALSE);
        VoronoiGraph->CalcVoronoi(Sensors, N, &Status, &Progress, GetDlgItem(IDD_EXPOSURE_DIALOG));
        VoronoiDone = TRUE;
        Status.Format("");
        UpdateData(FALSE);
      }
      VoronoiGraph->DrawVoronoi(GetDlgItem(IDC_Graph), &VoronoiPen, 2);
    }
    // Draw the Sensors
    DrawSensors();
    // Calculate the Path
    RefinePath();
    // Draw the Path
    DrawPath();
  }	
}

void CExposureDlg::InitRefine()
{
  int i = 0; // Index Variable
  int j = 0; // Index Variable
  CRect DrawWindow;          // Rectangle for Getting Width and Height
  CBrush Bkg(RGB (0, 0, 0)); // Brush to Draw the Background Color
  CPen Black(PS_SOLID, 1, RGB(0, 0, 0)); // Black Path Line

  // Update the SUVI
  SUVI = 0;
  // Calculate the Refine Bound
  RefineBound = 2.0 * GridMult / double (pow(GridSize, RefineLevel + 1));
  UpdateData();
  // Go to the Next Refinement Level
  ++RefineLevel;
  // Clear the Screen
  PicWindow->SelectObject(&Black);
  PicWindow->SelectObject(&Bkg);
  PicWindow->Rectangle(0, 0, Width + 4, Height + 4);
  // Delete Old Path
  if (Path != NULL)
  {
    delete[] Path;
  }
  // Get the Number of Vertices in the Path
  i = EI;
  NumPath = 0;
  while (i != -1)
  {
    // Increment the Number of Path Vertices
    ++NumPath;
    // Go to the Previous Vertex
    i = Vertices[i].Prev;
  }
  // Allocate New Memory for Path
  Path = new GridVertex[NumPath];
  // Assign the Path
  i = EI;
  j = 0;
  while (i != -1)
  {
    // Store the Coordinates
    Path[j].x = Vertices[i].x;
    Path[j].y = Vertices[i].y;
    // Go to the Next Path Vertex
    i = Vertices[i].Prev;
    // Increase the Path Array Index
    ++j;
  }
  // Clear All Vertices So They Are Not Visited
  for (i = 0; i < MaxVertices; ++i)
  {
    Vertices[i].Visited = 0;
    Vertices[i].Cost = -1;
    Vertices[i].Prev = -1;
    Vertices[i].x = 0;
    Vertices[i].y = 0;
  }
  // Erase the Number of Vertices
  NumVertices = 0;
  // Clear All Non-Path Vertices
  // ClearPrevious(EI);
}

void CExposureDlg::ClearPrevious(int CurrentIndex)
{
  int TempIndex;

  if (Vertices[CurrentIndex].Prev != -1)
  {
    // Store Previous Index
    TempIndex = Vertices[CurrentIndex].Prev;
    // Clear All Other Indeces
    ClearPrevious(Vertices[CurrentIndex].Prev);
    // Restore the Previous Vertex
    Vertices[CurrentIndex].Prev = TempIndex;
  }
  else
  {
    // All Previous Indeces Stored, Erase Non-Path Indeces
    for (int i = 0; i < NumVertices; ++i)
    {
      Vertices[i].Prev = -1;
    }
  }
}

void CExposureDlg::ShowRefineZone()
{
  int i = 0; // Index Variable

  // If there is no refinement path, show all vertices as light blue
  if (Path == NULL)
  {
    for (i = 0; i < NumVertices; ++i)
    {
      DrawSensor(Vertices[i].x / GridMult, Vertices[i].y / GridMult, 6);
    }
  }
  else
  {
    for (i = 0; i < NumVertices; ++i)
    {
      if (InBounds(i))
      {
        DrawSensor(Vertices[i].x / GridMult, Vertices[i].y / GridMult, 6);
      }
    }
  }
}

void CExposureDlg::OnInput() 
{
  // If Initialization is OK, Process Path
  if (Initialize())
  {
    // If the Grid Should Be Shown, Show It
    if (ShowGrid == TRUE)
    {
      DrawGrid();
    }
    // Input the Sensors the Sensors
    InputSensors(InFile.GetBuffer(0), N, &Sensors);
    // Update teh Screen with the Number of Sensors
    UpdateData(FALSE);
    // Draw Voronoi Stuff
    if (ShowVoronoi == TRUE)
    {
      if (VoronoiGraph != NULL)
      {
        delete VoronoiGraph;
        VoronoiGraph = NULL;
      }
      Status.Format("Calculating Voronoi Diagram...");
      UpdateData(FALSE);
      VoronoiGraph = new VI;
      VoronoiGraph->CalcVoronoi(Sensors, N, &Status, &Progress);
      VoronoiGraph->DrawVoronoi(GetDlgItem(IDC_Graph), &VoronoiPen, 2);
      VoronoiDone = TRUE;
      Status.Format("");
      UpdateData(FALSE);
    }
    // Draw the Sensors
    DrawSensors();
    // Calculate the Path
    CalcPath();
    // Draw the Path
    DrawPath();
  }
}

double CExposureDlg::FindNearestSensor(double X1, double Y1, double X2, double Y2)
{
  int i = 0; // Index Variable
  double CD; // Current Distance to Sensor
  double ND; // Distance to Next Sensor
  double XM; // X Midpoint
  double YM; // Y Midpoint

  // Calculate Midpoint
  XM = (X1 + X2) / 2;
  YM = (Y1 + Y2) / 2;
  // Find Initial Distance
  // CD = SDistance(Sensors[0].x * GridMult, Sensors[0].y * GridMult, X1, Y1, X2, Y2);
  CD = Distance(Sensors[0].x * GridMult, Sensors[0].y * GridMult, XM, YM);
  // Check All Other Distances
  for (i = 1; i < N; ++i)
  {
    // Get New Distance
    // ND = SDistance(Sensors[i].x * GridMult, Sensors[i].y * GridMult, X1, Y1, X2, Y2);
    ND = Distance(Sensors[i].x * GridMult, Sensors[i].y * GridMult, XM, YM);
    if (ND < CD)
    {
      // Set New Distance to Current Distance
      CD = ND;
    }
  }
  // Return the Current Distance
  return (CD);
}

double CExposureDlg::CalcWeight(double SX, double SY, double EX, double EY, double Step)
{
  double DX = 0;  // Change in X
  double DY = 0;  // Change in Y
  double Xi = SX; // Starting Segment Point
  double Yi = SY; // Starting Segment Point
  double Xf = 0;  // Final Segment Point
  double Yf = 0;  // Final Segment Point
  double W = 0;   // Current Weight
  double O = 0;   // Theta (Angle Variable)

  // Check for Horizontal or Vertical Line
  if ((SX == EX) || (SY == EY))
  {
    DX = Step;
    DY = Step;
    // Check for Vertical Line
    if (SX == EX)
    {
      DX = 0;
    }
    // Check for Horizontal Line
    if (SY == EY)
    {
      DY = 0;
    }
  }
  // Calculate the Change in X and Change in Y
  else
  {
    // Calculate the Angle
    O = asin(fabs(EX - SX) / Distance(SX, SY, EX, EY));
    DX = Step * sin(O);
    DY = Step * cos(O);
  }
  // If the End Point is to the Left, Negate the DX
  if (EX < SX)
  {
    DX = 0 - DX;
  }
  // If the End Point is Above, Negate the DY
  if (EY < SY)
  {
    DY = 0 - DY;
  }
  // Calculate the Ending Segment Points
  Xf = Xi + DX;
  Yf = Yi + DY;
  // Add the Weight
  W += WeightFunction(FindNearestSensor(Xi, Yi, Xf, Yf), Distance(Xi, Yi, Xf, Yf));
  // Go to the Next Segment Until the End Point is Reached
  // while (((Xf != EX) && (DX != 0)) || ((Yf != EY) && (DY != 0)))
  while (((Xf != EX) || (DX == 0)) && ((Yf != EY) || (DY == 0)) && ((DX != 0) || (DY != 0)))
  {
    // Update the Line Segment
    Xi = Xf;
    Yi = Yf;
    Xf += DX;
    Yf += DY;
    // If the Boundary Has Been Passed
    if (((Xf > EX) && (DX > 0)) || ((Xf < EX) && (DX < 0)) || ((Yf > EY) && (DY > 0)) || ((Yf < EY) && (DY < 0)))
    {
      // Snap the Segment to the End Point
      Xf = EX;
      Yf = EY;
    }
    W += WeightFunction(FindNearestSensor(Xi, Yi, Xf, Yf), Distance(Xi, Yi, Xf, Yf));
  }
  // Return the Weight of the Path
  return (W);
}

void CExposureDlg::SnapToGrid(double &X, double &Y)
{
  double X1;   // Temporary Coordinate
  double Y1;   // Temporary Coordinate
  double X2;   // Temporary Coordinate
  double Y2;   // Temporary Coordinate
  double X3;   // Temporary Coordinate
  double Y3;   // Temporary Coordinate
  double Step; // Step Value
  double D1;   // First Distance
  double D2;   // Second Distance
  double D3;   // Third Distance

  // Set the Step
  Step = GridMult / double (GridSize * GridAccuracy);
  // Snap it to the Grid Vertex
  X1 = double (int (X * double (pow(GridSize, RefineLevel + 1)) / GridMult + Step / Precision / GridMult)) / double (pow(GridSize, RefineLevel + 1)) * GridMult;
  Y1 = double (int (Y * double (pow(GridSize, RefineLevel + 1)) / GridMult + Step / Precision / GridMult)) / double (pow(GridSize, RefineLevel + 1)) * GridMult;
  // Snap it to the Vertical Axis
  X2 = double (int (X * double (pow(GridSize, RefineLevel + 1)) / GridMult + Step / Precision / GridMult)) / double (pow(GridSize, RefineLevel + 1)) * GridMult;
  Y2 = double (int (Y * double (GridAccuracy * pow(GridSize, RefineLevel + 1)) / GridMult + Step / Precision / GridMult)) / double (pow(GridSize, RefineLevel + 1) * GridAccuracy) * GridMult;
  // Snap it to the Horizontal Axis
  X3 = double (int (X * double (GridAccuracy * pow(GridSize, RefineLevel + 1)) / GridMult + Step / Precision / GridMult)) / double (pow(GridSize, RefineLevel + 1) * GridAccuracy) * GridMult;
  Y3 = double (int (Y * double (pow(GridSize, RefineLevel + 1)) / GridMult + Step / Precision / GridMult)) / double (pow(GridSize, RefineLevel + 1)) * GridMult;
  // Compare the Distances
  D1 = Distance(X, Y, X1, Y1);
  D2 = Distance(X, Y, X2, Y2);
  D3 = Distance(X, Y, X3, Y3);
  // Snap to the Closest Grid Point
  if ((D1 < D2) && (D1 < D3))
  {
    X = X1;
    Y = Y1;
  }
  else if ((D2 < D1) && (D2 < D3))
  {
    X = X2;
    Y = Y2;
  }
  else
  {
    X = X3;
    Y = Y3;
  }
}
