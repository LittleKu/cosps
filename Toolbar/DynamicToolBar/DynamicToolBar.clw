; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlg2
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DynamicToolBar.h"

ClassCount=6
Class1=CDynamicToolBarApp
Class2=CDynamicToolBarDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_DIALOG2
Resource2=IDR_MAINFRAME
Resource3=IDD_DYNAMICTOOLBAR_DIALOG
Resource4=IDD_DIALOG1
Resource5=IDD_ABOUTBOX
Class5=CDlg1
Class6=CDlg2
Resource6=IDR_MENU1

[CLS:CDynamicToolBarApp]
Type=0
HeaderFile=DynamicToolBar.h
ImplementationFile=DynamicToolBar.cpp
Filter=N

[CLS:CDynamicToolBarDlg]
Type=0
HeaderFile=DynamicToolBarDlg.h
ImplementationFile=DynamicToolBarDlg.cpp
Filter=D

[CLS:CAboutDlg]
Type=0
HeaderFile=DynamicToolBarDlg.h
ImplementationFile=DynamicToolBarDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DYNAMICTOOLBAR_DIALOG]
Type=1
Class=CDynamicToolBarDlg
ControlCount=1
Control1=IDC_STATUSBAR,msctls_statusbar32,1342242816

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=IDM_TEST1
CommandCount=1

[DLG:IDD_DIALOG1]
Type=1
Class=CDlg1
ControlCount=6
Control1=IDC_FRAME1,button,1342177287
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_BUTTON4,button,1342242816
Control6=IDC_BUTTON5,button,1342242816

[DLG:IDD_DIALOG2]
Type=1
Class=CDlg2
ControlCount=3
Control1=ID_OK2,button,1342242817
Control2=ID_CANCEL2,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631425

[CLS:CDlg1]
Type=0
HeaderFile=Dlg1.h
ImplementationFile=Dlg1.cpp
BaseClass=CResizableDialog
Filter=D
VirtualFilter=dWC

[CLS:CDlg2]
Type=0
HeaderFile=Dlg2.h
ImplementationFile=Dlg2.cpp
BaseClass=CResizableDialog
Filter=D
VirtualFilter=dWC

