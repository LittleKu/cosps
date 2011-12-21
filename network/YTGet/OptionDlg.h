// OptionDlg.h: interface for the COptionDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONDLG_H__4433E9D3_1792_4B21_8FF6_E16F0B188D9D__INCLUDED_)
#define AFX_OPTIONDLG_H__4433E9D3_1792_4B21_8FF6_E16F0B188D9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./ThirdParty/TreePropSheet/TreePropSheet.h"
#include "PPgGeneral.h"
#include "PPgConnection.h"
#include "PPgProxy.h"

class COptionDlg : public TreePropSheet::CTreePropSheet  
{
	DECLARE_DYNAMIC(COptionDlg)
public:
	COptionDlg();
	virtual ~COptionDlg();
	
	CPPgGeneral		m_wndGeneral;
	CPPgConnection	m_wndConnection;
	CPPgProxy		m_wndProxy;

	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
protected:	
	BOOL m_bSaveIniFile;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
};

#endif // !defined(AFX_OPTIONDLG_H__4433E9D3_1792_4B21_8FF6_E16F0B188D9D__INCLUDED_)
