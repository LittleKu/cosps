; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDragSliderCtrl
LastTemplate=CSliderCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SliderCtrlDemo.h"

ClassCount=5
Class1=CSliderCtrlDemoApp
Class2=CSliderCtrlDemoDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SLIDERCTRLDEMO_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Class4=CAdjustSlider
Class5=CDragSliderCtrl
Resource5=IDD_SLIDERCTRLDEMO_DIALOG (English (U.S.))

[CLS:CSliderCtrlDemoApp]
Type=0
HeaderFile=SliderCtrlDemo.h
ImplementationFile=SliderCtrlDemo.cpp
Filter=N

[CLS:CSliderCtrlDemoDlg]
Type=0
HeaderFile=SliderCtrlDemoDlg.h
ImplementationFile=SliderCtrlDemoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_MY_SLIDER

[CLS:CAboutDlg]
Type=0
HeaderFile=SliderCtrlDemoDlg.h
ImplementationFile=SliderCtrlDemoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_SLIDERCTRLDEMO_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CSliderCtrlDemoDlg

[DLG:IDD_SLIDERCTRLDEMO_DIALOG (English (U.S.))]
Type=1
Class=CSliderCtrlDemoDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_MY_SLIDER,msctls_trackbar32,1342242840

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:CAdjustSlider]
Type=0
HeaderFile=AdjustSlider.h
ImplementationFile=AdjustSlider.cpp
BaseClass=CSliderCtrl
Filter=W

[CLS:CDragSliderCtrl]
Type=0
HeaderFile=DragSliderCtrl.h
ImplementationFile=DragSliderCtrl.cpp
BaseClass=CSliderCtrl
Filter=W
VirtualFilter=KWC

