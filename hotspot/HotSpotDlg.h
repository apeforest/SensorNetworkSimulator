// HotSpotDlg.h : header file
//

#if !defined(AFX_HOTSPOTDLG_H__441C63B6_8214_4490_9D64_057CA2CCF0F8__INCLUDED_)
#define AFX_HOTSPOTDLG_H__441C63B6_8214_4490_9D64_057CA2CCF0F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HotSpotNetwork.h"
#define HOTSPOT_ARRAYSIZE 101
#define HOTSPOT_ARRAYMID  50

/////////////////////////////////////////////////////////////////////////////
// CHotSpotDlg dialog

class CHotSpotDlg : public CDialog
{
// Construction
public:
  int Width;                     // Width of Drawing Window
  int Height;                    // Height of Drawing Window
  int R[HOTSPOT_ARRAYSIZE];      // Red Array For Hot Spots
  int G[HOTSPOT_ARRAYSIZE];      // Red Array For Hot Spots
  int B[HOTSPOT_ARRAYSIZE];      // Red Array For Hot Spots
  int RSleep[HOTSPOT_ARRAYSIZE]; // Red Array For Hot Spots
  int GSleep[HOTSPOT_ARRAYSIZE]; // Red Array For Hot Spots
  int BSleep[HOTSPOT_ARRAYSIZE]; // Red Array For Hot Spots
  double PrevSleep;              // Previous Number Of Nodes Asleep
  double PrevRadius;             // Previous Radius
  CClientDC* PicWindow;          // Drawing Window
  HotSpotNetwork HSN;            // AStar Network Object

  void Initialize(void);
  void InitializeColorArray(void);
  void OnDraw(void);
  void DrawEdges(CClientDC* PicWindow, int GradientFill, int ER, int EG, int EB, int SR, int SG, int SB, int AR[], int AG[], int AB[], int SAR[], int SAG[], int SAB[], double EdgeBrightness, int Width, int Height);
  void DrawSensor(CClientDC* PicWindow, double XC, double YC, int R, int G, int B, int Width, int Height);
  void DrawGradientLine(double X1, double Y1, double X2, double Y2, int CR[], int CG[], int CB[], int Start, int End, double EdgeBrightness, int Width, int Height);
public:
	CHotSpotDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHotSpotDlg)
	enum { IDD = IDD_HOTSPOT_DIALOG };
	int		Border;
	int		BkgB;
	int		BkgG;
	int		BkgR;
	int		HighB;
	int		HighG;
	int		HighR;
	int		LowB;
	int		LowG;
	int		LowR;
	int		MedB;
	int		MedG;
	int		MedR;
	int		NumSensors;
	double	Radius;
	double	Sleep;
	int		SleepB;
	int		SleepG;
	int		SleepR;
	int		EdgeB;
	int		EdgeG;
	int		EdgeR;
	int		SensorSize;
	BOOL	GradientFill;
	double	EdgeBrightness;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHotSpotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHotSpotDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNew();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOTSPOTDLG_H__441C63B6_8214_4490_9D64_057CA2CCF0F8__INCLUDED_)
