// ExposureDlg.h : header file
//

#if !defined(AFX_EXPOSUREDLG_H__06A2CE52_9E94_4DDB_B1EE_70D4D06EA8F4__INCLUDED_)
#define AFX_EXPOSUREDLG_H__06A2CE52_9E94_4DDB_B1EE_70D4D06EA8F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridVertex.h"
#include "Sensor.h"

/////////////////////////////////////////////////////////////////////////////
// CExposureDlg dialog

class CExposureDlg : public CDialog
{
// Construction
public:
  void SnapToGrid(double &X, double &Y);
	double CalcWeight(double SX, double SY, double EX, double EY, double Step);
	BOOL VoronoiDone;
	double RefineBound;
	int SUVI;
	void ShowRefineZone(void);
	int NumPath;
	GridVertex* Path;
	void ClearPrevious(int CurrentIndex);
	void InitRefine(void);
	void RefinePath(void);
	int RefineLevel;
	BOOL InBounds(int Index);
	double FindPathDistance(int Index);
	void DrawGrid(void);
	void DrawPath(void);
	int EI;
	void DrawSensors(void);
	int Width;
	int Height;
	CClientDC* PicWindow;
	void DrawSensor(double X, double Y, int Type = 0);
	void InitSensors(void);
	int FindSmallestVertex(double &X, double &Y);
	int MaxVertices;
	int FindVertex(double X, double Y, double Acc);
	double WeightFunction(double D, double L);
	double Distance(double X1, double Y1, double X2, double Y2);
	double FindNearestSensor(double X, double Y);
  double FindNearestSensor(double X1, double Y1, double X2, double Y2);
  CExposureDlg(CWnd* pParent = NULL);	// standard constructor
  ~CExposureDlg();
  int Initialize(void);
	void CalcPath(void);

  int NumVertices;
  Sensor* Sensors;
	GridVertex* Vertices;

// Dialog Data
	//{{AFX_DATA(CExposureDlg)
	enum { IDD = IDD_EXPOSURE_DIALOG };
	CProgressCtrl	Progress;
	double	EX;
	double	EY;
	int		GridSize;
	double	SideLength;
	int		N;
	CString	OutFile;
	int		P;
	int		GenerateMode;
	CString	Status;
	double	SX;
	double	SY;
	double	u;
	int		GridAccuracy;
	BOOL	ShowGrid;
	BOOL	ShowRefZone;
	BOOL	ShowPathV;
	CString	InFile;
	CString	PCost;
	BOOL	ShowVoronoi;
	CString	PLen;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExposureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CExposureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGenerate();
	afx_msg void OnRefresh();
	afx_msg void OnRefine();
	afx_msg void OnInput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPOSUREDLG_H__06A2CE52_9E94_4DDB_B1EE_70D4D06EA8F4__INCLUDED_)
