dofile("ModuleConfig.lua")
dofile("ModuleHelper.lua")



if _ACTION == "help" then
	premake.showhelp()
	return

end



solution "vtPhysX"

	configurations { "Debug", "Release" , "ReleaseDebug" ; "ReleaseRedist" ; "ReleaseDemo"  }

	if _ACTION and _OPTIONS["Dev"] then
		setfields("location","./".._ACTION.._OPTIONS["Dev"])
	end


packageConfig_Stream =
{
	Name 		= 	"NxuStream2",
	Type		=	"StaticLib",
	Defines 	= { DEF_STD_DIRECTIVES 	; 	DEF_PHYSX_DIRECTIVES },
	Files		= { D_NXSTREAM.."*.cpp"	;	D_NXSTREAM.."*.h"	},
	Includes  	= { D_PHYSX_INCLUDES },
	Options = { "/W0"}

}


packageConfig_Common =
{
	Name 		= 	"NxCommon",
	Type		=	"StaticLib",
	Defines 	= { DEF_STD_DIRECTIVES 	; 	DEF_PHYSX_DIRECTIVES },
	Files		= { D_NXCOMMON.."*.cpp"	;	D_NXCOMMON.."*.h"	},
	Includes  	= { D_PHYSX_INCLUDES },
	Options = { "/W0"}
}

packageConfig_Character =
{
	Name 		= 	"NxCharacter",
	Type		=	"StaticLib",
	Defines 	= { DEF_STD_DIRECTIVES 	; 	DEF_PHYSX_DIRECTIVES ; "NX_USE_SDK_STATICLIBS" ; "NXCHARACTER_EXPORTS" },
	Files		= { D_NXCHARACTER.."**.cpp"	;	D_NXCHARACTER.."**.h"	},
	Includes  	= { D_PHYSX_INCLUDES },
	Options = { "/W0"}
}

