REM createSolutions402003.bat
REM createSolutions412005.bat
REM createSolutions52005.bat

rmdir ..\TEMP /s /q
rmdir ..\BIN /s /q

REM premake4 --ExternalVTDirectory=./VTPaths.lua --DeployDirect=TRUE --Dev=Dev40 --file="./premake4.lua" vs2003
REM premake4 --ExternalVTDirectory=./VTPaths.lua --DeployDirect=TRUE --Dev=Dev41 --file="./premake4.lua" vs2005
premake4 --ExternalVTDirectory=./VTPaths.lua --DeployDirect=TRUE --Dev=Dev5 --file="./premake4.lua" vs2005

REM start _build4Msvc7.bat
REM start _build41Msvc8.bat
start _build5Msvc8.bat

ReleaseDemoPost.bat




REM -----------------------------------------------------
REM --
REM --		Copy to release
REM --
REM --




