SET DEV40DIR="x:/sdk/dev4"
SET DEV41DIR="x:/sdk/dev41R"
SET DEV5DIR="x:/sdk/dev5GA"

xcopy /c /r /y %DEV40DIR%\BuildingBlocks\vtPhysX.dll ..\Release\Author\Dev40\BuildingBlocks\
xcopy /c /r /y %DEV40DIR%\vtPhysXLib.dll ..\Release\Author\Dev40\

xcopy /c /r /y %DEV41DIR%\BuildingBlocks\vtPhysX.dll ..\Release\Author\Dev41\BuildingBlocks\
xcopy /c /r /y %DEV41DIR%\vtPhysXLib.dll ..\Release\Author\Dev41\

xcopy /c /r /y %DEV5DIR%\BuildingBlocks\vtPhysX.dll ..\Release\Author\Dev5\BuildingBlocks\
xcopy /c /r /y %DEV5DIR%\vtPhysXLib.dll ..\Release\Author\Dev5\


svn ci -m "batch upload: release demo" ..\Release\Author\

cd ..\Tools
CreateAuthorZip.bat



