; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CListCtrlSortTestDlg
LastTemplate=CListCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ListCtrlSortTest.h"

ClassCount=6
Class1=CListCtrlSortTestApp
Class2=CListCtrlSortTestDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CSListCtrl
Class6=CMyListCtrl
Class4=CSHeaderCtrl
Resource3=IDD_ABOUTBOX (English (U.S.))
Resource4=IDD_LISTCTRLSORTTEST_DIALOG (English (U.S.))
Resource5=IDD_LISTCTRLSORTTEST_DIALOG

[CLS:CListCtrlSortTestApp]
Type=0
HeaderFile=ListCtrlSortTest.h
ImplementationFile=ListCtrlSortTest.cpp
Filter=N

[CLS:CListCtrlSortTestDlg]
Type=0
HeaderFile=ListCtrlSortTestDlg.h
ImplementationFile=ListCtrlSortTestDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST_MAIN

[CLS:CAboutDlg]
Type=0
HeaderFile=ListCtrlSortTestDlg.h
ImplementationFile=ListCtrlSortTestDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LISTCTRLSORTTEST_DIALOG]
Type=1
Class=CListCtrlSortTestDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_MAIN,SysListView32,1350632449
Control4=IDC_BTN_START,button,1342242816
Control5=IDC_BTN_STOP,button,1342242816
Control6=IDC_BTN_INIT,button,1342242816

[CLS:CSHeaderCtrl]
Type=0
HeaderFile=SHeaderCtrl.h
ImplementationFile=SHeaderCtrl.cpp
BaseClass=CHeaderCtrl
Filter=W
VirtualFilter=JWC

[CLS:CSListCtrl]
Type=0
HeaderFile=SListCtrl.h
ImplementationFile=SListCtrl.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[CLS:CMyListCtrl]
Type=0
HeaderFile=MyListCtrl.h
ImplementationFile=MyListCtrl.cpp
BaseClass=CSListCtrl
Filter=W
VirtualFilter=FWC

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LISTCTRLSORTTEST_DIALOG (English (U.S.))]
Type=1
Class=CListCtrlSortTestDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_MAIN,SysListView32,1350631425

