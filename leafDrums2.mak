# Microsoft Developer Studio Generated NMAKE File, Based on leafDrums2.dsp
!IF "$(CFG)" == ""
CFG=leafDrums2 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to leafDrums2 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "leafDrums2 - Win32 Release" && "$(CFG)" !=\
 "leafDrums2 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\leafDrums2.exe"

!ELSE 

ALL : "Library - Win32 Release" "$(OUTDIR)\leafDrums2.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Library - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AudioHandler.obj"
	-@erase "$(INTDIR)\BandFilter.obj"
	-@erase "$(INTDIR)\Beat.obj"
	-@erase "$(INTDIR)\Block.obj"
	-@erase "$(INTDIR)\Chorus.obj"
	-@erase "$(INTDIR)\Compressor.obj"
	-@erase "$(INTDIR)\ControlHandler.obj"
	-@erase "$(INTDIR)\CustomEdit.obj"
	-@erase "$(INTDIR)\Delay.obj"
	-@erase "$(INTDIR)\Distortion.obj"
	-@erase "$(INTDIR)\DrumsHandler.obj"
	-@erase "$(INTDIR)\DSoundPlayDevice.obj"
	-@erase "$(INTDIR)\Effect.obj"
	-@erase "$(INTDIR)\EQ.obj"
	-@erase "$(INTDIR)\Expander.obj"
	-@erase "$(INTDIR)\FileHierarchy.obj"
	-@erase "$(INTDIR)\FileSystem.obj"
	-@erase "$(INTDIR)\Filter.obj"
	-@erase "$(INTDIR)\Flanger.obj"
	-@erase "$(INTDIR)\FolderOptionsDlg.obj"
	-@erase "$(INTDIR)\FrameAnimation.obj"
	-@erase "$(INTDIR)\Gate.obj"
	-@erase "$(INTDIR)\leafDrums.obj"
	-@erase "$(INTDIR)\leafDrums.res"
	-@erase "$(INTDIR)\leafDrumsDoc.obj"
	-@erase "$(INTDIR)\leafDrumsView.obj"
	-@erase "$(INTDIR)\LicenseDialog.obj"
	-@erase "$(INTDIR)\Line.obj"
	-@erase "$(INTDIR)\LogoWnd.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MixBuffer.obj"
	-@erase "$(INTDIR)\NagDialog.obj"
	-@erase "$(INTDIR)\NewDlg.obj"
	-@erase "$(INTDIR)\OneShotDialog.obj"
	-@erase "$(INTDIR)\Pattern.obj"
	-@erase "$(INTDIR)\PatternHandler.obj"
	-@erase "$(INTDIR)\Phaser.obj"
	-@erase "$(INTDIR)\Quieten.obj"
	-@erase "$(INTDIR)\RegisterDialog.obj"
	-@erase "$(INTDIR)\Sample.obj"
	-@erase "$(INTDIR)\SectionHandler.obj"
	-@erase "$(INTDIR)\Song.obj"
	-@erase "$(INTDIR)\SongHandler.obj"
	-@erase "$(INTDIR)\Sound.obj"
	-@erase "$(INTDIR)\SoundsHandler.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Surround.obj"
	-@erase "$(INTDIR)\Track.obj"
	-@erase "$(INTDIR)\Tremolo.obj"
	-@erase "$(INTDIR)\VariableParameter.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\WavFile.obj"
	-@erase "$(INTDIR)\WriteDlg.obj"
	-@erase "$(OUTDIR)\leafDrums2.exe"
	-@erase "$(OUTDIR)\leafDrums2.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\leafDrums2.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x809 /fo"$(INTDIR)\leafDrums.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\leafDrums2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib htmlhelp.lib dxguid.lib ole32.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\leafDrums2.pdb"\
 /map:"$(INTDIR)\leafDrums2.map" /machine:I386 /out:"$(OUTDIR)\leafDrums2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AudioHandler.obj" \
	"$(INTDIR)\BandFilter.obj" \
	"$(INTDIR)\Beat.obj" \
	"$(INTDIR)\Block.obj" \
	"$(INTDIR)\Chorus.obj" \
	"$(INTDIR)\Compressor.obj" \
	"$(INTDIR)\ControlHandler.obj" \
	"$(INTDIR)\CustomEdit.obj" \
	"$(INTDIR)\Delay.obj" \
	"$(INTDIR)\Distortion.obj" \
	"$(INTDIR)\DrumsHandler.obj" \
	"$(INTDIR)\DSoundPlayDevice.obj" \
	"$(INTDIR)\Effect.obj" \
	"$(INTDIR)\EQ.obj" \
	"$(INTDIR)\Expander.obj" \
	"$(INTDIR)\FileHierarchy.obj" \
	"$(INTDIR)\FileSystem.obj" \
	"$(INTDIR)\Filter.obj" \
	"$(INTDIR)\Flanger.obj" \
	"$(INTDIR)\FolderOptionsDlg.obj" \
	"$(INTDIR)\FrameAnimation.obj" \
	"$(INTDIR)\Gate.obj" \
	"$(INTDIR)\leafDrums.obj" \
	"$(INTDIR)\leafDrums.res" \
	"$(INTDIR)\leafDrumsDoc.obj" \
	"$(INTDIR)\leafDrumsView.obj" \
	"$(INTDIR)\LicenseDialog.obj" \
	"$(INTDIR)\Line.obj" \
	"$(INTDIR)\LogoWnd.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MixBuffer.obj" \
	"$(INTDIR)\NagDialog.obj" \
	"$(INTDIR)\NewDlg.obj" \
	"$(INTDIR)\OneShotDialog.obj" \
	"$(INTDIR)\Pattern.obj" \
	"$(INTDIR)\PatternHandler.obj" \
	"$(INTDIR)\Phaser.obj" \
	"$(INTDIR)\Quieten.obj" \
	"$(INTDIR)\RegisterDialog.obj" \
	"$(INTDIR)\Sample.obj" \
	"$(INTDIR)\SectionHandler.obj" \
	"$(INTDIR)\Song.obj" \
	"$(INTDIR)\SongHandler.obj" \
	"$(INTDIR)\Sound.obj" \
	"$(INTDIR)\SoundsHandler.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Surround.obj" \
	"$(INTDIR)\Track.obj" \
	"$(INTDIR)\Tremolo.obj" \
	"$(INTDIR)\VariableParameter.obj" \
	"$(INTDIR)\WavFile.obj" \
	"$(INTDIR)\WriteDlg.obj" \
	"..\Library\Library-Release.lib" \
	"..\zlib\zlib-release.lib"

"$(OUTDIR)\leafDrums2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\leafDrums2.exe" "$(OUTDIR)\leafDrums2.bsc"

!ELSE 

