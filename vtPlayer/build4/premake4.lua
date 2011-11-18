dofile("ModuleConfig.lua")
dofile("ModuleHelper.lua")



if _ACTION == "help" then
	premake.showhelp()
	return

end



solution "vtPlayer"

	configurations { "Debug", "Release" , "ReleaseDebug"   }

	if _ACTION and _OPTIONS["Dev"] then
		setfields("location","./".._ACTION.._OPTIONS["Dev"])
	end


packageConfig_XSplash =
{
	Name 			= 	"xSplash",
	Type			=	"SharedLib",
	TargetSuffix 	= "",
	Defines 		= { DEF_STD_DIRECTIVES ; "_AFXDLL" ; "MODULE_BASE_EXPORTS" },
	Files			= { D_XSPLASH.."src/**.cpp"	;	D_XSPLASH.."include/**.h" },
	Includes  		= { D_XSPLASH.."include" ; D_CORE_INCLUDES ; D_STD_INCLUDES },
	Options 		= { "/W0" ;  }
}

packageConfig_XUtils =
{
	Name 			= 	"xUtils",
	Type			=	"SharedLib",
	TargetSuffix 	= "",
	Defines 		= { DEF_STD_DIRECTIVES ; "_AFXDLL" ; "MODULE_BASE_EXPORTS" },
	Files			= { D_XUTILS.."src/**.cpp"	;	D_XUTILS.."include/**.h" },
	Includes  		= { D_STD_INCLUDES; D_XUTILS.."include/3D"  ; D_CORE_INCLUDES ; D_DIRECTX9.."Include" },
	Libs			= {	"Version" },
	LibDirectories 	= {   D_DIRECTX9.."lib"  },
	Options 		= { "/W0" ;  }
}





packageConfig_vtWindowLib =
{
	Name 		= 	"vtWindowLib",
	Type		=	"SharedLib",
	TargetSuffix = "",
	Defines 	= { DEF_STD_DIRECTIVES  ; "VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" ; "_AFXDLL" },
	Files		= { DROOT.."SDK/src/core/**.cpp"	;	DROOT.."SDK/include/core/**.h"  ; F_BASE_STD_INC ;F_VT_STD_INC; F_BASE_VT_SRC ;F_BASE_SRC;F_SHARED_SRC },
	Includes  	= {  D_STD_INCLUDES ; D_VT_BASE_INC; D_CORE_INCLUDES ;  D_DIRECTX9.."Include" ; DROOT.."SDK/src/core" },
	Libs	= {	 "ck2" ; "vxmath"  ; "user32" ;  "kernel32" ; "xSplash" ; "xUtils"	; "dxguid" ; "Version" },
	LibDirectories = {  D_DIRECTX9.."lib"  },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower()
}

packageConfig_vtCSWindow =
{
	Name 		= 	"vtCSWindow",
	Type		=	"SharedLib",
	TargetSuffix = "",
	Defines 	= { DEF_STD_DIRECTIVES  ; "VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" ; "_AFXDLL"  ; "EXAMPLEVC_EXPORTS" },
	Files		= { DROOT.."SDK/include/core/CSharp/*.cxx"	;	DROOT.."SDK/include/core/CSharp/*.i" ; F_BASE_VT_SRC ; DROOT.."SDK/src/core/vtWindow.cpp"},
	Includes  	= {  D_STD_INCLUDES ; D_VT_BASE_INC; D_CORE_INCLUDES ;  D_DIRECTX9.."Include" },
	Libs	= {	 "ck2" ; "vxmath"  ; "user32" ;  "kernel32" ; "xSplash" ; "xUtils"	; "dxguid" ; "Version" ;"vtWindowLib" ; },
	LibDirectories = {  D_DIRECTX9.."lib"  ; DROOT.."SDK/Include/Core" ; DROOT.."SDK/lib" },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower()
}

packageConfig_vtCSPlayerApp =
{
	Name 		= 	"vtPlayerCS",
	Type		=	"WindowedApp",
	TargetSuffix = "",
	Language = "C#",
	Defines 	= { DEF_STD_DIRECTIVES  ; "VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" ; "_AFXDLL"  },
	Files		= { DROOT.."SDK/Src/CSharp/*.cs" },
	Includes  	= { },
	Libs	= {	"vtCSWindow" ; "System" ; "System.Drawing" ; "System.Data" ; "System.Windows.Forms" ; "System.XML"  },
	LibDirectories = {  },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower()
}

