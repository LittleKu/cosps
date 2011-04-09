; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CListBoxDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "resexplorer.h"
LastPage=0

ClassCount=19
Class1=CBitmapDlg
Class2=CDirPickDlg
Class3=CDirTreeCtrl
Class4=CDrawView
Class5=CEdtView
Class6=CFileDialogSa
Class7=CLeftView
Class8=CListCtrlEx
Class9=CEditEx
Class10=CMainFrame
Class11=CReportView
Class12=CResDoc
Class13=CResExplorer
Class14=CAboutDlg
Class15=CSymbolDlg
Class16=CToolbarDlg
Class17=CTooltipList
Class18=CUpperView

ResourceCount=12
Resource1=IDD_TOOLBAR_DLG
Resource2=IDR_RVCOPYPAST
Resource3=IDD_ABOUTBOX
Resource4=IDR_RVSYMBOLS
Resource5=IDD_DIRPICK_DLG
Resource6=IDR_UVSORTHD
Resource7=IDD_BITMAP_DLG
Resource8=IDD_SYMBOL_DLG
Resource9=IDR_MAINFRAME
Resource10=IDR_DVCOPYPAST
Resource11=IDR_EVCOPYPAST
Class19=CListBoxDlg
Resource12=IDD_LISTBOX_DLG

[CLS:CBitmapDlg]
Type=0
BaseClass=CDialog
HeaderFile=BitmapDlg.h
ImplementationFile=BitmapDlg.cpp

[CLS:CDirPickDlg]
Type=0
BaseClass=CDialog
HeaderFile=DirPickDlg.h
ImplementationFile=DirPickDlg.cpp

[CLS:CDirTreeCtrl]
Type=0
BaseClass=CTreeCtrl
HeaderFile=DirTreeCtrl.h
ImplementationFile=DirTreeCtrl.cpp

[CLS:CDrawView]
Type=0
BaseClass=CScrollView
HeaderFile=DrawView.h
ImplementationFile=DrawView.cpp

[CLS:CEdtView]
Type=0
BaseClass=CEditView
HeaderFile=EdtView.h
ImplementationFile=EdtView.cpp

[CLS:CFileDialogSa]
Type=0
BaseClass=CFileDialog
HeaderFile=FileDialogSa.h
ImplementationFile=FileDialogSa.cpp

[CLS:CLeftView]
Type=0
BaseClass=CTreeView
HeaderFile=LeftView.h
ImplementationFile=LeftView.cpp

[CLS:CListCtrlEx]
Type=0
BaseClass=CListCtrl
HeaderFile=listctrlex.h
ImplementationFile=ListCtrlEx.cpp

[CLS:CEditEx]
Type=0
BaseClass=CEdit
HeaderFile=listctrlex.h
ImplementationFile=ListCtrlEx.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CReportView]
Type=0
BaseClass=CListView
HeaderFile=ReportView.h
ImplementationFile=ReportView.cpp

[CLS:CResDoc]
Type=0
BaseClass=CDocument
HeaderFile=ResDoc.h
ImplementationFile=ResDoc.cpp

[CLS:CResExplorer]
Type=0
BaseClass=CWinApp
HeaderFile=ResExplorer.h
ImplementationFile=ResExplorer.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ResExplorer.cpp
ImplementationFile=ResExplorer.cpp
LastObject=CAboutDlg

[CLS:CSymbolDlg]
Type=0
BaseClass=CDialog
HeaderFile=SymbolDlg.h
ImplementationFile=SymbolDlg.cpp

[CLS:CToolbarDlg]
Type=0
BaseClass=CDialog
HeaderFile=ToolbarDlg.h
ImplementationFile=ToolbarDlg.cpp

[CLS:CTooltipList]
Type=0
BaseClass=CWnd
HeaderFile=TooltipEx.h
ImplementationFile=TooltipEx.cpp

[CLS:CUpperView]
Type=0
BaseClass=CListView
HeaderFile=UpperView.h
ImplementationFile=UpperView.cpp

[DLG:IDD_BITMAP_DLG]
Type=1
Class=CBitmapDlg
ControlCount=7
Control1=IDC_STATIC,static,1342308352
Control2=IDC_RD_BKCOLOR_1,button,1342308361
Control3=IDC_RD_BKCOLOR_2,button,1342177289
Control4=IDC_RD_BKCOLOR_3,button,1342177289
Control5=IDC_STATIC,button,1342177287
Control6=IDOK,button,1342242817
Control7=IDCANCEL,button,1342242816

