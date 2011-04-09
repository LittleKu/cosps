// DirTreeCtrl.h : interface of the CDirTreeCtrl class
//                            child of the CTreeCtrl class
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_DIRTREECTRL_H__944DED1B_00F4_4170_9DD2_A07BC1CCDB17__INCLUDED_)
#define AFX_DIRTREECTRL_H__944DED1B_00F4_4170_9DD2_A07BC1CCDB17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
/////////////////////////////////////////////////////////////////////////////

class CDirTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CDirTreeCtrl();
	virtual ~CDirTreeCtrl();

// Attributes
public:

// Operations
public:
	CString NewTree(LPCTSTR fileSelPath = "*.*", LPCTSTR defPath = NULL);
   BOOL DeleteAllItems();
	CString GetItemText(HTREEITEM hItem);
	HTREEITEM GetItem(const CString &path = "", BOOL exact = TRUE);
   HTREEITEM PathSelect(CString &path, BOOL exact = TRUE);
	CString GetItemPath(HTREEITEM hit = NULL);
	int GetItemPath(LPTSTR buffer, int len, HTREEITEM hit = NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirTreeCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CDirTreeCtrl)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL CheckDrive();
	BOOL PathIsDirEmpty(LPCTSTR path);
	void DirectoryUpdat(HTREEITEM hit);
	void DirectorySetEmpty(HTREEITEM hit);
	void SetFileItem(TVINSERTSTRUCT *pTv, LPCTSTR path, LPCTSTR fn);
	void SetDirItem(TVINSERTSTRUCT *pTv, LPCTSTR path, LPCTSTR fn, BOOL empty = FALSE);

	CString GetName(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, DWORD dwFlags);
	CString GetDrive(LPCTSTR path);

   CString m_fileSelPath;
   UINT m_timer;
   HTREEITEM m_hitDrv;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRTREECTRL_H__944DED1B_00F4_4170_9DD2_A07BC1CCDB17__INCLUDED_)
