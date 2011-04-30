// SortClass.h : header file
//

#ifndef __SORTCLASS_H__
#define __SORTCLASS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

enum SORT_TYPE{ 
	TYPE_TEXT,
	TYPE_NUMERIC,
	TYPE_DATE,
	TYPE_CHECKBOX
};

class CSortClass
{
public:
	CSortClass(CListCtrl* _pWnd, const int _iCol, const SORT_TYPE _sortType);
	//CSortClass(CListCtrl * _pWnd, const int _iCol, const bool _bIsNumeric, int iStartingItem, int iEndingItem );
	virtual ~CSortClass();		
	
	int iCol;	
	int m_iStartingItem;
	int m_iEndingItem;

	CListCtrl* pWnd;


	SORT_TYPE m_sortType;
	
	//bool bIsNumeric;
	
	void Sort(const bool bAsc);	

	//TEXT
	static int CALLBACK CompareAsc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareDes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	//NUMERIC
	static int CALLBACK CompareAscNumeric(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareDesNumeric(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	//DATE
	static int CALLBACK CompareAscDate(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareDesDate(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	//CHECKBOX
	static int CALLBACK CompareAscCheck(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareDesCheck(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

public:	
	static void CString_Replace(CString& cStr, LPCTSTR cToReplace, LPCTSTR cWithWhat);

	//we need this one to mark the last item that we want at the bottom
	class CSortItemGeneric
	{
		BOOL m_bLast;
	public:
		virtual ~CSortItemGeneric() {}
		CSortItemGeneric(const CListCtrl* _pWnd, const int _iRow);

		static int CheckLast( CSortItemGeneric* i1, CSortItemGeneric* i2 );
		const BOOL IsLast();
	};

	class CSortItem	: public CSortItemGeneric
	{	
	public:		
		virtual ~CSortItem() {}
		CSortItem(const DWORD _dw, const CString &_txt, const CListCtrl* _pWnd, const int _iRow);
		CString txt;		
		DWORD dw;	
	};
	class CSortItemNumeric : public CSortItemGeneric
	{	
	public:
		virtual ~CSortItemNumeric()	{}
		CSortItemNumeric(const DWORD _dw, const CString &_txt, const CListCtrl* _pWnd, const int _iRow);
		double dNum;
		DWORD dw;	
	};
	class CSortItemDate	: public CSortItemGeneric
	{	
	public:
		virtual ~CSortItemDate() {}
		CSortItemDate(const DWORD _dw, const CString &_txt, const CListCtrl* _pWnd, const int _iRow);
		COleDateTime oTime;
		DWORD dw;	
	};
	class CSortItemCheck : public CSortItemGeneric
	{	
	public:
		virtual ~CSortItemCheck() {}
		CSortItemCheck(const DWORD _dw, const CListCtrl* _pWnd, const int _iRow);
		BOOL bCheck;
		DWORD dw;	
	};
};

#endif //__SORTCLASS_H__