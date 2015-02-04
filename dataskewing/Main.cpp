// Main.cpp : Defines the class behaviors for the application.
//

#include <stdlib.h>
#include <time.h>
#include "stdafx.h"
#include "Main.h"

#include "MainFrm.h"
#include "MainDoc.h"
#include "CTrilaterationLiarSystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainApp

BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
	//{{AFX_MSG_MAP(CMainApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_FILE_RUN, OnFileRun)
	ON_COMMAND(ID_FILE_CONTINUE, OnFileContinue)
	ON_COMMAND(ID_FILE_NEW2, OnFileNew2)
	ON_COMMAND(ID_FILE_GENERATE, OnFileGenerate)
	ON_COMMAND(ID_FILE_TRIANGLE, OnFileTriangle)
	ON_COMMAND(ID_FILE_GENERATE_4, OnFileGenerate4)
	ON_COMMAND(ID_FILE_GENERATE_5, OnFileGenerate5)
	ON_COMMAND(ID_FILE_GENERATE_MC, OnFileGenerateMc)
	ON_COMMAND(ID_FILE_GENERATE_USECORRECT, OnFileGenerateUsecorrect)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainApp construction

CMainApp::CMainApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
  ResetTrilaterationSystem = 1;
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CMainApp object

CMainApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainApp initialization

