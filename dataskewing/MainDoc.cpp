// MainDoc.cpp : implementation of the CMainDoc class
//

#include "stdafx.h"
#include "Main.h"

#include "MainDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDoc

IMPLEMENT_DYNCREATE(CMainDoc, CDocument)

BEGIN_MESSAGE_MAP(CMainDoc, CDocument)
	//{{AFX_MSG_MAP(CMainDoc)
	ON_COMMAND(ID_FILE_NEW, OnNewDocument)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDoc construction/destruction

CMainDoc::CMainDoc()
{
  TLS = NULL;
  NumSensors = TRILATERATIONSYSTEM_NUMSENSORS;
  NumIterations = TRILATERATIONSYSTEM_NUMITERATIONS;
  NumSystemIterations = TRILATERATIONSYSTEM_NUMSYSTEMTIMES;
  GridX = TRILATERATIONSYSTEM_GRIDX;
  GridY = TRILATERATIONSYSTEM_GRIDY;
  Mean = TRILATERATIONSYSTEM_MEAN;
  StandardDeviation = TRILATERATIONSYSTEM_STD;
}

CMainDoc::~CMainDoc()
{
  Deinit();
}

void CMainDoc::Deinit(void)
{
  if (TLS != NULL) delete TLS;
  TLS = NULL;
}

void CMainDoc::OnNew(void)
{
  OnNewDocument();
}

BOOL CMainDoc::OnNewDocument()
{
  int i = 0;

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
  srand(time(NULL));
  if (TLS != NULL) delete TLS;
  TLS = new TrilaterationLiarSystem();
  TLS->Init(NumSensors, NumIterations, GridX, GridY, Mean, StandardDeviation);
  /*
  TLS->ActualPointInit(0.00, 0.20, 0.00);
  TLS->SensorInit(0, 0.0,  0.10, 0.00);
  TLS->SensorInit(1, 0.5,  0.35, 0.00);
  TLS->SensorInit(2, 0.0,  0.85, 0.00);
  */
  InitializeTrilaterationSystemRandom();
  // PostMessage(GetParent(NULL), WM_PAINT, 0, 0);
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMainDoc serialization

void CMainDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainDoc diagnostics

#ifdef _DEBUG
void CMainDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainDoc commands
void CMainDoc::TLSUpdate(void)
{
  // TLS->LiarUpdate(timeGetTime());
  TLS->Update();
}

int CMainDoc::TLSFinished(void)
{
  return(TLS->IsFinished());
}

int CMainDoc::TLSBreak(void)
{
  return(TLS->IsBreak());
}

TrilaterationLiarSystem* CMainDoc::TLSGet(void)
{
  return(TLS);
}

int CMainDoc::GetNumSensors(void)
{ // GetNumSensors
  return(NumSensors);
} // GetNumSensors



int CMainDoc::GetNumIterations(void)
{ // GetNumIterations
  return(NumIterations);
} // GetNumIterations



int CMainDoc::GetGridX(void)
{ // GetGridX
  return(GridX);
} // GetGridX



int CMainDoc::GetGridY(void)
{ // GetGridY
  return(GridY);
} // GetGridY



double CMainDoc::GetMean(void)
{ // GetMean
  return(Mean);
} // GetMean



double CMainDoc::GetStandardDeviation(void)
{ // GetStandardDeviation
  return(StandardDeviation);
} // GetStandardDeviation



void CMainDoc::SetNumSensors(int NS)
{ // SetNumSensors
  if (NS > TRILATERATIONSYSTEM_NUMSENSORS) NumSensors = NS;
  else NumSensors = TRILATERATIONSYSTEM_NUMSENSORS;
}



void CMainDoc::SetNumIterations(int NI)
{ // SetNumIterations
  if (NI > 0) NumIterations = NI;
  else NumIterations = TRILATERATIONSYSTEM_NUMITERATIONS;
} // SetNumIterations



void CMainDoc::SetGridX(int GX)
{ // SetGridX
  if (GX > 0) GridX = GX;
  else GridX = TRILATERATIONSYSTEM_GRIDX;
} // SetGridX



void CMainDoc::SetGridY(int GY)
{ // SetGridY
  if (GY > 0) GridY = GY;
  else GridY = TRILATERATIONSYSTEM_GRIDY;
} // SetGridY



void CMainDoc::SetMean(double M)
{ // SetMean
  Mean = M;
} // SetMean



void CMainDoc::SetStandardDeviation(double STD)
{ // SetStandardDeviation
  if (STD > 0.0) StandardDeviation = STD;
  else StandardDeviation = TRILATERATIONSYSTEM_STD;
} // SetStandardDeviation



void CMainDoc::InitializeTrilaterationSystemRandom(void)
{ // InitializeTrilaterationSystemRandom
  int i = 0;

  TLS->ActualPointInitRandom();
  for (i = 0; i < NumSensors; ++i)
  {
    TLS->SensorInitRandom(i);
  }
  TLS->MeasurementInit();
} // InitializeTrilaterationSystemRandom



int CMainDoc::GetNumSystemIterations(void)
{ // GetNumSystemIterations
  return(NumSystemIterations);
} // GetNumSystemIterations



void CMainDoc::SetNumSystemIterations(int NSI)
{ // SetNumSystemIterations
  if (NSI > 0) NumSystemIterations = NSI;
  else NSI = TRILATERATIONSYSTEM_NUMSYSTEMTIMES;
} // SetNumSystemIterations



void CMainDoc::SetMonteCarloMode(int MCMode)
{ // MCMode
  TLS->SetMonteCarloMode(MCMode);
} // MCMode



void CMainDoc::SetSkewMode(int PD)
{ // SetSkewMode
  TLS->SetSkewMode(PD);
} // SetSkewMode