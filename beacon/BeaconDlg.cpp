// BeaconDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Beacon.h"
#include "BeaconDlg.h"
#include "Sensor.cpp"
#include <math.h>
#include <fstream.h>

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
// CBeaconDlg dialog

CBeaconDlg::CBeaconDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBeaconDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBeaconDlg)
	NumSensors = 2500;
	NumSensorsH = 2500;
	NumSensorsL = 100;
	FileName = _T("Output.txt");
	PerSensors = 2.0;
	PerSensorsH = 2.0;
	PerSensorsL = 0.2;
	RanSensors = 0.2;
	RanSensorsH = 0.2;
	RanSensorsL = 0.02;
	NumTrials = 100;
	NumSteps = 10;
	BMaple = FALSE;
	BNS = TRUE;
	BFP = TRUE;
	BPS = TRUE;
	BRS = TRUE;
	InFile = _T("Input.txt");
	//}}AFX_DATA_INIT

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  InProgress = 0;
  PicWindow = NULL;
  Sensors = NULL;
  Edges = NULL;
}

void CBeaconDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBeaconDlg)
	DDX_Control(pDX, IDC_Ran, Ran);
	DDX_Control(pDX, IDC_Per, Per);
	DDX_Control(pDX, IDC_Num, Num);
	DDX_Control(pDX, IDC_Progress, ProgressBar);
	DDX_Control(pDX, IDC_Status, strStatus);
	DDX_Text(pDX, IDC_Num, NumSensors);
	DDX_Text(pDX, IDC_NumH, NumSensorsH);
	DDX_Text(pDX, IDC_NumL, NumSensorsL);
	DDX_Text(pDX, IDC_OutFile, FileName);
	DDX_Text(pDX, IDC_Per, PerSensors);
	DDX_Text(pDX, IDC_PerH, PerSensorsH);
	DDX_Text(pDX, IDC_PerL, PerSensorsL);
	DDX_Text(pDX, IDC_Ran, RanSensors);
	DDX_Text(pDX, IDC_RanH, RanSensorsH);
	DDX_Text(pDX, IDC_RanL, RanSensorsL);
	DDX_Text(pDX, IDC_Trials, NumTrials);
	DDX_Text(pDX, IDC_NumSteps, NumSteps);
	DDX_Check(pDX, IDC_chkMaple, BMaple);
	DDX_Check(pDX, IDC_chkNS, BNS);
	DDX_Check(pDX, IDC_chkFP, BFP);
	DDX_Check(pDX, IDC_chkPS, BPS);
	DDX_Check(pDX, IDC_chkRS, BRS);
	DDX_Text(pDX, IDC_InFile, InFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBeaconDlg, CDialog)
	//{{AFX_MSG_MAP(CBeaconDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Calc, OnCalc)
	ON_BN_CLICKED(IDC_Rand, OnRand)
	ON_BN_CLICKED(IDC_Step, OnStep)
	ON_BN_CLICKED(IDC_Input, OnInput)
	ON_BN_CLICKED(IDC_Refresh, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBeaconDlg message handlers

BOOL CBeaconDlg::OnInitDialog()
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

void CBeaconDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBeaconDlg::OnPaint() 
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
HCURSOR CBeaconDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBeaconDlg::Initialize()
{
  int Accuracy = 10000;          // Decimal Accuracy
  int Size = Accuracy / 500;     // Size of Sensor
  int StartingBeacons;           // Starting Number of Beacons
  int i = 0;                     // Index Variable
  int j = 0;                     // Index Variable
  CBrush BkgBrush(RGB(0, 0, 0)); // Black Brush for Window
  CRect DrawWindow;              // Coordinates of View Window

   // Initialize the Graphics Window ///////////////////////////////////////////
  PicWindow = new CClientDC(GetDlgItem(IDC_Picture));
  // Get the Width and Height of the Graphics Window
  GetDlgItem(IDC_Picture)->GetClientRect(&DrawWindow);
  Width = DrawWindow.BottomRight().x - 4;
  Height = DrawWindow.BottomRight().y - 4;
  // Clear the Screen
  PicWindow->SelectObject(&BkgBrush);
  PicWindow->Rectangle(0, 0, Width + 4, Height + 4);

  // Initialize the Function Call //////////////////////////////////////////////
  srand(time(NULL));
  // Set the Status Text to "Initializing..."
  strStatus.SetWindowText("Initializing...");
  // Put All New Data Into Their Variables
  UpdateData();
  // Set the Starting Number of Beacons
  StartingBeacons = int (NumSensors * (PerSensors / 100));
  // Allocate a New Sensor Array
  Sensors = new Sensor[NumSensors];
  // Initialize the Progress Bar
  ProgressBar.SetPos(0);
  ProgressBar.SetRange(0, 2 * NumSensors + StartingBeacons);
  ProgressBar.SetStep(1);

  // Initialize the Edge Array /////////////////////////////////////////////////
  // Set Window Text to "Initializing Edge Array..."
  strStatus.SetWindowText("Initializing Edges...");
  // Allocate a New Edge Two-Dimensional Array
  Edges = new char*[NumSensors];
  for (i = 0; i < NumSensors; ++i)
  {
    // Allocate the "Second" Dimension
    Edges[i] = new char[NumSensors];
  }
  // Set Each Edge to 0 (No Edge)
  for (i = 0; i < NumSensors; ++i)
  {
    for (j = 0; j < NumSensors; ++j)
    {
      // Clear the Edge
      Edges[i][j] = 0;
    }
    // Adjust the Progress Bar
    ProgressBar.StepIt();
  }

  // Initialize the Sensor Array ///////////////////////////////////////////////
  // Set Status to "Initializing Sensors..."
  strStatus.SetWindowText("Initializing Sensors...");
  // Clear the Number of Beacons
  InitBeacons = 0;
  // Initialize the Sensors to Random Positions
  for (i = 0; i < NumSensors; ++i)
  {
    // Set the Random Point
    int Temp = rand() % Accuracy;
    Sensors[i].x = double(rand() % Accuracy) / Accuracy; // Random Number 0 - 1
    Sensors[i].y = double(rand() % Accuracy) / Accuracy; // Random Number 0 - 1
    Sensors[i].Beacon = 0; // Not a Beacon
    // Update the Progress Bar
    ProgressBar.StepIt();
  }
  // Set the Number of Times Through the Loop
  i = 1;
  while(InitBeacons < StartingBeacons)
  {
    for (j = 0; (j < NumSensors) && (InitBeacons < StartingBeacons); ++j)
    {
      // If the Sensor is Randomly Picked to be a Beacon
      if ((((rand() % Accuracy) / Accuracy) < double ((PerSensors / 100) * i)) && (Sensors[j].Beacon != 1))
      {
        // Set a New Beacon
        Sensors[j].Beacon = 1;
        // Adjust the Progress Bar
        ProgressBar.StepIt();
        // Increase the Number of Beacons
        ++InitBeacons;
      }
    }
    // Increase the Number of Times Through the Loop
    ++i;
  }
  // Clear the Progress Bar
  ProgressBar.SetPos(0);
}

void CBeaconDlg::DoCalculations()
{
  if (InProgress >= 1)
  {
    while (InProgress >= 1);
  }
  else
  {
    ClearVariables();
    InProgress = 1;
    Initialize();
    CalcEdges();
    DrawInitialSensors();
    TotalBeacons = InitBeacons;
    NewBeacons = CalcNewBeacons();
    while (NewBeacons > 0)
    {
      TotalBeacons += NewBeacons;
      NewBeacons = CalcNewBeacons();
    }
    Output();
    InProgress = 0;
  }
}

void CBeaconDlg::ClearVariables()
{
  int i = 0; // Index Variables

  // Clear the Status
  strStatus.SetWindowText("Waiting...");
  ProgressBar.SetPos(0);

  // Delete the Picture Window
  if (PicWindow != NULL)
  {
    delete PicWindow;
    PicWindow = NULL;
  }
  
  // Delete the Sensor Array
  if (Sensors != NULL)
  {
    delete[] Sensors;
    Sensors = NULL;
  }
  
  // Delete the Edge Array
  if (Edges != NULL)
  {
    for (i = 0; i < NumSensors; ++i)
    {
      // Delete Each Individual Array
      delete[] Edges[i];
    }
    // Delete the Entire Edge Array
    delete[] Edges;
    Edges = NULL;
  }
}

void CBeaconDlg::OnCalc() 
{
  DoCalculations();	
  strStatus.SetWindowText("");
  UpdateWindow();
}

void CBeaconDlg::DrawSensor(double X, double Y, int Type)
{
  if (Type == 1)
  {
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 2, RGB(255, 0, 0));
    PicWindow->SetPixel(int (Width * X) + 3, int (Height * Y) + 2, RGB(255, 0, 0));
    PicWindow->SetPixel(int (Width * X) + 1, int (Height * Y) + 2, RGB(255, 0, 0));
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 3, RGB(255, 0, 0));
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 1, RGB(255, 0, 0));
  }
  else if (Type == 2)
  {
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 2, RGB(255, 255, 0));
    PicWindow->SetPixel(int (Width * X) + 3, int (Height * Y) + 2, RGB(255, 255, 0));
    PicWindow->SetPixel(int (Width * X) + 1, int (Height * Y) + 2, RGB(255, 255, 0));
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 3, RGB(255, 255, 0));
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 1, RGB(255, 255, 0));
  }
  else if (Type == 3)
  {
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 2, RGB(0, 192, 0));
    PicWindow->SetPixel(int (Width * X) + 3, int (Height * Y) + 2, RGB(0, 192, 0));
    PicWindow->SetPixel(int (Width * X) + 1, int (Height * Y) + 2, RGB(0, 192, 0));
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 3, RGB(0, 192, 0));
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 1, RGB(0, 192, 0));
  }
  else
  {
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 2, RGB(255, 0, 0));
    PicWindow->SetPixel(int (Width * X) + 3, int (Height * Y) + 2, RGB(255, 0, 0));
    PicWindow->SetPixel(int (Width * X) + 1, int (Height * Y) + 2, RGB(255, 0, 0));
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 3, RGB(255, 0, 0));
    PicWindow->SetPixel(int (Width * X) + 2, int (Height * Y) + 1, RGB(255, 0, 0));
  }
}

