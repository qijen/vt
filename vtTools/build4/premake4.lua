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

solution "vtTools"

	configurations { "Debug", "Release" , "ReleaseDebug" }

	if _ACTION and _OPTIONS["Dev"] then
		setfields("location","./".._ACTION.._OPTIONS["Dev"])
	end



packageConfig_Behaviours =
{
	Name 		= 	"vtToolkit",
	Type		=	"SharedLib",
	Defines 	= { DEF_STD_DIRECTIVES 	; "VIRTOOLS_USER_SDK" ; "_AFXDLL" },
	TargetSuffix = "/BuildingBlocks",
	Files		= { D_INCLUDE_ROOT.."*.h" ; D_INCLUDE_ROOT.."Core/**.h"  ; D_CORE_SRC.."**.c*";	D_BB_SRC.."**.c*" ; D_BB_SRC.."*.def"  },
	Includes  	= { D_STD_INCLUDES ; D_CORE_INCLUDE_DIRS ; D_DIRECTX9.."Include"	},
	Libs	= {	"user32" ;  "kernel32" ; "dxerr"; "dinput8" ; "dxguid"	},
	LibDirectories = { 	D_DIRECTX9.."lib"  },
	StaticOptions = { "convert" },
	Options = { "/W0 /wd4430"},

	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\\BuildingBlocks" .."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower().."\\BuildingBlocks"

}

packageConfig_RemoteConsole =
{
	Name 		= 	"vtRemoteConsole",
	Type		=	"ConsoleApp",
	Defines 	= { DEF_STD_DIRECTIVES ; "VIRTOOLS_USER_SDK" },
	TargetSuffix = "",
	Files		= { D_INCLUDE_ROOT.."*.h" ; D_INCLUDE_ROOT.."Core/**.h"  ; DROOT.."Test/RemoteConsole/*.**"},
	Includes  	= { D_STD_INCLUDES ; D_CORE_INCLUDE_DIRS ; 	},
	Libs	= {	"ck2" ; "vxmath" ; "user32" ;  "kernel32" ;	},
	LibDirectories = { 	 },
	Options = { "/W1"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\\BuildingBlocks" .."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower().."\\BuildingBlocks"

}

createStaticPackage(packageConfig_Behaviours)
--createStaticPackage(packageConfig_RemoteConsole)


function onclean()
	print("cleaning")
	os.rmdir("vs*")
end
