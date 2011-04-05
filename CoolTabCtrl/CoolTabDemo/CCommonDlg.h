#ifndef _C_COMMON_DLG_H
#define _C_COMMON_DLG_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResizableDialog.h"

class CCommonDlg : public CResizableDialog
{
public:
	CCommonDlg();
	CCommonDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CCommonDlg();
	DECLARE_DYNCREATE(CCommonDlg)	
protected:
	
	DECLARE_MESSAGE_MAP()
};


#endif