#include "StdAfx.h"
#include "Resource.h"
#include "Utilities.h"

static const int maxTitleLength = 32;
/////////////////////////////////////////////////////////////////////////////
// Initialise ou reinitialise le header d'une CListCtrl

// format : [[type]align]column_0_Title\n[[type]align]column_1_Title

// type   :  T=> text, N=> Number, H=> Hexanumber, A=> Absolu value
// align  :  L=> Left, C=> Center, R=> Right

int InitHeader(CListCtrl& list, UINT formatID, const CString& add)
{
   CString header;
   header.Format(formatID, add);

   return InitHeader(list, header, 0);
}

int InitHeader(CListCtrl& list, const CString& header, int first)
{
   int beg, end, nbCol, fmt, field, len;
   HDITEM hi;

   if (first == 0) {
      // Delete the old columns
      int nbCol = list.GetHeaderCtrl()->GetItemCount();
      while (--nbCol >= 0) list.DeleteColumn(0);
   }

   len = header.GetLength();
   hi.mask = HDI_LPARAM|HDI_IMAGE;
   hi.iImage = 0;

   // Loop for all columns (a field in the string for each)
   for (beg = nbCol = 0; beg < len; nbCol++, beg = end + 1) {
      // It's not '\n' at the last field
      if ( (end = header.Find(_TCHAR('\n'), beg)) < 0 ) end = len;

      if (end > beg) {
         // Read the data type
         switch ( header.GetAt(beg) ) {
         case _TCHAR('T'): hi.lParam =             STI_TEXT,   beg++; break;
         case _TCHAR('N'): hi.lParam = STI_REVERSE|STI_NUMBER, beg++; break;
         case _TCHAR('H'): hi.lParam = STI_REVERSE|STI_HEXADEC,beg++; break;
         case _TCHAR('A'): hi.lParam = STI_REVERSE|STI_ABSOLU ,beg++; break;
         default : hi.lParam = 0;   // sort disable
         }
      } else hi.lParam = 0;         // sort disable

      if (end > beg) {
         // Read the alignment of the column and skip this char
         switch ( header.GetAt(beg) ) {
         case _TCHAR('L'): fmt = LVCFMT_LEFT,   beg++; break;
         case _TCHAR('C'): fmt = LVCFMT_CENTER, beg++; break;
         case _TCHAR('R'): fmt = LVCFMT_RIGHT,  beg++; break;
         default : fmt = LVCFMT_LEFT;
         }
      } else fmt = LVCFMT_LEFT;

      if ( (field = end - beg) >= 0 ) {
         // Inserts a new column in the list view
         list.InsertColumn(first + nbCol, header.Mid(beg, field), fmt);
         list.GetHeaderCtrl()->SetItem(first + nbCol, &hi);
      }
   }

   // Set reasonable widths for our columns
   for (int i = 0; i < nbCol; i++)
      list.SetColumnWidth(first + i, LVSCW_AUTOSIZE_USEHEADER);

   return first + nbCol;
}
/////////////////////////////////////////////////////////////////////////////
// Set reasonable widths for our columns

void ColumnWidthAdjust(CListCtrl& list)
{
   int nbCol = list.GetHeaderCtrl()->GetItemCount();
   for (int i = 0; i < nbCol; i++)
      list.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
}
/////////////////////////////////////////////////////////////////////////////
// CListCtrl Sort

