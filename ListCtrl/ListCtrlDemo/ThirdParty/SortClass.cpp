// SortClass.cpp : implementation file
//

#include "stdafx.h"
#include "SortClass.h"

/////////////////////////////////////////////////////////////////////////////
// CSortClass

CSortClass::CSortClass(CListCtrl* _pWnd, const int _iCol, const SORT_TYPE _sortType )
{	
	iCol = _iCol;	
	pWnd = _pWnd;	
	m_sortType = _sortType;		
	ASSERT(pWnd);
	int max = pWnd->GetItemCount();	
	DWORD dw;	
	CString txt;	
	if( TYPE_NUMERIC == m_sortType )
	{
		for (int t = 0; t < max; t++)		
		{			
			dw = pWnd->GetItemData(t);
			txt = pWnd->GetItemText(t, iCol);
			pWnd->SetItemData(t, (DWORD) new CSortItemNumeric(dw, txt, _pWnd, t));		
		}	
	}
	else if( TYPE_DATE == m_sortType )
	{
		for (int t = 0; t < max; t++)	
		{			
			dw = pWnd->GetItemData(t);
			txt = pWnd->GetItemText(t, iCol);
			pWnd->SetItemData(t, (DWORD) new CSortItemDate(dw, txt, _pWnd, t));
		}
	}
	else if( TYPE_CHECKBOX == m_sortType )
	{
		for (int t = 0; t < max; t++)	
		{			
			dw = pWnd->GetItemData(t);
			txt = pWnd->GetItemText(t, iCol);
			pWnd->SetItemData(t, (DWORD) new CSortItemCheck(dw, _pWnd, t));
		}
	}
	else
	{
		for (int t = 0; t < max; t++)	
		{			
			dw = pWnd->GetItemData(t);
			txt = pWnd->GetItemText(t, iCol);
			pWnd->SetItemData(t, (DWORD) new CSortItem(dw, txt, _pWnd, t));
		}
	}
}

CSortClass::~CSortClass()
{	
	ASSERT(pWnd);
	int max = pWnd->GetItemCount();
	if( TYPE_NUMERIC == m_sortType )
	{		
		CSortItemNumeric* pItem;		
		for (int t = 0; t < max; t++)	
		{
			pItem = (CSortItemNumeric*)pWnd->GetItemData(t);
			ASSERT(pItem);
			pWnd->SetItemData(t, pItem->dw);	
			delete pItem;	
		}
	}
	else if( TYPE_DATE == m_sortType )
	{
		CSortItemDate* pItem;
		for (int t = 0; t < max; t++)
		{
			pItem = (CSortItemDate*)pWnd->GetItemData(t);
			ASSERT(pItem);
			pWnd->SetItemData(t, pItem->dw);
			delete pItem;
		}
	}
	else if( TYPE_CHECKBOX == m_sortType )
	{
		CSortItemCheck* pItem;
		for (int t = 0; t < max; t++)
		{
			pItem = (CSortItemCheck*)pWnd->GetItemData(t);
			ASSERT(pItem);
			pWnd->SetItemData(t, pItem->dw);
			delete pItem;
		}
	}
	else
	{
		CSortItem* pItem;
		for (int t = 0; t < max; t++)
		{
			pItem = (CSortItem*)pWnd->GetItemData(t);
			ASSERT(pItem);
			pWnd->SetItemData(t, pItem->dw);
			delete pItem;
		}
	}
}

void CSortClass::Sort(const bool bAsc)
{	
	if( TYPE_NUMERIC == m_sortType )
	{		
		if (bAsc)
			pWnd->SortItems(CompareAscNumeric, 0L);
		else	
			pWnd->SortItems(CompareDesNumeric, 0L);
	}
	else if( TYPE_DATE == m_sortType )
	{
		if (bAsc)
			pWnd->SortItems(CompareAscDate, 0L);
		else	
			pWnd->SortItems(CompareDesDate, 0L);
	}
	else if( TYPE_CHECKBOX == m_sortType )
	{
		if (bAsc)
			pWnd->SortItems(CompareAscCheck, 0L);
		else	
			pWnd->SortItems(CompareDesCheck, 0L);
	}
	else
	{	
		if (bAsc)	
			pWnd->SortItems(CompareAsc, 0L);	
		else
			pWnd->SortItems(CompareDes, 0L);
	}
}

