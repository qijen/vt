@ECHO OFF

rmdir .\CMakeFiles /s /q
del .\*.vcproj /q /f
del *.sln /q /f
del CMakeCache.txt /q /f
del cmake_install.cmake /q /f


@ECHO ON