int ColumnSort(CListCtrl& list,
               int iSubItem,  // column number (0..n-1) or -1=> last column
               UINT state)    // CS_SORT : sort in the same order
                              // CS_REVERSE : sort reverse
                              // CS_FOR, CS_BACK : sort command
                              // CS_SELVIS : 0=> show the selected item
{                             // CS_SORTICON : icon display
   CHeaderCtrl *pHeader = list.GetHeaderCtrl();
   int ix = pHeader->GetItemCount();
   HDITEM hi;

   if (iSubItem >= ix) iSubItem = -1;
   // search the old sorted column
   hi.mask = HDI_IMAGE|HDI_FORMAT|HDI_LPARAM;
   while (--ix >= 0) {
      pHeader->GetItem(ix, &hi);
      if (hi.iImage == 0) continue;

      if (iSubItem >= 0 && ix != iSubItem) {
         // delete old display icon
         hi.iImage = 0;
//         hi.lParam |= STI_REVERSE;     // *******************
         hi.fmt &= ~HDF_IMAGE;
         pHeader->SetItem(ix, &hi);
      }
      break;
   }
   if (iSubItem < 0) {
      if (ix < 0) return -3;           // a sorted column is not define
      iSubItem = ix;                   // there is the old column
   }

   // set the new display icon
   pHeader->GetItem(iSubItem, &hi);
   if (hi.lParam == 0) return -2;      // sort disable for this column

   switch (state & 0x0003) {
   case CS_SORT   : break;                   // in the same order
   case CS_REVERSE: hi.lParam ^= 1; break;   // in the revers order
   case CS_ASCEND : hi.lParam &= ~1; break;  // to impose ascending order
   case CS_DESCEND: hi.lParam |= 1; break;   // to inpose descending order
   }

   hi.iImage = hi.lParam;
   if (state & CS_SORTICON) hi.fmt |= HDF_IMAGE; // the icon
   pHeader->SetItem(iSubItem, &hi);

   SortTextItems(list, iSubItem, short(hi.lParam));

   if ((state & CS_SELVISIBLE) == 0) { 
      // Make that the selected item continue to stay visible
      POSITION pos = list.GetFirstSelectedItemPosition();
      if (pos) {
         // search the seleted item 
         int no = list.GetNextSelectedItem(pos);
         list.EnsureVisible(no, FALSE);
         return no;                 // selected item
      }
   }

   return -1;
}
/////////////////////////////////////////////////////////////////////////////
// Get/Set Sort status

int SortStatus(CListCtrl &list,
               int status)       // 0..(n-1)*2 => select column code
                                 // -1 => clear the sorted column
{                                // -2 => return only the sorted column code
   CHeaderCtrl *pHeader = list.GetHeaderCtrl();
   int ix = pHeader->GetItemCount(), rq = (status + 2) / 2 - 1;
   HDITEM hi;

   if (rq >= ix) status = -1;

   // search the old sorted column
   hi.mask = HDI_IMAGE|HDI_LPARAM;
   while (--ix >= 0) {
      pHeader->GetItem(ix, &hi);
      if (hi.iImage == 0) continue;

      // the old sorted column is found
      if (status == -2) {
         // Req = -2 : put the status (state and sorted column)
         return ix * 2 + (hi.iImage & 1);
      } else if (rq != ix) {
         // delete old display icon
         hi.iImage = 0;
//         hi.lParam |= STI_REVERSE;        //************************
         pHeader->SetItem(ix, &hi);
      }
      break;
   }
   if (status < 0) return -1;             // there is not sorted column !

   if (rq != ix) pHeader->GetItem(rq, &hi);
   hi.lParam = (hi.lParam & ~1) | (status & 1);
   hi.iImage = hi.lParam;
   pHeader->SetItem(rq, &hi);
   return status;
}
/////////////////////////////////////////////////////////////////////////////
// Initialize a Popup Menu with the sort header

void InsertMenu(CListCtrl& list,
                CMenu* pMenu,
                UINT nIndex,              // Menu id to remplace
                CImageList* pImageList) 
{	
   CHeaderCtrl *pHeader = list.GetHeaderCtrl();
   int nbCol = pHeader->GetItemCount();
   int nAfter = -1, id;
   char buffer[maxTitleLength];
   HDITEM hi;

   if (pImageList == NULL) pImageList = pHeader->GetImageList();

   hi.mask = HDI_FORMAT|HDI_IMAGE|HDI_LPARAM|HDI_TEXT;
   hi.pszText = buffer;
   hi.cchTextMax = maxTitleLength;

   while (--nbCol >= 0) {
      pHeader->GetItem(nbCol, &hi);
      if (hi.lParam == 0) continue;

      // there is a sort possible column
      id = nIndex + nbCol;
      if (nAfter < 0) {
         if (!pMenu->ModifyMenu(nIndex, MF_BYCOMMAND|MF_STRING, id, buffer))
            return;
      } else {
         pMenu->InsertMenu(nAfter, MF_BYCOMMAND|MF_STRING, id, buffer);
      }

      if (hi.iImage && pImageList) {
         static CBitmap bm, *pOldBm;
         IMAGEINFO ii;
         CDC dcMem;

         // fill a bitmap with the sort image
         pImageList->GetImageInfo(hi.iImage, &ii);
         int w = ii.rcImage.right - ii.rcImage.left;
         int h = ii.rcImage.bottom - ii.rcImage.top;
         dcMem.CreateCompatibleDC(NULL);
         if (bm.m_hObject) bm.DeleteObject();
         bm.CreateCompatibleBitmap(&dcMem, w, h);
         pOldBm = dcMem.SelectObject(&bm);
         dcMem.FillSolidRect(0, 0, w, h, 0xFFFFFF);
         pImageList->Draw(&dcMem, hi.iImage, CPoint(0,0), ILD_NORMAL);
         dcMem.SelectObject(pOldBm);

         // display the sort icon
         pMenu->SetMenuItemBitmaps(id, MF_BYCOMMAND, &bm, &bm);
      }
      nAfter = id;
   }
}
/////////////////////////////////////////////////////////////////////////////
// Read/Modifie column title

