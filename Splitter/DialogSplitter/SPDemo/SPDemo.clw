; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyListCtrl
LastTemplate=CListCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SPDemo.h"

ClassCount=4
Class1=CSPDemoApp
Class2=CSPDemoDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CMyListCtrl
Resource3=IDD_SPDEMO_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_SPDEMO_DIALOG (English (U.S.))

[CLS:CSPDemoApp]
Type=0
HeaderFile=SPDemo.h
ImplementationFile=SPDemo.cpp
Filter=N

[CLS:CSPDemoDlg]
Type=0
HeaderFile=SPDemoDlg.h
ImplementationFile=SPDemoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST

[CLS:CAboutDlg]
Type=0
HeaderFile=SPDemoDlg.h
ImplementationFile=SPDemoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SPDEMO_DIALOG]
Type=1
Class=CSPDemoDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SPLITTER1,static,1073741831
Control4=IDC_TREE,SysTreeView32,1350631424
Control5=IDC_LIST,SysListView32,1342242816
Control6=IDC_EDIT,edit,1342242948
Control7=IDC_SPLITTER2,static,1073741831
Control8=IDC_STATIC,static,1342177294

[CLS:CMyListCtrl]
Type=0
HeaderFile=MyListCtrl.h
ImplementationFile=MyListCtrl.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[DLG:IDD_SPDEMO_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SPLITTER1,static,1073741831
Control4=IDC_TREE,SysTreeView32,1350631424
Control5=IDC_LIST,SysListView32,1342242816
Control6=IDC_EDIT,edit,1342242948
Control7=IDC_SPLITTER2,static,1073741831
Control8=IDC_STATIC,static,1342177294

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

