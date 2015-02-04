// AStarDlg.h : header file
//

//#include "AStarNetwork.h"

#if !defined(AFX_ASTARDLG_H__0CEC0683_7016_41AF_AB71_E18BD174B5C2__INCLUDED_)
#define AFX_ASTARDLG_H__0CEC0683_7016_41AF_AB71_E18BD174B5C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AStarNetwork.h"

/////////////////////////////////////////////////////////////////////////////
// CAStarDlg dialog

class CAStarDlg : public CDialog
{
// Construction
public:
  int Width;              // Width of Drawing Window
  int Height;             // Height of Drawing Window
  CClientDC* PicWindow;   // Drawing Window
  AStarNetwork ASN;       // AStar Network Object

	CAStarDlg(CWnd* pParent = NULL);	// standard constructor

  // Algorithmic Functions
  void Initialize(void);
  void Delete(void);

  // Miscellaneous
  void Delay(int Milliseconds);
  void Output(void);

  // Display Functions
  void Draw(CClientDC* PicWindow, int Width, int Height);
  void DrawEdges(CClientDC* PicWindow, int R, int G, int B, int Width, int Height);
  void DrawSensor(CClientDC* PicWindow, double XC, double YC, int R, int G, int B, int Width, int Height);

// Dialog Data
	//{{AFX_DATA(CAStarDlg)
	enum { IDD = IDD_ASTAR_DIALOG };
	int		BkgB;
	int		BkgG;
	int		BkgR;
	int		EdgeB;
	int		EdgeG;
	int		EdgeR;	
	int		SAB;
	int		SAG;
	int		SAR;
	int		SIB;
	int		SIG;
	int		SIR;
	int		SVB;
	int		SVG;
	int		SVR;
	int		DestB;
	int		DestG;
	int		DestR;
	int		StartB;
	int		StartG;
	int		StartR;
  int		Border;
	int		Start;
	int		Dest;
	int		Mode;
	int		NumNodes;
	int		SensorSize;
	double	Radius;
	int		SleepTime;
	int		Version;
	double	PercentSleep;
	int		SleepR;
	int		SleepG;
	int		SleepB;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAStarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAStarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRun();
	afx_msg void OnNew();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASTARDLG_H__0CEC0683_7016_41AF_AB71_E18BD174B5C2__INCLUDED_)
