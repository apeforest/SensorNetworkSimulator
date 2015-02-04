// MainDoc.h : interface of the CMainDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include <mmsystem.h>
#include "TrilaterationLiarSystem.h"

#if !defined(AFX_MAINDOC_H__F968E9A4_D5B3_4809_8C59_E2A161D2D3F1__INCLUDED_)
#define AFX_MAINDOC_H__F968E9A4_D5B3_4809_8C59_E2A161D2D3F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMainDoc : public CDocument
{
protected: // create from serialization only
	CMainDoc();
	DECLARE_DYNCREATE(CMainDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDoc)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainDoc();
  void Deinit(void);
  void TLSUpdate(void);
  int  TLSFinished(void);
  int  TLSBreak(void);
  TrilaterationLiarSystem* TLSGet(void);
  void OnNew(void);
  int GetNumSensors(void);
  int GetNumIterations(void);
  int GetNumSystemIterations(void);
  int GetGridX(void);
  int GetGridY(void);
  double GetMean(void);
  double GetStandardDeviation(void);
  void SetNumSensors(int NS);
  void SetNumIterations(int NI);
  void SetNumSystemIterations(int NSI);
  void SetGridX(int GX);
  void SetGridY(int GY);
  void SetMean(double M);
  void SetStandardDeviation(double STD);
  void SetMonteCarloMode(int MCMode);
  void SetSkewMode(int PD);
  void InitializeTrilaterationSystemRandom(void);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  int NumSensors;
  int NumIterations;
  int NumSystemIterations;
  int GridX;
  int GridY;
  double Mean;
  double StandardDeviation;
  TrilaterationLiarSystem* TLS;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainDoc)
	afx_msg BOOL OnNewDocument();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDOC_H__F968E9A4_D5B3_4809_8C59_E2A161D2D3F1__INCLUDED_)
