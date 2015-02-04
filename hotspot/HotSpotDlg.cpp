// HotSpotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotSpot.h"
#include "HotSpotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CHotSpotDlg dialog

CHotSpotDlg::CHotSpotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHotSpotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHotSpotDlg)
	Border = 10;
	BkgB = 0;
	BkgG = 0;
	BkgR = 0;
	HighB = 255;
	HighG = 255;
	HighR = 255;
	LowB = 0;
	LowG = 0;
	LowR = 128;
	MedB = 0;
	MedG = 128;
	MedR = 255;
	NumSensors = 200;
	Radius = 0.11;
	Sleep = 0.20;
	SleepB = 128;
	SleepG = 0;
	SleepR = 0;
	EdgeB = 0;
	EdgeG = 0;
	EdgeR = 96;
	SensorSize = 3;
	GradientFill = TRUE;
	EdgeBrightness = 0.60;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHotSpotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHotSpotDlg)
	DDX_Text(pDX, IDC_Border, Border);
	DDX_Text(pDX, IDC_BkgB, BkgB);
	DDX_Text(pDX, IDC_BkgG, BkgG);
	DDX_Text(pDX, IDC_BkgR, BkgR);
	DDX_Text(pDX, IDC_HighB, HighB);
	DDX_Text(pDX, IDC_HighG, HighG);
	DDX_Text(pDX, IDC_HighR, HighR);
	DDX_Text(pDX, IDC_LowB, LowB);
	DDX_Text(pDX, IDC_LowG, LowG);
	DDX_Text(pDX, IDC_LowR, LowR);
	DDX_Text(pDX, IDC_MedB, MedB);
	DDX_Text(pDX, IDC_MedG, MedG);
	DDX_Text(pDX, IDC_MedR, MedR);
	DDX_Text(pDX, IDC_NumSensors, NumSensors);
	DDX_Text(pDX, IDC_Radius, Radius);
	DDX_Text(pDX, IDC_Sleep, Sleep);
	DDX_Text(pDX, IDC_SleepB, SleepB);
	DDX_Text(pDX, IDC_SleepG, SleepG);
	DDX_Text(pDX, IDC_SleepR, SleepR);
	DDX_Text(pDX, IDC_EdgeB, EdgeB);
	DDX_Text(pDX, IDC_EdgeG, EdgeG);
	DDX_Text(pDX, IDC_EdgeR, EdgeR);
	DDX_Text(pDX, IDC_SensorSize, SensorSize);
	DDX_Check(pDX, IDC_GradientEdges, GradientFill);
	DDX_Text(pDX, IDC_EdgeBright, EdgeBrightness);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHotSpotDlg, CDialog)
	//{{AFX_MSG_MAP(CHotSpotDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_New, OnNew)
	ON_BN_CLICKED(IDC_Refresh, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHotSpotDlg message handlers

BOOL CHotSpotDlg::OnInitDialog()
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
	Initialize();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHotSpotDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHotSpotDlg::OnPaint() 
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
HCURSOR CHotSpotDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


// Custom Functions ////////////////////////////////////////////////////////////
void CHotSpotDlg::Initialize(void)
{ // Initialize
  CRect DrawWindow;          // Temporary Drawing Window

  srand(time(NULL));
  // Initialize the Graphics Window ////////////////////////////////////////////
  PicWindow = new CClientDC(GetDlgItem(IDC_Picture));
  InitializeColorArray();

  // Get the Width and Height of the Graphics Window
  GetDlgItem(IDC_Picture)->GetClientRect(&DrawWindow);
  Width = DrawWindow.BottomRight().x - 2 * Border;
  Height = DrawWindow.BottomRight().y - 2 * Border;

  // Make a New Default AStar Network
  OnNew();
} // Initialize



void CHotSpotDlg::InitializeColorArray(void)
{ // InitializeColorArray
  int i = 0;                 // Index Variable
  double D1R = MedR - LowR;  // Medium Distance
  double D1G = MedG - LowG;  // Medium Distance
  double D1B = MedB - LowB;  // Medium Distance
  double D2R = HighR - MedR; // Medium Distance
  double D2G = HighG - MedG; // Medium Distance
  double D2B = HighB - MedB; // Medium Distance
  double Array = 0;          // Array For Indexing Color Arrays  

  // Initialize The Color Arrays
  for (i = 0; i < HOTSPOT_ARRAYMID; ++i)
  {
    Array = ((double) i) / ((double) HOTSPOT_ARRAYMID);
    R[i] = (int) (LowR + Array * D1R);
    G[i] = (int) (LowG + Array * D1G);
    B[i] = (int) (LowB + Array * D1B);
    RSleep[i] = SleepR;
    GSleep[i] = SleepG;
    BSleep[i] = SleepB;
  }
  for (i = HOTSPOT_ARRAYMID; i < HOTSPOT_ARRAYSIZE; ++i)
  {
    Array = ((double) i) / ((double) HOTSPOT_ARRAYSIZE) - ((double) HOTSPOT_ARRAYMID) / ((double) HOTSPOT_ARRAYSIZE);
    R[i] = (int) (MedR + Array * D2R);
    G[i] = (int) (MedG + Array * D2G);
    B[i] = (int) (MedB + Array * D2B);
    RSleep[i] = SleepR;
    GSleep[i] = SleepG;
    BSleep[i] = SleepB;
  }
} // InitializeColorArray
 


void CHotSpotDlg::DrawGradientLine(double U1, double V1, double U2, double V2, int CR[], int CG[], int CB[], int Start, int End, double EdgeBrightness, int Width, int Height)
/*
  This function draw a line from a percentage 
*/
{ // DrawGradientLine
  int i = 0;                             // Index Variable
  int Index = 0;                         // Color Array Index
  int Steep = 0;                         // Is The Line More Vertical?
  int X1 = (int) (Width * U1 + Border);  // Initial X Point In Area
  int Y1 = (int) (Height * V1 + Border); // Initial Y Point In Area
  int X2 = (int) (Width * U2 + Border);  // Final X Point In Area
  int Y2 = (int) (Height * V2 + Border); // Final Y Point In Area
  int DX = X2 - X1;                      // Difference In X Values
  int DY = Y2 - Y1;                      // Difference In Y Values
  int D = 0;                             // D-Value For Determining When To Move
  int SX = 1;                            // Does X Change Positively Or Negatively
  int SY = 1;                            // Does Y Change Positively Or Negatively
  int Temp = 0;                          // Temporary Variable Used For Swapping
  double DI = End - Start;               // Index Difference

  if (DX <= 0)
  {
    SX = -1;
    DX = -DX;
  }

  if (DY <= 0)
  {
    SY = -1;
    DY = -DY;
  }

  if (DY > DX)
  {
    Steep = 1;
    Temp = DX;
    DX = DY;
    DY = Temp;

    Temp = SX;
    SX = SY;
    SY = Temp;

    Temp = X1;
    X1 = Y1;
    Y1 = Temp;
  }

  D = (2 * DY) - DX;
  if (DX > 0)
  {
    for (i = 0; i <= DX; ++i)
    {
      Index = (int) ((((double) i) / ((double) DX)) * DI) + Start;
      if (Steep)
      {
      
        PicWindow->SetPixel(Y1, X1, RGB((int) CR[Index] * EdgeBrightness, (int) CG[Index] * EdgeBrightness, (int) CB[Index] * EdgeBrightness));
      }
      else
      {
        PicWindow->SetPixel(X1, Y1, RGB((int) CR[Index] * EdgeBrightness, (int) CG[Index] * EdgeBrightness, (int) CB[Index] * EdgeBrightness));
      }
      // If Steep Part
      while (D >= 0)
      {
        Y1 += SY;
        D -= (2 * DX);
      }
      X1 += SX;
      D += (2 * DY);
    }
  }
  PicWindow->SetPixel(X2, Y2, RGB(R[End], G[End], B[End]));
} // DrawGradientLine



void CHotSpotDlg::OnNew() 
{ // OnNew
  UpdateData();
  if (NumSensors < 0) NumSensors = 0;
  UpdateData(FALSE);
  HSN.Delete();
  HSN.InitializeHotSpotNetwork(0, NumSensors, Radius, Sleep);
  OnRefresh();
} // OnNew



void CHotSpotDlg::OnRefresh() 
{ // OnRefresh
  CRect DrawWindow;       // Drawing Window
  int CalculateFW = 0;    // ReCalculate The Floyd-Warshall
  double Error = 0.00001; // Error For Double Measurements

  // Store The Previous Information
  PrevRadius = Radius;
  PrevSleep = Sleep;

  UpdateData();
  if (NumSensors < 0) NumSensors= 0;
  if (EdgeBrightness < 0) EdgeBrightness = 0;
  else if (EdgeBrightness > 1) EdgeBrightness = 1;
  // Update New Information If Necessary
  if ((PrevSleep - Sleep > Error) || (Sleep - PrevSleep > Error))
  {
    CalculateFW = 1;
    HSN.CalculateSleepNodes(Sleep);
  }
  if ((PrevRadius - Radius > Error) || (Radius - PrevRadius > Error))
  {
    CalculateFW = 1;
    HSN.Radius = Radius;
    HSN.CalculateEdges();
  }
  if (CalculateFW)
  {
    HSN.InitializeFW();
  }

  // Get the New Width and Height of the Graphics Window
  InitializeColorArray();
  GetDlgItem(IDC_Picture)->GetClientRect(&DrawWindow);
  Width = DrawWindow.BottomRight().x - 2 * Border;
  Height = DrawWindow.BottomRight().y - 2 * Border;
  UpdateData(FALSE);

  OnDraw();
} // OnRefresh



void CHotSpotDlg::DrawSensor(CClientDC* PicWindow, double XC, double YC, int R, int G, int B, int Width, int Height)
/*
  This function draws a Generic Sensor
*/
{ // DrawSensor
  int i = 0;
  int j = 0;
  int k = 0;
  int X0 = int (Width * XC) + Border;
  int Y0 = int (Height * YC) + Border;
  int X = X0;
  int Y = Y0;
  
  for (i = -SensorSize; i <= SensorSize; ++i)
  {
    if (i < 0) j = -i; else j = i;
    for (k = -SensorSize + j; k <= SensorSize - j; ++k)
    {
      int X = X0 + i;
      int Y = Y0 + k;
      if ((X >= Border) && (X <= Width + Border) && (Y >= Border) && (Y <= Height + Border)) PicWindow->SetPixel(X, Y, RGB(R, G, B));
    }
  }
} // DrawSensor



void CHotSpotDlg::DrawEdges(CClientDC* PicWindow, int GradientFill, int ER, int EG, int EB, int SR, int SG, int SB, int AR[], int AG[], int AB[], int SAR[], int SAG[], int SAB[], double EdgeBrightness, int Width, int Height)
{ // DrawEdges
  int i = 0;                                  // Index Variable
  int j = 0;                                  // Index Variable
  int S = 0;                                  // Current Sensor
  int X = 0;                                  // X Coordinate
  int Y = 0;                                  // Y Coordinate
  int Index1 = 0;                             // First Color Index
  int Index2 = 0;                             // Second Color Index
  CPen EdgePen(PS_SOLID, 1, RGB(ER * EdgeBrightness, EG * EdgeBrightness, EB * EdgeBrightness));
  CPen SleepPen(PS_SOLID, 1, RGB(SR * EdgeBrightness, SG * EdgeBrightness, SB * EdgeBrightness));

  if (GradientFill)
  {
    for (i = 0; i < HSN.NumSensors; ++i)
    {
      Index1 = (int) (HSN.SensorArray[i].Cost * (HOTSPOT_ARRAYSIZE - 1));
      for (j = 0, S = *(HSN.EdgeTable + i * HSN.NumSensors + j); S >= 0; S = *(HSN.EdgeTable + i * HSN.NumSensors + (++j)))
      {
        Index2 = (int) (HSN.SensorArray[S].Cost * (HOTSPOT_ARRAYSIZE - 1));
        if ((HSN.SensorArray[i].Mode == SENSOR_MODE_SLEEP) || (HSN.SensorArray[S].Mode == SENSOR_MODE_SLEEP))
        {
          DrawGradientLine(HSN.SensorArray[i].X, HSN.SensorArray[i].Y, HSN.SensorArray[S].X, HSN.SensorArray[S].Y, SAR, SAG, SAB, Index1, Index2, EdgeBrightness, Width, Height);
        }
        else
        {
          DrawGradientLine(HSN.SensorArray[i].X, HSN.SensorArray[i].Y, HSN.SensorArray[S].X, HSN.SensorArray[S].Y, AR, AG, AB, Index1, Index2, EdgeBrightness, Width, Height);
        }
      }
    }
  }
  else
  {
    for (i = 0; i < HSN.NumSensors; ++i)
    {
      for (j = 0, S = *(HSN.EdgeTable + i * HSN.NumSensors + j); S >= 0; S = *(HSN.EdgeTable + i * HSN.NumSensors + (++j)))
      {
        if ((HSN.SensorArray[i].Mode == SENSOR_MODE_SLEEP) || (HSN.SensorArray[S].Mode == SENSOR_MODE_SLEEP))
        {
          PicWindow->SelectObject(&SleepPen);
        }
        else
        {
          PicWindow->SelectObject(&EdgePen);
        }        
        // Draw the Edge
        X = int (HSN.SensorArray[i].X * Width) + Border;
        Y = int (HSN.SensorArray[i].Y * Height) + Border;
        PicWindow->MoveTo(X, Y);
        X = int (HSN.SensorArray[S].X * Width) + Border;
        Y = int (HSN.SensorArray[S].Y * Height) + Border;
        PicWindow->LineTo(X, Y);
      }
    }
  }
} // DrawEdges



void CHotSpotDlg::OnDraw(void)
{ // OnDraw
  int i = 0;                                           // Index Variable
  int j = 0;                                           // Index Variable
  int Index = 0;                                       // Color Index
  CBrush BrushBkg(RGB(BkgR, BkgG, BkgB));              // Background Brush

  // Clear the Screen
  PicWindow->SelectObject(&BrushBkg);
  PicWindow->Rectangle(0, 0, Width + 2 * Border, Height + 2 * Border);

  // Draw All Edges and Sensors
  DrawEdges(PicWindow, GradientFill, EdgeR, EdgeG, EdgeB, SleepR, SleepG, SleepB, R, G, B, RSleep, GSleep, BSleep, EdgeBrightness, Width, Height);
  for (i = 0; i < HSN.NumSensors; ++i)
  {
    Index = (int) (HSN.SensorArray[i].Cost * (HOTSPOT_ARRAYSIZE - 1));
    if (HSN.SensorArray[i].Mode == SENSOR_MODE_SLEEP)
    {
      DrawSensor(PicWindow, HSN.SensorArray[i].X, HSN.SensorArray[i].Y, SleepR, SleepG, SleepB, Width, Height);
    }
    else
    {
      DrawSensor(PicWindow, HSN.SensorArray[i].X, HSN.SensorArray[i].Y, R[Index], G[Index], B[Index], Width, Height);
    }
  }
} // OnDraw
