// Splitter.cpp : implementation file
//

#include "stdafx.h"
#include "Splitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitter


IMPLEMENT_DYNAMIC(CSplitterWndEx, CSplitterWnd)

CSplitterWndEx::CSplitterWndEx()
{
}

CSplitterWndEx::~CSplitterWndEx()
{
}

void CSplitterWndEx::DeleteView(int row, int col)
{
//   HWND hWnd;
//   int ar=-1, ac=-1;
//   CWnd *pWnd = GetPane(row, col);
//   CWnd *pWndAct = GetActivePane(ar, ac);
//   if (pWnd) hWnd = pWnd->m_hWnd;
//   TRACE("CSplitterWndEx::DeleteView %d %d %08x %08x (%d,%d)\n", row, col, pWnd, hWnd, ar, ac);

   CSplitterWnd::DeleteView(row, col);
}

BOOL CSplitterWndEx::DestroyWindow()
{
//   TRACE("CSplitterWndEx::DestroyWindow\n");
   // Delete all view
   for (int cols = 0; cols < m_nCols; cols++) {
      for (int rows = 0; rows < m_nRows; rows++)
         DeleteView(rows, cols);
   }
   m_nCols = m_nRows = 0;

   // For DestroyWindow + Create
   if (m_pColInfo) delete m_pColInfo;
   if (m_pRowInfo) delete m_pRowInfo;
   m_pColInfo = m_pRowInfo = NULL;

//   return CWnd::DestroyWindow();
   return CSplitterWnd::DestroyWindow();
}
