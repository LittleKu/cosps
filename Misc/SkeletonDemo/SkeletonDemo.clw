; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTaskListCtrl
LastTemplate=CListCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SkeletonDemo.h"

ClassCount=7
Class1=CSkeletonDemoApp
Class2=CMainDlg
Class3=CAboutDlg

ResourceCount=9
Resource1=IDD_MAIN_DLG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Resource4=IDD_VC_DLG
Class4=CVCDlg
Class5=CMainToolBar
Class6=CMainReBar
Class7=CTaskListCtrl
Resource5=IDR_MAIN_MENU
Resource6=IDD_VC_DLG (English (U.S.))
Resource7=IDD_ABOUTBOX (English (U.S.))
Resource8=IDD_MAIN_DLG (English (U.S.))
Resource9=IDR_MAIN_MENU (English (U.S.))

[CLS:CSkeletonDemoApp]
Type=0
HeaderFile=SkeletonDemo.h
ImplementationFile=SkeletonDemo.cpp
Filter=N

[CLS:CMainDlg]
Type=0
HeaderFile=MainDlg.h
ImplementationFile=MainDlg.cpp
Filter=D
LastObject=CMainDlg
BaseClass=CResizableDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=MainDlg.h
ImplementationFile=MainDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_VC_DLG]
Type=1
Class=CVCDlg
ControlCount=10
Control1=IDC_TASK_TREE,SysTreeView32,1350631424
Control2=IDC_SPLITTER_TREE_TABLE,static,1342177287
Control3=IDC_TASK_LISTCTRL,SysListView32,1350631425
Control4=IDC_SPLITTER_TABLE_OUTPUT,static,1342177287
Control5=IDC_OUTPUT_GROUPBOX,button,1342177287
Control6=IDC_COMBOBOX_DEVICE,AdvComboBoxCtrl,1342243171
Control7=IDC_COMBOBOX_PROFILE,AdvComboBoxCtrl,1342243171
Control8=IDC_LABEL_DEVICE,static,1342308352
Control9=IDC_LABEL_PROFILE,static,1342308352
Control10=IDC_OUTPUT_PROP_LIST,static,1342177287

[DLG:IDD_MAIN_DLG]
Type=1
Class=CMainDlg
ControlCount=0

[MNU:IDR_MAIN_MENU]
Type=1
Class=?
Command1=IDM_ADD_FILES
Command2=IDM_FILE_EXIT
Command3=IDM_EDIT_SELECT_ALL
Command4=IDM_TOOLS_CUSTOM_PROFILE
Command5=IDM_TOOLS_OPTIONS
Command6=IDM_HELP_CONTENT
Command7=IDM_HELP_CHECK_UPGRADES
Command8=IDM_HELP_ABOUT
CommandCount=8

[CLS:CVCDlg]
Type=0
HeaderFile=VCDlg.h
ImplementationFile=VCDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_TASK_TREE
VirtualFilter=dWC

[CLS:CMainToolBar]
Type=0
HeaderFile=MainToolBar.h
ImplementationFile=MainToolBar.cpp
BaseClass=CToolBarCtrl
Filter=W

[CLS:CMainReBar]
Type=0
HeaderFile=MainReBar.h
ImplementationFile=MainReBar.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC

[CLS:CTaskListCtrl]
Type=0
HeaderFile=TaskListCtrl.h
ImplementationFile=TaskListCtrl.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[DLG:IDD_VC_DLG (English (U.S.))]
Type=1
Class=CVCDlg
ControlCount=10
Control1=IDC_TASK_TREE,SysTreeView32,1350631424
Control2=IDC_SPLITTER_TREE_TABLE,static,1342177287
Control3=IDC_TASK_LISTCTRL,SysListView32,1350631425
Control4=IDC_SPLITTER_TABLE_OUTPUT,static,1342177287
Control5=IDC_OUTPUT_GROUPBOX,button,1342177287
Control6=IDC_COMBOBOX_DEVICE,AdvComboBoxCtrl,1342243171
Control7=IDC_COMBOBOX_PROFILE,AdvComboBoxCtrl,1342243171
Control8=IDC_LABEL_DEVICE,static,1342308352
Control9=IDC_LABEL_PROFILE,static,1342308352
Control10=IDC_OUTPUT_PROP_LIST,static,1342177287

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MAIN_DLG (English (U.S.))]
Type=1
Class=CMainDlg
ControlCount=0

[MNU:IDR_MAIN_MENU (English (U.S.))]
Type=1
Class=?
Command1=IDM_ADD_FILES
Command2=IDM_FILE_EXIT
Command3=IDM_EDIT_SELECT_ALL
Command4=IDM_TOOLS_CUSTOM_PROFILE
Command5=IDM_TOOLS_OPTIONS
Command6=IDM_HELP_CONTENT
Command7=IDM_HELP_CHECK_UPGRADES
Command8=IDM_HELP_ABOUT
CommandCount=8

