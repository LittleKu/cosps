; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSlidePreviewDlg
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SlidePreview.h"

ClassCount=4
Class1=CSlidePreviewApp
Class2=CSlidePreviewDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CPreviewCtrl
Resource3=IDD_SLIDEPREVIEW_DIALOG

[CLS:CSlidePreviewApp]
Type=0
HeaderFile=SlidePreview.h
ImplementationFile=SlidePreview.cpp
Filter=N

[CLS:CSlidePreviewDlg]
Type=0
HeaderFile=SlidePreviewDlg.h
ImplementationFile=SlidePreviewDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_PREV_CTRL

[CLS:CAboutDlg]
Type=0
HeaderFile=SlidePreviewDlg.h
ImplementationFile=SlidePreviewDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SLIDEPREVIEW_DIALOG]
Type=1
Class=CSlidePreviewDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PREV_CTRL,static,1342308352
Control4=IDC_START_PREV,button,1342242816
Control5=IDC_STOP_PREV,button,1342242816
Control6=IDC_SAVE_FRAMES,button,1342242816

[CLS:CPreviewCtrl]
Type=0
HeaderFile=PreviewCtrl.h
ImplementationFile=PreviewCtrl.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC

