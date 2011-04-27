; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCDHeaderCtrl
LastTemplate=CHeaderCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CDListCtrl.h"

ClassCount=5
Class1=CCDListCtrlApp
Class2=CCDListCtrlDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CCDListCtrl
Class5=CCDHeaderCtrl
Resource3=IDD_CDLISTCTRL_DIALOG

[CLS:CCDListCtrlApp]
Type=0
HeaderFile=CDListCtrl.h
ImplementationFile=CDListCtrl.cpp
Filter=N

[CLS:CCDListCtrlDlg]
Type=0
HeaderFile=CDListCtrlDlg.h
ImplementationFile=CDListCtrlDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST1

[CLS:CAboutDlg]
Type=0
HeaderFile=CDListCtrlDlg.h
ImplementationFile=CDListCtrlDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CDLISTCTRL_DIALOG]
Type=1
Class=CCDListCtrlDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631433

[CLS:CCDListCtrl]
Type=0
HeaderFile=CDListCtrl1.h
ImplementationFile=CDListCtrl1.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[CLS:CCDHeaderCtrl]
Type=0
HeaderFile=CDHeaderCtrl.h
ImplementationFile=CDHeaderCtrl.cpp
BaseClass=CHeaderCtrl
Filter=W
VirtualFilter=JWC