CString GetColumnTitle(CListCtrl &list, int iSubItem, LPCTSTR pszTitle)
{
   CHeaderCtrl *pHeader = list.GetHeaderCtrl();
   char buffer[maxTitleLength] = {0, };
   HDITEM hi;

   hi.mask = HDI_FORMAT|HDI_TEXT;
   hi.pszText = buffer;
   hi.cchTextMax = maxTitleLength;
   if (pHeader->GetItem(iSubItem, &hi) && pszTitle) {
      hi.pszText = LPTSTR(pszTitle);
      pHeader->SetItem(iSubItem, &hi);
   }
   return CString(buffer);
}
/////////////////////////////////////////////////////////////////////////////

static double atofs(LPCTSTR s)
{
   const TCHAR tcBase[] = { TEXT("XxOoQqBb") };
   const int   base[] = {16, 8, 8, 2 };
   TCHAR *pC = LPSTR(s), c, *pB;

   while (isspace(c = *pC)) pC++;

   if ( c == TCHAR('0') && (pB = strchr(tcBase, pC[1])) )
      return (double)strtoul(pC + 2, NULL, base[(pB - tcBase)/ 2]);
   else
      return strtod(pC, NULL);
}

static double atofsx(LPCTSTR s)
{
   char *pC;
   double n = strtod(s, &pC);
   if (*pC == '=') n = -1.0; else if (*pC == '*') n = -2.0;
   return n;
}

static double atofsa(LPCTSTR s)
{
   char *pC;
   double n = strtod(s, &pC);
   if (*pC == '=') n = -1.0; else if (*pC == '*') n = -2.0;
   return (n < 0.0)? -n: n;
}

static long atodx(LPCTSTR s)
{
   char *pC;
   long n = strtol(s, &pC, 16);
   if (*pC == '=')      n = (long)0x80000000;
   else if (*pC == '*') n = (long)0x80000001;
   return n;
}
/////////////////////////////////////////////////////////////////////////////

// SortTextItems  - Sort the list based on column text
// Returns		   - Returns true for success
// nCol           - column that contains the text to be sorted
// mode           - indicate sort order & text or number
//                STI_REVERSE, STI_TEXT, STI_NUMBER, STI_HEXADEC, STI_ABSOLU
// low            - row to start scanning from - default row is 0
// high           - row to end scan. -1 indicates last row

