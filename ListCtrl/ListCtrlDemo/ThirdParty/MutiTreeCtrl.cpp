#include "stdafx.h"
#include "MutiTreeCtrl.h"
#include "tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT ID_TREE_ITEM_SELECTED_EVENT = ::RegisterWindowMessage(_T("ID_TREE_ITEM_SELECTED_EVENT"));

BOOL CALLBACK PrintProc(CMutiTreeCtrl* pTree, HTREEITEM hTreeItem, LPARAM lParam)
{
	CString sText = pTree->GetItemText(hTreeItem);
	AfxTrace("%s\n", sText);
	return TRUE;
}
BOOL CALLBACK ExpandProc(CMutiTreeCtrl* pTree, HTREEITEM hTreeItem, LPARAM lParam)
{
	UINT nCode = (UINT)lParam;
	if(pTree->ItemHasChildren(hTreeItem))
	{
		return pTree->Expand(hTreeItem, nCode);
	}
	return TRUE;
}

CMutiTreeCtrl::CMutiTreeCtrl()
{
	m_uFlags=0;
}

CMutiTreeCtrl::~CMutiTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMutiTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CMutiTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnStateIconClick)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMutiTreeCtrl message handlers
BOOL CMutiTreeCtrl::Init(LPCTSTR lpXMLFile)
{
	BOOL bResult = FALSE;

	TiXmlDocument doc( lpXMLFile );
	bool loadOkay = doc.LoadFile();	
	if ( !loadOkay )
	{
		AfxTrace(_T("Failed to load file %s. Error=%s.\n"), lpXMLFile, doc.ErrorDesc());
		return FALSE;
	}

	TiXmlNode *pNode = NULL;
	TiXmlElement *pElement = NULL;

	pNode = doc.FirstChild( "root" );
	ASSERT( pNode );
	pElement = pNode->ToElement();
	ASSERT(pElement);

	HTREEITEM hRoot = InsertSubItem(NULL, pElement);

	CList<TiXmlElement*, TiXmlElement*&> pElementList;
	CList<HTREEITEM, HTREEITEM&> hTreeItemList;

	pElementList.AddTail(pElement);
	hTreeItemList.AddTail(hRoot);

	HTREEITEM hParent = NULL, hti = NULL;
	while(!pElementList.IsEmpty())
	{
		pElement = pElementList.RemoveHead();
		hParent = hTreeItemList.RemoveHead();

		for(pNode = pElement->FirstChild(); pNode != NULL; pNode = pNode->NextSibling())
		{
			pElement = pNode->ToElement();
			ASSERT(pElement);

			hti = InsertSubItem(hParent, pElement);
			pElementList.AddTail(pElement);
			hTreeItemList.AddTail(hti);
		}
	}

	bResult = TRUE;

	return bResult;
}

HTREEITEM CMutiTreeCtrl::InsertSubItem(HTREEITEM hParent, TiXmlElement* pElement)
{
	const char* pBuffer = NULL;

	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(TVINSERTSTRUCT));
	tvis.hParent = hParent;
	tvis.hInsertAfter = TVI_LAST;

	// used fields
	const UINT nTVIFlags = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvis.item.mask = nTVIFlags;

	//Text
	// provide a buffer for the item text
	TCHAR szText[MAX_PATH];
	tvis.item.pszText = szText;
	tvis.item.cchTextMax = MAX_PATH;
	ZeroMemory(tvis.item.pszText, MAX_PATH);

	pBuffer = pElement->Attribute("text");
	if(pBuffer != NULL)
	{
		lstrcpyn(tvis.item.pszText, pBuffer, tvis.item.cchTextMax);
	}
	
	//State
	pBuffer = pElement->Attribute("checked");

	int nCheckStatus = TVIS_IMAGE_STATE_UNCHECK;
	if(pBuffer != NULL)
	{
		nCheckStatus = atoi(pBuffer);
	}
	tvis.item.state = INDEXTOSTATEIMAGEMASK( nCheckStatus );
	tvis.item.stateMask = TVIS_STATEIMAGEMASK;

	pBuffer = pElement->Value();
	if(pBuffer != NULL && strcmp(pBuffer, "file") != 0)
	{
		tvis.item.state |= TVIS_EXPANDED;
		tvis.item.stateMask |= TVIS_EXPANDED;
	}

	//Image
	tvis.item.iImage = 0;

	//Selected Image
	tvis.item.iSelectedImage = 1;
	
	//lparam : prepare item data
	tvis.item.lParam = (LPARAM)NULL;

	pBuffer = pElement->Attribute("type");
	if(pBuffer != NULL)
	{
		TVITEMDATA* pData = new TVITEMDATA;
		pData->type = pBuffer;
		tvis.item.lParam = (LPARAM)pData;
	}
	
	// then insert new item
	HTREEITEM hti = InsertItem(&tvis);
	return hti;
}

