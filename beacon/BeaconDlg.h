// BeaconDlg.h : header file
//

#include "Sensor.h"
#include <time.h>

#if !defined(AFX_BEACONDLG_H__A7DC481D_A5C3_4900_8462_8D4F589C3456__INCLUDED_)
#define AFX_BEACONDLG_H__A7DC481D_A5C3_4900_8462_8D4F589C3456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBeaconDlg dialog

class CBeaconDlg : public CDialog
{
// Construction
public:
	int InProgress;
	void Output(void);
	int CalcNewBeacons(void);
	void DrawInitialSensors();
	double Distance(double X1, double Y1, double X2, double Y2);
	void CalcEdges(void);
	int Height;
	int Width;
	CClientDC* PicWindow;
	void DrawSensor(double X, double Y, int Type);
	void ClearVariables(void);
	void DoCalculations(void);
	void Initialize(void);
	char** Edges;
	Sensor* Sensors;
	int NewBeacons;
	int TotalBeacons;
	int InitBeacons;
	CBeaconDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBeaconDlg)
	enum { IDD = IDD_BEACON_DIALOG };
	CEdit	Ran;
	CEdit	Per;
	CEdit	Num;
	CProgressCtrl	ProgressBar;
	CStatic	strStatus;
	int		NumSensors;
	int		NumSensorsH;
	int		NumSensorsL;
	CString	FileName;
	double	PerSensors;
	double	PerSensorsH;
	double	PerSensorsL;
	double	RanSensors;
	double	RanSensorsH;
	double	RanSensorsL;
	int		NumTrials;
	int		NumSteps;
	BOOL	BMaple;
	BOOL	BNS;
	BOOL	BFP;
	BOOL	BPS;
	BOOL	BRS;
	CString	InFile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBeaconDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBeaconDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCalc();
	afx_msg void OnRand();
	afx_msg void OnStep();
	afx_msg void OnInput();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEACONDLG_H__A7DC481D_A5C3_4900_8462_8D4F589C3456__INCLUDED_)