void CBeaconDlg::CalcEdges()
{
  int i = 0;                                 // Index Variable
  int j = 0;                                 // Index Variable

  // Initialize the Status Label
  strStatus.SetWindowText("Calculating Edges...");
  // Initialize the Progress Bar
  ProgressBar.SetRange(0, NumSensors);
  ProgressBar.SetPos(0);
  ProgressBar.SetStep(1);
  // Check Each Distance from the Sensors
  for (i = 0; i < NumSensors; ++i)
  {
    for (j = i + 1; j < NumSensors; ++j)
    {
      if (Distance(Sensors[i].x, Sensors[i].y, Sensors[j].x, Sensors[j].y) < RanSensors)
      {
        // Set an Edges Between the Two Sensors
        Edges[i][j] = 1;
        Edges[j][i] = 1;
      }
      
    }
    // Step the Progress Bar
    ProgressBar.StepIt();
  }
}

double CBeaconDlg::Distance(double X1, double Y1, double X2, double Y2)
{
  return (sqrt((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1)));
}

void CBeaconDlg::DrawInitialSensors()
{
  int i = 0;                                 // Index Variable
  int j = 0;                                 // Index Variable
  CPen PenBlue(PS_SOLID, 1, RGB(0, 0, 127)); // Blue Pen

  // Initialize the Progress ///////////////////////////////////////////////////
  ProgressBar.SetRange(0, NumSensors + NumSensors);
  ProgressBar.SetPos(0);
  ProgressBar.SetStep(1);
  // Set the Status Message
  strStatus.SetWindowText("Drawing Screen...");

  // Draw the Edges ////////////////////////////////////////////////////////////
  // Select the Blue Pen
  PicWindow->SelectObject(&PenBlue);
  // Check Each Pair of Sensors for an Edge
  for (i = 0; i < NumSensors; ++i)
  {
    for (j = i + 1; j < NumSensors; ++j)
    {
      // If there is an Edge Between the Two Sensors
      if (Edges[i][j] == 1)
      {
        // Draw the Edge
        PicWindow->MoveTo(int (Sensors[i].x * Width) + 2, int (Sensors[i].y * Height) + 2);
        PicWindow->LineTo(int (Sensors[j].x * Width) + 2, int (Sensors[j].y * Height) + 2);
      }
    }
    ProgressBar.StepIt();
  }

  // Draw the Sensors //////////////////////////////////////////////////////////
  for (i = 0; i < NumSensors; ++i)
  {
    if (Sensors[i].Beacon == 1)
    {
      DrawSensor(Sensors[i].x, Sensors[i].y, 2);
    }
    else if (Sensors[i].Beacon > 1)
    {
      DrawSensor(Sensors[i].x, Sensors[i].y, 3);
    }
    else
    {
      DrawSensor(Sensors[i].x, Sensors[i].y, 1);
    }
    ProgressBar.StepIt();
  }
}

