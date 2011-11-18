# Microsoft Developer Studio Project File - Name="MidiManagerLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104
# TARGTYPE "Xbox Static Library" 0x0b04

CFG=MidiManagerLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MidiManagerLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MidiManagerLib.mak" CFG="MidiManagerLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MidiManagerLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "MidiManagerLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "MidiManagerLib - Xbox Release" (based on "Xbox Static Library")
!MESSAGE "MidiManagerLib - Xbox Debug" (based on "Xbox Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0



!IF  "$(CFG)" == "MidiManagerLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\TempDebug\MidiManagerLib\Release"
# PROP Intermediate_Dir "\TempDebug\MidiManagerLib\Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "$(NEMORDIR)\SDK\Includes" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "VX_LIB" /D "CK_LIB" /Yu"CKAll.h" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"$(NEMORDIR)\SDK\Lib\MidiManagerLib.lib"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\TempDebug\MidiManagerLib\Debug"
# PROP Intermediate_Dir "\TempDebug\MidiManagerLib\Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /Zi /Od /I "$(NEMORDIR)\SDK\Includes" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "VX_LIB" /D "CK_LIB" /Yu"CKAll.h" /FD /GZ /c
RSC=rc.exe
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"$(NEMORDIR)\SDK\Lib\MidiManagerLibD.lib"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MidiManagerLib___Xbox_Release"
# PROP BASE Intermediate_Dir "MidiManagerLib___Xbox_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\TempDebug\MidiManagerLib___Xbox_Release"
# PROP Intermediate_Dir "\TempDebug\MidiManagerLib___Xbox_Release"
# PROP Target_Dir ""
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo /out:"$(NEMORDIR)\SDK\Lib\MidiManagerLib.lib"
# ADD LIB32 /nologo /out:"\Dev\XboxLib\Release\XBoxMidiManagerLib.lib"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /Ob2 /D "WIN32" /D "_XBOX" /D "NDEBUG" /D "VX_LIB" /D "CK_LIB" /Yu"CKAll.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /D "WIN32" /D "_XBOX" /D "NDEBUG" /D "VX_LIB" /D "CK_LIB" /Yu"CKAll.h" /FD /G6 /Zvc6 /c

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MidiManagerLib___Xbox_Debug"
# PROP BASE Intermediate_Dir "MidiManagerLib___Xbox_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\TempDebug\MidiManagerLib___Xbox_Debug"
# PROP Intermediate_Dir "\TempDebug\MidiManagerLib___Xbox_Debug"
# PROP Target_Dir ""
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo /out:"$(NEMORDIR)\SDK\Lib\MidiManagerLibD.lib"
# ADD LIB32 /nologo /out:"\Dev\XboxLib\Debug\XBoxMidiManagerLibD.lib"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_XBOX" /D "_DEBUG" /D "VX_LIB" /D "CK_LIB" /Yu"CKAll.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_XBOX" /D "_DEBUG" /D "VX_LIB" /D "CK_LIB" /Yu"CKAll.h" /FD /GZ /Zvc6 /G6 /c

!ENDIF 

# Begin Target

# Name "MidiManagerLib - Win32 Release"
# Name "MidiManagerLib - Win32 Debug"
# Name "MidiManagerLib - Xbox Release"
# Name "MidiManagerLib - Xbox Debug"
# Begin Group "MidiSound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\MidiSound\midisound.cpp

!IF  "$(CFG)" == "MidiManagerLib - Win32 Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\MidiSound\smf.cpp

!IF  "$(CFG)" == "MidiManagerLib - Win32 Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\MidiSound\smfread.cpp

!IF  "$(CFG)" == "MidiManagerLib - Win32 Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Behaviors"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\behavior src\MidiEvent.cpp"

!IF  "$(CFG)" == "MidiManagerLib - Win32 Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\behavior src\midiplayer.cpp"

!IF  "$(CFG)" == "MidiManagerLib - Win32 Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\behavior src\Read Midi Signal.cpp"

!IF  "$(CFG)" == "MidiManagerLib - Win32 Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\behavior src\SetInMidiPort.cpp"

!IF  "$(CFG)" == "MidiManagerLib - Win32 Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\behavior src\SwicthOnMidi.cpp"

!IF  "$(CFG)" == "MidiManagerLib - Win32 Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\MidiManager.h
# End Source File
# Begin Source File

SOURCE=..\MidiSound\midisound.h
# End Source File
# Begin Source File

SOURCE=..\MidiSound\muldiv32.h
# End Source File
# Begin Source File

SOURCE=..\MidiSound\smf.h
# End Source File
# Begin Source File

SOURCE=..\MidiSound\smfi.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\MidiBehaviors.cpp

!IF  "$(CFG)" == "MidiManagerLib - Win32 Release"

# ADD CPP /Yc"CKAll.h"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Win32 Debug"

# ADD CPP /Yc"CKAll.h"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\MidiManager.cpp

!IF  "$(CFG)" == "MidiManagerLib - Win32 Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Release"

!ELSEIF  "$(CFG)" == "MidiManagerLib - Xbox Debug"

!ENDIF 

# End Source File
# End Target
# End Project
