FIND_PATH(VTDEV40DIR
  NAMES dev.exe devr.exe
  PATHS
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Virtools\\Dev\\4.0;InstallPath]
  )
MARK_AS_ADVANCED(CMAKE_MAKE_PROGRAM)
SET(VTDEV40 1)

