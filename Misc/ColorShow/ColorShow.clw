; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CColorShowDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ColorShow.h"

ClassCount=4
Class1=CColorShowApp
Class2=CColorShowDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_COLORSHOW_DIALOG

[CLS:CColorShowApp]
Type=0
HeaderFile=ColorShow.h
ImplementationFile=ColorShow.cpp
Filter=N

[CLS:CColorShowDlg]
Type=0
HeaderFile=ColorShowDlg.h
ImplementationFile=ColorShowDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ColorShowDlg.h
ImplementationFile=ColorShowDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_COLORSHOW_DIALOG]
Type=1
Class=CColorShowDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COLOR_VALUE,edit,1350631552

