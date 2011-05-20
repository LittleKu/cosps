; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCheckLCDemoDlg
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CheckLCDemo.h"

ClassCount=7
Class1=CCheckLCDemoApp
Class2=CCheckLCDemoDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CCheckListCtrl
Class5=CCheckHeadCtrl
Class6=CMyComboBox
Class7=CMyGroupBox
Resource3=IDD_CHECKLCDEMO_DIALOG

[CLS:CCheckLCDemoApp]
Type=0
HeaderFile=CheckLCDemo.h
ImplementationFile=CheckLCDemo.cpp
Filter=N

[CLS:CCheckLCDemoDlg]
Type=0
HeaderFile=CheckLCDemoDlg.h
ImplementationFile=CheckLCDemoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_GROUPBOX1

[CLS:CAboutDlg]
Type=0
HeaderFile=CheckLCDemoDlg.h
ImplementationFile=CheckLCDemoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CHECKLCDEMO_DIALOG]
Type=1
Class=CCheckLCDemoDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631425
Control4=IDC_COMBO1,combobox,1344340226
Control5=IDC_COMBO2,combobox,1344340226
Control6=IDC_GROUPBOX1,button,1342177287

[CLS:CCheckListCtrl]
Type=0
HeaderFile=CheckListCtrl.h
ImplementationFile=CheckListCtrl.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC
LastObject=CCheckListCtrl

[CLS:CCheckHeadCtrl]
Type=0
HeaderFile=CheckHeadCtrl.h
ImplementationFile=CheckHeadCtrl.cpp
BaseClass=CHeaderCtrl
Filter=W

[CLS:CMyComboBox]
Type=0
HeaderFile=MyComboBox.h
ImplementationFile=MyComboBox.cpp
BaseClass=CComboBox
Filter=W
VirtualFilter=cWC

[CLS:CMyGroupBox]
Type=0
HeaderFile=MyGroupBox.h
ImplementationFile=MyGroupBox.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC

