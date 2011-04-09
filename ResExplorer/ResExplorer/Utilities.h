
#ifndef Utilities_h
#define Utilities_h

typedef enum {
   STI_REVERSE = 0x0001,
   STI_TEXT    = 0x0002,
   STI_NUMBER  = 0x0004,
   STI_HEXADEC = 0x0006,
   STI_ABSOLU  = 0x0008
};

typedef enum {          // sort state request
   CS_SORT     =0,      // trier comme coup precedent
   CS_REVERSE  =1,      // trier sens contraire
   CS_ASCEND   =2,      // impose le tri croissant
   CS_DESCEND  =3,      // impose le tri decroissant
   CS_SELVISIBLE=4,     // l'item selecte reste visible
   CS_SORTICON =8       // affiche l'icon de sens
};

#define SS_REMOVE  (-1) // SortStatus
#define SS_REQUEST (-2)

extern int InitHeader(CListCtrl& list, UINT formatID, const CString& add = "");
extern int InitHeader(CListCtrl& list, const CString& header, int first = 0);
extern void ColumnWidthAdjust(CListCtrl& list);
extern int ColumnSort(CListCtrl& list, int iSubItem=-1, UINT state=CS_SORT);
extern void InsertMenu(CListCtrl& list, CMenu* pMenu, UINT nIndex, CImageList* pImageList=NULL);
extern int SortStatus(CListCtrl &list, int status=SS_REQUEST);
extern CString GetColumnTitle(CListCtrl &list, int iSubItem, LPCTSTR pszTitle=NULL);

extern BOOL SortTextItems(CListCtrl& list, int nCol, short mode, int low=0, int high=-1);
extern void DisplayErrorBox(); 
extern CString ErrorMsg();

#endif