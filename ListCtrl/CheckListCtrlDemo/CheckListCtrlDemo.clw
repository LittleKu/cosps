; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRMenuCheckListCtrl
LastTemplate=CListCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CheckListCtrlDemo.h"

ClassCount=6
Class1=CCheckListCtrlDemoApp
Class2=CCheckListCtrlDemoDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CCheckListCtrl
Class5=CCheckHeaderCtrl
Resource3=IDD_CHECKLISTCTRLDEMO_DIALOG
Resource4=IDR_TOOLBAR1
Class6=CRMenuCheckListCtrl
Resource5=IDD_ABOUTBOX (English (U.S.))
Resource6=IDD_CHECKLISTCTRLDEMO_DIALOG (English (U.S.))

[CLS:CCheckListCtrlDemoApp]
Type=0
HeaderFile=CheckListCtrlDemo.h
ImplementationFile=CheckListCtrlDemo.cpp
Filter=N

[CLS:CCheckListCtrlDemoDlg]
Type=0
HeaderFile=CheckListCtrlDemoDlg.h
ImplementationFile=CheckListCtrlDemoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST1

[CLS:CAboutDlg]
Type=0
HeaderFile=CheckListCtrlDemoDlg.h
ImplementationFile=CheckListCtrlDemoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CHECKLISTCTRLDEMO_DIALOG]
Type=1
Class=CCheckListCtrlDemoDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631425

[CLS:CCheckListCtrl]
Type=0
HeaderFile=CheckListCtrl.h
ImplementationFile=CheckListCtrl.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[CLS:CCheckHeaderCtrl]
Type=0
HeaderFile=CheckHeaderCtrl.h
ImplementationFile=CheckHeaderCtrl.cpp
BaseClass=CHeaderCtrl
Filter=W
VirtualFilter=JWC
LastObject=CCheckHeaderCtrl

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CHECKLISTCTRLDEMO_DIALOG (English (U.S.))]
Type=1
Class=CCheckListCtrlDemoDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631425

[CLS:CRMenuCheckListCtrl]
Type=0
HeaderFile=RMenuCheckListCtrl.h
ImplementationFile=RMenuCheckListCtrl.cpp
BaseClass=CCheckListCtrl
Filter=W
VirtualFilter=FWC

[TB:IDR_TOOLBAR1]
Type=1
Class=?
Command1=ID_CXIMAGE_MIRROR
Command2=ID_CXIMAGE_FLIP
Command3=ID_CXIMAGE_ROTATEL
Command4=ID_CXIMAGE_ROTATER
Command5=ID_CXIMAGE_ROTATE
Command6=ID_CXIMAGE_RESAMPLE
Command7=ID_CXIMAGE_NEGATIVE
Command8=ID_CXIMAGE_GRAYSCALE
Command9=ID_CXIMAGE_THRESHOLD
Command10=ID_CXIMAGE_DECREASEBPP
Command11=ID_CXIMAGE_INCREASEBPP
Command12=ID_CXIMAGE_LIGHTEN
Command13=ID_CXIMAGE_DARKEN
Command14=ID_CXIMAGE_CONTRAST
Command15=ID_CXIMAGE_LESSCONTRAST
Command16=ID_COLORS_MORESATURATIONHSL
Command17=ID_COLORS_MORESATURATIONYUV
Command18=ID_COLORS_LESSSATURATION
Command19=ID_CXIMAGE_COLORIZE
Command20=ID_CXIMAGE_GAMMA
Command21=ID_CXIMAGE_SOFTEN
Command22=ID_CXIMAGE_GAUSSIAN3X3
Command23=ID_CXIMAGE_SHARPEN
Command24=ID_CXIMAGE_SELECTIVEBLUR
Command25=ID_CXIMAGE_UNSHARPMASK
Command26=ID_CXIMAGE_ERODE
Command27=ID_CXIMAGE_DILATE
CommandCount=27

