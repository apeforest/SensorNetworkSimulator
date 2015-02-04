// VisualSleepDlg.h : header file
//

#include "SleepNetwork.h"

#if !defined(AFX_VISUALSLEEPDLG_H__2BAD1FA6_6BAE_422B_B7EE_A23E4172821D__INCLUDED_)
#define AFX_VISUALSLEEPDLG_H__2BAD1FA6_6BAE_422B_B7EE_A23E4172821D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVisualSleepDlg dialog

class CVisualSleepDlg : public CDialog
{
// Construction
public:
	CVisualSleepDlg(CWnd* pParent = NULL);	// standard constructor
  void Initialize(void);
  void DrawSensor(CClientDC* PicWindow, double XC, double YC, int R, int G, int B, int Width, int Height);
  void DrawConsumer(CClientDC* PicWindow, double XC, double YC, int R, int G, int B, int Width, int Height);
  void DrawProducer(CClientDC* PicWindow, double XC, double YC, int R, int G, int B, int Width, int Height);
  CClientDC* PicWindow1;
  CClientDC* PicWindow2;
  void Draw(SleepNetwork* S, CClientDC* PicWindow, int Width, int Height);
  void Copy(void);
  int Width1;
  int Height1;
  int Width2;
  int Height2;
  int Border;
  int IconSize;

// Dialog Data
	//{{AFX_DATA(CVisualSleepDlg)
	enum { IDD = IDD_VISUALSLEEP_DIALOG };
	int		Mode;
	int		NC;
	int		NS;
	double	Rad;
	int	NC1;
	int	NC2;
	int	NS1;
	int	NS2;
	double	Avg1;
	double	Avg2;
	double	Max1;
	double	Max2;
	double	Min1;
	double	Min2;
	int		RandSeed;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisualSleepDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVisualSleepDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnExit();
	afx_msg void OnRefresh();
	afx_msg void OnButseed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUALSLEEPDLG_H__2BAD1FA6_6BAE_422B_B7EE_A23E4172821D__INCLUDED_)
