; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWriteDlg
LastTemplate=CFileDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "leafdrums.h"
LastPage=0

ClassCount=16
Class1=CLeafDrumsApp
Class2=CAboutDlg
Class3=CLeafDrumsDoc
Class4=CLeafDrumsView
Class5=CLicenseDialog
Class6=CMainFrame
Class7=CNagDialog
Class8=COneShotDialog
Class9=CRegisterDialog
Class10=CWriteDlg

ResourceCount=10
Resource1=IDD_SETTEMPO
Resource2=IDD_FOLDEROPTIONSDIALOG
Resource3=IDD_CUSTOMSAVE
Resource4=IDD_LICENSE (English (U.S.))
Resource5=IDD_REGISTER (English (U.S.))
Resource6=IDR_MAINFRAME (English (U.S.))
Class11=CNewDlg
Resource7=IDD_ONESHOTDLG (English (U.S.))
Class12=CCustomEdit
Class13=CLogoWnd
Resource8=IDD_CUSTOMPRESETOPEN
Class14=CFolderOptionsDlg
Resource9=IDD_NAGDIALOG (English (U.S.))
Class15=CSetTempoDlg
Class16=CWriteFileDialog
Resource10=IDD_WRITE

[CLS:CLeafDrumsApp]
Type=0
BaseClass=CWinApp
HeaderFile=leafDrums.h
ImplementationFile=leafDrums.cpp
LastObject=CLeafDrumsApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=leafDrums.cpp
ImplementationFile=leafDrums.cpp
LastObject=CAboutDlg

[CLS:CLeafDrumsDoc]
Type=0
BaseClass=CDocument
HeaderFile=leafDrumsDoc.h
ImplementationFile=leafDrumsDoc.cpp
Filter=N
VirtualFilter=DC

[CLS:CLeafDrumsView]
Type=0
BaseClass=CView
HeaderFile=leafDrumsView.h
ImplementationFile=leafDrumsView.cpp
Filter=C
VirtualFilter=VWC
LastObject=CLeafDrumsView

[CLS:CLicenseDialog]
Type=0
BaseClass=CDialog
HeaderFile=LicenseDialog.h
ImplementationFile=LicenseDialog.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=CMainFrame

[CLS:CNagDialog]
Type=0
BaseClass=CDialog
HeaderFile=NagDialog.h
ImplementationFile=NagDialog.cpp

[CLS:COneShotDialog]
Type=0
BaseClass=CDialog
HeaderFile=OneShotDialog.h
ImplementationFile=OneShotDialog.cpp

[CLS:CRegisterDialog]
Type=0
BaseClass=CDialog
HeaderFile=registerdialog.h
ImplementationFile=RegisterDialog.cpp

[CLS:CWriteDlg]
Type=0
BaseClass=CDialog
HeaderFile=WriteDlg.h
ImplementationFile=WriteDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_FORMAT

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_LICENSE]
Type=1
Class=CLicenseDialog

[DLG:IDD_NAGDIALOG]
Type=1
Class=CNagDialog

[DLG:IDD_ONESHOTDLG]
Type=1
Class=COneShotDialog

[DLG:IDD_REGISTER]
Type=1
Class=CRegisterDialog

[DLG:IDD_WRITE]
Type=1
Class=CWriteDlg
ControlCount=17
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COUNT,edit,1350639744
Control4=IDC_OUTPUTSTYLE0,button,1342373897
Control5=IDC_OUTPUTSTYLE1,button,1342177289
Control6=IDC_OUTPUTSTYLE2,button,1342177289
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308352
Control11=IDC_FILESIZE,static,1342308352
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342177296
Control16=IDC_STATIC,button,1342177287
Control17=IDC_FORMAT,combobox,1344339971

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_REGISTER
Command5=ID_FILE_SAVE
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_NEXT_PANE
Command10=ID_PREV_PANE
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_EDIT_CUT
Command14=ID_EDIT_UNDO
CommandCount=14

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
CommandCount=0

[DLG:IDD_REGISTER (English (U.S.))]
Type=1
Class=?
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342177296
Control7=IDC_NAMEEDIT,edit,1350631552
Control8=IDC_NUMBEREDIT,edit,1350639744

[DLG:IDD_ONESHOTDLG (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_DONTSHOW,button,1342242819
Control3=IDC_STATIC,static,1342177296
Control4=IDC_TEXT,static,1342308352

[DLG:IDD_NAGDIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=7
Control1=IDC_OTHER,button,1342242816
Control2=IDOK,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342177283
Control6=IDC_STATIC,static,1342308352
Control7=IDC_ORDERONLINE,button,1342242816

[DLG:IDD_LICENSE (English (U.S.))]
Type=1
Class=?
ControlCount=5
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDITLICENSE,edit,1352730628
Control5=IDC_STATIC,static,1342308352

[CLS:CNewDlg]
Type=0
HeaderFile=NewDlg.h
ImplementationFile=NewDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CNewDlg

[CLS:CCustomEdit]
Type=0
HeaderFile=CustomEdit.h
ImplementationFile=CustomEdit.cpp
BaseClass=CEdit
Filter=W
VirtualFilter=WC
LastObject=CCustomEdit

[CLS:CLogoWnd]
Type=0
HeaderFile=LogoWnd.h
ImplementationFile=LogoWnd.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=CLogoWnd

[DLG:IDD_CUSTOMSAVE]
Type=1
Class=?
ControlCount=2
Control1=IDC_EMBEDSOUNDS,button,1342242819
Control2=IDC_STATIC,static,1342308352

[DLG:IDD_FOLDEROPTIONSDIALOG]
Type=1
Class=CFolderOptionsDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_FOLDERLIST,listbox,1352728833
Control5=IDC_ADD,button,1342242816
Control6=IDC_DEL,button,1342242816
Control7=IDC_SETDEFAULT,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_FOLDEREDIT,edit,1350631552

[CLS:CFolderOptionsDlg]
Type=0
HeaderFile=FolderOptionsDlg.h
ImplementationFile=FolderOptionsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CFolderOptionsDlg
VirtualFilter=dWC

[DLG:IDD_CUSTOMPRESETOPEN]
Type=1
Class=?
ControlCount=7
Control1=IDC_STATIC,button,1342177287
Control2=IDC_SOUND,button,1342242819
Control3=IDC_ENVELOPE,button,1342242819
Control4=IDC_TUNING,button,1342242819
Control5=IDC_EFFECTS,button,1342242819
Control6=IDC_STRETCH,button,1342242819
Control7=IDC_ACCENTPAN,button,1342242819

[DLG:IDD_SETTEMPO]
Type=1
Class=CSetTempoDlg
ControlCount=11
Control1=IDC_TEMPO,edit,1350631552
Control2=IDC_TIMESIGNATURE,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_APPLY,button,1342242816
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352

[CLS:CSetTempoDlg]
Type=0
HeaderFile=SetTempoDlg.h
ImplementationFile=SetTempoDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_TEMPO

[CLS:CWriteFileDialog]
Type=0
HeaderFile=WriteFileDialog.h
ImplementationFile=WriteFileDialog.cpp
BaseClass=CFileDialog
Filter=D
LastObject=CWriteFileDialog

