// VisualSleepDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VisualSleep.h"
#include "VisualSleepDlg.h"
#include "SleepNetwork.h"
#include <fstream.h>

SleepNetwork S1;
SleepNetwork S2;
int Calculated = 0;

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
// CVisualSleepDlg dialog

CVisualSleepDlg::CVisualSleepDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVisualSleepDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVisualSleepDlg)
	Mode = 0;
	NC = 10;
	NS = 200;
	Rad = 0.125;
	NC1 = 0;
	NC2 = 0;
	NS1 = 0;
	NS2 = 0;
	Avg1 = 0;
	Avg2 = 0;
	Max1 = 0;
	Max2 = 0;
	Min1 = 0;
	Min2 = 0;
	RandSeed = 8467547;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVisualSleepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisualSleepDlg)
	DDX_Text(pDX, IDC_Mode, Mode);
	DDX_Text(pDX, IDC_NumConsumers, NC);
	DDX_Text(pDX, IDC_NumSensors, NS);
	DDX_Text(pDX, IDC_Radius, Rad);
	DDX_Text(pDX, IDC_NumComs1, NC1);
	DDX_Text(pDX, IDC_NumComs2, NC2);
	DDX_Text(pDX, IDC_NumSensors1, NS1);
	DDX_Text(pDX, IDC_NumSensors2, NS2);
	DDX_Text(pDX, IDC_Avg1, Avg1);
	DDX_Text(pDX, IDC_Avg2, Avg2);
	DDX_Text(pDX, IDC_Max1, Max1);
	DDX_Text(pDX, IDC_Max2, Max2);
	DDX_Text(pDX, IDC_Min1, Min1);
	DDX_Text(pDX, IDC_Min2, Min2);
	DDX_Text(pDX, IDC_RandSeed, RandSeed);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVisualSleepDlg, CDialog)
	//{{AFX_MSG_MAP(CVisualSleepDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Exit, OnExit)
	ON_BN_CLICKED(IDC_Refresh, OnRefresh)
	ON_BN_CLICKED(IDC_BUTSEED, OnButseed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisualSleepDlg message handlers

BOOL CVisualSleepDlg::OnInitDialog()
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

void CVisualSleepDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVisualSleepDlg::OnPaint() 
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
HCURSOR CVisualSleepDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CVisualSleepDlg::Initialize(void)
{ // Initialize
  CRect DrawWindow;
  CBrush BkgBrush(RGB(0, 0, 0)); // Black Brush for Window

  // srand(time(NULL));
  srand(RandSeed);
  // Initialize the Graphics Window ////////////////////////////////////////////
  PicWindow1 = new CClientDC(GetDlgItem(IDC_Picture));
  PicWindow2 = new CClientDC(GetDlgItem(IDC_Picture2));
  
  // Get the Width and Height of the Graphics Window
  GetDlgItem(IDC_Picture)->GetClientRect(&DrawWindow);
  Border = 4;
  Width1 = DrawWindow.BottomRight().x - 2 * Border;
  Height1 = DrawWindow.BottomRight().y - 2 * Border;

  GetDlgItem(IDC_Picture2)->GetClientRect(&DrawWindow);
  Border = 4;
  Width2 = DrawWindow.BottomRight().x - 2 * Border;
  Height2 = DrawWindow.BottomRight().y - 2 * Border;

  IconSize = 5;
} // Initialize

void CVisualSleepDlg::DrawSensor(CClientDC* PicWindow, double XC, double YC, int R, int G, int B, int Width, int Height)
{
  int i = 0;
  int j = 0;
  int k = 0;
  int X0 = int (Width * XC) + Border;
  int Y0 = int (Height * YC) + Border;
  int X = X0;
  int Y = Y0;
  
  for (i = -IconSize; i <= IconSize; ++i)
  {
    if (i < 0) j = -i; else j = i;
    for (k = -IconSize + j; k <= IconSize - j; ++k)
    {
      int X = X0 + i;
      int Y = Y0 + k;
      if ((Y == Y0 + IconSize - j) || (Y == Y0 + j - IconSize))
			{
				if ((X >= Border) && (X <= Width + Border) && (Y >= Border) && (Y <= Height + Border)) PicWindow->SetPixel(X, Y, RGB(0, 0, 0));
			}
			else
			{
				if ((X >= Border) && (X <= Width + Border) && (Y >= Border) && (Y <= Height + Border)) PicWindow->SetPixel(X, Y, RGB(R, G, B));
			}
    }
  }
}



void CVisualSleepDlg::DrawConsumer(CClientDC* PicWindow, double XC, double YC, int R, int G, int B, int Width, int Height)
{
  int i = 0;
  int j = 0;
  int k = 0;
  int X0 = int (Width * XC) + Border;
  int Y0 = int (Height * YC) + Border;
  int X = X0;
  int Y = Y0;
  
  for (i = -IconSize; i <= IconSize; ++i)
  {
    for (j = -IconSize; j <= IconSize; ++j)
    {
      int X = X0 + i;
      int Y = Y0 + j;
      if ((X == X0 + IconSize) || (X == X0 - IconSize) || (Y == Y0 + IconSize) || (Y == Y0 - IconSize))
			{
				if ((X >= Border) && (X <= Width + Border) && (Y >= Border) && (Y <= Height + Border)) PicWindow->SetPixel(X, Y, RGB(0, 0, 0));
			}
			else
			{
				if ((X >= Border) && (X <= Width + Border) && (Y >= Border) && (Y <= Height + Border)) PicWindow->SetPixel(X, Y, RGB(R, G, B));
			}
    }
  }
}

void CVisualSleepDlg::DrawProducer(CClientDC* PicWindow, double XC, double YC, int R, int G, int B, int Width, int Height)
{
  int i = 0;
  int j = 0;
  int k = 0;
  int X0 = int (Width * XC) + Border;
  int Y0 = int (Height * YC) + Border;
  int X = X0;
  int Y = Y0;
  
  for (i = -IconSize; i <= IconSize; ++i)
  {
    if (i < 0) k = -i; else k = i;
    for (j = -k; j <= k; ++j)
    {
      int X = X0 + i;
      int Y = Y0 + j;
			if ((X == X0 + IconSize) || (X == X0 - IconSize) || (i == j) || (i == -j))
			{
				if ((X >= Border) && (X <= Width + Border) && (Y >= Border) && (Y <= Height + Border)) PicWindow->SetPixel(X, Y, RGB(0, 0, 0));
			}
			else
			{
				if ((X >= Border) && (X <= Width + Border) && (Y >= Border) && (Y <= Height + Border)) PicWindow->SetPixel(X, Y, RGB(R, G, B));
			}
    }
  }
}

void CVisualSleepDlg::OnOK() 
{
  Calculated = 0;
  UpdateData();
  S1.InitializeSleep(Mode, NS, Rad, NC);
  Draw(&S1, PicWindow1, Width1, Height1);
  Copy();
  Draw(&S2, PicWindow2, Width2, Height2);
}

void CVisualSleepDlg::OnCancel() 
{
  ofstream OutputFile;

	// TODO: Add extra cleanup here
  if (!Calculated)
  {
    Calculated = 1;
		/*
		for (NC = 20; NC <= 30; NC += 5)
		{
			for (NS = 100; NS <= 1000; NS += 100)
			{
				for (Rad = 0.1; Rad <= 0.3; Rad += 0.05)
				{ */
					S1.InitializeSleep(Mode, NS, Rad, NC);
					Copy();
					for (S1.OnIndex = 0; S1.OnIndex < S1.NumSensors; ++S1.OnIndex)
					{
						S1.SensorsOn[S1.OnIndex] = S1.OnIndex;
						S1.SensorArray[S1.OnIndex].SubType = SENSOR_MSTTYPE_NONE;
						S1.SensorArray[S1.OnIndex].Mode = SENSOR_MODE_ON;
					}
					S1.CalculateSleepNodes();
					S1.CalculateMST();
					S1.PruneMST();
					Draw(&S1, PicWindow1, Width1, Height1);
					S1.CalculateCommunications();
					S1.CalcNumNodesOn();
					S1.CalcPathStats();
					NS1 = S1.OnIndex;
					NC1 = S1.NumComs;
					Min1 = S1.MinSavings;
					Max1 = S1.MaxSavings;
					Avg1 = S1.AvgSavings;
					UpdateData(FALSE);
					for (S2.OnIndex = 0; S2.OnIndex < S2.NumSensors; ++S2.OnIndex)
					{
						S2.SensorsOn[S2.OnIndex] = S2.OnIndex;
						S2.SensorArray[S2.OnIndex].SubType = SENSOR_MSTTYPE_NONE;
						S2.SensorArray[S2.OnIndex].Mode = SENSOR_MODE_ON;
					}
					S2.CalculateMST();
					S2.PruneMST();
					Draw(&S2, PicWindow2, Width2, Height2);
					S2.CalculateCommunications();
					S2.CalcNumNodesOn();
					S2.CalcPathStats();
					Min2 = S2.MinSavings;
					Max2 = S2.MaxSavings;
					Avg2 = S2.AvgSavings;
					NS2 = S2.OnIndex;
					NC2 = S2.NumComs;
					UpdateData(FALSE);

					OutputFile.open("Results.txt", ios::app);
					OutputFile << NS  << ", " << NC  << ", " << Rad  << ", "
										 << NS1 << ", " << NC1 << ", " << Min1 << ", " << Max1 << ", " << Avg1 << ", "
										 << NS2 << ", " << NC2 << ", " << Min2 << ", " << Max2 << ", " << Avg2 << endl;
					OutputFile.close();
				/*}
			}
		} */
  }
}


void CVisualSleepDlg::Draw(SleepNetwork* S, CClientDC* PicWindow, int Width, int Height)
{
  int i = 0; // Index Variable
  int j = 0; // Index Variable
  int C = 0; // Sensor Number
  int X = 0;
  int Y = 0;
	int OldIconSize = 0;
  double k = 0; // Temp Variable
  CBrush BkgBrush(RGB(255, 255, 255)); // Black Brush for Window
  CPen PenLightGray(PS_SOLID, 3, RGB(0, 0, 0)); // Gray Pen
  CPen PenDarkGray(PS_SOLID, 1, RGB(128, 128, 128));       // Gray Pen
  CPen PenBlack(PS_SOLID, 1, RGB(255, 255, 255));           // Gray Pen

  // Clear the Screen
  PicWindow->SelectObject(&BkgBrush);  
  PicWindow->Rectangle(0, 0, Width + 2 * Border, Height + 2 * Border);
  PicWindow->SelectObject(&PenBlack);

  // Draw NonCommunication Edges
  PicWindow->SelectObject(&PenDarkGray);
  for (i = 0; i < S->NumSensors; ++i)
  {
    j = 0;
    C = *(S->EdgeTable + j + i * S->NumSensors);
    while (C >= 0)
    {
      if (!(S->SensorArray[i].Mode == SENSOR_MODE_ON) || !(S->SensorArray[C].Mode == SENSOR_MODE_ON))
      {
        // Draw the Edge
        X = int (S->SensorArray[i].X * Width) + Border;
        Y = int (S->SensorArray[i].Y * Height) + Border;
        PicWindow->MoveTo(X, Y);
        X = int (S->SensorArray[C].X * Width) + Border;
        Y = int (S->SensorArray[C].Y * Height) + Border;
        PicWindow->LineTo(X, Y);
      }
      ++j;
      C = *(S->EdgeTable + j + i * S->NumSensors);
    }
  }

  // Draw Sensors
  k = 0.75;
  for (i = 0; i < S->NumSensors; ++i)
  {
    if (S->SensorArray[i].Mode != SENSOR_MODE_ON)
    {
      switch(S->SensorArray[i].Type)
      {
        case SENSOR_TYPE_PRODUCER:
          DrawProducer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255), int(255), int(0), Width, Height);
					OldIconSize = IconSize;
					IconSize = 8;
					DrawProducer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255 * k), int(255 * k), int(255 * k), Width, Height);
					IconSize = OldIconSize;
        break;
        case SENSOR_TYPE_CONSUMER:
          DrawConsumer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255), int(0), int (0), Width, Height);
					OldIconSize = IconSize;
					IconSize = 5;
					DrawConsumer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255 * k), int(255 * k), int (255 * k), Width, Height);
					IconSize = OldIconSize;
        break;
        case SENSOR_TYPE_LISTENER:
          DrawSensor(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(0), int(0), int(128), Width, Height);
					OldIconSize = IconSize;
					IconSize = 5;
					DrawSensor(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255 * k), int(255 * k), int(255 * k), Width, Height);
					IconSize = OldIconSize;
        break;
        default:
          DrawSensor(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, 0, 0, 192, Width, Height);
					DrawSensor(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, 255, 255, 255, Width, Height);
        break;
      }
    }
  }

  // Draw Communication Edges
  PicWindow->SelectObject(&PenLightGray);
  for (i = 0; i < S->NumSensors; ++i)
  {
    j = 0;
    C = *(S->EdgeTable + j + i * S->NumSensors);
    while (C >= 0)
    {
      if ((S->SensorArray[i].Mode == SENSOR_MODE_ON) && (S->SensorArray[C].Mode == SENSOR_MODE_ON))
      {
        // Draw the Edge
        X = int (S->SensorArray[i].X * Width) + Border;
        Y = int (S->SensorArray[i].Y * Height) + Border;
        PicWindow->MoveTo(X, Y);
        X = int (S->SensorArray[C].X * Width) + Border;
        Y = int (S->SensorArray[C].Y * Height) + Border;
        PicWindow->LineTo(X, Y);
      }
      ++j;
      C = *(S->EdgeTable + j + i * S->NumSensors);
    }
  }
  // Draw Sensors
  for (i = 0; i < S->NumSensors; ++i)
  {
    k = 0.375;
    if (S->SensorArray[i].Mode == SENSOR_MODE_ON)
    {
      switch(S->SensorArray[i].Type)
      {
        case SENSOR_TYPE_PRODUCER:
					DrawProducer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255), int(255), int(0), Width, Height);
					OldIconSize = IconSize;
					IconSize = 10;
					DrawProducer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255 * k), int(255 * k), int(255 * k), Width, Height);
					DrawProducer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255), int(255), int(255), Width, Height);
					DrawProducer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(0), int(0), int(0), Width, Height);
					IconSize = OldIconSize;
        break;
        case SENSOR_TYPE_CONSUMER:
					DrawConsumer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255), int(0), int (0), Width, Height);
					OldIconSize = IconSize;
					IconSize = 5;
					DrawConsumer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255 * k), int(255 * k), int (255 * k), Width, Height);
					DrawConsumer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(0), int(0), int (0), Width, Height);
					IconSize = OldIconSize;
        break;
        case SENSOR_TYPE_LISTENER:
					DrawSensor(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255), int(255), int(255), Width, Height);
					OldIconSize = IconSize;
					IconSize = 6;
					DrawSensor(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(224 * k), int(224 * k), int(224 * k), Width, Height);
					DrawSensor(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(0), int(0), int(0), Width, Height);
					IconSize = OldIconSize;
        break;
        default:
          DrawSensor(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, 0, 0, 192, Width, Height);
					DrawSensor(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, 0, 0, 0, Width, Height);
        break;
      }
    }
  }
	/*
	for (i = 0; i < S->NumSensors; ++i)
  {
    k = 0.75;
    if ((S->SensorArray[i].Mode == SENSOR_MODE_ON) && (S->SensorArray[i].Type == SENSOR_TYPE_PRODUCER))
    {
      DrawProducer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255), int(255), int(0), Width, Height);
			DrawProducer(PicWindow, S->SensorArray[i].X, S->SensorArray[i].Y, int(255 * k), int(255 * k), int(255 * k), Width, Height);
		}
	}
	*/
}

void CVisualSleepDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
  CDialog::OnOK();
  if (PicWindow1 != NULL) delete PicWindow1;
  if (PicWindow2 != NULL) delete PicWindow2;
}

void CVisualSleepDlg::OnRefresh() 
{
	// TODO: Add your control notification handler code here
	Draw(&S1, PicWindow1, Width1, Height1);
  Draw(&S2, PicWindow2, Width2, Height2);
}

void CVisualSleepDlg::Copy(void)
{ // Copy
  int i = 0;
  int j = 0;

  S2.Delete();
  S2.InitializeSensorNetwork(S1.NumSensors, S1.Radius);

  if (S2.SensorsOn != NULL) delete[] S2.SensorsOn;
  S2.SensorsOn = new int[S2.NumSensors + 1];
  S2.OnIndex = S1.OnIndex;

  if (S2.ConsumerArray != NULL) delete[] S2.ConsumerArray;
  S2.NumConsumers = S1.NumConsumers;
  S2.ConsumerArray = new int[S2.NumConsumers];

  S2.SensorsOn[S2.NumSensors] = -1;
  for (i = 0; i < S2.NumSensors; ++i)
  {
    S2.SensorsOn[i] = S1.SensorsOn[i];
    S2.SensorArray[i].X = S1.SensorArray[i].X;
    S2.SensorArray[i].Y = S1.SensorArray[i].Y;
    S2.SensorArray[i].Z = S1.SensorArray[i].Z;
    S2.SensorArray[i].Type = S1.SensorArray[i].Type;
    S2.SensorArray[i].Mode = S1.SensorArray[i].Mode;
    S2.SensorArray[i].SubType = S1.SensorArray[i].SubType;
    S2.SensorArray[i].Parent = S1.SensorArray[i].Parent;

    for (j = 0; j < S2.NumSensors; ++j)
    {
      *(S2.EdgeTable + j + S2.NumSensors * i) = *(S1.EdgeTable + j + S2.NumSensors * i);
    }
  }
  for (i = 0; i < S2.NumConsumers; ++i)
  {
    S2.ConsumerArray[i] = S1.ConsumerArray[i];
  }
  S2.ProducerNum = S1.ProducerNum;
} // Copy

void CVisualSleepDlg::OnButseed() 
{
  UpdateData();
  srand(RandSeed);	
}