ALL : "Library - Win32 Debug" "$(OUTDIR)\leafDrums2.exe"\
 "$(OUTDIR)\leafDrums2.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Library - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AudioHandler.obj"
	-@erase "$(INTDIR)\AudioHandler.sbr"
	-@erase "$(INTDIR)\BandFilter.obj"
	-@erase "$(INTDIR)\BandFilter.sbr"
	-@erase "$(INTDIR)\Beat.obj"
	-@erase "$(INTDIR)\Beat.sbr"
	-@erase "$(INTDIR)\Block.obj"
	-@erase "$(INTDIR)\Block.sbr"
	-@erase "$(INTDIR)\Chorus.obj"
	-@erase "$(INTDIR)\Chorus.sbr"
	-@erase "$(INTDIR)\Compressor.obj"
	-@erase "$(INTDIR)\Compressor.sbr"
	-@erase "$(INTDIR)\ControlHandler.obj"
	-@erase "$(INTDIR)\ControlHandler.sbr"
	-@erase "$(INTDIR)\CustomEdit.obj"
	-@erase "$(INTDIR)\CustomEdit.sbr"
	-@erase "$(INTDIR)\Delay.obj"
	-@erase "$(INTDIR)\Delay.sbr"
	-@erase "$(INTDIR)\Distortion.obj"
	-@erase "$(INTDIR)\Distortion.sbr"
	-@erase "$(INTDIR)\DrumsHandler.obj"
	-@erase "$(INTDIR)\DrumsHandler.sbr"
	-@erase "$(INTDIR)\DSoundPlayDevice.obj"
	-@erase "$(INTDIR)\DSoundPlayDevice.sbr"
	-@erase "$(INTDIR)\Effect.obj"
	-@erase "$(INTDIR)\Effect.sbr"
	-@erase "$(INTDIR)\EQ.obj"
	-@erase "$(INTDIR)\EQ.sbr"
	-@erase "$(INTDIR)\Expander.obj"
	-@erase "$(INTDIR)\Expander.sbr"
	-@erase "$(INTDIR)\FileHierarchy.obj"
	-@erase "$(INTDIR)\FileHierarchy.sbr"
	-@erase "$(INTDIR)\FileSystem.obj"
	-@erase "$(INTDIR)\FileSystem.sbr"
	-@erase "$(INTDIR)\Filter.obj"
	-@erase "$(INTDIR)\Filter.sbr"
	-@erase "$(INTDIR)\Flanger.obj"
	-@erase "$(INTDIR)\Flanger.sbr"
	-@erase "$(INTDIR)\FolderOptionsDlg.obj"
	-@erase "$(INTDIR)\FolderOptionsDlg.sbr"
	-@erase "$(INTDIR)\FrameAnimation.obj"
	-@erase "$(INTDIR)\FrameAnimation.sbr"
	-@erase "$(INTDIR)\Gate.obj"
	-@erase "$(INTDIR)\Gate.sbr"
	-@erase "$(INTDIR)\leafDrums.obj"
	-@erase "$(INTDIR)\leafDrums.res"
	-@erase "$(INTDIR)\leafDrums.sbr"
	-@erase "$(INTDIR)\leafDrumsDoc.obj"
	-@erase "$(INTDIR)\leafDrumsDoc.sbr"
	-@erase "$(INTDIR)\leafDrumsView.obj"
	-@erase "$(INTDIR)\leafDrumsView.sbr"
	-@erase "$(INTDIR)\LicenseDialog.obj"
	-@erase "$(INTDIR)\LicenseDialog.sbr"
	-@erase "$(INTDIR)\Line.obj"
	-@erase "$(INTDIR)\Line.sbr"
	-@erase "$(INTDIR)\LogoWnd.obj"
	-@erase "$(INTDIR)\LogoWnd.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MixBuffer.obj"
	-@erase "$(INTDIR)\MixBuffer.sbr"
	-@erase "$(INTDIR)\NagDialog.obj"
	-@erase "$(INTDIR)\NagDialog.sbr"
	-@erase "$(INTDIR)\NewDlg.obj"
	-@erase "$(INTDIR)\NewDlg.sbr"
	-@erase "$(INTDIR)\OneShotDialog.obj"
	-@erase "$(INTDIR)\OneShotDialog.sbr"
	-@erase "$(INTDIR)\Pattern.obj"
	-@erase "$(INTDIR)\Pattern.sbr"
	-@erase "$(INTDIR)\PatternHandler.obj"
	-@erase "$(INTDIR)\PatternHandler.sbr"
	-@erase "$(INTDIR)\Phaser.obj"
	-@erase "$(INTDIR)\Phaser.sbr"
	-@erase "$(INTDIR)\Quieten.obj"
	-@erase "$(INTDIR)\Quieten.sbr"
	-@erase "$(INTDIR)\RegisterDialog.obj"
	-@erase "$(INTDIR)\RegisterDialog.sbr"
	-@erase "$(INTDIR)\Sample.obj"
	-@erase "$(INTDIR)\Sample.sbr"
	-@erase "$(INTDIR)\SectionHandler.obj"
	-@erase "$(INTDIR)\SectionHandler.sbr"
	-@erase "$(INTDIR)\Song.obj"
	-@erase "$(INTDIR)\Song.sbr"
	-@erase "$(INTDIR)\SongHandler.obj"
	-@erase "$(INTDIR)\SongHandler.sbr"
	-@erase "$(INTDIR)\Sound.obj"
	-@erase "$(INTDIR)\Sound.sbr"
	-@erase "$(INTDIR)\SoundsHandler.obj"
	-@erase "$(INTDIR)\SoundsHandler.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Surround.obj"
	-@erase "$(INTDIR)\Surround.sbr"
	-@erase "$(INTDIR)\Track.obj"
	-@erase "$(INTDIR)\Track.sbr"
	-@erase "$(INTDIR)\Tremolo.obj"
	-@erase "$(INTDIR)\Tremolo.sbr"
	-@erase "$(INTDIR)\VariableParameter.obj"
	-@erase "$(INTDIR)\VariableParameter.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\WavFile.obj"
	-@erase "$(INTDIR)\WavFile.sbr"
	-@erase "$(INTDIR)\WriteDlg.obj"
	-@erase "$(INTDIR)\WriteDlg.sbr"
	-@erase "$(OUTDIR)\leafDrums2.bsc"
	-@erase "$(OUTDIR)\leafDrums2.exe"
	-@erase "$(OUTDIR)\leafDrums2.ilk"
	-@erase "$(OUTDIR)\leafDrums2.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\leafDrums2.pch" /YX"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x809 /fo"$(INTDIR)\leafDrums.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\leafDrums2.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AudioHandler.sbr" \
	"$(INTDIR)\BandFilter.sbr" \
	"$(INTDIR)\Beat.sbr" \
	"$(INTDIR)\Block.sbr" \
	"$(INTDIR)\Chorus.sbr" \
	"$(INTDIR)\Compressor.sbr" \
	"$(INTDIR)\ControlHandler.sbr" \
	"$(INTDIR)\CustomEdit.sbr" \
	"$(INTDIR)\Delay.sbr" \
	"$(INTDIR)\Distortion.sbr" \
	"$(INTDIR)\DrumsHandler.sbr" \
	"$(INTDIR)\DSoundPlayDevice.sbr" \
	"$(INTDIR)\Effect.sbr" \
	"$(INTDIR)\EQ.sbr" \
	"$(INTDIR)\Expander.sbr" \
	"$(INTDIR)\FileHierarchy.sbr" \
	"$(INTDIR)\FileSystem.sbr" \
	"$(INTDIR)\Filter.sbr" \
	"$(INTDIR)\Flanger.sbr" \
	"$(INTDIR)\FolderOptionsDlg.sbr" \
	"$(INTDIR)\FrameAnimation.sbr" \
	"$(INTDIR)\Gate.sbr" \
	"$(INTDIR)\leafDrums.sbr" \
	"$(INTDIR)\leafDrumsDoc.sbr" \
	"$(INTDIR)\leafDrumsView.sbr" \
	"$(INTDIR)\LicenseDialog.sbr" \
	"$(INTDIR)\Line.sbr" \
	"$(INTDIR)\LogoWnd.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\MixBuffer.sbr" \
	"$(INTDIR)\NagDialog.sbr" \
	"$(INTDIR)\NewDlg.sbr" \
	"$(INTDIR)\OneShotDialog.sbr" \
	"$(INTDIR)\Pattern.sbr" \
	"$(INTDIR)\PatternHandler.sbr" \
	"$(INTDIR)\Phaser.sbr" \
	"$(INTDIR)\Quieten.sbr" \
	"$(INTDIR)\RegisterDialog.sbr" \
	"$(INTDIR)\Sample.sbr" \
	"$(INTDIR)\SectionHandler.sbr" \
	"$(INTDIR)\Song.sbr" \
	"$(INTDIR)\SongHandler.sbr" \
	"$(INTDIR)\Sound.sbr" \
	"$(INTDIR)\SoundsHandler.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Surround.sbr" \
	"$(INTDIR)\Track.sbr" \
	"$(INTDIR)\Tremolo.sbr" \
	"$(INTDIR)\VariableParameter.sbr" \
	"$(INTDIR)\WavFile.sbr" \
	"$(INTDIR)\WriteDlg.sbr"

