#ifndef _PREFERENCES_DLG_H_
#define _PREFERENCES_DLG_H_

#pragma once

#include "./ThirdParty/TreePropSheet/TreePropSheet.h"
#include "PPgGeneral.h"
#include "PPgStatistic.h"

class CPreferencesDlg : public TreePropSheet::CTreePropSheet
{
	DECLARE_DYNAMIC(CPreferencesDlg)
public:
	CPreferencesDlg();
	virtual ~CPreferencesDlg();
	
	CPPgGeneral		m_wndGeneral;
	CPPgStatistic	m_wndStatistic;

	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
protected:	
	BOOL m_bSaveIniFile;
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#endif