# Microsoft Developer Studio Project File - Name="Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Library.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Library.mak" CFG="Library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Library - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Library - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "Library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GR /GX /O2 /Ob2 /I "C:\Documents and Settings\Samuel Marshall\My Documents\MyProjects\leafChatUnicode" /D "_MBCS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Library-Release.lib"

!ELSEIF  "$(CFG)" == "Library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /MTd /W3 /GR /GX /Zi /Od /I "C:\Documents and Settings\Samuel Marshall\My Documents\MyProjects\leafChatUnicode" /D "_MBCS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Library-Debug.lib"

!ENDIF 

# Begin Target

# Name "Library - Win32 Release"
# Name "Library - Win32 Debug"
# Begin Source File

SOURCE=.\CompressedResourceData.cpp
# End Source File
# Begin Source File

SOURCE=.\CompressedResourceData.h
# End Source File
# Begin Source File

SOURCE=.\Graphic.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphic.h
# End Source File
# Begin Source File

SOURCE=.\GraphicalDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicalDialog.h
# End Source File
# Begin Source File

SOURCE=.\GraphicalOutputSink.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicalOutputSink.h
# End Source File
# Begin Source File

SOURCE=.\GraphicAlpha.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicAlpha.h
# End Source File
# Begin Source File

SOURCE=.\GraphicAlphaChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicAlphaChannel.h
# End Source File
# Begin Source File

SOURCE=.\GraphicalProducer.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicalProducer.h
# End Source File
# Begin Source File

SOURCE=.\GraphicButton.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicButton.h
# End Source File
# Begin Source File

SOURCE=.\GraphicCheckbox.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicCheckbox.h
# End Source File
# Begin Source File

SOURCE=.\GraphicFont.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicFont.h
# End Source File
# Begin Source File

SOURCE=.\GraphicHScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicHScroll.h
# End Source File
# Begin Source File

SOURCE=.\GraphicListbox.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicListbox.h
# End Source File
# Begin Source File

SOURCE=.\GraphicNumberOption.h
# End Source File
# Begin Source File

SOURCE=.\GraphicOnScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicOnscreen.h
# End Source File
# Begin Source File

SOURCE=.\GraphicOption.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicOption.h
# End Source File
# Begin Source File

SOURCE=.\GraphicScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicScrollbar.h
# End Source File
# Begin Source File

SOURCE=.\GraphicTextOption.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicTextOption.h
# End Source File
# Begin Source File

SOURCE=.\GraphicVScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicVScroll.h
# End Source File
# Begin Source File

SOURCE=.\PaletteDC.cpp
# End Source File
# Begin Source File

SOURCE=.\PaletteDC.h
# End Source File
# Begin Source File

SOURCE=.\ResourceData.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceData.h
# End Source File
# Begin Source File

SOURCE=.\Security.cpp
# End Source File
# Begin Source File

SOURCE=.\Security.h
# End Source File
# Begin Source File

SOURCE=.\TextLineReader.cpp
# End Source File
# Begin Source File

SOURCE=.\TextLineReader.h
# End Source File
# Begin Source File

SOURCE=.\UnicodeDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\UnicodeDisplay.h
# End Source File
# End Target
# End Project
