// AStar.h : main header file for the ASTAR application
//

#if !defined(AFX_ASTAR_H__AF9F0263_6171_41C1_82F1_25C0BF2542FA__INCLUDED_)
#define AFX_ASTAR_H__AF9F0263_6171_41C1_82F1_25C0BF2542FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAStarApp:
// See AStar.cpp for the implementation of this class
//

class CAStarApp : public CWinApp
{
public:
	CAStarApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAStarApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAStarApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASTAR_H__AF9F0263_6171_41C1_82F1_25C0BF2542FA__INCLUDED_)
