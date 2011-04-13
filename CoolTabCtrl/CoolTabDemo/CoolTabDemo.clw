; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTempDlg2
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CoolTabDemo.h"

ClassCount=5
Class1=CCoolTabDemoApp
Class2=CCoolTabDemoDlg
Class3=CAboutDlg

ResourceCount=9
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG_TEMP2
Resource4=IDR_MENU_MAIN (Neutral)
Resource5=IDD_DIALOG_TEMP2 (English (U.S.))
Resource6=IDD_ABOUTBOX (English (U.S.))
Class4=CTempDlg
Resource7=IDD_COOLTABDEMO_DIALOG
Class5=CTempDlg2
Resource8=IDD_COOLTABDEMO_DIALOG (English (U.S.))
Resource9=IDD_DIALOG_TEMP (Neutral)

[CLS:CCoolTabDemoApp]
Type=0
HeaderFile=CoolTabDemo.h
ImplementationFile=CoolTabDemo.cpp
Filter=N

[CLS:CCoolTabDemoDlg]
Type=0
HeaderFile=CoolTabDemoDlg.h
ImplementationFile=CoolTabDemoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CCoolTabDemoDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=CoolTabDemoDlg.h
ImplementationFile=CoolTabDemoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_COOLTABDEMO_DIALOG]
Type=1
Class=CCoolTabDemoDlg
ControlCount=0

[DLG:IDD_COOLTABDEMO_DIALOG (English (U.S.))]
Type=1
Class=CCoolTabDemoDlg
ControlCount=0

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MENU_MAIN (Neutral)]
Type=1
Class=CCoolTabDemoDlg
Command1=ID_APP_EXIT
Command2=ID_APP_ABOUT
CommandCount=2

[DLG:IDD_DIALOG_TEMP (Neutral)]
Type=1
Class=CTempDlg
ControlCount=1
Control1=IDC_STATIC,button,1342177287

[CLS:CTempDlg]
Type=0
HeaderFile=TempDlg.h
ImplementationFile=TempDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CTempDlg

[DLG:IDD_DIALOG_TEMP2]
Type=1
Class=CTempDlg2
ControlCount=3
Control1=IDC_STATIC_TMP1,static,1342308352
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816

[CLS:CTempDlg2]
Type=0
HeaderFile=TempDlg2.h
ImplementationFile=TempDlg2.cpp
BaseClass=CResizableDialog
Filter=D
LastObject=IDC_BUTTON2
VirtualFilter=dWC

[DLG:IDD_DIALOG_TEMP2 (English (U.S.))]
Type=1
Class=CTempDlg2
ControlCount=3
Control1=IDC_STATIC_TMP1,static,1342308352
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816