int CALLBACK CSortClass::CompareAsc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int nRes = 0;

	CSortItem* i1 = (CSortItem*)lParam1;
	CSortItem* i2 = (CSortItem*)lParam2;	
	ASSERT(i1 && i2);

	nRes = CSortItemGeneric::CheckLast(i1, i2);
	if( 0L == nRes )
	{
		nRes = i1->txt.CompareNoCase(i2->txt);
	}
	
	return nRes;
}

int CALLBACK CSortClass::CompareDes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{	
	int nRes = 0;
	
	CSortItem* i1 = (CSortItem*)lParam1;
	CSortItem* i2 = (CSortItem*)lParam2;	
	ASSERT(i1 && i2);
	
	nRes = CSortItemGeneric::CheckLast(i1, i2);
	if( 0L == nRes )
	{
		nRes = i2->txt.CompareNoCase(i1->txt);
	}
	
	return nRes;
}

int CALLBACK CSortClass::CompareAscNumeric(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int nRes = 0;
	
	CSortItemNumeric* i1 = (CSortItemNumeric*)lParam1;
	CSortItemNumeric* i2 = (CSortItemNumeric*)lParam2;
	ASSERT(i1 && i2);
	
	nRes = CSortItemGeneric::CheckLast(i1, i2);
	if( 0L == nRes )
	{
		if (i1->dNum == i2->dNum) 
			nRes = 0;
		else
			nRes = i1->dNum > i2->dNum ? 1 : -1;
	}

	return nRes;
}