void CMutiTreeCtrl::OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TVITEM& item = ((LPNMTREEVIEW)pNMHDR)->itemOld;

	// free item data, ignore invalid items
	if (item.lParam != 0)
		delete (TVITEMDATA*)item.lParam;
	
	*pResult = 0;
}

BOOL CMutiTreeCtrl::BFSEnumItems(HTREEITEM hTreeItemRoot, ENUM_TREEITEMPROC enumProc, LPARAM lParam)
{
	if(hTreeItemRoot == NULL)
	{
		return TRUE;
	}
	CList<HTREEITEM, HTREEITEM> hTreeItemList;
	hTreeItemList.AddTail(hTreeItemRoot);	

	HTREEITEM hCurTreeItem = hTreeItemRoot;
	HTREEITEM hTreeItem = NULL;

	//Add all siblings to the list firstly
	for(hTreeItem = GetNextItem(hCurTreeItem, TVGN_NEXT); hTreeItem != NULL; hTreeItem = GetNextItem(hTreeItem, TVGN_NEXT))
	{
		hTreeItemList.AddTail(hTreeItem);
	}
	
	while(!hTreeItemList.IsEmpty()) 
	{
		hCurTreeItem = hTreeItemList.RemoveHead();

		//Add all the children items to the list
		for(hTreeItem = GetChildItem(hCurTreeItem); hTreeItem != NULL; hTreeItem = GetNextItem(hTreeItem, TVGN_NEXT))
		{
			hTreeItemList.AddTail(hTreeItem);
		}
		if(!enumProc(this, hCurTreeItem, lParam))
		{
			return FALSE;
		}
	}
	return TRUE;
}

//This algorithm is similar to the way of pre-order visiting binary tree.
//The first child   ==> left  child
//The first sibling ==> right child
BOOL CMutiTreeCtrl::DFSEnumItems(HTREEITEM hTreeItemRoot, ENUM_TREEITEMPROC enumProc, LPARAM lParam)
{
	if(hTreeItemRoot == NULL)
	{
		return TRUE;
	}
	//This list mimic the stack
	CList<HTREEITEM, HTREEITEM> hTreeItemList;

	HTREEITEM hTreeItem = hTreeItemRoot;
	while(hTreeItem != NULL || !hTreeItemList.IsEmpty())
	{
		while(hTreeItem != NULL)  
		{  
			if(!enumProc(this, hTreeItem, lParam))
			{
				return FALSE;
			}
			
			hTreeItemList.AddTail(hTreeItem);

			//The first child: mimic the left child of binary tree
			hTreeItem = GetChildItem(hTreeItem);
		}
		//FILO, Remove from tail to mimic Stack
		hTreeItem = hTreeItemList.RemoveTail();

		//The first sibling: mimic the right child of binary tree
		hTreeItem = GetNextItem(hTreeItem, TVGN_NEXT);
	}
	return TRUE;
}

void CMutiTreeCtrl::ExpandAllItems(UINT nCode)
{
	SetRedraw(FALSE);
	BOOL nResult = BFSEnumItems(GetRootItem(), ExpandProc, (LPARAM)nCode);
	if(!nResult)
	{
		AfxTrace("Failed to ExpandAllItems\n");
	}
	SetRedraw(TRUE);
	UpdateWindow();
}
void CMutiTreeCtrl::OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	if(m_uFlags & TVHT_ONITEMSTATEICON) 
	{
		*pResult = 1;
		CWnd* pOwner = GetOwner();
		
		if(pOwner != NULL)
		{
			pOwner->SendMessage(ID_TREE_ITEM_SELECTED_EVENT, 0, m_uFlags);
		}
	}
}

void CMutiTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	HTREEITEM hItem =HitTest(point, &m_uFlags);
	if ( (m_uFlags&TVHT_ONITEMSTATEICON ))
	{
		//nState: 0->无选择钮 1->没有选择 2->部分选择 3->全部选择
		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
		nState = (nState == TVIS_IMAGE_STATE_FULL_CHECK) ? TVIS_IMAGE_STATE_UNCHECK : TVIS_IMAGE_STATE_FULL_CHECK;
		SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
		Select(hItem, TVGN_CARET);

		CList<HTREEITEM, HTREEITEM> hItemList;
		GetSelectedItems(hItemList);
	}
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CMutiTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
 	//处理空格键
 	if(nChar==0x20)
 	{
 		HTREEITEM hItem =GetSelectedItem();
 		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
 		if(nState!=0)
 		{
 			nState=(nState==3)?1:3;
 			SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
 		}
 	}
	else CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMutiTreeCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	
	*pResult = 0;
}

BOOL CMutiTreeCtrl::SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearch)
{
	BOOL bReturn=CTreeCtrl::SetItemState( hItem, nState, nStateMask );

	UINT iState = nState >> 12;
	if(iState!=0)
	{
		if(bSearch) TravelChild(hItem, iState);
		TravelSiblingAndParent(hItem,iState);
	}
	return bReturn;
}

