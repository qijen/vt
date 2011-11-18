project( NxuStreamLib )

add_subdirectory (./) 

include_directories( 
  ../../include/Physics
  ../../include/ageia
  ../../src/NXStream
  
)

add_definitions( 
  -DMODULE_BASE_EXPORTS
  -DNOMINMAX
  -DVIRTOOLS_MODULE
  -DVIRTOOLS_USER_SDK
  -DWIN32
  -D_CRT_SECURE_NO_DEPRECATE
  -D_DEBUG
  -D_USRDLL
)

add_library( NxuStreamLib STATIC ${SOURCES} )

