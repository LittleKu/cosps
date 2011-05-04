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

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDR_MENU1
Class4=CToolBarCtrEx
Resource4=IDD_TOOLBARDEMO_DIALOG

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
LastObject=IDC_CUSTOM1
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
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287

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

