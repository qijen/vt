dofile("ModuleConfig.lua")

function createDummyProject (packageConfig)

	project(packageConfig.Name)

	kind     "StaticLib"

	language "C++"

	premake.buildconfigs()


	-- 	External Virtools Directories ? If not the variables from ModuleConfig.lua will be
	--	used ! This assumes that the virtools headers and libs are in inside the module path
	if _OPTIONS["ExternalVTDirectory"] then
		dofile(_OPTIONS["ExternalVTDirectory"])
	end

	return premake.CurrentContainer

end

function evaluateConfigs(prj,packageConfig)

	for cfg in premake.eachconfig(prj) do


		configuration(cfg.name)


		setfields("targetdir", getBinDir(1).."/"..cfg.name.."/".._OPTIONS["Dev"])


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


		end

		setfields("objdir", OUTPUT_PATH_OFFSETT_TO_TMP.."/".._ACTION.."/"..prj.name.."/"..cfg.name )

		cfgDefine = { }

		if cfg.name == "Debug" then
			cfgDefine = { "DEBUG" ; "_DEBUG" }
		else
			cfgDefine = { "NDEBUG" }
		end

		if ( _ACTION == "vs2005") then
			cfgDefine  = table.join( cfgDefine, { "_CRT_SECURE_NO_DEPRECATE" } )
		end


		setfields("buildoptions", table.join( packageConfig.Options ) )
		setfields("defines", table.join( table.join(packageConfig.Defines),cfgDefine ) )
		setfields("flags", "Symbols" )
	end

end





function createStaticPackage (packageConfig)



	lastProject = createDummyProject(packageConfig)
	lastProject.name = packageConfig.Name

	setfields("location","./".._ACTION.._OPTIONS["Dev"])
	setfields("files",table.join(packageConfig.Files))
	setfields("kind",packageConfig.Type)
	setfields("links", table.join( packageConfig.Libs ) )


	--~ 	Handling a Virtools Package
	if table.contains(packageConfig.Defines,"VIRTOOLS_USER_SDK") then

		--~ 		Virtools Include Directories

		DevIncludeDir  = {	getVTDirectory():lower().."/SDK/Includes"	}
		packageConfig.Includes = table.join(packageConfig.Includes,DevIncludeDir)

		--~ 		Virtools Library Directories
		DevLibDir  = {	getVTDirectory():lower().."/SDK/Lib/Win32/Release"	}
		packageConfig.LibDirectories = table.join(packageConfig.LibDirectories,DevLibDir)

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
	trigger     = "Dev",
	value       = "Dev40",
	description = "The Virtools version"
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
