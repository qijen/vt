--/************************************************************************/


	--Build/Compiler Setup.


--/************************************

	--Internal Paths Substion :

	FROOT  	= "../../"
	DROOT  	= "../"
	DDEPS   	= DROOT.."Dependencies/"
	DDOCS	= DROOT.."Docs"
	D_INCLUDE	= DROOT.."SDK/Include/"
	D_LIBS	= DROOT.."SDK/Lib/"
	D_SRC	= DROOT.."SDK/Src/"
	D_BB		= DROOT.."SDK/Behaviors/"

	D_BASE_INC  	= "../../usr/include/"
	D_VT_BASE_INC  	= D_BASE_INC.."virtools/"

	D_BASE_SRC = "../../usr/src/"
	D_BASE_VT_SRC = D_BASE_SRC.."virtools/"


    D_XSPLASH		= DDEPS.."xSplash/"
	D_XUTILS		= DDEPS.."xUtils/"
	D_DIRECTX9		= DDEPS.."dx/"

	D_CORE_INCLUDES  =
	{
		DROOT.."SDK/Include/Core";
		DROOT.."SDK/Include/Core/Common";
		DROOT.."SDK/Include/Core/CSharp";
	}

	D_STD_INCLUDES =
	{
		"../Shared";
		"../../usr/include";
		"../../usr/include/virtools";
		D_XSPLASH.."include";
		D_XUTILS.."include";
	}

	F_VT_STD_INC  =
	{
		D_VT_BASE_INC.."**.h";
	}

	F_BASE_STD_INC =
	{
		D_BASE_INC.."BaseMacros.h";
		D_BASE_INC.."DllTools.h";
		D_BASE_INC.."xBitSet.h";
		D_BASE_INC.."xLogger.h";
		D_BASE_INC.."uxString.h";
		D_BASE_INC.."pch.h";
	}

	F_BASE_VT_SRC =
	{
		D_BASE_VT_SRC.."vtTools.cpp";
	}

	F_BASE_SRC =
	{
		D_BASE_SRC.."xLogger.cpp";
		D_BASE_SRC.."ConStream.cpp";
	}

	F_SHARED_SRC =
	{
		"../Shared/*.cpp";
	}





	DEF_STD_DIRECTIVES  	=
	{
		"WIN32";"_WINDOWS"; "WINVER=0x0500"
	}


	--/************************************

	--~ 	Virtools SDK Paths. All files have to be inside the module path !


	DEV35DIR=DDEPS.."vt/Dev35/"
	--DEV40DIR="x:/sdk/dev4/SDK/"

	DEV40DIR=DDEPS.."vt/Dev40/"
	DEV41DIR=DDEPS.."vt/Dev41/"
	DEV41DIR=DDEPS.."vt/Dev5/"
	DEV5DIR=DDEPS.."vt/Dev5/"


	--~ 	Virtools Real Paths. This paths will be used as deployment targets due the post build command

	DEV_40_BIN="x:\\sdk\\dev4"
	DEV_41_BIN="x:\\sdk\\dev41"
	DEV_35_BIN="x:\\sdk\\dev35"
	DEV_35_BIN="x:\\sdk\\dev5"


	OUTPUT_PATH_OFFSETT_TO_PROJECTFILES	="../../Bin"
	OUTPUT_PATH_OFFSETT_TO_PROJECTFILES_MINUS_ONE	= "../Bin"

	OUTPUT_PATH_OFFSETT_TO_INTERNAL_LIBS="../SDK/Lib/"
	OUTPUT_PATH_OFFSETT_TO_TMP="../TEMP"


--*********************************

	--"Control how the outputs will be named and odererd in a filesystem"


	PSTORE_PER_VS_VERSION = true
	PSTORE_PER_VT_VERSION = true
	PSTORE_PER_CONFIG= false

	PSTORE_NAME_VS_VERSION = false
	PSTORE_NAME_VT_VERSION = true
	PSTORE_NAME_CONFIG= false



--/************************************

	--Internal Path Cache :

	MERGED_DST_PATH = OUTOUT_PATH_OFFSETT_TO_PROJECTFILES

--/************************************************************************/

	--Feauteres :


	F_VC_DOC_PROJECT = "true"
	F_VC_PREMAKE_PROJECT = "true"

--/************************************************************************/


	--POST COMPILE ROUTINES :

	DEPLOY_TO_DEV = 1
	DEPLOY_TO_ZIP = 0

--/************************************************************************/
















