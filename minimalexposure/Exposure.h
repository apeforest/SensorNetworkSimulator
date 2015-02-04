// Exposure.h : main header file for the EXPOSURE application
//
#if !defined(AFX_EXPOSURE_H__616E9F61_A5F3_4605_9A79_0F2EC5001E93__INCLUDED_)
#define AFX_EXPOSURE_H__616E9F61_A5F3_4605_9A79_0F2EC5001E93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CExposureApp:
// See Exposure.cpp for the implementation of this class
//

class CExposureApp : public CWinApp
{
public:
	CExposureApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExposureApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CExposureApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPOSURE_H__616E9F61_A5F3_4605_9A79_0F2EC5001E93__INCLUDED_)
