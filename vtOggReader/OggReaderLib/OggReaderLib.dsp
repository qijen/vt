# Microsoft Developer Studio Project File - Name="OggReaderLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=OggReaderLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OggReaderLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OggReaderLib.mak" CFG="OggReaderLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OggReaderLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "OggReaderLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0


CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OggReaderLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\TempDebug\OggReaderLib\Release"
# PROP Intermediate_Dir "\TempDebug\OggReaderLib\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\$(NEMORDIR)\SDK\Includes" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "VX_LIB" /D "CK_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"$(NEMORDIR)\SDK\Lib/OggReaderLib.lib"

!ELSEIF  "$(CFG)" == "OggReaderLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\TempDebug\OggReaderLib\Debug"
# PROP Intermediate_Dir "\TempDebug\OggReaderLib\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "..\$(NEMORDIR)\SDK\Includes" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "VX_LIB" /D "CK_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"$(NEMORDIR)\SDK\Lib/OggReaderLibD.lib"

!ENDIF 

# Begin Target

# Name "OggReaderLib - Win32 Release"
# Name "OggReaderLib - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Header\OggReader.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=..\Source\OggReader.cpp
# End Source File
# Begin Source File

SOURCE=..\OggReaderDll.cpp
# End Source File
# End Group
# End Target
# End Project
