#ifndef _MULTI_SELECTED_TREE_CTRL_H_
#define _MULTI_SELECTED_TREE_CTRL_H_

#pragma once

#define TVIS_IMAGE_STATE_NONE			0
#define TVIS_IMAGE_STATE_UNCHECK		1
#define TVIS_IMAGE_STATE_PARTIAL_CHECK	2
#define TVIS_IMAGE_STATE_FULL_CHECK		3

extern UINT ID_TREE_ITEM_SELECTED_EVENT; 

class TiXmlElement;
class CMultiSelTreeCtrl;

// Definitions
typedef BOOL (CALLBACK* ENUM_TREEITEMPROC)(CMultiSelTreeCtrl* pTree, HTREEITEM hTreeItem, LPARAM lParam);

class CMultiSelTreeCtrl : public CTreeCtrl
{
public:
	struct TVITEMDATA
	{
		CString type;
	};


public:
	CMultiSelTreeCtrl();
	virtual ~CMultiSelTreeCtrl();

	BOOL Init(LPCTSTR lpXMLFile);
	HTREEITEM InsertSubItem(HTREEITEM hParent, TiXmlElement* pElement);

	BOOL BFSEnumItems(HTREEITEM hTreeItemRoot, ENUM_TREEITEMPROC enumProc, LPARAM lParam);
	BOOL DFSEnumItems(HTREEITEM hTreeItemRoot, ENUM_TREEITEMPROC enumProc, LPARAM lParam);

	void ExpandAllItems(UINT nCode = TVE_EXPAND);
	BOOL GetSelectedItems(CList<HTREEITEM, HTREEITEM>& hItemList);

	UINT GetItemStateImage(HTREEITEM hItem);
	BOOL SetItemStateImage(HTREEITEM hItem, UINT nStateImage);
	// Generated message map functions
protected:
	//{{AFX_MSG(CMutiTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	UINT m_uFlags;
	BOOL IsSameStateImageWithOtherSiblings(HTREEITEM hItem, int nState);
	void SetSiblingAndParentStateImage(HTREEITEM hItem, int nState);
};

#endif
