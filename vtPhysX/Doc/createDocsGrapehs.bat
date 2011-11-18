copy X:\ProjectRoot\svn\local\vtPhysX\Doc\examples\*.cmo X:\ProjectRoot\svn\local\vtPhysX\Doc\doxyOutput\html /y
copy .\doxyOutput\vtPhysX.CHM x:\ProjectRoot\vdev\Documentation\
copy .\doxyOutput\vtPhysX.chi x:\ProjectRoot\vdev\Documentation\
doxygen DoxyfileGraphs.dox
cd doxyOutput
vtPhysX.chm


REM copy X:\ProjectRoot\vdev\Documentation\vtAgeia.CHM X:\ProjectRoot\vtmod_vtAgeia\Doc /y
REM copy X:\ProjectRoot\vdev\Documentation\vtAgeia.CHM X:\ProjectRoot\vtmodsvn\Plugins\vtAgeia\Documentation /y





