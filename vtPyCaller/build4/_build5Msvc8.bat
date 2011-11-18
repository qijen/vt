SET devdir=X:\sdk\msvc8\Common7\IDE\
SET devexe=X:\sdk\msvc8\Common7\IDE\devenv.com
SET solution=X:\ProjectRoot\svn\pearls-media\private\virtools\vtPhysX\build4\vs2005Dev5\vtPhysX.sln

REM cd %devdir%
%devexe% %solution% /out x:\log422005.txt /Build "ReleaseDemo"