BOOL CMainApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMainDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CTrilaterationLiarSystemView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

  
  srand(time(NULL));
  pView = (CTrilaterationLiarSystemView*) ((CFrameWnd*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
  pDoc = (CMainDoc*) ((CFrameWnd*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
  SetWindowPos(pView->m_hWnd, NULL, 0, 0, 550, 500, 0);
	return TRUE;
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMainApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CMainApp::OnAppExit() 
{
  pDoc->Deinit();
  pView = NULL;
  pDoc = NULL;

  // This Is How You Quit
  PostMessage(GetParent(NULL), WM_QUIT, 0, 0);
}

void CMainApp::OnFileRun() 
{
  CDC* pDC = pView->GetActiveWindow()->GetDC();

  // Store The Trilateration System From The Document Into The Viewer
  if (ResetTrilaterationSystem)
  {
    pView->TSSet(pDoc->TLSGet());
    pView->ResetTrilaterationSystem();
    pView->DisplayUpdates(1);
    ResetTrilaterationSystem = 0;
  }

  while(!pDoc->TLSFinished())
  {
    pView->Update();
    pView->UpdateTime();
    pView->OnDraw(pDC);
  }

  ResetTrilaterationSystem = 1;
}

void CMainApp::OnFileContinue() 
{
  CDC* pDC = pView->GetActiveWindow()->GetDC();
	
	// Store The Trilateration System From The Document Into The Viewer
	if (ResetTrilaterationSystem)
  {
    pView->TSSet(pDoc->TLSGet());
    pView->ResetTrilaterationSystem();
    pView->DisplayUpdates(1);
    ResetTrilaterationSystem = 0;
  }

  pView->InitializeTime();
  pView->Update();
  pView->OnDraw(pDC);
  while(!pDoc->TLSBreak())
  {
    pView->Update();
    pView->UpdateTime();
    pView->OnDraw(pDC);
  }

  if (pDoc->TLSFinished())
  {
    ResetTrilaterationSystem = 1;
  }
}

void CMainApp::OnFileNew2() 
{
	// TODO: Add your command handler code here
  CDC* pDC = pView->GetActiveWindow()->GetDC();
	pDoc->OnNew();
  pView->TSSet(pDoc->TLSGet());
  pView->OnDraw(pDC);
  ResetTrilaterationSystem = 1;
  // PostMessage(GetParent(NULL), WM_PAINT, 0, 0);
}

void CMainApp::OnFileGenerate() 
{
  int i = 0;
  CDC* pDC = pView->GetActiveWindow()->GetDC();

  // Clear The Screen
  pView->TSSet(NULL);
  pView->OnDraw(pDC);
  pDoc->SetNumSensors(3);
  OutputHeader("Results_Generated_3.txt");
  for (i = 0; i < pDoc->GetNumSystemIterations(); ++i)
  {
    pDoc->OnNew();
    pDoc->InitializeTrilaterationSystemRandom();
    pView->TSSet(pDoc->TLSGet());
    pView->DisplayUpdates(0);
    pView->ResetTrilaterationSystem();
    while(!pDoc->TLSFinished())
    {
      pView->Update();
      pView->UpdateTime();
    }
    pView->OnDraw(pDC);
    OutputData("Results_Generated_3.txt");
  }
  ResetTrilaterationSystem = 1;
}

void CMainApp::OnFileTriangle() 
{
  int i = 0;
  int j = 0;
	int MaxRuns = 10000;
  int MaxDegrees = 180;
  char OutputFile[] = "Results_TriangleData.txt";
  double t = 0.0;
  double u = 0.0;
  double v = 0.0;
  double Ratio = 0.0;
  double Radius = 0.5;
  double Pi = 3.14159;
  double X[4] = { 0.0 };
  double Y[4] = { 0.0 };
  ofstream OutFile;
  CDC* pDC = pView->GetActiveWindow()->GetDC();

  pView->TSSet(pDoc->TLSGet());
  pView->DisplayUpdates(0);
  srand(time(NULL));
  // Set Up Initial Staionary Points
  X[0] = 0.0;
  Y[0] = 0.0;
  X[1] = Radius;
  Y[1] = 0.0;
  // Open The Output File
  OutFile.open(OutputFile, ios::out);
  OutFile << "Degree "
          << "Actual_X "
          << "Actual_Y "
          << "Measured_X "
          << "Measured_Y "
          << "Measured_Error "
          << "Lied_X "
          << "Lied_Y "
          << "Lied_Error "
          << "Lied_Measured_Ratio "
          << "Sensor_0_X "
          << "Sensor_0_Y "
          << "Sensor_0_Actual_Radius "
          << "Sensor_0_Measured_Radius "
          << "Sensor_1_X "
          << "Sensor_1_Y "
          << "Sensor_1_Actual_Radius "
          << "Sensor_1_Measured_Radius "
          << "Sensor_2_X "
          << "Sensor_2_Y "
          << "Sensor_2_Actual_Radius "
          << "Sensor_2_Measured_Radius "
          << endl;
  // Loop Through Triangle Rungs
  for (i = 1; i < MaxDegrees; ++i)
  {
    for (j = 0; j < MaxRuns; ++j)
    {
      // Initialize The Trilateration System
      pView->InitTrilaterationSystem();
      // Calculate Radial Point
      X[2] = Radius * cos(i * Pi / 180.0);
      Y[2] = Radius * sin(i * Pi / 180.0);
      // Generate Random Numbers Such That t, u, v positive and t + u + v = 1
      t = GenerateRandomDouble(0.0, 1.0);
      u = GenerateRandomDouble(0.0, 1.0 - t);
      v = 1.0 - t - u;
      // Generate Actual Point Based Off Of The Random Number
      X[3] = t * X[0] + u * X[1] + v * X[2];
      Y[3] = t * Y[0] + u * Y[1] + v * Y[2];
      // Initialize Points In Trilateration System
      pView->SensorInit(0, X[0], Y[0], 0.0);
      pView->SensorInit(1, X[1], Y[1], 0.0);
      pView->SensorInit(2, X[2], Y[2], 0.0);
      pView->ActualPointInit(X[3], Y[3], 0.0);
      // Calculate Trilateration System Measurements
      pView->SetTrilaterationSystemMeasurements();
      // Reset The Trilateration System For Running
      pView->TSSet(pDoc->TLSGet());
      pView->ResetTrilaterationSystem();
      // Run The Trilateration System
      while(!pDoc->TLSFinished())
      {
        pView->Update();
        pView->UpdateTime();
      }
      pView->OnDraw(pDC);
      // Calculate Final Ratio
      Ratio = pView->GetTrilaterationSystemLiarResultError() / pView->GetTrilaterationSystemMeasuredError();
      // Output The Results
      OutFile << i <<                                                    "     "
              <<                                                         "     "
              << pView->GetTrilaterationSystemActualX()               << "     "
              << pView->GetTrilaterationSystemActualY()               << "     "
              <<                                                         "     "
              << pView->GetTrilaterationSystemMeasuredX()             << "     "
              << pView->GetTrilaterationSystemMeasuredY()             << "     "
              << pView->GetTrilaterationSystemMeasuredError()         << "     "
              <<                                                         "     "
              << pView->GetTrilaterationSystemLiarResultX()           << "     "
              << pView->GetTrilaterationSystemLiarResultY()           << "     "
              << pView->GetTrilaterationSystemLiarResultError()       << "     "
              <<                                                         "     "
              << Ratio                                                << "     "
              <<                                                         "     "
              << pView->GetTrilaterationSystemSensorX(0)              << "     "
              << pView->GetTrilaterationSystemSensorY(0)              << "     "
              << pView->GetTrilaterationSystemSensorActualRadius(0)   << "     "
              << pView->GetTrilaterationSystemSensorMeasuredRadius(0) << "     "
              <<                                                         "     "
              << pView->GetTrilaterationSystemSensorX(1)              << "     "
              << pView->GetTrilaterationSystemSensorY(1)              << "     "
              << pView->GetTrilaterationSystemSensorActualRadius(1)   << "     "
              << pView->GetTrilaterationSystemSensorMeasuredRadius(1) << "     "
              <<                                                         "     "
              << pView->GetTrilaterationSystemSensorX(2)              << "     "
              << pView->GetTrilaterationSystemSensorY(2)              << "     "
              << pView->GetTrilaterationSystemSensorActualRadius(2)   << "     "
              << pView->GetTrilaterationSystemSensorMeasuredRadius(2) << "     "
              <<                                                         "     "
              << endl;
    }
  }
  // Close Output File
  OutFile.close();
  ResetTrilaterationSystem = 1;
}

double CMainApp::GenerateRandomDouble(double LowVal, double HighVal)
{ // GenerateRandomDouble
  int MaxAccuracy = 10000;
  double Percentage = ((double) (rand() % (MaxAccuracy + 1))) / ((double) MaxAccuracy);
  return(LowVal + Percentage * (HighVal - LowVal));
} // GenerateRandomDouble

void CMainApp::OnFileGenerate4() 
{
	int i = 0;
  int j = 0;
  CDC* pDC = pView->GetActiveWindow()->GetDC();

  // Clear The Screen
  pView->TSSet(NULL);
  pView->OnDraw(pDC);
  pDoc->SetNumSensors(4);
  // pDoc->SetNumSystemIterations(1);
  OutputHeader("Results_Generated_4.txt");
  for (i = 0; i < pDoc->GetNumSystemIterations(); ++i)
  {
    pDoc->OnNew();
    pDoc->InitializeTrilaterationSystemRandom();
    pView->TSSet(pDoc->TLSGet());
    pView->DisplayUpdates(0);
    pView->ResetTrilaterationSystem();
    while(!pDoc->TLSFinished())
    {
      pView->Update();
      pView->UpdateTime();
    }
    pView->OnDraw(pDC);
    OutputData("Results_Generated_4.txt");
  }
  ResetTrilaterationSystem = 1;
}

void CMainApp::OnFileGenerate5() 
{
	// TODO: Add your command handler code here
	int i = 0;
  int j = 0;
  CDC* pDC = pView->GetActiveWindow()->GetDC();

  // Clear The Screen
  pView->TSSet(NULL);
  pView->OnDraw(pDC);
  pDoc->SetNumSensors(5);
  // pDoc->SetNumSystemIterations(1);
  OutputHeader("Results_Generated_5.txt");
  for (i = 0; i < pDoc->GetNumSystemIterations(); ++i)
  {
    pDoc->OnNew();
    pDoc->InitializeTrilaterationSystemRandom();
    pView->TSSet(pDoc->TLSGet());
    pView->DisplayUpdates(0);
    pView->ResetTrilaterationSystem();
    while(!pDoc->TLSFinished())
    {
      pView->Update();
      pView->UpdateTime();
    }
    pView->OnDraw(pDC);
    OutputData("Results_Generated_5.txt");
  }
  ResetTrilaterationSystem = 1;
}


void CMainApp::OutputHeader(char* FileName)
{ // OutputHeader
  int i = 0;
  ofstream OutFile;

  OutFile.open(FileName, ios::out);
  OutFile   << "Num_Sensors "
            << "Actual_X "
            << "Actual_Y "
            << "Measured_X "
            << "Measured_Y "
            << "Measured_Error "
            << "Lied_X "
            << "Lied_Y "
            << "Lied_Error "
            << "Lied_Measured_Ratio ";
  for (i = 0; i < pDoc->GetNumSensors(); ++i)
  {
    OutFile << "Sensor_" << i << "_X "
            << "Sensor_" << i << "_Y "
            << "Sensor_" << i << "_Actual_Radius "
            << "Sensor_" << i << "_Measured_Radius ";
  }
  OutFile   << endl;
  OutFile.flush();
  Delay(100);
  OutFile.close();
} // OutputHeader



void CMainApp::OutputData(char* FileName)
{ // OutputData
  int i = 0;
  double Ratio = pView->GetTrilaterationSystemLiarResultError() / pView->GetTrilaterationSystemMeasuredError();
  ofstream OutFile;

  OutFile.open(FileName, ios::app);
  OutFile     << pDoc->GetNumSensors()                                << "     "
              <<                                                         "     "
              << pView->GetTrilaterationSystemActualX()               << "     "
              << pView->GetTrilaterationSystemActualY()               << "     "
              <<                                                         "     "
              << pView->GetTrilaterationSystemMeasuredX()             << "     "
              << pView->GetTrilaterationSystemMeasuredY()             << "     "
              << pView->GetTrilaterationSystemMeasuredError()         << "     "
              <<                                                         "     "
              << pView->GetTrilaterationSystemLiarResultX()           << "     "
              << pView->GetTrilaterationSystemLiarResultY()           << "     "
              << pView->GetTrilaterationSystemLiarResultError()       << "     "
              <<                                                         "     "
              << Ratio                                                << "     "
              <<                                                         "     ";
  for (i = 0; i < pDoc->GetNumSensors(); ++i)
  {
    OutFile   << pView->GetTrilaterationSystemSensorX(i)              << "     "
              << pView->GetTrilaterationSystemSensorY(i)              << "     "
              << pView->GetTrilaterationSystemSensorActualRadius(i)   << "     "
              << pView->GetTrilaterationSystemSensorMeasuredRadius(i) << "     "
              <<                                                         "     ";
  }
  OutFile     << endl;
  Delay(1000);
  OutFile.flush();
  OutFile.close();
} // OutputData



void CMainApp::Delay(unsigned long Milliseconds)
{ // Delay
  unsigned long Now = timeGetTime();
  while (timeGetTime() - Now < Milliseconds);
} // Delay




void CMainApp::OnFileGenerateMc() 
{
  int i = 0;
  CDC* pDC = pView->GetActiveWindow()->GetDC();

  // Clear The Screen
  pView->TSSet(NULL);
  pView->OnDraw(pDC);
  OutputHeader("Results_Generated_3_ReportMC_40.txt");
  pDoc->OnNew();
  pDoc->SetNumSensors(3);
  // pDoc->SetNumSystemIterations(1);
  pDoc->SetNumSystemIterations(100);
  pDoc->InitializeTrilaterationSystemRandom();
  pView->TSSet(pDoc->TLSGet());
  pView->InitTrilaterationSystem();
  pView->DisplayUpdates(0);
  for (i = 0; i < pDoc->GetNumSystemIterations(); ++i)
  {
    pView->RunMonteCarloSimulation(100, 0.40, 100);
    pView->OnDraw(pDC);
    OutputData("Results_Generated_3_ReportMC_40.txt");
  }
  ResetTrilaterationSystem = 1;	
}

void CMainApp::OnFileGenerateUsecorrect() 
{
  int i = 0;
  CDC* pDC = pView->GetActiveWindow()->GetDC();

  // Clear The Screen
  pView->TSSet(NULL);
  pView->OnDraw(pDC);
  pDoc->SetNumSensors(3);
  // pDoc->SetNumSystemIterations(1);
  OutputHeader("Results_Generated_3UC.txt");
  for (i = 0; i < pDoc->GetNumSystemIterations(); ++i)
  {
    pDoc->OnNew();
    pDoc->InitializeTrilaterationSystemRandom();
    pDoc->SetSkewMode(0);
    pView->TSSet(pDoc->TLSGet());
    pView->DisplayUpdates(0);
    pView->ResetTrilaterationSystem();
    while(!pDoc->TLSFinished())
    {
      pView->Update();
      pView->UpdateTime();
      // pView->OnDraw(pDC);
    }
    pView->OnDraw(pDC);
    OutputData("Results_Generated_3UC.txt");
  }
  ResetTrilaterationSystem = 1;
}
