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

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CTaskListCtrl
Resource3=IDD_MULTIGET_DIALOG

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
LastObject=IDC_EDIT_URL

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
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_TASK,SysListView32,1350631425
Control4=IDC_EDIT_URL,edit,1350631552
Control5=IDC_BUTTON_ADD,button,1342242816

[CLS:CTaskListCtrl]
Type=0
HeaderFile=TaskListCtrl.h
ImplementationFile=TaskListCtrl.cpp
BaseClass=CListCtrl
Filter=W

