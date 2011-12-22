; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAddTaskDlg
LastTemplate=CPropertyPage
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "YTGet.h"

ClassCount=10
Class1=CYTGetApp
Class2=CMainDlg
Class3=CAboutDlg

ResourceCount=9
Resource1=IDD_PROPPAGE_CONNECTION
Resource2=IDR_MAINFRAME
Resource3=IDD_PROPPAGE_PROXY
Resource4=IDD_PROPPAGE_GENERAL
Resource5=IDD_ADD_TASK_DIALOG
Class4=CMainToolBarCtrl
Class5=CYTGetDlg
Class6=CTaskListCtrl
Class7=CAddTaskDlg
Resource6=IDD_YTGET_DIALOG
Resource7=IDD_MAIN_DIALOG
Class8=CPPgGeneral
Class9=CPPgConnection
Resource8=IDD_ABOUTBOX
Class10=CPPgProxy
Resource9=IDR_MAIN_MENU

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
Command2=IDM_TOOLS_OPTIONS
Command3=IDM_HELP_ABOUT
CommandCount=3

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
LastObject=CYTGetDlg
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
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TXT_NEW_ADDRESS,static,1342308352
Control4=IDC_NEW_ADDRESS,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_SAVE_TO_FILE_NAME,edit,1350631552
Control7=IDC_STATIC,button,1342177287
Control8=IDC_RADIO_START_AUTO,button,1342177289
Control9=IDC_RADIO_START_MANUALLY,button,1342177289

[CLS:CAddTaskDlg]
Type=0
HeaderFile=AddTaskDlg.h
ImplementationFile=AddTaskDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAddTaskDlg
VirtualFilter=dWC

[DLG:IDD_PROPPAGE_GENERAL]
Type=1
Class=CPPgGeneral
ControlCount=7
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_OUTPUT_DIR,edit,1350631552
Control4=IDC_BTN_OUTPUT_DIR,button,1342242816
Control5=IDC_EDIT_TEMP_DIR,edit,1350631552
Control6=IDC_BTN_TEMP_DIR,button,1342242816
Control7=IDC_STATIC,static,1342308352

[DLG:IDD_PROPPAGE_CONNECTION]
Type=1
Class=CPPgConnection
ControlCount=9
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_MAX_TASK_COUNT,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_MAX_CONNECTION_COUNT,edit,1350631552
Control6=IDC_EDIT_MIN_SEGMENT_SIZE,edit,1350631552
Control7=IDC_EDIT_MAX_RETRY_TIMES,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[CLS:CPPgGeneral]
Type=0
HeaderFile=PPgGeneral.h
ImplementationFile=PPgGeneral.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CPPgGeneral
VirtualFilter=idWC

[CLS:CPPgConnection]
Type=0
HeaderFile=PPgConnection.h
ImplementationFile=PPgConnection.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_EDIT_MAX_TASK_COUNT
VirtualFilter=idWC

[DLG:IDD_PROPPAGE_PROXY]
Type=1
Class=CPPgProxy
ControlCount=8
Control1=IDC_STATIC,button,1342177287
Control2=IDC_RADIO_NO_PROXY,button,1342177289
Control3=IDC_RADIO_PROXY_MANUALLY,button,1342177289
Control4=IDC_RADIO_PROXY_FROM_IE,button,1342177289
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_PROXY_ADDRESS,edit,1350631552
Control8=IDC_EDIT_PROXY_PORT,edit,1350631552

[CLS:CPPgProxy]
Type=0
HeaderFile=PPgProxy.h
ImplementationFile=PPgProxy.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_EDIT_PROXY_ADDRESS
VirtualFilter=idWC