BOOL SortTextItems(CListCtrl& list, int nCol, short mode, int low, int high)
{
   int nColCount = list.GetHeaderCtrl()->GetItemCount();
   if (high == -1) high = list.GetItemCount() - 1;
	int lo = low;
	int hi = high;
	CString midItem;

	if ( hi <= lo || hi >= list.GetItemCount()) return FALSE;

	midItem = list.GetItemText( (lo + hi) / 2, nCol );
   double midItemDbl = atofs(midItem);
   long   midItemLg  = atodx(midItem);

	// loop through the list until indices cross
	while ( lo <= hi ) {

		// rowText will hold all column text for one row
      switch ( mode ) {
      case STI_TEXT|STI_REVERSE:  // text ascendant
         // find the first element that is greater than or equal to 
         // the partition element starting from the left Index.
			while ( ( lo < high ) &&
            ( midItem.CollateNoCase(list.GetItemText(lo, nCol)) > 0 ) )
				++lo;
         // find an element that is smaller than or equal to 
         // the partition element starting from the right Index.
			while ( ( hi > low ) &&
            ( midItem.CollateNoCase(list.GetItemText(hi, nCol)) < 0 ) )
				--hi;
         break;
      case STI_TEXT:  // text descendant
			while ( ( lo < high ) &&
            ( midItem.CollateNoCase(list.GetItemText(lo, nCol)) < 0 ) )
				++lo;
			while ( ( hi > low ) &&
            ( midItem.CollateNoCase(list.GetItemText(hi, nCol)) > 0 ) )
				--hi;
         break;
      // Trie a l'envers les numeriques (par defaut le plus grand au debut)
      case STI_NUMBER:  // number descendant
			while ( ( lo < high ) &&
            ( midItemDbl < atofs(list.GetItemText(lo, nCol))) )
				++lo;
			while ( ( hi > low ) &&
            ( midItemDbl > atofs(list.GetItemText(hi, nCol))) )
				--hi;
         break;
      case STI_NUMBER|STI_REVERSE:  // number ascendant
			while ( ( lo < high ) &&
            ( midItemDbl > atofs(list.GetItemText(lo, nCol))) )
				++lo;
			while ( ( hi > low ) &&
            ( midItemDbl < atofs(list.GetItemText(hi, nCol))) )
				--hi;
         break;
      case STI_HEXADEC:  // number descendant
			while ( ( lo < high ) &&
            ( midItemLg < atodx(list.GetItemText(lo, nCol))) )
				++lo;
			while ( ( hi > low ) &&
            ( midItemLg > atodx(list.GetItemText(hi, nCol))) )
				--hi;
         break;
      case STI_HEXADEC|STI_REVERSE:  // number ascendant
			while ( ( lo < high ) &&
            ( midItemLg > atodx(list.GetItemText(lo, nCol))) )
				++lo;
			while ( ( hi > low ) &&
            ( midItemLg < atodx(list.GetItemText(hi, nCol))) )
				--hi;
         break;
      // Trie a l'envers les numeriques (par defaut le plus grand au debut)
      case STI_ABSOLU:  // number descendant
			while ( ( lo < high ) &&
            ( midItemDbl < atofsa(list.GetItemText(lo, nCol))) )
				++lo;
			while ( ( hi > low ) &&
            ( midItemDbl > atofsa(list.GetItemText(hi, nCol))) )
				--hi;
         break;
      case STI_ABSOLU|STI_REVERSE:  // number ascendant
			while ( ( lo < high ) &&
            ( midItemDbl > atofsa(list.GetItemText(lo, nCol))) )
				++lo;
			while ( ( hi > low ) &&
            ( midItemDbl < atofsa(list.GetItemText(hi, nCol))) )
				--hi;
         break;
      default: return FALSE; 
      }

		// if the indexes have not crossed, swap
		// and if the items are not equal
		if ( lo <= hi ) {
			// swap only if the items are not equal
			if ( list.GetItemText(lo, nCol).CollateNoCase(list.GetItemText(hi, nCol)) != 0) {
				// swap the rows
				LV_ITEM lvitemlo, lvitemhi;

				lvitemlo.mask = LVIF_IMAGE|LVIF_STATE|LVIF_PARAM;
				lvitemlo.iSubItem = 0;
				lvitemlo.stateMask = -1;

				lvitemhi = lvitemlo;
				lvitemlo.iItem = lo;
				lvitemhi.iItem = hi;

				list.GetItem( &lvitemhi );
				list.GetItem( &lvitemlo );

				lvitemhi.iItem = lo;
				lvitemlo.iItem = hi;
				list.SetItem( &lvitemhi );
				list.SetItem( &lvitemlo );

            for (int i = 0; i < nColCount; i++) {
               CString &text = list.GetItemText(lo, i);
					list.SetItemText(lo, i, list.GetItemText(hi, i));
					list.SetItemText(hi, i, text);
            }
			}
			++lo;
			--hi;
		}
	}

	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if ( low < hi  ) SortTextItems( list, nCol, mode, low, hi);

	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if ( lo < high ) SortTextItems( list, nCol, mode, lo, high );

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// System error display

void DisplayErrorBox()
{
   LPTSTR lpMsgBuf;
   FormatMessage( 
       FORMAT_MESSAGE_ALLOCATE_BUFFER | 
       FORMAT_MESSAGE_FROM_SYSTEM | 
       FORMAT_MESSAGE_IGNORE_INSERTS,
       NULL,
       GetLastError(),
       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
       (LPTSTR)&lpMsgBuf,
       0,
       NULL 
   );
   // Process any inserts in lpMsgBuf.
   // ...
   // Display the string.
   MessageBox( NULL, lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
   // Free the buffer.
   LocalFree( lpMsgBuf );
}

CString ErrorMsg()
{
   LPTSTR lpMsgBuf;
   FormatMessage( 
       FORMAT_MESSAGE_ALLOCATE_BUFFER | 
       FORMAT_MESSAGE_FROM_SYSTEM | 
       FORMAT_MESSAGE_IGNORE_INSERTS,
       NULL,
       GetLastError(),
       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
       (LPTSTR)&lpMsgBuf,
       0,
       NULL 
   );
   CString msg(lpMsgBuf);
   LocalFree( lpMsgBuf );
   return msg;
}


//////////////////////////////////////////////////////////////////////