packageConfig_TinyXML =
{
	Name 		= 	"TinyXML",
	Type		=	"StaticLib",
	Defines 	= { DEF_STD_DIRECTIVES },
	Files		= { D_TINYXML.."*.cpp"	;	D_TINYXML.."*.h"	},
	Includes  	= { D_PHYSX_INCLUDES },
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
packageConfig_vtAgeiaLib =
{
	Name 		= 	"vtPhysXLib",
	Type		=	"SharedLib",
	TargetSuffix = "",
	Defines 	= { DEF_STD_DIRECTIVES 	; 	DEF_PHYSX_DIRECTIVES ; "NX_USE_SDK_STATICLIBS" ; "NXCHARACTER_EXPORTS" ; "VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" ; },
	Files		= { DROOT.."SDK/src/core/**.cpp"	;	DROOT.."SDK/include/core/**.h"  ; F_BASE_STD_INC ;F_VT_STD_INC; F_BASE_VT_SRC ;F_BASE_SRC;F_SHARED_SRC },
	Includes  	= { D_PHYSX_INCLUDES ; D_STD_INCLUDES ; D_VT_BASE_INC; D_CORE_INCLUDES ; DDEPS.."TinyXML" },
	Libs	= {	"TinyXML" , "NxCharacter" , "NxCommon" , "NxuStream2"; "ck2" ; "vxmath"  ; "user32" ;  "kernel32" ; "PhysXLoader" ; "Winmm"  },
	LibDirectories = { D_PHYSX.."SDKs/lib/Win32" },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower()
}

--	The building blocks for vtPhysX, depending on packageConfig_vtAgeiaLib
packageConfig_vtAgeiaBeh =
{
	Name 		= 	"vtPhysX",
	Type		=	"SharedLib",
	TargetSuffix = "/BuildingBlocks",
	Defines 	= { DEF_STD_DIRECTIVES 	; 	DEF_PHYSX_DIRECTIVES ; "NX_USE_SDK_STATICLIBS" ; "NXCHARACTER_EXPORTS" ; "VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" },
	Files		= { DROOT.."SDK/src/Behaviors/**.cpp"	; DROOT.."SDK/src/Behaviors/*.def" ; D_DOCS_PAGES.."*.page" ; F_SHARED_SRC ; DROOT.."build4/**.lua" ; F_EXTRA_BB_SRC ; F_BASE_SRC },
	Includes  	= { D_PHYSX_INCLUDES ; D_STD_INCLUDES ; D_CORE_INCLUDES ; DDEPS.."TinyXML" ; DDEPS.."dx/Include" },
	Libs	= {	"ck2" ; "vxmath"  ; "user32" ;  "kernel32" ; "vtPhysXLib" ;"Winmm" ; "dxerr"; "dinput8";"dxguid" },
	LibDirectories = { D_PHYSX.."SDKs/lib/Win32" ; D_DX.."lib" },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\\BuildingBlocks" .."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower().."\\BuildingBlocks"


}

--	The vtPhysX Reader plug-in supports NXU-Stream formats or collada, exporters for Maya and 3D-SMax
--	are avaiable. Depends on packageConfig_vtAgeiaLib
packageConfig_vtAgeiaReader =
{
	Name 		= 	"vtPhysXReader",
	Type		=	"SharedLib",
	TargetSuffix = "/Plugins",
	Defines 	= { DEF_STD_DIRECTIVES 	; 	DEF_PHYSX_DIRECTIVES ; "NX_USE_SDK_STATICLIBS" ; "NXCHARACTER_EXPORTS" ; "VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" },
	Files		= { D_READER_SRC.."*.cpp" ; D_READER_SRC.."*.def" ;  },
	Includes  	= { D_PHYSX_INCLUDES ; D_STD_INCLUDES ; D_CORE_INCLUDES ; DDEPS.."TinyXML" },
	Libs	= {	"ck2" ; "vxmath"  ; "user32" ;  "kernel32" ; "vtPhysXLib"	},
	LibDirectories = { D_PHYSX.."SDKs/lib/Win32" },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\\BuildingBlocks" .."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower().."\\Plugins"
}


--	An interface plug-in to create dialogs for physic related parameters.
-- Depends on packageConfig_vtAgeiaLib
packageConfig_vtAgeiaInterface =
{
	Name 		= 	"vtPhysXInterface",
	Type		=	"SharedLib",
	TargetSuffix = "/InterfacePlugins",
	Defines 	= { DEF_STD_DIRECTIVES 	; 	DEF_PHYSX_DIRECTIVES ; "NX_USE_SDK_STATICLIBS" ; "NXCHARACTER_EXPORTS" ; "VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" },
	Files		= { D_UI_SRC.."*.cpp" ; D_UI_SRC.."*.def" ; D_INCLUDE.."Interface/*.h" },
	Includes  	= { D_PHYSX_INCLUDES ; D_STD_INCLUDES ; D_CORE_INCLUDES ; DDEPS.."TinyXML" ; D_INCLUDE.."Interface" },
	Libs	= {	"ck2" ; "vxmath"  ; "user32" ;  "kernel32" ; "vtPhysXLib" ; "DllEditor" ; "InterfaceControls" ;  "CKControls" ; "CK2Ui"	},
	LibDirectories = { D_PHYSX.."SDKs/lib/Win32" },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\\InterfacePlugins" .."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower().."\\InterfacePlugins"
}



--	If the command line contains --ExtraDefines="WebPack" , we add "WebPack" to the
--	pre-processor directives and also create a package to include the camera building blocks
--	as defined in packageConfig_CameraRepack
if _OPTIONS["ExtraDefines"]  then

	if _OPTIONS["ExtraDefines"]=="WebPack" then
		createStaticPackage(packageConfig_CameraRepack)
	end
end

createStaticPackage(packageConfig_Common)
createStaticPackage(packageConfig_Stream)
createStaticPackage(packageConfig_Character)
createStaticPackage(packageConfig_TinyXML)
createStaticPackage(packageConfig_vtAgeiaLib)
createStaticPackage(packageConfig_vtAgeiaBeh)

--createStaticPackage(packageConfig_vtAgeiaReader)
--createStaticPackage(packageConfig_vtAgeiaInterface)



--include "CppConsoleApp"

function onclean()
	os.rmdir("vs**")
end
