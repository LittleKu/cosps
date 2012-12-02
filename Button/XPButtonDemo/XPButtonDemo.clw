; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CXPButton
LastTemplate=CButton
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "XPButtonDemo.h"

ClassCount=4
Class1=CXPButtonDemoApp
Class2=CXPButtonDemoDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_XPBUTTONDEMO_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Class4=CXPButton
Resource5=IDD_XPBUTTONDEMO_DIALOG (English (U.S.))

[CLS:CXPButtonDemoApp]
Type=0
HeaderFile=XPButtonDemo.h
ImplementationFile=XPButtonDemo.cpp
Filter=N

[CLS:CXPButtonDemoDlg]
Type=0
HeaderFile=XPButtonDemoDlg.h
ImplementationFile=XPButtonDemoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_BTN_BIG

[CLS:CAboutDlg]
Type=0
HeaderFile=XPButtonDemoDlg.h
ImplementationFile=XPButtonDemoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_XPBUTTONDEMO_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CXPButtonDemoDlg

[DLG:IDD_XPBUTTONDEMO_DIALOG (English (U.S.))]
Type=1
Class=CXPButtonDemoDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BTN_NORMAL,button,1342242816
Control4=IDC_BTN_BIG,button,1342242816

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:CXPButton]
Type=0
HeaderFile=XPButton.h
ImplementationFile=XPButton.cpp
BaseClass=CButton
Filter=W

