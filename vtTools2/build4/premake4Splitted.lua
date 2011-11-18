dofile("ModuleConfig.lua")
dofile("ModuleHelper.lua")


if _ACTION == "clean" then
	os.rmdir("./vs*")
	os.rmdir("../Temp")
	return
end

if _ACTION == "help" then
	premake.showhelp()
	return

end


solution "vtToolsAll"

	configurations { "Debug", "Release" , "ReleaseDebug" }

	if _ACTION and _OPTIONS["Dev"] then
		setfields("location","./".._ACTION.._OPTIONS["Dev"])
	end




packageConfig_TCP4 =
{
	Name 		= 	"Tcp4u",
	Type		=	"StaticLib",
	Defines 	= { DEF_STD_DIRECTIVES 	},
	Files		= { DDEPS.."tcp4u/include/*.h"	;	DDEPS.."tcp4u/src/*.c*"	},
	Includes  	= { DDEPS.."tcp4u/include" },
	Options = { "/W0"}

}

packageConfig_FTP4W32 =
{
	Name 		= 	"FTP4W32",
	Type		=	"StaticLib",
	Defines 	= { DEF_STD_DIRECTIVES 	},
	Files		= { DDEPS.."ftpw32/*.h"	;	DDEPS.."ftpw32/*.c*"	},
	Includes  	= { DDEPS.."tcp4u/include"; DDEPS.."ftpw32" },
	Options = { "/W0"}

}


D_NetIncDirs  = { DDEPS.."ftpw32" ; DDEPS.."tcp4u/include" }


--This compiles only the manager and exports all. Copies to Dev main directory
packageConfig_vtToolsLib =
{
	Name 		= 	"vtToolkitLib",
	Type		=	"SharedLib",
	Defines 	= { DEF_STD_DIRECTIVES 	; "VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" },
	TargetSuffix = "",
	Files		= { D_INCLUDE_ROOT.."*.h" ; D_INCLUDE_ROOT.."Core/**.h"  ; D_CORE_SRC.."**.c*"},
	Includes  	= { D_STD_INCLUDES ; D_CORE_INCLUDE_DIRS ; D_NetIncDirs	},
	Libs	= {	"ck2" ; "vxmath"  ; "user32" ;  "kernel32" ; "FTP4W32" ; "Tcp4u" ; "Ws2_32"	},
	LibDirectories = { 	 },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower()

}

packageConfig_vtToolsBehaviours =
{
	Name 		= 	"vtToolkit",
	Type		=	"SharedLib",
	Defines 	= { DEF_STD_DIRECTIVES 	; "VIRTOOLS_USER_SDK" ; },
	TargetSuffix = "/BuildingBlocks",
	Files		= { D_INCLUDE_ROOT.."*.h" ; D_INCLUDE_ROOT.."Core/**.h"  ; D_CORE_SRC.."**.c*";	D_BB_SRC.."**.c*" ; D_BB_SRC.."*.def"  },
	Includes  	= { D_STD_INCLUDES ; D_CORE_INCLUDE_DIRS ; D_NetIncDirs	},
	Libs	= {	"ck2" ; "vxmath"  ; "user32" ;  "kernel32" ; "vtToolkitLib"	},
	LibDirectories = { 	 },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\\BuildingBlocks" .."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower().."\\BuildingBlocks"

}

createStaticPackage(packageConfig_TCP4)
createStaticPackage(packageConfig_FTP4W32)
createStaticPackage(packageConfig_vtToolsLib)
createStaticPackage(packageConfig_vtToolsBehaviours)


function onclean()
	print("cleaning")
	os.rmdir("vs*")
end
