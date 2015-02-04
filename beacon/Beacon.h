// Beacon.h : main header file for the BEACON application
//

#if !defined(AFX_BEACON_H__A4B10AA2_41D4_4440_B14B_8FDFEF7B3706__INCLUDED_)
#define AFX_BEACON_H__A4B10AA2_41D4_4440_B14B_8FDFEF7B3706__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBeaconApp:
// See Beacon.cpp for the implementation of this class
//

class CBeaconApp : public CWinApp
{
public:
	CBeaconApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBeaconApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBeaconApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEACON_H__A4B10AA2_41D4_4440_B14B_8FDFEF7B3706__INCLUDED_)
