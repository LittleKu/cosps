; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CBmp2aviDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "bmp2avi.h"

ClassCount=4
Class1=CBmp2aviApp
Class2=CBmp2aviDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_BMP2AVI_DIALOG

[CLS:CBmp2aviApp]
Type=0
HeaderFile=bmp2avi.h
ImplementationFile=bmp2avi.cpp
Filter=N

[CLS:CBmp2aviDlg]
Type=0
HeaderFile=bmp2aviDlg.h
ImplementationFile=bmp2aviDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=bmp2aviDlg.h
ImplementationFile=bmp2aviDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_BMP2AVI_DIALOG]
Type=1
Class=CBmp2aviDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_AVI_TO_BMP,button,1342242816
Control4=IDC_BMP_TO_AVI,button,1342242816

