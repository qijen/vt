FIND_PATH(VTDEV41DIR
  NAMES dev.exe devr.exe
  PATHS
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Virtools\\Dev\\4.1;InstallPath]
  )
MARK_AS_ADVANCED(CMAKE_MAKE_PROGRAM)
SET(VTDEV41 1)

