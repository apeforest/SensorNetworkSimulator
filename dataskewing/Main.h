// Main.h : main header file for the MAIN application
//

#include <mmsystem.h>
#include <iostream.h>
#include <fstream.h>
#include "CTrilaterationLiarSystemView.h"                 // Used For Getting View

#if !defined(AFX_MAIN_H__E0D801BA_7FB1_4FCB_BAF2_39DC8BA718F2__INCLUDED_)
#define AFX_MAIN_H__E0D801BA_7FB1_4FCB_BAF2_39DC8BA718F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMainApp:
// See Main.cpp for the implementation of this class
//

class CMainApp : public CWinApp
{
public:
	CMainApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMainApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
	afx_msg void OnFileRun();
	afx_msg void OnFileContinue();
	afx_msg void OnFileNew2();
	afx_msg void OnFileGenerate();
	afx_msg void OnFileTriangle();
	afx_msg void OnFileGenerate4();
	afx_msg void OnFileGenerate5();
	afx_msg void OnFileGenerateMc();
	afx_msg void OnFileGenerateUsecorrect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  private:
    int ResetTrilaterationSystem;
    CTrilaterationLiarSystemView* pView;
    CMainDoc* pDoc;

    void OutputHeader(char* FileName);
    void OutputData(char* FileName);
    void Delay(unsigned long Milliseconds);
    double GenerateRandomDouble(double LowVal, double HighVal);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAIN_H__E0D801BA_7FB1_4FCB_BAF2_39DC8BA718F2__INCLUDED_)
