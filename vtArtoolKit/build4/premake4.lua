dofile("ModuleConfig.lua")
dofile("ModuleHelper.lua")



if _ACTION == "help" then
	premake.showhelp()
	return

end

solution "vtArToolkit"

	configurations { "Debug", "Release" , "ReleaseDebug" ; "ReleaseRedist" ; "ReleaseDemo"  }

	if _ACTION and _OPTIONS["Dev"] then
		setfields("location","./".._ACTION.._OPTIONS["Dev"])
	end


packageConfig_ARToolkit =
{
	Name 		= 	"ArtToolkitLib",
	Type		=	"StaticLib",
	Defines 	= { DEF_STD_DIRECTIVES ; D_AT_DEFINES  },
	Files		= { D_AT_SRC ; D_AT.."include/**.h"},
	Includes  	= { D_AT_INCLUDES },
	Options = { "/W0"}
}


--	Static library of the built-in camera building blocks
--
--	This package needs to be compiled for web player distributions.
--	Call createSolutions40Web2005.bat for instance
--
packageConfig_CameraRepack=
{
	Name 		= 	"Camera",
	Type		=	"StaticLib",
	TargetSuffix = "",
	Defines 	= { DEF_STD_DIRECTIVES 	;  "VIRTOOLS_USER_SDK" },
	Files		= { DDEPS.."camera/behaviors/**.cpp"	},
	Includes  	= { D_STD_INCLUDES ; },
	Libs	= {	"ck2" ; "vxmath" },
	LibDirectories = { },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower()
}

--	The core vtPhysX SDK, compiled as DLL with exports. This is the entire base library for
--
--		-	building blocks
--		-	interface plug-ins
--		-	custom parameter types
--		-	custom file readers
--
--

--	The building blocks for vtPhysX, depending on packageConfig_vtAgeiaLib
packageConfig_vtArToolkitBeh =
{
	Name 		= 	"ARToolkitBehaviors",
	Type		=	"SharedLib",
	TargetSuffix = "/BuildingBlocks",
	Defines 	= { DEF_STD_DIRECTIVES 	; "VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" },
	Files		= { D_CORE_SRC ; DROOT.."SDK/src/Behaviors/**.cpp"	; DROOT.."SDK/src/Behaviors/*.def" ; F_SHARED_SRC ; DROOT.."build4/**.lua" ; F_EXTRA_BB_SRC ; F_BASE_SRC },
	Includes  	= { D_STD_INCLUDES ; D_CORE_INCLUDES ; D_AT_INCLUDES },
	Libs	= {	"ck2" ; "vxmath"  ; "user32" ;  "kernel32" ; "ArtToolkitLib"  },
	LibDirectories = { },
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

createStaticPackage(packageConfig_ARToolkit)
createStaticPackage(packageConfig_vtArToolkitBeh)




--include "CppConsoleApp"

function onclean()
	os.rmdir("vs**")
end