"$(OUTDIR)\leafDrums2.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib htmlhelp.lib dxguid.lib ole32.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\leafDrums2.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)\leafDrums2.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AudioHandler.obj" \
	"$(INTDIR)\BandFilter.obj" \
	"$(INTDIR)\Beat.obj" \
	"$(INTDIR)\Block.obj" \
	"$(INTDIR)\Chorus.obj" \
	"$(INTDIR)\Compressor.obj" \
	"$(INTDIR)\ControlHandler.obj" \
	"$(INTDIR)\CustomEdit.obj" \
	"$(INTDIR)\Delay.obj" \
	"$(INTDIR)\Distortion.obj" \
	"$(INTDIR)\DrumsHandler.obj" \
	"$(INTDIR)\DSoundPlayDevice.obj" \
	"$(INTDIR)\Effect.obj" \
	"$(INTDIR)\EQ.obj" \
	"$(INTDIR)\Expander.obj" \
	"$(INTDIR)\FileHierarchy.obj" \
	"$(INTDIR)\FileSystem.obj" \
	"$(INTDIR)\Filter.obj" \
	"$(INTDIR)\Flanger.obj" \
	"$(INTDIR)\FolderOptionsDlg.obj" \
	"$(INTDIR)\FrameAnimation.obj" \
	"$(INTDIR)\Gate.obj" \
	"$(INTDIR)\leafDrums.obj" \
	"$(INTDIR)\leafDrums.res" \
	"$(INTDIR)\leafDrumsDoc.obj" \
	"$(INTDIR)\leafDrumsView.obj" \
	"$(INTDIR)\LicenseDialog.obj" \
	"$(INTDIR)\Line.obj" \
	"$(INTDIR)\LogoWnd.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MixBuffer.obj" \
	"$(INTDIR)\NagDialog.obj" \
	"$(INTDIR)\NewDlg.obj" \
	"$(INTDIR)\OneShotDialog.obj" \
	"$(INTDIR)\Pattern.obj" \
	"$(INTDIR)\PatternHandler.obj" \
	"$(INTDIR)\Phaser.obj" \
	"$(INTDIR)\Quieten.obj" \
	"$(INTDIR)\RegisterDialog.obj" \
	"$(INTDIR)\Sample.obj" \
	"$(INTDIR)\SectionHandler.obj" \
	"$(INTDIR)\Song.obj" \
	"$(INTDIR)\SongHandler.obj" \
	"$(INTDIR)\Sound.obj" \
	"$(INTDIR)\SoundsHandler.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Surround.obj" \
	"$(INTDIR)\Track.obj" \
	"$(INTDIR)\Tremolo.obj" \
	"$(INTDIR)\VariableParameter.obj" \
	"$(INTDIR)\WavFile.obj" \
	"$(INTDIR)\WriteDlg.obj" \
	"..\Library\Library-Debug.lib" \
	"..\zlib\zlib-debug.lib"

"$(OUTDIR)\leafDrums2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "leafDrums2 - Win32 Release" || "$(CFG)" ==\
 "leafDrums2 - Win32 Debug"

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

"Library - Win32 Release" : 
   cd "\Documents and Settings\Samuel Marshall\My Documents\MyProjects\Library"
   $(MAKE) /$(MAKEFLAGS) /F ".\Library.mak" CFG="Library - Win32 Release" 
   cd "..\leafDrums2"

"Library - Win32 ReleaseCLEAN" : 
   cd "\Documents and Settings\Samuel Marshall\My Documents\MyProjects\Library"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F ".\Library.mak" CFG="Library - Win32 Release"\
 RECURSE=1 
   cd "..\leafDrums2"

!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

"Library - Win32 Debug" : 
   cd "\Documents and Settings\Samuel Marshall\My Documents\MyProjects\Library"
   $(MAKE) /$(MAKEFLAGS) /F ".\Library.mak" CFG="Library - Win32 Debug" 
   cd "..\leafDrums2"

"Library - Win32 DebugCLEAN" : 
   cd "\Documents and Settings\Samuel Marshall\My Documents\MyProjects\Library"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F ".\Library.mak" CFG="Library - Win32 Debug"\
 RECURSE=1 
   cd "..\leafDrums2"

!ENDIF 

