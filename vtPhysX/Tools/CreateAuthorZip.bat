rmdir .\Release /s /q
mkdir Release
xcopy ..\Release\Author .\Release /s /e

REM #rar a -ep1 -r -s -x@RarExclusionList.txt vtPhysXDemo.zip ..\Release\Author\

winrar a -ep1 -r -s -x@RarExclusionList.txt -n@RarInclusionList.txt vtPhysXDemo.zip Release


REM rar a -r -s -x@RarExclusionList.txt -agYYYYMMDD-HHMM GBLSourceSnapshot.rar ..\\..
