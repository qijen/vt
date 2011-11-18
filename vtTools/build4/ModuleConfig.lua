--/************************************************************************/


	--Build/Compiler Setup.








--/************************************

	--Internal Paths Substitution :


	DROOT  	= "../"
	DDEPS   = DROOT.."Dependencies/"

	DDOCS	= DROOT.."Docs"

	D_INCLUDE_ROOT	= DROOT.."SDK/Include/"
	D_LIBS	= DROOT.."SDK/Lib/"


	D_CORE_SRC	= DROOT.."SDK/Src/Core/"
	D_BB_SRC		= DROOT.."SDK/Src/Behaviors/"
	D_DIRECTX9		= DDEPS.."dx/"

	D_CORE_INCLUDE_DIRS  =
	{
		D_INCLUDE_ROOT.."Core/Manager";
		D_INCLUDE_ROOT.."Core";
		D_INCLUDE_ROOT.."Core/Common";

	}

	D_STD_INCLUDES =
	{
		"../Shared";
		"../../usr/include";
	}

	DEF_STD_DIRECTIVES  	=
	{
		"WIN32";"_WINDOWS"
	}

	--/************************************

	--Your Paths settings :


	DEV35DIR=DDEPS.."vt/Dev35/"
	DEV40DIR=DDEPS.."vt/Dev40/"
	DEV41DIR=DDEPS.."vt/Dev41/"


	OUTPUT_PATH_OFFSETT_TO_PROJECTFILES	="../../Bin"
	OUTPUT_PATH_OFFSETT_TO_PROJECTFILES_MINUS_ONE	= "../Bin"

	OUTPUT_PATH_OFFSETT_TO_INTERNAL_LIBS="../SDK/Lib/"
	OUTPUT_PATH_OFFSETT_TO_TMP="../TEMP"