[DLG:IDD_DIRPICK_DLG]
Type=1
Class=CDirPickDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ED_SELECT,edit,1350631552
Control4=IDC_TREE_FILES,SysTreeView32,1350633527
Control5=IDC_COMBO_EXT,combobox,1344339970

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_RESEXICON,static,1342177539
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342242817
Control5=IDC_BTN_PROGRAM,button,1342275584

[DLG:IDD_SYMBOL_DLG]
Type=1
Class=CSymbolDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CHK_SYMSTD,button,1342242819
Control4=IDC_CHK_SYMEXT,button,1342242819
Control5=IDC_LST_SYMBOL,SysListView32,1350632009
Control6=IDC_STATIC,button,1342177287
Control7=IDC_BTN_SYMFILE,button,1342242817
Control8=IDC_BTN_GENERATE,button,1342242817

[DLG:IDD_TOOLBAR_DLG]
Type=1
Class=CToolbarDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ED_WIDTH,edit,1350639745
Control4=IDC_SPIN_WIDTH,msctls_updown32,1342177590
Control5=IDC_ED_IMAGES,edit,1350639745
Control6=IDC_SPIN_IMAGES,msctls_updown32,1342177590
Control7=IDC_ED_BMP_DIM,edit,1350633601
Control8=IDC_STATIC,static,1342308864
Control9=IDC_STATIC,static,1342308864
Control10=IDC_STATIC,static,1342308864

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE_AS
Command3=ID_PANE_BACK
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_FILE_PRINT
Command7=ID_APP_ABOUT
Command8=ID_CONTEXT_HELP
CommandCount=8

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE_AS
Command3=ID_FILE_PRINT
Command4=ID_FILE_PRINT_PREVIEW
Command5=ID_FILE_PRINT_SETUP
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
Command8=ID_EDIT_COPY
Command9=ID_EDIT_PASTE
Command10=ID_PANE_BACK
Command11=ID_MAKE_TOOLBAR
Command12=ID_BITMAP_CVRT
Command13=ID_SYMBOL_DLG
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_VIEW_LARGEICON
Command17=ID_DSPL_BKCOLOR
Command18=ID_HELP_FINDER
Command19=ID_APP_ABOUT
CommandCount=19

[MNU:IDR_RVCOPYPAST]
Type=1
Class=?
Command1=ID_EDRV_COPY
Command2=ID_EDRV_TRUE
Command3=ID_EDRV_QUOT
CommandCount=3

[MNU:IDR_DVCOPYPAST]
Type=1
Class=?
Command1=ID_EDDV_COPY
Command2=ID_ICON_BKCOLOR
Command3=ID_ICON_BKCOLOR1
Command4=ID_ICON_BKCOLOR2
CommandCount=4

[MNU:IDR_EVCOPYPAST]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_EDIT_SELECT_ALL
CommandCount=2

[MNU:IDR_RVSYMBOLS]
Type=1
Class=?
Command1=ID_HSYM_SORT0
Command2=ID_HSYM_SORT1
Command3=ID_HSYM_SORT2
Command4=ID_HSYM_SORT3
Command5=ID_HSYM_SORT4
Command6=ID_SYM_COPY
Command7=ID_SYM_PAST
Command8=ID_SYM_REPLACE
Command9=ID_SYM_SEARCH
Command10=ID_SYM_MODIF
Command11=ID_SYM_DELETE
Command12=ID_SYM_DELETEALL
Command13=ID_SYM_FORMAT
Command14=ID_SYM_CUTFAT
Command15=ID_SYM_MLTDEF
Command16=ID_SYM_SELALL
CommandCount=16

[MNU:IDR_UVSORTHD]
Type=1
Class=?
Command1=ID_UVSORTCOL00
CommandCount=1

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
Command15=ID_CONTEXT_HELP
Command16=ID_HELP
CommandCount=16

[DLG:IDD_LISTBOX_DLG]
Type=1
Class=CListBoxDlg
ControlCount=0

[CLS:CListBoxDlg]
Type=0
HeaderFile=symboldlg.h
ImplementationFile=symboldlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CListBoxDlg
VirtualFilter=dWC

