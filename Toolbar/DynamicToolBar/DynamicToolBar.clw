; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyToolBarCtrl
LastTemplate=CToolBarCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DynamicToolBar.h"

ClassCount=8
Class1=CDynamicToolBarApp
Class2=CDynamicToolBarDlg
Class3=CAboutDlg

ResourceCount=11
Resource1=IDD_DYNAMICTOOLBAR_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG2
Resource4=IDD_ABOUTBOX
Resource5=IDD_DYNAMICTOOLBAR_DIALOG (English (U.S.))
Class5=CDlg2
Class6=CMyReBar
Resource6=IDD_DIALOG1
Resource7=IDR_MENU1
Resource8=IDD_ABOUTBOX (English (U.S.))
Resource9=IDD_DIALOG2 (English (U.S.))
Resource10=IDD_DIALOG1 (English (U.S.))
Class4=CDlg1
Class7=CToolBarCtrlX
Class8=CMyToolBarCtrl
Resource11=IDR_MENU1 (English (U.S.))

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
LastObject=CDynamicToolBarDlg

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

[CLS:CMyReBar]
Type=0
HeaderFile=MyReBar.h
ImplementationFile=MyReBar.cpp
BaseClass=CReBar
Filter=W
LastObject=CMyReBar

[DLG:IDD_DIALOG1 (English (U.S.))]
Type=1
Class=CDlg1
ControlCount=6
Control1=IDC_FRAME1,button,1342177287
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_BUTTON4,button,1342242816
Control6=IDC_BUTTON5,button,1342242816

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DYNAMICTOOLBAR_DIALOG (English (U.S.))]
Type=1
Class=CDynamicToolBarDlg
ControlCount=1
Control1=IDC_STATUSBAR,msctls_statusbar32,1342242816

[DLG:IDD_DIALOG2 (English (U.S.))]
Type=1
Class=CDlg2
ControlCount=3
Control1=ID_OK2,button,1342242817
Control2=ID_CANCEL2,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631425

[MNU:IDR_MENU1 (English (U.S.))]
Type=1
Class=?
Command1=IDM_TEST1
CommandCount=1

[CLS:CToolBarCtrlX]
Type=0
HeaderFile=ToolBarCtrlX.h
ImplementationFile=ToolBarCtrlX.cpp
BaseClass=CToolBarCtrl
Filter=W

[CLS:CMyToolBarCtrl]
Type=0
HeaderFile=MyToolBarCtrl.h
ImplementationFile=MyToolBarCtrl.cpp
BaseClass=CToolBarCtrlX
Filter=W
VirtualFilter=YWC

