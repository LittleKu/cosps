; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCheckHeaderCtrl
LastTemplate=CHeaderCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CheckListCtrlDemo.h"

ClassCount=5
Class1=CCheckListCtrlDemoApp
Class2=CCheckListCtrlDemoDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CCheckListCtrl
Class5=CCheckHeaderCtrl
Resource3=IDD_CHECKLISTCTRLDEMO_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_CHECKLISTCTRLDEMO_DIALOG (English (U.S.))

[CLS:CCheckListCtrlDemoApp]
Type=0
HeaderFile=CheckListCtrlDemo.h
ImplementationFile=CheckListCtrlDemo.cpp
Filter=N

[CLS:CCheckListCtrlDemoDlg]
Type=0
HeaderFile=CheckListCtrlDemoDlg.h
ImplementationFile=CheckListCtrlDemoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST1

[CLS:CAboutDlg]
Type=0
HeaderFile=CheckListCtrlDemoDlg.h
ImplementationFile=CheckListCtrlDemoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CHECKLISTCTRLDEMO_DIALOG]
Type=1
Class=CCheckListCtrlDemoDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631425

[CLS:CCheckListCtrl]
Type=0
HeaderFile=CheckListCtrl.h
ImplementationFile=CheckListCtrl.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[CLS:CCheckHeaderCtrl]
Type=0
HeaderFile=CheckHeaderCtrl.h
ImplementationFile=CheckHeaderCtrl.cpp
BaseClass=CHeaderCtrl
Filter=W
VirtualFilter=JWC
LastObject=CCheckHeaderCtrl

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CHECKLISTCTRLDEMO_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631425