int CBeaconDlg::CalcNewBeacons()
{
  int i = 0;        // Index Variable
  int j = 0;        // Index Variable
  int Beacons = 0;  // Number of New Beacons
  int Adjacent = 0; // Beacons Adjacent to Current Sensors

  // Initialize the Progress ///////////////////////////////////////////////////
  // Set the Window Text
  strStatus.SetWindowText("Finding New Beacons...");
  // Set the PRogress Bar
  ProgressBar.SetRange(0, NumSensors);
  ProgressBar.SetPos(0);
  ProgressBar.SetStep(1);

  // Find New Beacons //////////////////////////////////////////////////////////
  for (i = 0; (i < NumSensors) && (TotalBeacons + Beacons < NumSensors); ++i)
  {
    // If the Current Sensor is Not a Beacon
    if (Sensors[i].Beacon == 0)
    {
      // Clear the Number of Adjacent Beacons
      Adjacent = 0;
      // Check Each Other Sensor Until 3 Beacons Found or End of List
      j = 0;
      while ((j < NumSensors) && (Adjacent < 3))
      {
        if ((Edges[i][j] > 0) && (Sensors[j].Beacon >= 1))
        {
          // Increment the Number of Adjacent Beacons
          ++Adjacent;
        }
        // Go to the Next Sensor
        ++j;
      }
      // Check to See if Enough Adjacent Beacons were Found
      if (Adjacent >= 3)
      {
        // Set the Current Sensor to a Beacon
        Sensors[i].Beacon = 2;
        // Increase the Number of New Beacons
        ++Beacons;
        // Draw the Current Sensor as a New Beacon
        DrawSensor(Sensors[i].x, Sensors[i].y, 3);
      }
    }
    ProgressBar.StepIt();
  }
  ProgressBar.SetPos(0);
  // Return the Number of New Beacons
  return (Beacons);
}