int CALLBACK CSortClass::CompareDesNumeric(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{	
	int nRes = 0;
	
	CSortItemNumeric* i1 = (CSortItemNumeric*)lParam1;
	CSortItemNumeric* i2 = (CSortItemNumeric*)lParam2;
	ASSERT(i1 && i2);
	
	nRes = CSortItemGeneric::CheckLast(i1, i2);
	if( 0L == nRes )
	{
		if (i1->dNum == i2->dNum)
			nRes = 0;
		else
			nRes = i1->dNum < i2->dNum ? 1 : -1;
	}

	return nRes;
}

int CALLBACK CSortClass::CompareAscDate(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int nRes = 0;
	
	CSortItemDate* i1 = (CSortItemDate*)lParam1;
	CSortItemDate* i2 = (CSortItemDate*)lParam2;
	ASSERT(i1 && i2);
	
	nRes = CSortItemGeneric::CheckLast(i1, i2);
	if( 0L == nRes )
	{
		if( i1->oTime.GetStatus() != COleDateTime::valid && 
			i2->oTime.GetStatus() != COleDateTime::valid )
		{
			nRes = 0;
		}
		else if( i1->oTime.GetStatus() != COleDateTime::valid )
		{
			nRes = -1;
		}
		else if( i2->oTime.GetStatus() != COleDateTime::valid )
		{
			nRes = 1;
		}
		else if( i1->oTime == i2->oTime )
		{
			nRes = 0;	
		}
		else
		{
			nRes = i1->oTime > i2->oTime ? 1 : -1;
		}
	}
	
	return nRes;
}

int CALLBACK CSortClass::CompareDesDate(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{	
	int nRes = 0;
	
	CSortItemDate* i1 = (CSortItemDate*)lParam1;
	CSortItemDate* i2 = (CSortItemDate*)lParam2;
	ASSERT(i1 && i2);
	
	nRes = CSortItemGeneric::CheckLast(i1, i2);
	if( 0L == nRes )
	{
		if( i1->oTime.GetStatus() != COleDateTime::valid && 
			i2->oTime.GetStatus() != COleDateTime::valid )
		{
			nRes = 0;
		}
		else if( i1->oTime.GetStatus() != COleDateTime::valid )
		{
			nRes = 1;
		}
		else if( i2->oTime.GetStatus() != COleDateTime::valid )
		{
			nRes = -1;
		}
		else if( i1->oTime == i2->oTime )
		{
			nRes = 0;	
		}
		else
		{
			nRes = i1->oTime < i2->oTime ? 1 : -1;
		}
	}
	
	return nRes;
}

int CALLBACK CSortClass::CompareAscCheck(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int nRes = 0;
	
	CSortItemCheck* i1 = (CSortItemCheck*)lParam1;
	CSortItemCheck* i2 = (CSortItemCheck*)lParam2;
	ASSERT(i1 && i2);
	
	nRes = CSortItemGeneric::CheckLast(i1, i2);
	if( 0L == nRes )
	{
		if (i1->bCheck == i2->bCheck) 
			nRes = 0;
		else
			nRes = i1->bCheck > i2->bCheck ? 1 : -1;
	}
	
	return nRes;
}

int CALLBACK CSortClass::CompareDesCheck(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int nRes = 0;
	
	CSortItemCheck* i1 = (CSortItemCheck*)lParam1;
	CSortItemCheck* i2 = (CSortItemCheck*)lParam2;
	ASSERT(i1 && i2);
	
	nRes = CSortItemGeneric::CheckLast(i1, i2);
	if( 0L == nRes )
	{
		if (i1->bCheck == i2->bCheck)
			nRes = 0;
		else
			nRes = i1->bCheck < i2->bCheck ? 1 : -1;
	}
	
	return nRes;
}

CSortClass::CSortItem::CSortItem(const DWORD _dw, const CString & _txt, const CListCtrl* _pWnd, const int _iRow)
	: CSortItemGeneric( _pWnd, _iRow )
{
	dw = _dw;
	txt = _txt;
}

CSortClass::CSortItemNumeric::CSortItemNumeric(const DWORD _dw, const CString & _txt, const CListCtrl* _pWnd, const int _iRow)
	: CSortItemGeneric( _pWnd, _iRow )
{
	//remove the thousands separator
	CString strTxt( _txt );
	CSortClass::CString_Replace(strTxt, ",", "");

	dNum = atof(strTxt);	
	dw = _dw;
}

CSortClass::CSortItemDate::CSortItemDate(const DWORD _dw, const CString & _txt, const CListCtrl* _pWnd, const int _iRow)
	: CSortItemGeneric( _pWnd, _iRow )
{
	dw = _dw;
	if( !_txt.IsEmpty() )
	{
		int nPos = _txt.Find( " " );

		CString strDate = _txt.Mid( nPos );					//get rid of the day of the week
		strDate = _txt.Mid( _txt.GetLength()-4 ) + strDate;	//copy the year to the begining
		strDate = strDate.Left( strDate.GetLength() - 4 );	//remove the trailing year

		oTime.ParseDateTime( strDate );
	}
	else
	{
		oTime.ParseDateTime(_txt);
	}
}

CSortClass::CSortItemCheck::CSortItemCheck(const DWORD _dw, const CListCtrl* _pWnd, const int _iRow)
	: CSortItemGeneric( _pWnd, _iRow )
{
	bCheck = _pWnd->GetCheck(_iRow);
	dw = _dw;
}

// VC5 & VC60 compatible replace function
// originally by Oz Solomonowich, patched to replace all the occurences - thus recursive
void CSortClass::CString_Replace(CString& cStr, LPCTSTR cToReplace, LPCTSTR cWithWhat)
{
#if _MSC_VER >= 1200
	cStr.Replace( cToReplace, cWithWhat );
#else
	int nPos = cStr.Find(cToReplace);
	
	if( nPos >= 0 )
	{
		CString cRight = cStr.Mid(nPos + _tcslen(cToReplace));
		CString_Replace( cRight, cToReplace, cWithWhat );	//watch out - recursive!
		cStr = cStr.Left(nPos);
		cStr += cWithWhat + cRight;
	}
#endif
}

CSortClass::CSortItemGeneric::CSortItemGeneric(const CListCtrl* _pWnd, const int _iRow)
{
	m_bLast = (_pWnd->GetItemData(_iRow) == DWORD(-1));
}

//the adjustment to the compare functions used to place certain item at the bottom
//return zero indicates that it's not the special item, 
//other return values are apropriate to the 
int CSortClass::CSortItemGeneric::CheckLast( CSortItemGeneric* i1, CSortItemGeneric* i2 )
{
	int nRes = 0;
	
	if( i1->IsLast() )
		nRes = 1;
	if( i2->IsLast() )
		nRes = -1;
	
	return nRes;
}

const BOOL CSortClass::CSortItemGeneric::IsLast() 
{ 
	return m_bLast;
}

