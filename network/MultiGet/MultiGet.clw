; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMultiGetDlg
LastTemplate=CListCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MultiGet.h"

ClassCount=4
Class1=CMultiGetApp
Class2=CMultiGetDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX (English (U.S.))
Resource2=IDR_MAINFRAME
Class4=CTaskListCtrl
Resource3=IDD_ABOUTBOX
Resource4=IDD_MULTIGET_DIALOG
Resource5=IDD_MULTIGET_DIALOG (English (U.S.))

[CLS:CMultiGetApp]
Type=0
HeaderFile=MultiGet.h
ImplementationFile=MultiGet.cpp
Filter=N

[CLS:CMultiGetDlg]
Type=0
HeaderFile=MultiGetDlg.h
ImplementationFile=MultiGetDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_BUTTON_REDOWNLOAD

[CLS:CAboutDlg]
Type=0
HeaderFile=MultiGetDlg.h
ImplementationFile=MultiGetDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MULTIGET_DIALOG]
Type=1
Class=CMultiGetDlg
ControlCount=11
Control1=IDCANCEL,button,1342242816
Control2=IDC_LIST_TASK,SysListView32,1350631425
Control3=IDC_EDIT_URL,edit,1350631552
Control4=IDC_ADD1,button,1342242816
Control5=IDC_BUTTON_START1,button,1342242816
Control6=IDC_BUTTON_PAUSE,button,1342242816
Control7=IDC_BUTTON_RESUME,button,1342242816
Control8=IDC_BUTTON_STOP,button,1342242816
Control9=IDC_BUTTON_REMOVE,button,1342242816
Control10=IDC_BUTTON_ADD,button,1342242816
Control11=IDC_BUTTON_REDOWNLOAD,button,1342242816

[CLS:CTaskListCtrl]
Type=0
HeaderFile=TaskListCtrl.h
ImplementationFile=TaskListCtrl.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[DLG:IDD_MULTIGET_DIALOG (English (U.S.))]
Type=1
Class=CMultiGetDlg
ControlCount=9
Control1=IDCANCEL,button,1342242816
Control2=IDC_LIST_TASK,SysListView32,1350631425
Control3=IDC_EDIT_URL,edit,1350631552
Control4=IDC_ADD1,button,1342242816
Control5=IDC_BUTTON_START1,button,1342242816
Control6=IDC_BUTTON_PAUSE,button,1342242816
Control7=IDC_BUTTON_REMOVE,button,1342242816
Control8=IDC_BUTTON_ADD,button,1342242816
Control9=IDC_BUTTON_REDOWNLOAD,button,1342242816

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

