// ListCtrlDemoDlg.h : header file
//

#if !defined(AFX_LISTCTRLDEMODLG_H__4D53D2D3_8FBA_48C7_8ACD_7520E8E9B13D__INCLUDED_)
#define AFX_LISTCTRLDEMODLG_H__4D53D2D3_8FBA_48C7_8ACD_7520E8E9B13D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoDlg dialog
#include "FileParser.h"
#include "SplitterControl.h"
#include "ThirdParty/MultiSelTreeCtrl.h"
#include "ProgressDlg.h"
#include "Counter.h"
#include "SourceDirListCtrl.h"
#include "ResultListCtrl.h"
#include "HistoryComboBox.h"

#define EXPORT_TYPE_CSV		1
#define EXPORT_TYPE_EXCEL	2
#define EXPORT_TYPE_XML		3
#define EXPORT_TYPE_HTML	4

class CListCtrlDemoDlg : public CResizableDialog
{
	friend class CMainDlg;
// Construction
public:
	CListCtrlDemoDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CListCtrlDemoDlg();
	CTotalInfo* GetTotalInfo();
	void OnExport(DWORD nTypeIndex = EXPORT_TYPE_CSV);
	void UpdateFilterTreeBkgnd();
// Dialog Data
	//{{AFX_DATA(CListCtrlDemoDlg)
	enum { IDD = IDD_LISTCTRLDEMO_DIALOG };
	CMultiSelTreeCtrl	m_filterTree;
	CImageList m_selectedStateIL;
	CImageList m_checkBoxStateIL;
	
	CHistoryComboBox	m_filterComboBox;
	CSourceDirListCtrl m_srcDirListCtrl;
	CResultListCtrl m_resultListCtrl;
	CSplitterControl m_splitterVertical;
	CSplitterControl m_splitterHorizontal;
	//}}AFX_DATA
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonClear();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
// Implementation
protected:
	void ResetResult();
	void DeleteAllListItems();
	// Generated message map functions
	//{{AFX_MSG(CListCtrlDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnStartCount(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCount(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSummaryUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnTreeItemSelected(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitResultListCtrl();
	void InitResizableDlgAnchor();
	void InitSplitters();
	void InitFilterTree();
	void RefreshFilterArrays();
	//Member variables
protected:
	void SetSplitterHRange();
	void SetSplitterVRange();
	void UpdateResizableDlgAnchor();
	void DoSizeHorizontal(int delta);
	void DoSizeVertical(int delta);
	HICON m_hIcon;
	CStringArray m_sFilterArray;
	CTotalInfo m_totalInfo;
private:
	CBitmap m_splitterVBkBitmap;
	CBitmap m_splitterHBkBitmap;
	void InitSrcDirListCtrl();
	void SetPair(int idc, int idp, int count, int total = 0);
	CProgressDlg* m_pProgressDlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLDEMODLG_H__4D53D2D3_8FBA_48C7_8ACD_7520E8E9B13D__INCLUDED_)