void CBeaconDlg::OnRand() 
{
  int Accuracy = 100;   // Accuracy of Random Number
  int i = 0;            // Index Variable

  // Update the Data to Get the Number of Trails
  UpdateData();
  // Do the Trials A Specified Number of Times
  for (i = 0; i < NumTrials; ++i)
  {
    // Pick a Random Number Within the Ranges
    NumSensors = int (NumSensorsL + (double (rand() % Accuracy) / Accuracy) * double (NumSensorsH - NumSensorsL));
    RanSensors = RanSensorsL + (double (rand() % Accuracy) / Accuracy) * (RanSensorsH - RanSensorsL);
    PerSensors = PerSensorsL + (double (rand() % Accuracy) / Accuracy) * (PerSensorsH - PerSensorsL);
    // Update the Data
    UpdateData(FALSE);
    UpdateWindow();
    DoCalculations();
    UpdateData();
  }
}

void CBeaconDlg::OnStep() 
{
	double NSStep = 0; // Step Size for Number of Sensors
  double PSStep = 0; // Step Size for Percent of Beacons
  double RSStep = 0; // Step Size for Range of Sensors
  double i = 0;      // Index Variable
  double j = 0;      // Index Variable
  double k = 0;      // Index Variable
  ofstream TempFile; // Temporary Output File if in Maple V Mode

  // Update the Data to Get the Appropriate Numbers
  UpdateData();
  // Calculate the Step Values
  NSStep = (double (NumSensorsH) - double (NumSensorsL)) / double (NumSteps);
  PSStep = (double (PerSensorsH) - double (PerSensorsL)) / double (NumSteps);
  RSStep = (double (RanSensorsH) - double (RanSensorsL)) / double (NumSteps);
	

  // For the Number of Sensors
  for (i = NumSensorsL; i <= NumSensorsH; i += NSStep)
  {
    if (BMaple == TRUE)
    {
      // Open the File
      TempFile.open(FileName, ios::app);
      // Output the List Separator
      TempFile << "[";
      // Close the File
      TempFile.close();
    }
    // For the Range of the Sensors
    for (j = RanSensorsL; j <= RanSensorsH; j += RSStep)
    {
      if (BMaple == TRUE)
      {
        // Open the File
        TempFile.open(FileName, ios::app);
        // Output the List Separator
        TempFile << "[";
        // Close the File
        TempFile.close();
      }
      // For the Percent of Beacons
      for (k = PerSensorsL; k <= PerSensorsH; k += PSStep)
      {
            // Update the Screen Values
            NumSensors = int (i);
            RanSensors = j;
            PerSensors = k;
            // Update the Data
            UpdateData(FALSE);
            UpdateWindow();
            DoCalculations();
            UpdateData();
            if (PSStep == 0)
            {
              k = PerSensorsH + 1;
            }
            if (((k + PSStep) <= PerSensorsH) && (BMaple == TRUE))
            {
              // Separate Values with a Comma if Not Last Value
              TempFile.open(FileName, ios::app);
              TempFile << ",";
              TempFile.close();
            }
      }
      if (RSStep == 0)
      {
        j = RanSensorsH + 1;
      }
      if (BMaple == TRUE)
      {
        // Open the File
        TempFile.open(FileName, ios::app);
        // Output the List Separator
        TempFile << "]";
        if (((j + RSStep) <= RanSensorsH) && (BMaple == TRUE))
        {
          // Output a Comma if Not at the End of the List
          TempFile << ",";
        }
        // Close the File
        TempFile.close();
      }
    }
    if (NSStep == 0)
    {
      i = NumSensorsH + 1;
    }
    if (BMaple == TRUE)
    {
      // Open the File
      TempFile.open(FileName, ios::app);
      // Output the List Separator
      TempFile << "]" << endl;
      // Close the File
      TempFile.close();
    }
  }
}

