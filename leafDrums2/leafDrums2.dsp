# Microsoft Developer Studio Project File - Name="leafDrums2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=leafDrums2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "leafDrums2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "leafDrums2.mak" CFG="leafDrums2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "leafDrums2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "leafDrums2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib htmlhelp.lib dxguid.lib ole32.lib msacm32.lib ogg_static.lib vorbis_static.lib vorbisenc_static.lib vorbisfile_static.lib /nologo /subsystem:windows /map /machine:I386

!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib htmlhelp.lib dxguid.lib ole32.lib msacm32.lib ogg_static.lib vorbis_static.lib vorbisenc_static.lib vorbisfile_static.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "leafDrums2 - Win32 Release"
# Name "leafDrums2 - Win32 Debug"
# Begin Source File

SOURCE=.\AudioHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioHandler.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\BandFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\BandFilter.h
# End Source File
# Begin Source File

SOURCE=.\Beat.cpp
# End Source File
# Begin Source File

SOURCE=.\Beat.h
# End Source File
# Begin Source File

SOURCE=.\Block.cpp
# End Source File
# Begin Source File

SOURCE=.\Block.h
# End Source File
# Begin Source File

SOURCE=.\ByteBuffer.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\Chorus.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Chorus.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\Compressor.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Compressor.h
# End Source File
# Begin Source File

SOURCE=.\ControlHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlHandler.h
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00005.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00006.cur
# End Source File
# Begin Source File

SOURCE=.\CustomEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomEdit.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\Delay.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Delay.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\Distortion.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Distortion.h
# End Source File
# Begin Source File

SOURCE=.\DrumsHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\DrumsHandler.h
# End Source File
# Begin Source File

SOURCE=.\DSoundPlayDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\DSoundPlayDevice.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\Effect.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Effect.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\EQ.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\EQ.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\Expander.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Expander.h
# End Source File
# Begin Source File

SOURCE=.\FileHierarchy.cpp
# End Source File
# Begin Source File

SOURCE=.\FileHierarchy.h
# End Source File
# Begin Source File

SOURCE=.\FileSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\FileSystem.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Filter.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\Flanger.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Flanger.h
# End Source File
# Begin Source File

SOURCE=.\FolderOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\FrameAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\FrameAnimation.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\Gate.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Gate.h
# End Source File
# Begin Source File

SOURCE=.\Identifiers.h
# End Source File
# Begin Source File

SOURCE=.\leafDrums.cpp
# End Source File
# Begin Source File

SOURCE=.\leafDrums.h
# End Source File
# Begin Source File

SOURCE=.\res\leafDrums.ico
# End Source File
# Begin Source File

SOURCE=.\leafDrums.rc
# End Source File
# Begin Source File

SOURCE=.\res\leafDrums.rc2
# End Source File
# Begin Source File

SOURCE=.\leafDrumsDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\leafDrumsDoc.h
# End Source File
# Begin Source File

SOURCE=.\res\leafDrumsDoc.ico
# End Source File
# Begin Source File

SOURCE=.\leafDrumsView.cpp
# End Source File
# Begin Source File

SOURCE=.\leafDrumsView.h
# End Source File
# Begin Source File

SOURCE="..\Library\Library-Debug.lib"

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\Library\Library-Release.lib"

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LicenseDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\LicenseDialog.h
# End Source File
# Begin Source File

SOURCE=.\Line.cpp
# End Source File
# Begin Source File

SOURCE=.\Line.h
# End Source File
# Begin Source File

SOURCE=.\LogoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\LogoWnd.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MixBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\MixBuffer.h
# End Source File
# Begin Source File

SOURCE=.\NagDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\NagDialog.h
# End Source File
# Begin Source File

SOURCE=.\OneShotDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\OneShotDialog.h
# End Source File
# Begin Source File

SOURCE=.\Pattern.cpp
# End Source File
# Begin Source File

SOURCE=.\Pattern.h
# End Source File
# Begin Source File

SOURCE=.\PatternHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\PatternHandler.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\Phaser.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Phaser.h
# End Source File
# Begin Source File

SOURCE=.\PlayLimits.h
# End Source File
# Begin Source File

SOURCE=.\res\pointera.cur
# End Source File
# Begin Source File

SOURCE=.\res\pointerb.cur
# End Source File
# Begin Source File

SOURCE=.\FXcode\Quieten.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Quieten.h
# End Source File
# Begin Source File

SOURCE=.\RegisterDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\registerdialog.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Sample.cpp
# End Source File
# Begin Source File

SOURCE=.\Sample.h
# End Source File
# Begin Source File

SOURCE=.\SectionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SectionHandler.h
# End Source File
# Begin Source File

SOURCE=.\SetTempoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTempoDlg.h
# End Source File
# Begin Source File

SOURCE=.\Song.cpp
# End Source File
# Begin Source File

SOURCE=.\Song.h
# End Source File
# Begin Source File

SOURCE=.\SongHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SongHandler.h
# End Source File
# Begin Source File

SOURCE=.\Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\Sound.h
# End Source File
# Begin Source File

SOURCE=.\SoundsHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundsHandler.h
# End Source File
# Begin Source File

SOURCE=.\res\spare.cur
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\Surround.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Surround.h
# End Source File
# Begin Source File

SOURCE=.\Synchronization.h
# End Source File
# Begin Source File

SOURCE=.\Track.cpp
# End Source File
# Begin Source File

SOURCE=.\Track.h
# End Source File
# Begin Source File

SOURCE=.\FXcode\Tremolo.cpp
# End Source File
# Begin Source File

SOURCE=.\FXcode\Tremolo.h
# End Source File
# Begin Source File

SOURCE=.\VariableParameter.cpp
# End Source File
# Begin Source File

SOURCE=.\VariableParameter.h
# End Source File
# Begin Source File

SOURCE=.\vec.h
# End Source File
# Begin Source File

SOURCE=.\WavFile.cpp
# End Source File
# Begin Source File

SOURCE=.\WavFile.h
# End Source File
# Begin Source File

SOURCE=.\WriteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WriteDlg.h
# End Source File
# Begin Source File

SOURCE="..\zlib\zlib-debug.lib"

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\zlib\zlib-release.lib"

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
