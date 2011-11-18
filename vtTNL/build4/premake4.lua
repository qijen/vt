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


packageConfig_libtomcrypt =
{
	Name 		= 	"libtomcrypt",
	Type		=	"StaticLib",
	Defines 	= { DEF_STD_DIRECTIVES },
	Files		= { DDEPS.."libtomcrypt/*.c"	; DDEPS.."libtomcrypt/*.h"},
	Includes  	= { DDEPS.."libtomcrypt" },
	ExludeFiles		= { DDEPS.."libtomcrypt/aes_tab.c" ;  DDEPS.."libtomcrypt/sha384.c" ;  DDEPS.."libtomcrypt/sha224.c" ; DDEPS.."libtomcrypt/rsa_sys.c" ;  DDEPS.."libtomcrypt/ecc_sys.c" ; DDEPS.."libtomcrypt/dh_sys.c" ; },
	Options = { "/W0"}
}

packageConfig_libTNL =
{
	Name 		= 	"libTNL",
	Type		=	"StaticLib",
	Defines 	= { DEF_STD_DIRECTIVES; "TNL_ENABLE_LOGGING" ; "TNL_DEBUG" },
	Files		= { DDEPS.."tnl/*.cpp"	;	DDEPS.."tnl/*.h"},
	Includes  	= { DDEPS.."libtomcrypt" ; },
	Libs	= {	},
	Options = { "/W0"}

}



--	The building blocks for vtPhysX, depending on packageConfig_vtAgeiaLib
packageConfig_vtTNL =
{
	Name 		= 	"vtTNL",
	Type		=	"SharedLib",
	TargetSuffix = "/BuildingBlocks",
	Defines 	= { DEF_STD_DIRECTIVES 	; 	"VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" ; "TNL_ENABLE_LOGGING" ; "TNL_DEBUG" },
	Files		= { DROOT.."SDK/src/Behaviors/**.cpp"	; DROOT.."SDK/src/Behaviors/*.def" ; D_DOCS_PAGES.."*.page" ; F_SHARED_SRC ; DROOT.."build4/**.lua" ; F_BASE_SRC ; D_CORE_SRC ; F_BASE_VT_SRC ; F_BASE_STD_INC},
	Includes  	= { D_STD_INCLUDES ; D_CORE_INCLUDES },
	Libs	= {	"ck2" ; "vxmath"  ; "user32" ;  "kernel32" ; "libTNL" ; "Ws2_32" ; "libtomcrypt" },
	LibDirectories = { },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\\BuildingBlocks" .."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower().."\\BuildingBlocks"

}

packageConfig_xConsoleServer =
{
	Name 		= 	"xConsoleServer",
	Type		=	"ConsoleApp",
	TargetSuffix = "",
	Defines 	= { DEF_STD_DIRECTIVES 	; 	"VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS"; "TNL_ENABLE_LOGGING" ; "TNL_DEBUG"  },
	Files		= { DROOT.."SDK/src/ConsoleServer/*.cpp"; F_SHARED_SRC ; F_BASE_SRC ; D_CORE_SRC ; F_BASE_VT_SRC ;  F_BASE_STD_INC  },
	Includes  	= { D_STD_INCLUDES ; D_CORE_INCLUDES },
	Libs	= {	"ck2" ; "vxmath" ; "user32" ;  "kernel32" ; "libTNL" ; "Ws2_32" ; "libtomcrypt" },
	LibDirectories = { },
	Options = { "/W0"},
	ExludeFiles		= { DROOT.."SDK/src/core/vtNetworkManager*.cpp" },
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower()

}

--	If the command line contains --ExtraDefines="WebPack" , we add "WebPack" to the
--	pre-processor directives and also create a package to include the camera building blocks
--	as defined in packageConfig_CameraRepack
if _OPTIONS["ExtraDefines"]  then

	if _OPTIONS["ExtraDefines"]=="WebPack" then
		createStaticPackage(packageConfig_CameraRepack)
	end
end

createStaticPackage(packageConfig_vtTNL)
createStaticPackage(packageConfig_libtomcrypt)
createStaticPackage(packageConfig_libTNL)
createStaticPackage(packageConfig_xConsoleServer)


--include "CppConsoleApp"

function onclean()
	os.rmdir("vs**")
end
