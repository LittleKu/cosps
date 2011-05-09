#ifndef _TOOLBARCTRLX_H_
#define _TOOLBARCTRLX_H_

#pragma once

//#define UM_TOOLBARCTRLX_REFRESH (WM_USER + 48297)

#define REBAR_BAND_INDEX0 0

enum EToolbarLabelType 
{
	NoLabels	= 0,
	LabelsBelow = 1,
	LabelsRight = 2
};

class CToolBarCtrlEx : public CToolBarCtrl
{
	DECLARE_DYNAMIC(CToolBarCtrlEx)

public:
	CToolBarCtrlEx();
	virtual ~CToolBarCtrlEx();
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	EToolbarLabelType GetLabelType() const;
	void SetMaxTextRows(int iMaxRows);
	void Refresh();
	void SetParentReBarWnd(HWND hReBarWnd);
	void UpdateBackground();
	BOOL GetMaxSize(LPSIZE pSize) const;
	void ChangeTextLabelStyle(EToolbarLabelType eLabelType, bool bRefresh, bool bForceUpdateButtons = false);

protected:
	EToolbarLabelType m_eLabelType;
	HWND m_hReBarWnd;
	CArray<HWND, HWND> m_msgReceivers;

	void UpdateReBarIdealSize();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

extern UINT UM_TOOLBARCTRLX_REFRESH;

#endif
