; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=MyListCtrl
LastTemplate=CListCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "FlickerFreeListCtrlDemo.h"

ClassCount=5
Class1=CFlickerFreeListCtrlDemoApp
Class2=CFlickerFreeListCtrlDemoDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_FLICKERFREELISTCTRLDEMO_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Class4=CFlickerFreeListCtrl
Class5=MyListCtrl
Resource5=IDD_FLICKERFREELISTCTRLDEMO_DIALOG (English (U.S.))

[CLS:CFlickerFreeListCtrlDemoApp]
Type=0
HeaderFile=FlickerFreeListCtrlDemo.h
ImplementationFile=FlickerFreeListCtrlDemo.cpp
Filter=N

[CLS:CFlickerFreeListCtrlDemoDlg]
Type=0
HeaderFile=FlickerFreeListCtrlDemoDlg.h
ImplementationFile=FlickerFreeListCtrlDemoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CFlickerFreeListCtrlDemoDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=FlickerFreeListCtrlDemoDlg.h
ImplementationFile=FlickerFreeListCtrlDemoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_FLICKERFREELISTCTRLDEMO_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CFlickerFreeListCtrlDemoDlg

[DLG:IDD_FLICKERFREELISTCTRLDEMO_DIALOG (English (U.S.))]
Type=1
Class=CFlickerFreeListCtrlDemoDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BTN_START,button,1342242816
Control4=IDC_BTN_STOP,button,1342242816
Control5=IDC_DATA_LISTCTRL,SysListView32,1350632449

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:CFlickerFreeListCtrl]
Type=0
HeaderFile=FlickerFreeListCtrl.h
ImplementationFile=FlickerFreeListCtrl.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[CLS:MyListCtrl]
Type=0
HeaderFile=MyListCtrl.h
ImplementationFile=MyListCtrl.cpp
BaseClass=CListCtrl
Filter=W

