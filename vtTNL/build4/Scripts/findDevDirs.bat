
rmdir .\CMakeFiles /s /q
del .\*.vcproj /q /f
del *.sln /q /f
del CMakeCache.txt /q /f
del cmake_install.cmake /q /f
del VTPaths.lua


cmake -Wno-dev -p CMakeLists.txt

copy VTPaths.lua .\..\


rmdir .\CMakeFiles /s /q
del .\*.vcproj /q /f
del *.sln /q /f
del CMakeCache.txt /q /f
del cmake_install.cmake /q /f




