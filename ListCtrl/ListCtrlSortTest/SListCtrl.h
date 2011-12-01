#if !defined(AFX_SLISTCTRL_H__F6812ED2_94A8_4CA9_9198_92450DF7C7F8__INCLUDED_)
#define AFX_SLISTCTRL_H__F6812ED2_94A8_4CA9_9198_92450DF7C7F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SListCtrl.h : header file
//

#include "SHeaderCtrl.h"
#include "SortSupport.h"
#include "Sorter.h"

extern UINT WM_CHECK_LIST_CTRL_CHECKBOX_CLICKED;

class CListImage
{
public:
	//The image list where the image is.
	//Default value: A pointer to the image list in the list
	//control that is used small images (LVSIL_SMALL)
	CImageList* m_imageList;

	//The image position in the image list.
	//-1 if no image.
	//Default value: -1
	int			m_nImage;
};

class CListProgressBar
{
public:
	//Note: The GetItemText() specifies the text in
	//the progress bar
	
	//The max value of progress bar. Use -1 to disable
	//progress bar.The min value is supposed to be 0.
	//Default value: -1
	int m_maxvalue;
	
	//The value the progress bar has. The width of the 
	//progress bar is calculated with use m_value and 
	//m_maxvalue.
	//Default value: 0
	int m_value;
	
	//The color the progress bar should be drawn with. 
	//Default value: DEFAULTCOLOR
	COLORREF m_fillColor;
	
	//The color of the text on the progress bar
	//Default value: DEFAULTCOLOR
	COLORREF m_fillTextColor;
};

class CListSubItemData
{
public:
	int					m_nCheckState;
	CListImage*			m_pListImage;
	CListProgressBar*	m_pListPrgsBar;

	CListSubItemData() : m_nCheckState(0), m_pListImage(0), m_pListPrgsBar(0) {}
	~CListSubItemData()
	{
		if(m_pListImage)
		{
			delete m_pListImage;
			m_pListImage = NULL;
		}
		if(m_pListPrgsBar)
		{
			delete m_pListPrgsBar;
			m_pListPrgsBar = NULL;
		}
	}
};
class CCheckListItemData
{
public:
	CCheckListItemData()
	{
		m_pSubItemDatas		= NULL;
		dwItemData          = 0;
	}
	~CCheckListItemData()
	{
		if(m_pSubItemDatas != NULL)
		{
			delete [] m_pSubItemDatas;
			m_pSubItemDatas = NULL;
		}
	}
	CListSubItemData*	m_pSubItemDatas;	//SubItem Data array				
	DWORD				dwItemData;			//pointer to app's data				
};
/////////////////////////////////////////////////////////////////////////////
// CSListCtrl window

class CSListCtrl : public CListCtrl
{
// Construction
public:
	CSListCtrl();
	virtual ~CSListCtrl();
// Attributes
public:
	CSHeaderCtrl	m_HeaderCtrl;
	COLORREF		m_crBtnFace;
	COLORREF		m_crHighLight;
	COLORREF		m_crHighLightText;
	COLORREF		m_crWindow;
	COLORREF		m_crWindowText;
	int             m_nRowHeight;
	CImageList      m_ILRowHeight;
// Operations
public:
	//Sort Support
	virtual LPARAM GetAppData(LPARAM lParam);
	virtual CComparator* CreateComparator(CSortCondition* pSortCondtions, int nCount);

	//Insert Functions
	int  InsertItem( int nItem, LPCTSTR lpszItem );
	int  InsertItem(const LVITEM* pItem, BOOL bVirgin = FALSE);

	//Delete Functions
	BOOL DeleteItem(int nItem);
	BOOL DeleteAllItems();

	//List Ctrl interface
	DWORD GetItemData(int nItem) const;
	BOOL GetSubItemRect(int nItem, int nSubItem, int nArea, CRect& rect);
	void InvalidateSubItem(int nItem, int nSubItem);

	CListSubItemData* GetSubItemData(int nItem, int nSubItem);
	
	//Check Box Related functions
	void SetItemCheckedState(int nItem, int nSubItem, int nCheckedState, BOOL bUpdateImmediately = TRUE);
	int  GetItemCheckedState(int nItem, int nSubItem);
	void SetHeaderCheckedState(int nSubItem, int nCheckedState);
	int	 GetHeaderCheckedState(int nSubItem);
	int  CountCheckedItems(int nSubItem);
	void ValidateCheck();

	void SetRowHeight(int nRowHeight);
	int  GetRowHeight();
private:
	//Functions related to Custom Draw
	void DrawItem(int item, int subitem, LPARAM lItemlParam, CDC* pDC);
	void DrawCheckBox(int nItem, int nSubItem, CDC *pDC);
	void DrawImage(int nItem, int nSubItem, CDC *pDC);
	void DrawText(int nItem, int nSubItem, CDC *pDC);
	void DrawProgressBar(int nItem, int nSubItem, CDC *pDC);

	BOOL CalcCheckBoxRect(int nItem, int nSubItem, CRect& checkboxRect);
	BOOL CalcImageRect(int nItem, int nSubItem, CRect& rcImage);
	BOOL CalcTextRect(int nItem, int nSubItem, CRect& rcText);
	BOOL CalcProgressRect(int nItem, int nSubItem, CRect& rcProgress);

	void GetDrawColors(int nItem, int nSubItem, COLORREF& colorText, COLORREF& colorBkgnd);
	void DrawCheckbox(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect, BOOL bDrawMark);
	void DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect& rect);

	void SetItemCheckedStateByClick(int nItem, int nSubItem, int nCheckedState, BOOL bUpdateHeader = TRUE);
	BOOL IsPtInSubItemCheckBox(int nItem, int nSubItem, POINT pt);
	inline int SwitchCheckedState(int nCheckedState);
private:
	//Attributes
	CSortable* m_pSortable;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	// Generated message map functions
protected:
	//{{AFX_MSG(CSListCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg BOOL OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLISTCTRL_H__F6812ED2_94A8_4CA9_9198_92450DF7C7F8__INCLUDED_)
