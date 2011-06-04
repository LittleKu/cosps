#if !defined(AFX_MUTITREECTRL_H__1298A0CF_BFBF_414E_A1E1_BA18D9B39FBB__INCLUDED_)
#define AFX_MUTITREECTRL_H__1298A0CF_BFBF_414E_A1E1_BA18D9B39FBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#define TVIS_IMAGE_STATE_NONE			0
#define TVIS_IMAGE_STATE_UNCHECK		1
#define TVIS_IMAGE_STATE_PARTIAL_CHECK	2
#define TVIS_IMAGE_STATE_FULL_CHECK		3

extern UINT ID_TREE_ITEM_SELECTED_EVENT; 

class TiXmlElement;

class CMutiTreeCtrl : public CTreeCtrl
{
private:
	struct TVITEMDATA
	{
		CString type;
		UINT nCheckStatus;
	};
// Construction
public:
	CMutiTreeCtrl();
// Attributes
public:

// Operations
public:
	BOOL Init(LPCTSTR lpXMLFile);
	HTREEITEM InsertSubItem(HTREEITEM hParent, TiXmlElement* pElement);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMutiTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SetItemState( HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearch=TRUE);
	UINT GetItemImageState(HTREEITEM hItem);
	BOOL GetSelectedItems(CList<HTREEITEM, HTREEITEM>& hItemList);
	virtual ~CMutiTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMutiTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	UINT m_uFlags;
	void TravelSiblingAndParent(HTREEITEM hItem, int nState);
	void TravelChild(HTREEITEM hItem,int nState);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUTITREECTRL_H__1298A0CF_BFBF_414E_A1E1_BA18D9B39FBB__INCLUDED_)
