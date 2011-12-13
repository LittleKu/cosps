; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CYTGetDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "YTGet.h"

ClassCount=7
Class1=CYTGetApp
Class2=CMainDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_YTGET_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_MAIN_DIALOG
Resource4=IDD_ABOUTBOX
Resource5=IDD_ADD_TASK_DIALOG
Class4=CMainToolBarCtrl
Class5=CYTGetDlg
Class6=CTaskListCtrl
Class7=CAddTaskDlg
Resource6=IDR_MAIN_MENU

[CLS:CYTGetApp]
Type=0
HeaderFile=YTGet.h
ImplementationFile=YTGet.cpp
Filter=N

[CLS:CMainDlg]
Type=0
HeaderFile=MainDlg.h
ImplementationFile=MainDlg.cpp
Filter=D
LastObject=CMainDlg
BaseClass=CResizableDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=MainDlg.h
ImplementationFile=MainDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAIN_MENU]
Type=1
Class=?
Command1=IDM_FILE_EXIT
Command2=IDM_HELP_ABOUT
CommandCount=2

[DLG:IDD_MAIN_DIALOG]
Type=1
Class=CMainDlg
ControlCount=0

[DLG:IDD_YTGET_DIALOG]
Type=1
Class=CYTGetDlg
ControlCount=1
Control1=IDC_TASK_LIST,SysListView32,1350631433

[CLS:CMainToolBarCtrl]
Type=0
HeaderFile=MainToolBarCtrl.h
ImplementationFile=MainToolBarCtrl.cpp
BaseClass=CToolBarCtrl
Filter=W
LastObject=CMainToolBarCtrl

[CLS:CYTGetDlg]
Type=0
HeaderFile=YTGetDlg.h
ImplementationFile=YTGetDlg.cpp
BaseClass=CResizableDialog
Filter=D
LastObject=IDC_TASK_LIST
VirtualFilter=dWC

[CLS:CTaskListCtrl]
Type=0
HeaderFile=TaskListCtrl.h
ImplementationFile=TaskListCtrl.cpp
BaseClass=CSListCtrl
Filter=W
LastObject=CTaskListCtrl
VirtualFilter=FWC

[DLG:IDD_ADD_TASK_DIALOG]
Type=1
Class=CAddTaskDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TXT_NEW_ADDRESS,static,1342308352
Control4=IDC_NEW_ADDRESS,edit,1350631552

[CLS:CAddTaskDlg]
Type=0
HeaderFile=AddTaskDlg.h
ImplementationFile=AddTaskDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_NEW_ADDRESS
VirtualFilter=dWC

