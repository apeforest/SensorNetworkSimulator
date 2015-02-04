// AStarDlg.cpp : implementation file
//


#include "stdafx.h"
#include "AStar.h"
#include "AStarDlg.h"
#include <fstream.h>
#include <time.h>
#include <math.h>

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
// CAStarDlg dialog

CAStarDlg::CAStarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAStarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAStarDlg)
	BkgB = 0;
	BkgG = 0;
	BkgR = 0;
	EdgeB = 64;
	EdgeG = 64;
	EdgeR = 64;
	SAB = 255;
	SAG = 255;
	SAR = 255;
	SIB = 128;
	SIG = 0;
	SIR = 0;
	SVB = 255;
	SVG = 0;
	SVR = 0;
	DestB = 0;
	DestG = 0;
	DestR = 255;
	StartB = 0;
	StartG = 255;
	StartR = 255;
	Border = 3;
	Start = 0;
	Dest = 1;
	Mode = 0;
	NumNodes = 100;
	SensorSize = 4;
	Radius = 0.2;
	SleepTime = 50;
	Version = 0;
	PercentSleep = 0.0;
	SleepB = 96;
	SleepG = 96;
	SleepR = 96;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAStarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAStarDlg)
	DDX_Text(pDX, IDC_BkgB, BkgB);
	DDX_Text(pDX, IDC_BkgG, BkgG);
	DDX_Text(pDX, IDC_BkgR, BkgR);
	DDX_Text(pDX, IDC_EdgeB, EdgeB);
	DDX_Text(pDX, IDC_EdgeG, EdgeG);
	DDX_Text(pDX, IDC_EdgeR, EdgeR);
	DDX_Text(pDX, IDC_SAB, SAB);
	DDX_Text(pDX, IDC_SAG, SAG);
	DDX_Text(pDX, IDC_SAR, SAR);
	DDX_Text(pDX, IDC_SIB, SIB);
	DDX_Text(pDX, IDC_SIG, SIG);
	DDX_Text(pDX, IDC_SIR, SIR);
	DDX_Text(pDX, IDC_SVB, SVB);
	DDX_Text(pDX, IDC_SVG, SVG);
	DDX_Text(pDX, IDC_SVR, SVR);
	DDX_Text(pDX, IDC_DestB, DestB);
	DDX_Text(pDX, IDC_DestG, DestG);
	DDX_Text(pDX, IDC_DestR, DestR);
	DDX_Text(pDX, IDC_StartB, StartB);
	DDX_Text(pDX, IDC_StartG, StartG);
	DDX_Text(pDX, IDC_StartR, StartR);
	DDX_Text(pDX, IDC_Border, Border);
	DDX_Text(pDX, IDC_Start, Start);
	DDX_Text(pDX, IDC_Dest, Dest);
	DDX_Text(pDX, IDC_Mode, Mode);
	DDX_Text(pDX, IDC_NumNodes, NumNodes);
	DDX_Text(pDX, IDC_SensorSize, SensorSize);
	DDX_Text(pDX, IDC_Radius, Radius);
	DDX_Text(pDX, IDC_SleepTime, SleepTime);
	DDX_Text(pDX, IDC_Version, Version);
	DDX_Text(pDX, IDC_PercentSleep, PercentSleep);
	DDX_Text(pDX, IDC_SleepB, SleepB);
	DDX_Text(pDX, IDC_SleepR, SleepR);
	DDX_Text(pDX, IDC_SleepG, SleepG);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAStarDlg, CDialog)
	//{{AFX_MSG_MAP(CAStarDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RUN, OnRun)
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAStarDlg message handlers

BOOL CAStarDlg::OnInitDialog()
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

void CAStarDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAStarDlg::OnPaint() 
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
HCURSOR CAStarDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



// Additional Functions ////////////////////////////////////////////////////////
void CAStarDlg::Initialize(void)
{ // Initialize
  CRect DrawWindow;              // Temporary Drawing Window

  srand(time(NULL));
  // Initialize the Graphics Window ////////////////////////////////////////////
  PicWindow = new CClientDC(GetDlgItem(IDC_Picture));

  // Get the Width and Height of the Graphics Window
  GetDlgItem(IDC_Picture)->GetClientRect(&DrawWindow);
  Width = DrawWindow.BottomRight().x - 2 * Border;
  Height = DrawWindow.BottomRight().y - 2 * Border;

  // Make a New Default AStar Network
  OnNew();
} // Initialize



void CAStarDlg::Delete(void)
/*
  This function Deletes the AStarDlg allocated objects.
*/
{ // Delete
  if (PicWindow != NULL) delete PicWindow;
} // Delete



void CAStarDlg::OnRun()
{ // Run
  int i = 0;                             // Loop Counter to Prevent Infinite Loops
  int Done = 0;                          // Done with Processing AStar Search
  int Node = 0;                          // Current Node
  double Maxi = pow(NumNodes, NumNodes); // Maximum Index Value

  ASN.InitializeAStar(Start, Dest);
  OnRefresh();
  while ((i < Maxi) && (!Done))
  {
    // Turn Off the Sensor Highlight
    if (ASN.CurrentNode == Start)
    {
      DrawSensor(PicWindow, ASN.SensorArray[ASN.CurrentNode].X, ASN.SensorArray[ASN.CurrentNode].Y, StartR, StartG, StartB, Width, Height);
    }
    else if (ASN.CurrentNode == Dest)
    {
      DrawSensor(PicWindow, ASN.SensorArray[ASN.CurrentNode].X, ASN.SensorArray[ASN.CurrentNode].Y, DestR, DestG, DestB, Width, Height);
    }
    else
    {
      DrawSensor(PicWindow, ASN.SensorArray[ASN.CurrentNode].X, ASN.SensorArray[ASN.CurrentNode].Y, SVR, SVG, SVB, Width, Height);
    }

    // Cycle the Algorithm and Pause
    Done = ASN.CycleAStar(Version);
    ++i;

    // Turn on the Sensor Highlight
    DrawSensor(PicWindow, ASN.SensorArray[ASN.CurrentNode].X, ASN.SensorArray[ASN.CurrentNode].Y, SAR, SAG, SAB, Width, Height);

    // Pause
    Delay(SleepTime);
  }

  // Turn Off the Sensor Highlight
  if (ASN.CurrentNode == Start)
  {
    DrawSensor(PicWindow, ASN.SensorArray[ASN.CurrentNode].X, ASN.SensorArray[ASN.CurrentNode].Y, StartR, StartG, StartB, Width, Height);
  }
  else if (ASN.CurrentNode == Dest)
  {
    DrawSensor(PicWindow, ASN.SensorArray[ASN.CurrentNode].X, ASN.SensorArray[ASN.CurrentNode].Y, DestR, DestG, DestB, Width, Height);
  }
  else
  {
    DrawSensor(PicWindow, ASN.SensorArray[ASN.CurrentNode].X, ASN.SensorArray[ASN.CurrentNode].Y, SVR, SVG, SVB, Width, Height);
  }

  // Display the Final Results
  ASN.CalcNumHops();
  Output();
  OnRefresh();

  // Debugging Purposes Only
  if (i >= Maxi)
  {
    i = i;
  }
} // Run



void CAStarDlg::Draw(CClientDC* PicWindow, int Width, int Height)
/*
  This function Draws the Graph.
*/
{ // Draw
  int i = 0;                              // Index Variable
  CBrush BrushBkg(RGB(BkgR, BkgG, BkgB)); // Background Brush

  // Clear the Screen
  PicWindow->SelectObject(&BrushBkg);
  PicWindow->Rectangle(0, 0, Width + 2 * Border, Height + 2 * Border);

  // Draw the Edges
  DrawEdges(PicWindow, EdgeR, EdgeG, EdgeB, Width, Height);
  // Draw the Sensors
  for (i = 0; i < ASN.NumSensors; ++i)
  {
    if (ASN.SensorArray[i].Mode == SENSOR_MODE_SLEEP)
    {
      DrawSensor(PicWindow, ASN.SensorArray[i].X, ASN.SensorArray[i].Y, SleepR, SleepG, SleepB, Width, Height);
    }
    else if (ASN.SensorArray[i].Enqueued)
    {
      DrawSensor(PicWindow, ASN.SensorArray[i].X, ASN.SensorArray[i].Y, SVR, SVG, SVB, Width, Height);
    }
    else
    {
      DrawSensor(PicWindow, ASN.SensorArray[i].X, ASN.SensorArray[i].Y, SIR, SIG, SIB, Width, Height);
    }
  }
  // Draw a Path from the Destination to the Producer
  for (i = Dest; (i >= 0) && (i != Start); i = ASN.SensorArray[i].Parent)
  {
    DrawSensor(PicWindow, ASN.SensorArray[i].X, ASN.SensorArray[i].Y, SAR, SAG, SAB, Width, Height);
  }
  DrawSensor(PicWindow, ASN.SensorArray[Start].X, ASN.SensorArray[Start].Y, StartR, StartG, StartB, Width, Height);
  DrawSensor(PicWindow, ASN.SensorArray[Dest].X, ASN.SensorArray[Dest].Y, DestR, DestG, DestB, Width, Height);
} // Draw



