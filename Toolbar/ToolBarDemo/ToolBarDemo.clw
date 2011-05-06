; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CToolBarDemoDlg
LastTemplate=CToolBarCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ToolBarDemo.h"

ClassCount=4
Class1=CToolBarDemoApp
Class2=CToolBarDemoDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_TOOLBARDEMO_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CToolBarCtrEx
Resource4=IDR_MENU1 (English (U.S.))
Resource5=IDD_TOOLBARDEMO_DIALOG (English (U.S.))
Resource6=IDD_ABOUTBOX (English (U.S.))
Resource7=IDR_MENU1

[CLS:CToolBarDemoApp]
Type=0
HeaderFile=ToolBarDemo.h
ImplementationFile=ToolBarDemo.cpp
Filter=N

[CLS:CToolBarDemoDlg]
Type=0
HeaderFile=ToolBarDemoDlg.h
ImplementationFile=ToolBarDemoDlg.cpp
Filter=D
LastObject=IDC_STATIC_TB
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ToolBarDemoDlg.h
ImplementationFile=ToolBarDemoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TOOLBARDEMO_DIALOG]
Type=1
Class=CToolBarDemoDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_TB,static,1342308352
Control4=IDC_LIST1,SysListView32,1350631425

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=IDM_TEST
CommandCount=1

[CLS:CToolBarCtrEx]
Type=0
HeaderFile=ToolBarCtrEx.h
ImplementationFile=ToolBarCtrEx.cpp
BaseClass=CToolBarCtrl
Filter=W
VirtualFilter=YWC

[DLG:IDD_TOOLBARDEMO_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MENU1 (English (U.S.))]
Type=1
Class=?
Command1=IDM_TEST
CommandCount=1