SOURCE=.\AudioHandler.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_AUDIO=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\AudioHandler.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\DrumsHandler.h"\
	".\DSoundPlayDevice.h"\
	".\FileHierarchy.h"\
	".\FXcode\BandFilter.h"\
	".\FXcode\Chorus.h"\
	".\FXcode\Compressor.h"\
	".\FXcode\Delay.h"\
	".\FXcode\Distortion.h"\
	".\FXcode\Effect.h"\
	".\FXcode\EQ.h"\
	".\FXcode\Expander.h"\
	".\FXcode\Filter.h"\
	".\FXcode\Flanger.h"\
	".\FXcode\Gate.h"\
	".\FXcode\Phaser.h"\
	".\FXcode\Quieten.h"\
	".\FXcode\Surround.h"\
	".\FXcode\Tremolo.h"\
	".\Identifiers.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\Sound.h"\
	".\SoundsHandler.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\AudioHandler.obj" : $(SOURCE) $(DEP_CPP_AUDIO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_AUDIO=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\AudioHandler.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\DrumsHandler.h"\
	".\DSoundPlayDevice.h"\
	".\FileHierarchy.h"\
	".\FXcode\BandFilter.h"\
	".\FXcode\Chorus.h"\
	".\FXcode\Compressor.h"\
	".\FXcode\Delay.h"\
	".\FXcode\Distortion.h"\
	".\FXcode\Effect.h"\
	".\FXcode\EQ.h"\
	".\FXcode\Expander.h"\
	".\FXcode\Filter.h"\
	".\FXcode\Flanger.h"\
	".\FXcode\Gate.h"\
	".\FXcode\Phaser.h"\
	".\FXcode\Quieten.h"\
	".\FXcode\Surround.h"\
	".\FXcode\Tremolo.h"\
	".\Identifiers.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\Sound.h"\
	".\SoundsHandler.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\AudioHandler.obj"	"$(INTDIR)\AudioHandler.sbr" : $(SOURCE)\
 $(DEP_CPP_AUDIO) "$(INTDIR)"


!ENDIF 

SOURCE=.\FXcode\BandFilter.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_BANDF=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\BandFilter.h"\
	".\FXcode\Effect.h"\
	

