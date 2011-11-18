SET devdir=X:\sdk\msvc\Common7\IDE\
SET devexe=X:\sdk\msvc\Common7\IDE\devenv.com
SET solution=X:\ProjectRoot\svn\pearls-media\private\virtools\vtPhysX\build4\vs2003Dev40\vtPhysX.sln

REM cd %devdir%
%devexe% %solution% /out x:\logDev42003.txt /Build "ReleaseDemo"



