FIND_PATH(WEBPLAYERDIR
  NAMES WebPlayerConfig.exe
  PATHS
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Virtools\\WebPlayer;InstallPath]
  "C://ProgramFiles/3D Life Player"
  )
MARK_AS_ADVANCED(CMAKE_MAKE_PROGRAM)
SET(WEBPLAYER 1)

