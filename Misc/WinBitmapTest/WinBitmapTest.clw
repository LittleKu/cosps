; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWinBitmapTestDlg
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WinBitmapTest.h"

ClassCount=4
Class1=CWinBitmapTestApp
Class2=CWinBitmapTestDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CTestStatic
Resource3=IDD_WINBITMAPTEST_DIALOG

[CLS:CWinBitmapTestApp]
Type=0
HeaderFile=WinBitmapTest.h
ImplementationFile=WinBitmapTest.cpp
Filter=N

[CLS:CWinBitmapTestDlg]
Type=0
HeaderFile=WinBitmapTestDlg.h
ImplementationFile=WinBitmapTestDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_TEST_STATIC

[CLS:CAboutDlg]
Type=0
HeaderFile=WinBitmapTestDlg.h
ImplementationFile=WinBitmapTestDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_WINBITMAPTEST_DIALOG]
Type=1
Class=CWinBitmapTestDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LABEL_DEFAUL,static,1342308352
Control4=IDC_MBITMAP,button,1342242816
Control5=IDC_MBITAMP2,button,1342242816
Control6=IDC_PREIVEW_CTRL,static,1342308352
Control7=IDC_START_PREV,button,1342242816
Control8=IDC_STOP_PREV,button,1342242816
Control9=IDC_TEST_STATIC,static,1342308352

[CLS:CTestStatic]
Type=0
HeaderFile=TestStatic.h
ImplementationFile=TestStatic.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC

