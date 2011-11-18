# Microsoft Developer Studio Project File - Name="MidiManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MidiManager - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MidiManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MidiManager.mak" CFG="MidiManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MidiManager - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MidiManager - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MidiManager - Win32 ReleaseDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MidiManager - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\TempDebug\MidiManager\Release"
# PROP Intermediate_Dir "\TempDebug\MidiManager\Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=xilink6.exe -lib
F90=fl32.exe
shaders=midl.exe


CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MIDIMANAGER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(NEMO3DIR)\SDK\Includes" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"CKAll.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ck2.lib vxmath.lib winmm.lib user32.lib /nologo /dll /machine:I386 /out:"$(NEMO3DIR)\BuildingBlocks\MidiManager.dll" /libpath:"$(NEMO3DIR)\SDK\Lib"

!ELSEIF  "$(CFG)" == "MidiManager - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\TempDebug\MidiManager\Debug"
# PROP Intermediate_Dir "\TempDebug\MidiManager\Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=xilink6.exe -lib
F90=fl32.exe
shaders=midl.exe


CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MIDIMANAGER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /Zi /Od /I "$(NEMORDIR)\SDK\Includes" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"CKAll.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ck2.lib VxMath.lib winmm.lib user32.lib /nologo /dll /incremental:no /pdb:"\TempDebug\MidiManager\MidiManagerD.pdb" /debug /machine:I386 /out:"$(NEMORDIR)\BuildingBlocks\MidiManager.dll" /pdbtype:sept /libpath:"$(NEMORDIR)\SDK\Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MidiManager - Win32 ReleaseDebug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MidiManager\ReleaseDebug"
# PROP BASE Intermediate_Dir "MidiManager\ReleaseDebug"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\TempDebug\MidiManager\ReleaseDebug"
# PROP Intermediate_Dir "\TempDebug\MidiManager\ReleaseDebug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=xilink6.exe -lib
F90=fl32.exe
shaders=midl.exe


CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"CKAll.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /Ob2 /I "$(NEMORDIR)\SDK\Includes" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"CKAll.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ck2.lib vxmath.lib winmm.lib user32.lib /nologo /dll /machine:I386 /out:"$(NEMORDIR)\BuildingBlocks\MidiManager.dll"
# ADD LINK32 ck2.lib vxmath.lib winmm.lib user32.lib /nologo /dll /debug /machine:I386 /out:"$(NEMORDIR)\BuildingBlocks\MidiManager.dll"

!ENDIF 

# Begin Target

# Name "MidiManager - Win32 Release"
# Name "MidiManager - Win32 Debug"
# Name "MidiManager - Win32 ReleaseDebug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MidiManager.h
# End Source File
# Begin Source File

SOURCE=.\MidiSound\midisound.h
# End Source File
# Begin Source File

SOURCE=.\MidiSound\muldiv32.h
# End Source File
# Begin Source File

SOURCE=.\MidiSound\smf.h
# End Source File
# Begin Source File

SOURCE=.\MidiSound\smfi.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Behaviors"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\behavior src\GetMidiINDevices.cpp"
# End Source File
# Begin Source File

SOURCE=".\behavior src\GetMidiOutDevices.cpp"
# End Source File
# Begin Source File

SOURCE=".\behavior src\MidiEvent.cpp"

!IF  "$(CFG)" == "MidiManager - Win32 Release"

!ELSEIF  "$(CFG)" == "MidiManager - Win32 Debug"

# ADD CPP /Yc"CKAll.h"

!ELSEIF  "$(CFG)" == "MidiManager - Win32 ReleaseDebug"

# ADD CPP /Yc"CKAll.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\behavior src\midiplayer.cpp"
# End Source File
# Begin Source File

SOURCE=".\behavior src\Read Midi Signal.cpp"
# End Source File
# Begin Source File

SOURCE=".\behavior src\Send Midi Signal.cpp"
# End Source File
# Begin Source File

SOURCE=".\behavior src\SetInMidiPort.cpp"
# End Source File
# Begin Source File

SOURCE=".\behavior src\SetMidiOutPort.cpp"
# End Source File
# Begin Source File

SOURCE=".\behavior src\SwicthOnMidi.cpp"
# End Source File
# End Group
# Begin Group "MidiSound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MidiSound\midisound.cpp
# End Source File
# Begin Source File

SOURCE=.\MidiSound\smf.cpp
# End Source File
# Begin Source File

SOURCE=.\MidiSound\smfread.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\MidiBehaviors.cpp

!IF  "$(CFG)" == "MidiManager - Win32 Release"

# ADD CPP /Yc"CKAll.h"

!ELSEIF  "$(CFG)" == "MidiManager - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "MidiManager - Win32 ReleaseDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MidiBehaviors.def
# End Source File
# Begin Source File

SOURCE=.\MidiManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MidiManager.rc
# End Source File
# End Target
# End Project