packageConfig_vtCSharpBuildingBlocks =
{
	Name 		= 	"vtCSharpBuildingBlocks",
	Type		=	"SharedLib",
	TargetSuffix = "/BuildingBlocks",
	Defines 	= { DEF_STD_DIRECTIVES 	; "VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" },
	Files		= { DROOT.."SDK/src/Behaviors/**.cpp"	; DROOT.."SDK/src/Behaviors/*.rc" ; DROOT.."SDK/src/Behaviors/*.def" ; F_BASE_STD_INC ;F_VT_STD_INC; F_BASE_VT_SRC ;F_BASE_SRC;F_SHARED_SRC },
	Includes  	= { D_STD_INCLUDES ; D_CORE_INCLUDES ;},
	Libs	= {	"ck2" ; "vxmath"  ; "user32" ;  "kernel32" ; "Winmm" ;  },
	LibDirectories = {  },
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\\BuildingBlocks" .."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower().."\\BuildingBlocks"


}


packageConfig_vtPlayerApp =
{
	Name 		= 	"vtPlayer",
	Type		=	"WindowedApp",
	TargetSuffix = "",
	Defines 	= { DEF_STD_DIRECTIVES  ; "VIRTOOLS_USER_SDK" ; "MODULE_BASE_EXPORTS" ; "_AFXDLL" },
	Files		= { DROOT.."SDK/src/core/**.cpp"	;	DROOT.."SDK/include/core/**.h"  ; F_BASE_STD_INC ;F_VT_STD_INC; F_BASE_VT_SRC ;F_BASE_SRC;F_SHARED_SRC ; DROOT.."SDK/src/core/*.rc" },
	Includes  	= {  D_STD_INCLUDES ; D_VT_BASE_INC; D_CORE_INCLUDES ;  D_DIRECTX9.."Include" ; DROOT.."SDK/src/core"},
	Libs	= {	 "ck2" ; "vxmath"  ; "user32" ;  "kernel32" ; "xSplash" ; "xUtils"	; "dxguid" ; "Version" },
	LibDirectories = {  D_DIRECTX9.."lib"  ; DROOT.."SDK/Include/Core"},
	Options = { "/W0"},
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower()
}


packageConfig_vtConsoleApp =
{
	Name 		= 	"vtPlayerConsole",
	Type		=	"ConsoleApp",
	TargetSuffix = "",
	Defines 	= { DEF_STD_DIRECTIVES  ; "VIRTOOLS_USER_SDK" ; "_AFXDLL"  ;	},
	Files		= { DROOT.."SDK/src/Console/**.cpp"	;	DROOT.."SDK/include/core/**.h"  ; F_BASE_STD_INC ;F_VT_STD_INC; F_BASE_VT_SRC ;F_BASE_SRC;F_SHARED_SRC ; DROOT.."SDK/src/core/*.rc"; DROOT.."SDK/src/core/*.rc" ; DROOT.."SDK/src/core/vtWindow.cpp" },
	Includes  	= {  D_STD_INCLUDES ; D_VT_BASE_INC; D_CORE_INCLUDES ;  D_DIRECTX9.."Include" ;DROOT.."SDK/src/core"  },
	Libs	= {	  "user32" ;  "kernel32" ; "xSplash" ; "xUtils"	; "dxguid" ; "Version" ; "vtWindowLib" },
	LibDirectories = {  D_DIRECTX9.."lib"  ; DROOT.."SDK/Include/Core" ; DROOT.."SDK/lib" },
	Options = { "/W0"},
	StaticOptions = { "vtstatic" },
	StaticModules = { "vtWidgets" ; "vtToolkit" ; "vtPhysX" },
	PostCommand = "copy $(TargetDir)$(TargetFileName) "..getVTDirectory():lower().."\n".."copy $(TargetDir)$(InputName).pdb "..getVTDirectory():lower()
}

createStaticPackage(packageConfig_vtConsoleApp)
createStaticPackage(packageConfig_XSplash)
createStaticPackage(packageConfig_vtCSWindow)
createStaticPackage(packageConfig_XUtils)
createStaticPackage(packageConfig_vtWindowLib)
createStaticPackage(packageConfig_vtCSPlayerApp)
createStaticPackage(packageConfig_vtCSharpBuildingBlocks)
createStaticPackage(packageConfig_vtPlayerApp)




--createStaticPackage(packageConfig_vtAgeiaReader)
--createStaticPackage(packageConfig_vtAgeiaInterface)



--include "CppConsoleApp"

function onclean()
	os.rmdir("vs**")
end