void CBeaconDlg::Output()
{
  fstream OutFile;            // Output File
  double InitPerBeacons = 0;  // Initial Percent of Beacons
  double TotalPerBeacons = 0; // TOtal Percent of Beacons

  InitPerBeacons = double (InitBeacons) / double (NumSensors) * 100.0;
  TotalPerBeacons = double (TotalBeacons) / double (NumSensors) * 100.0;

  // Open the Output File for Appending
  OutFile.open(FileName, ios::app);
  // Output the Appropriate Characters for the Appropriate Mode
  if (BMaple == TRUE)
  {
    // Output the Opening Brace
    OutFile << "[";
  }

  if (BNS == TRUE)
  {
    OutFile << NumSensors;
    if ((BRS == TRUE) || (BPS == TRUE) || (BFP == TRUE))
    {
      if (BMaple == TRUE)
      {
        OutFile << ", ";
      }
      else
      {
        OutFile << "   ";
      }
    }
  }

  if (BRS == TRUE)
  {
    OutFile << RanSensors;
    if ((BPS == TRUE) || (BFP == TRUE))
    {
      if (BMaple == TRUE)
      {
        OutFile << ", ";
      }
      else
      {
        OutFile << "   ";
      }
    }
  }

  if (BPS == TRUE)
  {
    OutFile << PerSensors;
    if (BFP == TRUE)
    {
      if (BMaple == TRUE)
      {
        OutFile << ", ";
      }
      else
      {
        OutFile << "   ";
      }
    }
  }
  if (BFP == TRUE)
  {
    OutFile << TotalPerBeacons;
  }
  if (BMaple == TRUE)
  {
    OutFile << "]";
  }
  else
  {
    OutFile << endl;
  }
  OutFile.close();
}

