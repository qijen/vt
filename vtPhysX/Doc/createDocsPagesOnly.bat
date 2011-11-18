copy /y .\examples\*.cmo .\doxyOutput\html

doxygen .\doxyConfigPagesOnly.dox
cd .\doxyOutput\
vtPhysX.CHM



REM copy X:\ProjectRoot\vdev\Documentation\vtAgeia.CHM X:\ProjectRoot\vtmod_vtAgeia\Doc /y
REM copy X:\ProjectRoot\vdev\Documentation\vtAgeia.CHM X:\ProjectRoot\vtmodsvn\Plugins\vtAgeia\Documentation /y
REM exit
