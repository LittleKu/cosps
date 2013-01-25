#if !defined(AFX_VCDLG_H__42E8ED94_BE1F_4D7E_B9B2_CAD0618EA58E__INCLUDED_)
#define AFX_VCDLG_H__42E8ED94_BE1F_4D7E_B9B2_CAD0618EA58E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VCDlg.h : header file
//

#include "cflmfc/SplitterControl.h"
#include "TaskListCtrl.h"
#include "AdvComboBox.h"
#include "PropListMgr.h"
#include "ProfileLoader.h"
#include "ProcessExecutor.h"
#include "CmdBuilder.h"

/////////////////////////////////////////////////////////////////////////////
// CVCDlg dialog

class CVCDlg : public CResizableDialog
{
// Construction
public:
	CVCDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVCDlg();
// Dialog Data
	//{{AFX_DATA(CVCDlg)
	enum { IDD = IDD_VC_DLG };
// 	CStatic	m_deviceComboBox;
// 	CStatic	m_profileComboBox;
	CAdvComboBox	m_deviceComboBox;
	CAdvComboBox	m_profileComboBox;
	CStatic	m_wndOutputPropListPos;
	cfl::CSplitterControl	m_splitterTableOutput;
	cfl::CSplitterControl	m_splitterTreeTable;
	CTaskListCtrl	m_taskListCtrl;
	CTreeCtrl	m_taskTreeCtrl;
	CPropListMgr m_propListMgr;
	//}}AFX_DATA

	
	ProcessExecutor* m_pProcExecutor;
	std::vector<CmdInfo> m_cmdInfos;
	int m_nCurCmd;
	TStrVector m_delList;
	int m_nCurRow;
	std::vector<int> m_selRows;
	int m_nProgressBase;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVCDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();

	afx_msg LRESULT OnRspStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyProcEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyProcProgress(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void UpdateTaskTreeWindow();
	void AddFiles();
	void OnStartButton();
	void OnStopButton();
	ProfileNode* GetSelectedProfile();

private:
	void InitResizableDlgAnchor();
	void InitTaskListCtrl();
	void InitTaskTree();
	void InitDeviceComboBox();
	void InitProfileComboBox();
	void InitSplitters();
	void SetSplitterTreeTableRange();
	void SetSplitterTableOutputRange();
	void DoSizeTreeTable(int delta);
	void DoSizeTableOutput(int delta);
	void InitPropList();
	void OnCategorySelChanged();
	void OnProfileSelChanged();
	PLIST_ITEM AddProfile(ProfileNode* pProfile, PLIST_ITEM pListItem);

	void GetStreamDumpFile(cfl::tstring& szDumpFile, const cfl::tstring& szBinFQPath, LPCTSTR lpszStreamName, int nIndex);
	void StartProcess(const CmdInfo& cmdInfo, int nIndex);
	void StartTask();
	void UpdateProgress(int nItem, double dPercent);
	void UpdateStatus(int nItem, int nState);
	void EnableButtons(BOOL bRunning);
	void ChangeState(int newState);

private:
	CBitmap m_splitterVBkBitmap;
	CBitmap m_splitterHBkBitmap;
	int		m_nMinWidth;
	//CProfileLoader m_profileLoader;

	BOOL	m_bStopped;
	int		m_nCurTaskState;

	TCHAR*	m_lpFileBuffer;
	int		m_nFileBufferLen;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCDLG_H__42E8ED94_BE1F_4D7E_B9B2_CAD0618EA58E__INCLUDED_)