void CBeaconDlg::OnInput() 
{
  int i = 0;                     // Index Variable
  int j = 0;                     // Index Variable
  CBrush BkgBrush(RGB(0, 0, 0)); // Black Brush for Window
  CRect DrawWindow;              // Coordinates of View Window

  ClearVariables();
  // Initialize the Graphics Window
  PicWindow = new CClientDC(GetDlgItem(IDC_Picture));
  // Get the Width and Height of the Graphics Window
  GetDlgItem(IDC_Picture)->GetClientRect(&DrawWindow);
  Width = DrawWindow.BottomRight().x - 4;
  Height = DrawWindow.BottomRight().y - 4;
  // Clear the Screen
  PicWindow->SelectObject(&BkgBrush);
  PicWindow->Rectangle(0, 0, Width + 4, Height + 4);

  // Initialize the Function Call
  UpdateData();

  if (InputSensors(InFile.GetBuffer(0), NumSensors, &Sensors))
  {
    UpdateData(FALSE);
    // Clear the Initial Number of Beacons
    InitBeacons = 0;
    // Calculate the Number of Beacons
    for (i = 0; i < NumSensors; ++i)
    {
      if (Sensors[i].Beacon != 0)
      {
        ++InitBeacons;
      }
    }
    InProgress = 1;
    // Initialize the Edge Array
    // Set Window Text to "Initializing Edge Array..."
    strStatus.SetWindowText("Inputting File...");
    // Allocate a New Edge Two-Dimensional Array
    Edges = new char*[NumSensors];
    for (i = 0; i < NumSensors; ++i)
    {
      // Allocate the "Second" Dimension
      Edges[i] = new char[NumSensors];
    }
    // Set Each Edge to 0 (No Edge)
    for (i = 0; i < NumSensors; ++i)
    {
      for (j = 0; j < NumSensors; ++j)
      {
        // Clear the Edge
        Edges[i][j] = 0;
      }
    }
    CalcEdges();
    DrawInitialSensors();
    TotalBeacons = InitBeacons;
    NewBeacons = CalcNewBeacons();
    while (NewBeacons > 0)
    {
      TotalBeacons += NewBeacons;
      NewBeacons = CalcNewBeacons();
    }
    Output();
    InProgress = 0;
  }
  else
  {
    MessageBox("Input File Not Found.", "File Not Found");
  }
  strStatus.SetWindowText("");
  UpdateWindow();
}

void CBeaconDlg::OnRefresh() 
{
  CBrush BkgBrush(RGB(0, 0, 0)); // Black Brush for Window
  CPen Bkg(PS_SOLID, 1, RGB(0, 0, 0)); // Black Pen

  if ((Sensors != NULL) && (Edges != NULL) && (PicWindow != NULL))
  {
    PicWindow->SelectObject(&Bkg);
    PicWindow->SelectObject(&BkgBrush);
    PicWindow->Rectangle(0, 0, Width + 4, Height + 4);
	  DrawInitialSensors();
    ProgressBar.SetPos(0);
  }
  else
  {
    MessageBox("No Sensor Network Available!  Please Make One By Clicking on Calculate.", "No Sensor Network Available.");
  }
  strStatus.SetWindowText("");
  UpdateWindow();
}