void CMutiTreeCtrl::TravelChild(HTREEITEM hItem, int nState)
{
	HTREEITEM hChildItem,hBrotherItem;
	
	//查找子节点，没有就结束
	hChildItem=GetChildItem(hItem);
	if(hChildItem!=NULL)
	{
		//设置子节点的状态与当前节点的状态一致
		CTreeCtrl::SetItemState( hChildItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
		//再递归处理子节点的子节点和兄弟节点
		TravelChild(hChildItem, nState);
		
		//处理子节点的兄弟节点和其子节点
		hBrotherItem=GetNextSiblingItem(hChildItem);
		while (hBrotherItem)
		{
			//设置子节点的兄弟节点状态与当前节点的状态一致
			int nState1 = GetItemState( hBrotherItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=0)
			{
				CTreeCtrl::SetItemState( hBrotherItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
			}
			//再递归处理子节点的兄弟节点的子节点和兄弟节点
			TravelChild(hBrotherItem, nState);
			hBrotherItem=GetNextSiblingItem(hBrotherItem);
		}
	}
}

void CMutiTreeCtrl::TravelSiblingAndParent(HTREEITEM hItem, int nState)
{
	HTREEITEM hNextSiblingItem,hPrevSiblingItem,hParentItem;
	
	//查找父节点，没有就结束
	hParentItem=GetParentItem(hItem);
	if(hParentItem!=NULL)
	{
		int nState1=nState;//设初始值，防止没有兄弟节点时出错
		
		//查找当前节点下面的兄弟节点的状态
		hNextSiblingItem=GetNextSiblingItem(hItem);
		while(hNextSiblingItem!=NULL)
		{
			nState1 = GetItemState( hNextSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=nState && nState1!=0) break;
			else hNextSiblingItem=GetNextSiblingItem(hNextSiblingItem);
		}
		
		if(nState1==nState)
		{
			//查找当前节点上面的兄弟节点的状态
			hPrevSiblingItem=GetPrevSiblingItem(hItem);
			while(hPrevSiblingItem!=NULL)
			{
				nState1 = GetItemState( hPrevSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
				if(nState1!=nState && nState1!=0) break;
				else hPrevSiblingItem=GetPrevSiblingItem(hPrevSiblingItem);
			}
		}
		
		if(nState1==nState || nState1==0)
		{
			nState1 = GetItemState( hParentItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=0)
			{
				//如果状态一致，则父节点的状态与当前节点的状态一致
				CTreeCtrl::SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
			}
			//再递归处理父节点的兄弟节点和其父节点
			TravelSiblingAndParent(hParentItem,nState);
		}
		else
		{
			//状态不一致，则当前节点的父节点、父节点的父节点……状态均为第三态
			hParentItem=GetParentItem(hItem);
			while(hParentItem!=NULL)
			{
				nState1 = GetItemState( hParentItem, TVIS_STATEIMAGEMASK ) >> 12;
				if(nState1!=0)
				{
					CTreeCtrl::SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );
				}
				hParentItem=GetParentItem(hParentItem);
			}
		}
	}	
}

UINT CMutiTreeCtrl::GetItemImageState(HTREEITEM hItem)
{
	UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
	return nState;
}

BOOL CMutiTreeCtrl::GetSelectedItems(CList<HTREEITEM, HTREEITEM>& hItemList)
{
	// Look at all of the root-level items
	HTREEITEM hCurrent = /*GetNextItem(TVI_ROOT, TVGN_NEXT);*/GetRootItem();
	if(hCurrent == NULL)
	{
		return FALSE;
	}

	//Iterator all the selected leaf nodes.
	CList<HTREEITEM, HTREEITEM> htiParentList;
	htiParentList.AddTail(hCurrent);
	UINT nState;
	while(!htiParentList.IsEmpty())
	{
		hCurrent = htiParentList.RemoveHead();
		nState = GetItemImageState(hCurrent);
		if(nState != TVIS_IMAGE_STATE_FULL_CHECK && nState != TVIS_IMAGE_STATE_PARTIAL_CHECK)
		{
			continue;
		}
		//Not a leaf node
		if(ItemHasChildren(hCurrent))
		{
			HTREEITEM hChildItem = GetChildItem(hCurrent);
			while (hChildItem != NULL)
			{
				htiParentList.AddTail(hChildItem);
				hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
			}
		}
		//leaf node can't have a partial check state
		else
		{
			hItemList.AddTail(hCurrent);
// 			// Get the text for the item. Notice we use TVIF_TEXT because
// 			// we want to retrieve only the text, but also specify TVIF_HANDLE
// 			// because we're getting the item by its handle.
// 			TVITEM item;
// 			TCHAR szText[1024];
// 			item.hItem = hCurrent;
// 			item.mask = TVIF_TEXT | TVIF_HANDLE;
// 			item.pszText = szText;
// 			item.cchTextMax = 1024;
// 			
// 			BOOL bWorked = GetItem(&item);
// 			if(bWorked)
// 			{
// 				hItemList.AddTail(hCurrent);
// 				AfxTrace("Selected Item: %s\n", szText);
// 			}
		}
	}
	return TRUE;
}