void CAStarDlg::DrawSensor(CClientDC* PicWindow, double XC, double YC, int R, int G, int B, int Width, int Height)
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



void CAStarDlg::DrawEdges(CClientDC* PicWindow, int R, int G, int B, int Width, int Height)
/*
  This function draws the edges of the graph
*/
{ // DrawEdges
  int i = 0;                               // Index Variable
  int j = 0;                               // Index Variable
  int S = 0;                               // Current Sensor
  int X = 0;                               // X Coordinate
  int Y = 0;                               // Y Coordinate
  CPen EdgePen(PS_SOLID, 1, RGB(R, G, B)); // Pen for Edge


  PicWindow->SelectObject(&EdgePen);
  for (i = 0; i < ASN.NumSensors; ++i)
  {
    for (j = 0, S = *(ASN.EdgeTable + i * ASN.NumSensors + j); S >= 0; S = *(ASN.EdgeTable + i * ASN.NumSensors + (++j)))
    {
      // Draw the Edge
      X = int (ASN.SensorArray[i].X * Width) + Border;
      Y = int (ASN.SensorArray[i].Y * Height) + Border;
      PicWindow->MoveTo(X, Y);
      X = int (ASN.SensorArray[S].X * Width) + Border;
      Y = int (ASN.SensorArray[S].Y * Height) + Border;
      PicWindow->LineTo(X, Y);
    }
  }
} // DrawEdges



void CAStarDlg::OnNew() 
/*
  This function creates a new AStar Network.
*/
{ // OnNew
	UpdateData();
  if (NumNodes < 0) NumNodes = 0;
  if (Start < 0) Start = 0; else if (Start >= NumNodes) Start = NumNodes - 1;
  if (Dest < 0) Dest = 0; else if (Dest >= NumNodes) Dest = NumNodes - 1;
  UpdateData(FALSE);
  ASN.Delete();
  ASN.InitializeAStarNetwork(Mode, NumNodes, Radius, Start, Dest, PercentSleep);	
  OnRefresh();
} // OnNew



void CAStarDlg::OnRefresh() 
/*
  This function refreshes the screen.
*/
{ // OnRefresh
  CRect DrawWindow;

  UpdateData();
  if (NumNodes < 0) NumNodes = 0;
  if (Start < 0) Start = 0; else if (Start >= NumNodes) Start = NumNodes - 1;
  if (Dest < 0) Dest = 0; else if (Dest >= NumNodes) Dest = NumNodes - 1;
  UpdateData(FALSE);
  // Get the New Width and Height of the Graphics Window
  GetDlgItem(IDC_Picture)->GetClientRect(&DrawWindow);
  Width = DrawWindow.BottomRight().x - 2 * Border;
  Height = DrawWindow.BottomRight().y - 2 * Border;

  Draw(PicWindow, Width, Height);
} // OnRefresh



void CAStarDlg::Delay(int Milliseconds)
{ // Delay
  int CurrentTime = clock();
  int PreviousTime = CurrentTime;
  double DelayTime = 0;

  while (DelayTime < Milliseconds)
  {
    CurrentTime = clock();
    DelayTime = ((double) (CurrentTime - PreviousTime)) / ((double) CLOCKS_PER_SEC) * 1000;
  }
} // Delay



void CAStarDlg::Output(void)
/*
  This function outputs the results
*/
{ // Output
  ofstream OutFile; // Output File
  int FileOpen = 0; // Is File Open?

  // Open the Output File for Appending
  if (Version == ASTAR_VERSION_NORMAL)
  {
    OutFile.open("Output0.txt", ios::app);
    FileOpen = 1;
  }
  else if (Version == ASTAR_VERSION_SHORT)
  {
    OutFile.open("Output1.txt", ios::app);
    FileOpen = 1;
  }
  if (FileOpen)
  {
    OutFile << ASN.NumSensors << ", "
            << ASN.Radius << ", "
            << ASN.Message.NumVisited << ", "
            << ASN.Message.NumCommunications << ", "
            << ASN.NumHops << endl;
    OutFile.close();
  }
} // Output
