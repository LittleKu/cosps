// DialogRW.h : header file
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_DIALOGRW_H__2AACFCAB_AA72_43BC_ABED_2DB166D7E474__INCLUDED_)
#define AFX_DIALOGRW_H__2AACFCAB_AA72_43BC_ABED_2DB166D7E474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern void DlgDestroy();
extern void DlgSetFocus();
extern UINT GetChildDlgWnd(CPoint pos);
extern void DlgItemSetFocus(CDC *pDC, UINT item, BOOL hand);
extern CPoint DlgItemSetFocus(UINT item, HWND hView = NULL);
extern UINT GetSelectedItem(POINT point);
extern CSize DlgDisplay(HWND hWnd, HINSTANCE hinst, LPCTSTR resName, CPoint pos, BOOL limited);
extern BOOL DlgDisplayFrame(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, DWORD type, CRect &pos);
extern CSize MenuDisplay(HWND hWnd, HINSTANCE hinst, LPCTSTR resName, int width);
extern CSize AniDisplay(HWND hWnd, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType, CPoint pos);
extern void AnimErase();


#endif // !defined(AFX_DIALOGRW_H__2AACFCAB_AA72_43BC_ABED_2DB166D7E474__INCLUDED_)