"$(INTDIR)\BandFilter.obj" : $(SOURCE) $(DEP_CPP_BANDF) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_BANDF=\
	".\Block.h"\
	".\FXcode\BandFilter.h"\
	".\FXcode\Effect.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BandFilter.obj"	"$(INTDIR)\BandFilter.sbr" : $(SOURCE)\
 $(DEP_CPP_BANDF) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Beat.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_BEAT_=\
	".\Beat.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\Beat.obj" : $(SOURCE) $(DEP_CPP_BEAT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_BEAT_=\
	".\Beat.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\Beat.obj"	"$(INTDIR)\Beat.sbr" : $(SOURCE) $(DEP_CPP_BEAT_)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\Block.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_BLOCK=\
	".\Block.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Block.obj" : $(SOURCE) $(DEP_CPP_BLOCK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_BLOCK=\
	".\Block.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Block.obj"	"$(INTDIR)\Block.sbr" : $(SOURCE) $(DEP_CPP_BLOCK)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\FXcode\Chorus.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_CHORU=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\Chorus.h"\
	".\FXcode\Effect.h"\
	

"$(INTDIR)\Chorus.obj" : $(SOURCE) $(DEP_CPP_CHORU) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_CHORU=\
	".\Block.h"\
	".\FXcode\Chorus.h"\
	".\FXcode\Effect.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Chorus.obj"	"$(INTDIR)\Chorus.sbr" : $(SOURCE) $(DEP_CPP_CHORU)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\FXcode\Compressor.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_COMPR=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\Compressor.h"\
	".\FXcode\Effect.h"\
	

"$(INTDIR)\Compressor.obj" : $(SOURCE) $(DEP_CPP_COMPR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_COMPR=\
	".\Block.h"\
	".\FXcode\Compressor.h"\
	".\FXcode\Effect.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Compressor.obj"	"$(INTDIR)\Compressor.sbr" : $(SOURCE)\
 $(DEP_CPP_COMPR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\ControlHandler.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_CONTR=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\ControlHandler.h"\
	".\CustomEdit.h"\
	".\FXcode\Effect.h"\
	".\Identifiers.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\NewDlg.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\SongHandler.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	".\WriteDlg.h"\
	

"$(INTDIR)\ControlHandler.obj" : $(SOURCE) $(DEP_CPP_CONTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_CONTR=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\ControlHandler.h"\
	".\CustomEdit.h"\
	".\FXcode\Effect.h"\
	".\Identifiers.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\NewDlg.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\SongHandler.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	".\WriteDlg.h"\
	

"$(INTDIR)\ControlHandler.obj"	"$(INTDIR)\ControlHandler.sbr" : $(SOURCE)\
 $(DEP_CPP_CONTR) "$(INTDIR)"


!ENDIF 

SOURCE=.\CustomEdit.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_CUSTO=\
	".\CustomEdit.h"\
	".\leafDrums.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CustomEdit.obj" : $(SOURCE) $(DEP_CPP_CUSTO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_CUSTO=\
	".\CustomEdit.h"\
	".\leafDrums.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CustomEdit.obj"	"$(INTDIR)\CustomEdit.sbr" : $(SOURCE)\
 $(DEP_CPP_CUSTO) "$(INTDIR)"


!ENDIF 

SOURCE=.\FXcode\Delay.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_DELAY=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\Delay.h"\
	".\FXcode\Effect.h"\
	

"$(INTDIR)\Delay.obj" : $(SOURCE) $(DEP_CPP_DELAY) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_DELAY=\
	".\Block.h"\
	".\FXcode\Delay.h"\
	".\FXcode\Effect.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Delay.obj"	"$(INTDIR)\Delay.sbr" : $(SOURCE) $(DEP_CPP_DELAY)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\FXcode\Distortion.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_DISTO=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\Distortion.h"\
	".\FXcode\Effect.h"\
	

"$(INTDIR)\Distortion.obj" : $(SOURCE) $(DEP_CPP_DISTO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_DISTO=\
	".\Block.h"\
	".\FXcode\Distortion.h"\
	".\FXcode\Effect.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Distortion.obj"	"$(INTDIR)\Distortion.sbr" : $(SOURCE)\
 $(DEP_CPP_DISTO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\DrumsHandler.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_DRUMS=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\DrumsHandler.h"\
	".\FileHierarchy.h"\
	".\FXcode\Effect.h"\
	".\Identifiers.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\PatternHandler.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\Sound.h"\
	".\SoundsHandler.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\DrumsHandler.obj" : $(SOURCE) $(DEP_CPP_DRUMS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_DRUMS=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\DrumsHandler.h"\
	".\FileHierarchy.h"\
	".\FXcode\Effect.h"\
	".\Identifiers.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\PatternHandler.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\Sound.h"\
	".\SoundsHandler.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\DrumsHandler.obj"	"$(INTDIR)\DrumsHandler.sbr" : $(SOURCE)\
 $(DEP_CPP_DRUMS) "$(INTDIR)"


!ENDIF 

SOURCE=.\DSoundPlayDevice.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_DSOUN=\
	".\Block.h"\
	".\DSoundPlayDevice.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DSoundPlayDevice.obj" : $(SOURCE) $(DEP_CPP_DSOUN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_DSOUN=\
	".\Block.h"\
	".\DSoundPlayDevice.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DSoundPlayDevice.obj"	"$(INTDIR)\DSoundPlayDevice.sbr" : $(SOURCE)\
 $(DEP_CPP_DSOUN) "$(INTDIR)"


!ENDIF 

SOURCE=.\FXcode\Effect.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_EFFEC=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	

"$(INTDIR)\Effect.obj" : $(SOURCE) $(DEP_CPP_EFFEC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_EFFEC=\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Effect.obj"	"$(INTDIR)\Effect.sbr" : $(SOURCE) $(DEP_CPP_EFFEC)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\FXcode\EQ.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_EQ_CP=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\EQ.h"\
	

"$(INTDIR)\EQ.obj" : $(SOURCE) $(DEP_CPP_EQ_CP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_EQ_CP=\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\EQ.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\EQ.obj"	"$(INTDIR)\EQ.sbr" : $(SOURCE) $(DEP_CPP_EQ_CP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\FXcode\Expander.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_EXPAN=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Expander.h"\
	

"$(INTDIR)\Expander.obj" : $(SOURCE) $(DEP_CPP_EXPAN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_EXPAN=\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Expander.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Expander.obj"	"$(INTDIR)\Expander.sbr" : $(SOURCE) $(DEP_CPP_EXPAN)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\FileHierarchy.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_FILEH=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\FileHierarchy.h"\
	".\FileSystem.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\FileHierarchy.obj" : $(SOURCE) $(DEP_CPP_FILEH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_FILEH=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\FileHierarchy.h"\
	".\FileSystem.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\FileHierarchy.obj"	"$(INTDIR)\FileHierarchy.sbr" : $(SOURCE)\
 $(DEP_CPP_FILEH) "$(INTDIR)"


!ENDIF 

SOURCE=.\FileSystem.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_FILES=\
	".\Beat.h"\
	".\Block.h"\
	".\FileSystem.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\FileSystem.obj" : $(SOURCE) $(DEP_CPP_FILES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_FILES=\
	".\Beat.h"\
	".\Block.h"\
	".\FileSystem.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\FileSystem.obj"	"$(INTDIR)\FileSystem.sbr" : $(SOURCE)\
 $(DEP_CPP_FILES) "$(INTDIR)"


!ENDIF 

SOURCE=.\FXcode\Filter.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_FILTE=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Filter.h"\
	

"$(INTDIR)\Filter.obj" : $(SOURCE) $(DEP_CPP_FILTE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_FILTE=\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Filter.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Filter.obj"	"$(INTDIR)\Filter.sbr" : $(SOURCE) $(DEP_CPP_FILTE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\FXcode\Flanger.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_FLANG=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Flanger.h"\
	

"$(INTDIR)\Flanger.obj" : $(SOURCE) $(DEP_CPP_FLANG) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_FLANG=\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Flanger.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Flanger.obj"	"$(INTDIR)\Flanger.sbr" : $(SOURCE) $(DEP_CPP_FLANG)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\FolderOptionsDlg.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_FOLDE=\
	".\Beat.h"\
	".\Block.h"\
	".\FileSystem.h"\
	".\FolderOptionsDlg.h"\
	".\FXcode\Effect.h"\
	".\leafDrums.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\FolderOptionsDlg.obj" : $(SOURCE) $(DEP_CPP_FOLDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_FOLDE=\
	".\Beat.h"\
	".\Block.h"\
	".\FileSystem.h"\
	".\FolderOptionsDlg.h"\
	".\FXcode\Effect.h"\
	".\leafDrums.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\FolderOptionsDlg.obj"	"$(INTDIR)\FolderOptionsDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_FOLDE) "$(INTDIR)"


!ENDIF 

SOURCE=.\FrameAnimation.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_FRAME=\
	".\FrameAnimation.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FrameAnimation.obj" : $(SOURCE) $(DEP_CPP_FRAME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_FRAME=\
	".\FrameAnimation.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FrameAnimation.obj"	"$(INTDIR)\FrameAnimation.sbr" : $(SOURCE)\
 $(DEP_CPP_FRAME) "$(INTDIR)"


!ENDIF 

SOURCE=.\FXcode\Gate.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_GATE_=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Gate.h"\
	

"$(INTDIR)\Gate.obj" : $(SOURCE) $(DEP_CPP_GATE_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_GATE_=\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Gate.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Gate.obj"	"$(INTDIR)\Gate.sbr" : $(SOURCE) $(DEP_CPP_GATE_)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\leafDrums.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_LEAFD=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\PaletteDC.h"\
	"..\Library\Security.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\FileSystem.h"\
	".\FXcode\Effect.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\LicenseDialog.h"\
	".\Line.h"\
	".\LogoWnd.h"\
	".\MainFrm.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\leafDrums.obj" : $(SOURCE) $(DEP_CPP_LEAFD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_LEAFD=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\PaletteDC.h"\
	"..\Library\Security.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\FileSystem.h"\
	".\FXcode\Effect.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\LicenseDialog.h"\
	".\Line.h"\
	".\LogoWnd.h"\
	".\MainFrm.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\leafDrums.obj"	"$(INTDIR)\leafDrums.sbr" : $(SOURCE)\
 $(DEP_CPP_LEAFD) "$(INTDIR)"


!ENDIF 

SOURCE=.\leafDrums.rc
DEP_RSC_LEAFDR=\
	".\res\CloseButtonD.raw_c"\
	".\res\CloseButtonO.raw_c"\
	".\res\CloseButtonU.raw_c"\
	".\res\Control4_a.bmp_C"\
	".\res\Control4_rgb.bmp_C"\
	".\res\Control5_a.bmp_C"\
	".\res\Control5_rgb.bmp_C"\
	".\res\Control6_a.bmp_C"\
	".\res\Control6_rgb.bmp_C"\
	".\res\Control7_a.bmp_C"\
	".\res\Control7_rgb.bmp_C"\
	".\res\Control8_a.bmp_C"\
	".\res\Control8_rgb.bmp_C"\
	".\res\ControlBar_Background.bmp_C"\
	".\res\ControlBar_Buttons_a.bmp_C"\
	".\res\ControlBar_Buttons_rgb.bmp_C"\
	".\res\ControlBar_ButtonsOver_a.bmp_C"\
	".\res\ControlBar_ButtonsOver_rgb.bmp_C"\
	".\res\ControlBar_ButtonsPressed_a.bmp_C"\
	".\res\ControlBar_ButtonsPressed_rgb.bmp_C"\
	".\res\Copyright.bmp_C"\
	".\res\cur00001.cur"\
	".\res\cur00002.cur"\
	".\res\cur00003.cur"\
	".\res\cur00004.cur"\
	".\res\cur00005.cur"\
	".\res\cur00006.cur"\
	".\res\DrumScreen_Background.bmp_C"\
	".\res\DrumScreen_Buttons_a.bmp_C"\
	".\res\DrumScreen_Buttons_rgb.bmp_C"\
	".\res\DrumScreen_ButtonsAlt_a.bmp_C"\
	".\res\DrumScreen_ButtonsAlt_rgb.bmp_C"\
	".\res\DrumScreen_ButtonsOver_a.bmp_C"\
	".\res\DrumScreen_ButtonsOver_rgb.bmp_C"\
	".\res\DrumScreen_ButtonsPressed_a.bmp_C"\
	".\res\DrumScreen_ButtonsPressed_rgb.bmp_C"\
	".\res\DrumScreen_ButtonsPressedAlt_a.bmp_C"\
	".\res\DrumScreen_ButtonsPressedAlt_rgb.bmp_C"\
	".\res\global.pal"\
	".\res\leafDrums.ico"\
	".\res\leafDrums.rc2"\
	".\res\Logo.bmp_C"\
	".\res\MaxButtonDis.raw_c"\
	".\res\MinButtonD.raw_c"\
	".\res\MinButtonO.raw_c"\
	".\res\MinButtonU.raw_c"\
	".\res\Pattern_AccentHit_a.bmp_C"\
	".\res\Pattern_AccentHit_rgb.bmp_C"\
	".\res\Pattern_MouseOver_a.bmp_C"\
	".\res\Pattern_MouseOver_rgb.bmp_C"\
	".\res\Pattern_NormalHit_a.bmp_C"\
	".\res\Pattern_NormalHit_rgb.bmp_C"\
	".\res\pointera.cur"\
	".\res\pointerb.cur"\
	".\res\RepeatColour_a.bmp_C"\
	".\res\RepeatColour_rgb.bmp_C"\
	".\res\RepeatGrey_a.bmp_C"\
	".\res\RepeatGrey_rgb.bmp_C"\
	".\res\ScrollNormal_a.bmp_C"\
	".\res\ScrollNormal_rgb.bmp_C"\
	".\res\ScrollThumb_Mid_a.bmp_C"\
	".\res\ScrollThumb_Mid_rgb.bmp_C"\
	".\res\ScrollThumb_Top_a.bmp_C"\
	".\res\ScrollThumb_Top_rgb.bmp_C"\
	".\res\ScrollUp_Normal_a.bmp_C"\
	".\res\ScrollUp_Normal_rgb.bmp_C"\
	".\res\ScrollUp_Over_a.bmp_C"\
	".\res\ScrollUp_Over_rgb.bmp_C"\
	".\res\ScrollUp_Pressed_a.bmp_C"\
	".\res\ScrollUp_Pressed_rgb.bmp_C"\
	".\res\Song_Blank_a.bmp_C"\
	".\res\Song_Blank_rgb.bmp_C"\
	".\res\Song_GridLine_a.bmp_C"\
	".\res\Song_GridLine_rgb.bmp_C"\
	".\res\Song_L_a.bmp_C"\
	".\res\Song_L_rgb.bmp_C"\
	".\res\Song_MarkerLine_a.bmp_C"\
	".\res\Song_MarkerLine_rgb.bmp_C"\
	".\res\Song_Pattern_a.bmp_C"\
	".\res\Song_Pattern_rgb.bmp_C"\
	".\res\Song_R_a.bmp_C"\
	".\res\Song_R_rgb.bmp_C"\
	".\res\SongScreen_Background.bmp_C"\
	".\res\SongScreen_Buttons_a.bmp_C"\
	".\res\SongScreen_Buttons_rgb.bmp_C"\
	".\res\SongScreen_ButtonsAlt_a.bmp_C"\
	".\res\SongScreen_ButtonsAlt_rgb.bmp_C"\
	".\res\SongScreen_ButtonsOver_a.bmp_C"\
	".\res\SongScreen_ButtonsOver_rgb.bmp_C"\
	".\res\SongScreen_ButtonsPressed_a.bmp_C"\
	".\res\SongScreen_ButtonsPressed_rgb.bmp_C"\
	".\res\SongScreen_ButtonsPressedAlt_a.bmp_C"\
	".\res\SongScreen_ButtonsPressedAlt_rgb.bmp_C"\
	".\res\Sounds_Minus_a.bmp_C"\
	".\res\Sounds_Minus_rgb.bmp_C"\
	".\res\Sounds_Plus_a.bmp_C"\
	".\res\Sounds_Plus_rgb.bmp_C"\
	".\res\spare.cur"\
	".\res\Stop_Button_a.bmp_C"\
	".\res\Stop_Button_rgb.bmp_C"\
	".\res\Stop_ButtonOver_a.bmp_C"\
	".\res\Stop_ButtonOver_rgb.bmp_C"\
	".\res\Stop_ButtonPressed_a.bmp_C"\
	".\res\Stop_ButtonPressed_rgb.bmp_C"\
	

"$(INTDIR)\leafDrums.res" : $(SOURCE) $(DEP_RSC_LEAFDR) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\leafDrumsDoc.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_LEAFDRU=\
	".\Beat.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\leafDrumsDoc.obj" : $(SOURCE) $(DEP_CPP_LEAFDRU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_LEAFDRU=\
	".\Beat.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\leafDrumsDoc.obj"	"$(INTDIR)\leafDrumsDoc.sbr" : $(SOURCE)\
 $(DEP_CPP_LEAFDRU) "$(INTDIR)"


!ENDIF 

SOURCE=.\leafDrumsView.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_LEAFDRUM=\
	"..\..\..\..\..\program files\html help workshop\include\htmlhelp.h"\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\PaletteDC.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\AudioHandler.h"\
	".\Beat.h"\
	".\Block.h"\
	".\ControlHandler.h"\
	".\CustomEdit.h"\
	".\DrumsHandler.h"\
	".\FileHierarchy.h"\
	".\FrameAnimation.h"\
	".\FXcode\Effect.h"\
	".\Identifiers.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MainFrm.h"\
	".\MixBuffer.h"\
	".\OneShotDialog.h"\
	".\Pattern.h"\
	".\PatternHandler.h"\
	".\registerdialog.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\SongHandler.h"\
	".\Sound.h"\
	".\SoundsHandler.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	".\WriteDlg.h"\
	

"$(INTDIR)\leafDrumsView.obj" : $(SOURCE) $(DEP_CPP_LEAFDRUM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_LEAFDRUM=\
	"..\..\..\..\..\program files\html help workshop\include\htmlhelp.h"\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\PaletteDC.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\AudioHandler.h"\
	".\Beat.h"\
	".\Block.h"\
	".\ControlHandler.h"\
	".\CustomEdit.h"\
	".\DrumsHandler.h"\
	".\FileHierarchy.h"\
	".\FrameAnimation.h"\
	".\FXcode\Effect.h"\
	".\Identifiers.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MainFrm.h"\
	".\MixBuffer.h"\
	".\OneShotDialog.h"\
	".\Pattern.h"\
	".\PatternHandler.h"\
	".\registerdialog.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\SongHandler.h"\
	".\Sound.h"\
	".\SoundsHandler.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	".\WriteDlg.h"\
	

"$(INTDIR)\leafDrumsView.obj"	"$(INTDIR)\leafDrumsView.sbr" : $(SOURCE)\
 $(DEP_CPP_LEAFDRUM) "$(INTDIR)"


!ENDIF 

SOURCE=.\LicenseDialog.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_LICEN=\
	".\LicenseDialog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\LicenseDialog.obj" : $(SOURCE) $(DEP_CPP_LICEN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_LICEN=\
	".\LicenseDialog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\LicenseDialog.obj"	"$(INTDIR)\LicenseDialog.sbr" : $(SOURCE)\
 $(DEP_CPP_LICEN) "$(INTDIR)"


!ENDIF 

SOURCE=.\Line.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_LINE_=\
	".\Beat.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\Line.obj" : $(SOURCE) $(DEP_CPP_LINE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_LINE_=\
	".\Beat.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\Line.obj"	"$(INTDIR)\Line.sbr" : $(SOURCE) $(DEP_CPP_LINE_)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\LogoWnd.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_LOGOW=\
	"..\Library\Graphic.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\PaletteDC.h"\
	"..\library\vec.h"\
	".\leafDrums.h"\
	".\LogoWnd.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\LogoWnd.obj" : $(SOURCE) $(DEP_CPP_LOGOW) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_LOGOW=\
	"..\Library\Graphic.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\PaletteDC.h"\
	"..\library\vec.h"\
	".\leafDrums.h"\
	".\LogoWnd.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\LogoWnd.obj"	"$(INTDIR)\LogoWnd.sbr" : $(SOURCE) $(DEP_CPP_LOGOW)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_MAINF=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\PaletteDC.h"\
	"..\Library\Security.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\leafDrums.h"\
	".\LogoWnd.h"\
	".\MainFrm.h"\
	".\NagDialog.h"\
	".\OneShotDialog.h"\
	".\registerdialog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_MAINF=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\PaletteDC.h"\
	"..\Library\Security.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\leafDrums.h"\
	".\LogoWnd.h"\
	".\MainFrm.h"\
	".\NagDialog.h"\
	".\OneShotDialog.h"\
	".\registerdialog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) $(DEP_CPP_MAINF)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\MixBuffer.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_MIXBU=\
	".\MixBuffer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MixBuffer.obj" : $(SOURCE) $(DEP_CPP_MIXBU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_MIXBU=\
	".\MixBuffer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MixBuffer.obj"	"$(INTDIR)\MixBuffer.sbr" : $(SOURCE)\
 $(DEP_CPP_MIXBU) "$(INTDIR)"


!ENDIF 

SOURCE=.\NagDialog.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_NAGDI=\
	"..\..\..\..\..\program files\html help workshop\include\htmlhelp.h"\
	".\leafDrums.h"\
	".\NagDialog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\NagDialog.obj" : $(SOURCE) $(DEP_CPP_NAGDI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_NAGDI=\
	"..\..\..\..\..\program files\html help workshop\include\htmlhelp.h"\
	".\leafDrums.h"\
	".\NagDialog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\NagDialog.obj"	"$(INTDIR)\NagDialog.sbr" : $(SOURCE)\
 $(DEP_CPP_NAGDI) "$(INTDIR)"


!ENDIF 

SOURCE=.\NewDlg.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_NEWDL=\
	".\leafDrums.h"\
	".\NewDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\NewDlg.obj" : $(SOURCE) $(DEP_CPP_NEWDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_NEWDL=\
	".\leafDrums.h"\
	".\NewDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\NewDlg.obj"	"$(INTDIR)\NewDlg.sbr" : $(SOURCE) $(DEP_CPP_NEWDL)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\OneShotDialog.cpp
DEP_CPP_ONESH=\
	".\OneShotDialog.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"


"$(INTDIR)\OneShotDialog.obj" : $(SOURCE) $(DEP_CPP_ONESH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"


"$(INTDIR)\OneShotDialog.obj"	"$(INTDIR)\OneShotDialog.sbr" : $(SOURCE)\
 $(DEP_CPP_ONESH) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pattern.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_PATTE=\
	".\Beat.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\Pattern.obj" : $(SOURCE) $(DEP_CPP_PATTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_PATTE=\
	".\Beat.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\Pattern.obj"	"$(INTDIR)\Pattern.sbr" : $(SOURCE) $(DEP_CPP_PATTE)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\PatternHandler.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_PATTER=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\DrumsHandler.h"\
	".\FXcode\Effect.h"\
	".\Identifiers.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\PatternHandler.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\SongHandler.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\PatternHandler.obj" : $(SOURCE) $(DEP_CPP_PATTER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_PATTER=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\DrumsHandler.h"\
	".\FXcode\Effect.h"\
	".\Identifiers.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\PatternHandler.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\SongHandler.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\PatternHandler.obj"	"$(INTDIR)\PatternHandler.sbr" : $(SOURCE)\
 $(DEP_CPP_PATTER) "$(INTDIR)"


!ENDIF 

SOURCE=.\FXcode\Phaser.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_PHASE=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Phaser.h"\
	

"$(INTDIR)\Phaser.obj" : $(SOURCE) $(DEP_CPP_PHASE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_PHASE=\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Phaser.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Phaser.obj"	"$(INTDIR)\Phaser.sbr" : $(SOURCE) $(DEP_CPP_PHASE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\FXcode\Quieten.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_QUIET=\
	"..\library\stdafx.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Quieten.h"\
	

"$(INTDIR)\Quieten.obj" : $(SOURCE) $(DEP_CPP_QUIET) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_QUIET=\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Quieten.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Quieten.obj"	"$(INTDIR)\Quieten.sbr" : $(SOURCE) $(DEP_CPP_QUIET)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\RegisterDialog.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_REGIS=\
	".\registerdialog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RegisterDialog.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_REGIS=\
	".\registerdialog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RegisterDialog.obj"	"$(INTDIR)\RegisterDialog.sbr" : $(SOURCE)\
 $(DEP_CPP_REGIS) "$(INTDIR)"


!ENDIF 

SOURCE=.\Sample.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_SAMPL=\
	".\Sample.h"\
	".\StdAfx.h"\
	".\WavFile.h"\
	

"$(INTDIR)\Sample.obj" : $(SOURCE) $(DEP_CPP_SAMPL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_SAMPL=\
	".\Sample.h"\
	".\StdAfx.h"\
	".\WavFile.h"\
	

"$(INTDIR)\Sample.obj"	"$(INTDIR)\Sample.sbr" : $(SOURCE) $(DEP_CPP_SAMPL)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\SectionHandler.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_SECTI=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\FXcode\Effect.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\SectionHandler.obj" : $(SOURCE) $(DEP_CPP_SECTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_SECTI=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\FXcode\Effect.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\SectionHandler.obj"	"$(INTDIR)\SectionHandler.sbr" : $(SOURCE)\
 $(DEP_CPP_SECTI) "$(INTDIR)"


!ENDIF 

SOURCE=.\Song.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_SONG_=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\DSoundPlayDevice.h"\
	".\FileSystem.h"\
	".\FXcode\Effect.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	".\WavFile.h"\
	

"$(INTDIR)\Song.obj" : $(SOURCE) $(DEP_CPP_SONG_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_SONG_=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\DSoundPlayDevice.h"\
	".\FileSystem.h"\
	".\FXcode\Effect.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	".\WavFile.h"\
	

"$(INTDIR)\Song.obj"	"$(INTDIR)\Song.sbr" : $(SOURCE) $(DEP_CPP_SONG_)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\SongHandler.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_SONGH=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\FXcode\Effect.h"\
	".\Identifiers.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\PatternHandler.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\SongHandler.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\SongHandler.obj" : $(SOURCE) $(DEP_CPP_SONGH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_SONGH=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\FXcode\Effect.h"\
	".\Identifiers.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\PatternHandler.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\SongHandler.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\SongHandler.obj"	"$(INTDIR)\SongHandler.sbr" : $(SOURCE)\
 $(DEP_CPP_SONGH) "$(INTDIR)"


!ENDIF 

SOURCE=.\Sound.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_SOUND=\
	".\Beat.h"\
	".\Block.h"\
	".\FileSystem.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\Sound.obj" : $(SOURCE) $(DEP_CPP_SOUND) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_SOUND=\
	".\Beat.h"\
	".\Block.h"\
	".\FileSystem.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\Sound.obj"	"$(INTDIR)\Sound.sbr" : $(SOURCE) $(DEP_CPP_SOUND)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\SoundsHandler.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_SOUNDS=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\AudioHandler.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\DrumsHandler.h"\
	".\FileHierarchy.h"\
	".\FileSystem.h"\
	".\FolderOptionsDlg.h"\
	".\FXcode\Effect.h"\
	".\Identifiers.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\Sound.h"\
	".\SoundsHandler.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\SoundsHandler.obj" : $(SOURCE) $(DEP_CPP_SOUNDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_SOUNDS=\
	"..\leafchatunicode\leafchatunicodeexport.h"\
	"..\Library\Graphic.h"\
	"..\Library\GraphicAlpha.h"\
	"..\Library\GraphicAlphaChannel.h"\
	"..\Library\GraphicalProducer.h"\
	"..\Library\GraphicButton.h"\
	"..\Library\GraphicCheckbox.h"\
	"..\Library\GraphicFont.h"\
	"..\Library\GraphicHScroll.h"\
	"..\Library\GraphicListbox.h"\
	"..\Library\GraphicNumberOption.h"\
	"..\Library\GraphicOnscreen.h"\
	"..\Library\GraphicOption.h"\
	"..\Library\GraphicScrollbar.h"\
	"..\Library\GraphicTextOption.h"\
	"..\Library\GraphicVScroll.h"\
	"..\Library\UnicodeDisplay.h"\
	"..\library\vec.h"\
	".\AudioHandler.h"\
	".\Beat.h"\
	".\Block.h"\
	".\CustomEdit.h"\
	".\DrumsHandler.h"\
	".\FileHierarchy.h"\
	".\FileSystem.h"\
	".\FolderOptionsDlg.h"\
	".\FXcode\Effect.h"\
	".\Identifiers.h"\
	".\leafDrums.h"\
	".\leafDrumsDoc.h"\
	".\leafDrumsView.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\SectionHandler.h"\
	".\Song.h"\
	".\Sound.h"\
	".\SoundsHandler.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\SoundsHandler.obj"	"$(INTDIR)\SoundsHandler.sbr" : $(SOURCE)\
 $(DEP_CPP_SOUNDS) "$(INTDIR)"


!ENDIF 

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"


"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"


"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\FXcode\Surround.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_SURRO=\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Surround.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Surround.obj" : $(SOURCE) $(DEP_CPP_SURRO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_SURRO=\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Surround.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Surround.obj"	"$(INTDIR)\Surround.sbr" : $(SOURCE) $(DEP_CPP_SURRO)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Track.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_TRACK=\
	".\Beat.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\Track.obj" : $(SOURCE) $(DEP_CPP_TRACK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_TRACK=\
	".\Beat.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\Track.obj"	"$(INTDIR)\Track.sbr" : $(SOURCE) $(DEP_CPP_TRACK)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\FXcode\Tremolo.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_TREMO=\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Tremolo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Tremolo.obj" : $(SOURCE) $(DEP_CPP_TREMO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_TREMO=\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\FXcode\Tremolo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Tremolo.obj"	"$(INTDIR)\Tremolo.sbr" : $(SOURCE) $(DEP_CPP_TREMO)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\VariableParameter.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_VARIA=\
	".\StdAfx.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\VariableParameter.obj" : $(SOURCE) $(DEP_CPP_VARIA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_VARIA=\
	".\StdAfx.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	

"$(INTDIR)\VariableParameter.obj"	"$(INTDIR)\VariableParameter.sbr" : $(SOURCE)\
 $(DEP_CPP_VARIA) "$(INTDIR)"


!ENDIF 

SOURCE=.\WavFile.cpp
DEP_CPP_WAVFI=\
	".\StdAfx.h"\
	".\WavFile.h"\
	

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"


"$(INTDIR)\WavFile.obj" : $(SOURCE) $(DEP_CPP_WAVFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"


"$(INTDIR)\WavFile.obj"	"$(INTDIR)\WavFile.sbr" : $(SOURCE) $(DEP_CPP_WAVFI)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\WriteDlg.cpp

!IF  "$(CFG)" == "leafDrums2 - Win32 Release"

DEP_CPP_WRITE=\
	".\Beat.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\leafDrums.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	".\WriteDlg.h"\
	

"$(INTDIR)\WriteDlg.obj" : $(SOURCE) $(DEP_CPP_WRITE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "leafDrums2 - Win32 Debug"

DEP_CPP_WRITE=\
	".\Beat.h"\
	".\Block.h"\
	".\FXcode\Effect.h"\
	".\leafDrums.h"\
	".\Line.h"\
	".\MixBuffer.h"\
	".\Pattern.h"\
	".\Sample.h"\
	".\Song.h"\
	".\Sound.h"\
	".\StdAfx.h"\
	".\Track.h"\
	".\VariableParameter.h"\
	".\vec.h"\
	".\WriteDlg.h"\
	

"$(INTDIR)\WriteDlg.obj"	"$(INTDIR)\WriteDlg.sbr" : $(SOURCE) $(DEP_CPP_WRITE)\
 "$(INTDIR)"


!ENDIF 


!ENDIF 

