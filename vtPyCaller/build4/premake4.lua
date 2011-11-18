dofile("ModuleConfig.lua")
dofile("ModuleHelper.lua")



if _ACTION == "help" then
	premake.showhelp()
	return

end



solution "vtPython"

	configurations { "Debug", "Release" , "ReleaseDebug" ; "ReleaseRedist" }

	if _ACTION and _OPTIONS["Dev"] then
		setfields("location","./".._ACTION.._OPTIONS["Dev"])
	end



--	The building blocks for vtPythonCaller
packageConfig_vtPythonCaller =
{
	Name 		= 	"vtPythonCaller",
	Type		=	"SharedLib",
	TargetSuffix = "/BuildingBlocks",
	Defines 	= { DEF_STD_DIRECTIVES 	;  "VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" },
	Files		= { DROOT.."SDK/src/Behaviors/**.cpp"	; DROOT.."SDK/src/core/*.cpp" ; DROOT.."SDK/src/Behaviors/*.def" ; F_SHARED_SRC ; DROOT.."build4/**.lua" ;  F_BASE_VT_SRC ; F_VT_STD_INC },
	Includes  	= { D_STD_INCLUDES ; D_CORE_INCLUDES ; D_PY.."include" },
	Libs	= {	"ck2" ; "vxmath"  ; "user32" ;  "kernel32" ;},
	LibDirectories = { D_PY.."lib" },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\\BuildingBlocks" .."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower().."\\BuildingBlocks"

}

--	If the command line contains --ExtraDefines="WebPack" , we add "WebPack" to the
--	pre-processor directives and also create a package to include the camera building blocks
--	as defined in packageConfig_CameraRepack
if _OPTIONS["ExtraDefines"]  then

	if _OPTIONS["ExtraDefines"]=="WebPack" then
		createStaticPackage(packageConfig_CameraRepack)
	end
end

createStaticPackage(packageConfig_vtPythonCaller)


function onclean()
	os.rmdir("vs**")
end
