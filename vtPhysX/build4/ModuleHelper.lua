dofile("ModuleConfig.lua")


--	Prepares a Visual Studio Project
function createDummyProject (packageConfig)

	project(packageConfig.Name)

	kind     "StaticLib"

	language "C++"

	premake.buildconfigs()

	-- 	External Virtools Directories ? If not, the variables from ModuleConfig.lua will be used !
	--		This assumes that the virtools headers and libs are in inside the module path : Dependencies\vt\...
	if _OPTIONS["ExternalVTDirectory"] then
		dofile(_OPTIONS["ExternalVTDirectory"])
	end

	return premake.CurrentContainer

end


--	Modifies an project according to a supplied package config
function evaluateConfigs(prj,packageConfig)

	for cfg in premake.eachconfig(prj) do

	configuration(cfg.name)

		if packageConfig.TargetSuffix then
			setfields("targetdir", getBinDir(1).."/"..cfg.name.."/".._OPTIONS["Dev"]..packageConfig.TargetSuffix)
		else
			setfields("targetdir", getBinDir(1).."/"..cfg.name.."/".._OPTIONS["Dev"])
		end


		if packageConfig.Language then
			setfields("language", packageConfig.Language)
		end


		cfgDefine = { 	}
		cfgLinks = 	{	}

		--~ 	Handling Virtools Packages
		if table.contains(packageConfig.Defines,"VIRTOOLS_USER_SDK") then

			--			Compile to Virtools Dev directory ? :
 			if _OPTIONS["DeployDirect"] =="TRUE" then
 				setfields("targetdir", getVTDirectory():lower()..packageConfig.TargetSuffix )
 			end

			-- 	No?, then we compile to ../Bin/CfgName/Dev-Version  and add a post compile step`which copies all stuff to dev
			if (_OPTIONS["DeployDirect"]==nil and packageConfig.PostCommand and packageConfig.Type == "SharedLib") then
				setfields("postbuildcommands", packageConfig.PostCommand)
			end

			if _OPTIONS["Static"] =="TRUE" then
				cfgDefine  = table.join( cfgDefine, {  "VX_LIB" ; "CK_LIB" ; "VNK_LI" ; "VSU_LIB" ; "VSM_LIB" ; "VSS_LIB" ; "CUSTOM_PLAYER_STATIC";"_AFXDLL"   } )
				if (packageConfig.Type=="WindowApp") or ( packageConfig.StaticOptions and (table.contains(packageConfig.StaticOptions,"vtstatic")) ) then
					cfgLinks  = table.join( cfgLinks, {  "VxMathStatic" ; "CK2Static" ; "CKZlibStatic" } )
				end

			else
				cfgDefine  = table.join( cfgDefine, {  "MODULE_BASE_EXPORTS"   } )
				cfgLinks  = table.join( cfgLinks, {  "VxMath" ; "CK2" } )
			end

		end


		if _OPTIONS["Static"] =="TRUE" then
			cfgDefine  = table.join( cfgDefine, {  "MODULE_STATIC"   } )
		else
			cfgDefine  = table.join( cfgDefine, {  "MODULE_BASE_EXPORTS"   } )
		end







		setfields("objdir", OUTPUT_PATH_OFFSETT_TO_TMP.."/".._ACTION.._OPTIONS["Dev"].."/".._ACTION.."/"..prj.name.."/"..cfg.name )

		--print(OUTPUT_PATH_OFFSETT_TO_TMP.."/".._OPTIONS["Dev"].."/".._ACTION.."/"..prj.name.."/"..cfg.name)





		-- If the compilation is intended as end-user distribution, please define ReleaseRedist
		--	Otherwise the end user can use OUR building blocks for authoring. This is explicitly
		--	restricted and a license violation towards VTMOD
		if cfg.name == "ReleaseRedist" then
			cfgDefine  = table.join( cfgDefine, { "ReleaseRedist" ; "VIRTOOLS_RUNTIME_VERSION" } )
		end

		if cfg.name == "ReleaseDemo" then
			cfgDefine  = table.join( cfgDefine, { "ReleaseDemo" } )
		end

		--	Solving a Premake Bug
		if cfg.name == "Debug" then
			cfgDefine  = table.join( cfgDefine, {  "DEBUG" ; "_DEBUG" } )
            setfields("flags", "Symbols" )
		else
			cfgDefine  = table.join( cfgDefine, {  "NDEBUG" } )
			setfields("flags", "Optimize" )
		end

		if cfg.name == "ReleaseDebug" then
            setfields("flags", "Symbols" )
		end

		-- Visual Studio 2005 specific

		if ( _ACTION == "vs2005") then
			cfgDefine  = table.join( cfgDefine, { "_CRT_SECURE_NO_DEPRECATE" } )
		end


		--	web player fix
		if _OPTIONS["ExtraDefines"] and _OPTIONS["ExtraDefines"]=="WebPack" then
				packageConfig.Defines = table.join(packageConfig.Defines,{ "WebPack"  })
				--cfgDefine  = table.join( cfgDefine, { "ReleaseRedist" ; "VIRTOOLS_RUNTIME_VERSION" } )
		end


		setfields("buildoptions", table.join( packageConfig.Options ) )
		setfields("defines", table.join( table.join(packageConfig.Defines),cfgDefine ) )
		setfields("links", table.join( table.join(packageConfig.links),cfgLinks) )

	end

end





function createStaticPackage (packageConfig)



	lastProject = createDummyProject(packageConfig)
	lastProject.name = packageConfig.Name

	setfields("location","./".._ACTION.._OPTIONS["Dev"])
	setfields("files",table.join(packageConfig.Files))

	--if table.contains(packageConfig.Defines,"VIRTOOLS_USER_SDK") then
		if (_OPTIONS["Static"] =="TRUE" and packageConfig.Type=="SharedLib" ) then
			if ( packageConfig.StaticOptions and (table.contains(packageConfig.StaticOptions,"convert")) ) then
			packageConfig.Type ="StaticLib"
			end
		end
	--end

	setfields("kind",packageConfig.Type)
	setfields("links", table.join( packageConfig.Libs ) )


	--~ 	Handling a Virtools Package
	if table.contains(packageConfig.Defines,"VIRTOOLS_USER_SDK") then

		--~ 		Virtools Include Directories


		if _OPTIONS["ExtraDefines"] and _OPTIONS["ExtraDefines"]=="WebPack" then
			packageConfig.Libs = table.join(packageConfig.Libs,{ "Camera" })
			setfields("links", table.join( packageConfig.Libs ) )
		end

		DevIncludeDir  = {	getVTDirectory():lower().."/SDK/Includes"	}
		packageConfig.Includes = table.join(packageConfig.Includes,DevIncludeDir)

		--~ 		Virtools Library Directories
		DevLibDir  = {	getVTDirectory():lower().."/SDK/Lib/Win32/Release"	}
		packageConfig.LibDirectories = table.join(packageConfig.LibDirectories,DevLibDir)

		--		Handling Static  ----


	end


	setfields("includedirs", table.join(packageConfig.Includes))
	setfields("libdirs", table.join( packageConfig.LibDirectories ) )


	evaluateConfigs(lastProject,packageConfig)

end

function getVTDirectory()

	if _OPTIONS["Dev"] == "Dev35" then
		return DEV35DIR
	end

	if _OPTIONS["Dev"] == "Dev40" then
		return DEV40DIR
	end

	if _OPTIONS["Dev"] == "Dev41" then
		return DEV41DIR
	end

	if _OPTIONS["Dev"] == "Dev5" then
		return DEV5DIR
	end

	return DEV40DIR

end

function setfields (name, value)

	local kind    = premake.fields[name].kind
	local scope   = premake.fields[name].scope
	local allowed = premake.fields[name].allowed

	if (kind == "string") then
		return premake.setstring(scope, name, value, allowed)
	elseif (kind == "path") then
		return premake.setstring(scope, name, path.getabsolute(value))
	elseif (kind == "list") then
		return premake.setarray(scope, name, value, allowed)
	elseif (kind == "dirlist") then
		return premake.setdirarray(scope, name, value)
	elseif (kind == "filelist") then
		return premake.setfilearray(scope, name, value)
	end


end

function getBinDir(oneUp)
	res= OUTPUT_PATH_OFFSETT_TO_PROJECTFILES

	if oneUp==1 then
		res= OUTPUT_PATH_OFFSETT_TO_PROJECTFILES_MINUS_ONE
	end

	return res

end

newoption
{
	trigger     = "Static",
	value       = "false",
	description = "Compiles static library or output"
}

newoption
{
	trigger     = "Dev",
	value       = "Dev40",
	description = "Sets the Virtools version"
}


newoption
{
	trigger     = "DeployDirect",
	value       = "TRUE",
	description = "Compiles to ../Bin/ConfigName/DevVersion/ or to DEV_XX_BIN"

}

newoption
{
	trigger     = "ExternalVTDirectory",
	value       = "TRUE",
	description = "Uses the Virtools directories, specified in VTPaths.lua or ModuleConfig.lua(DEVXXDIR)"

}

newoption
{
	trigger     = "ExtraDefines",
	value       = "",
	description = "Adds additional macro directives",

}
