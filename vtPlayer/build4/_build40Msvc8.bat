SET devdir=X:\sdk_ide\msvc8\Common7\IDE
SET devexe=X:\sdk_ide\msvc8\Common7\IDE\devenv.exe
SET solution=X:\ProjectRoot\svnLocal\local\vtPhysX\build4\vs2005\vtAgeia.sln

cd %devdir%
%devexe% %solution% /out x:\log.txt /Build "Debug|Win32"




