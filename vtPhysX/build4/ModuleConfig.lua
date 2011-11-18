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
	D_DX	= DDEPS.."dx/"

	D_BASE_INC  	= "../../usr/include/"
	D_VT_BASE_INC  	= D_BASE_INC.."virtools/"

	D_BASE_SRC = "../../usr/src/"
	D_BASE_VT_SRC = D_BASE_SRC.."virtools/"


	D_DOCS_PAGES = DROOT.."Doc/pages/"
	D_READER_SRC = D_SRC.."xmlstream/"
	D_UI_SRC = D_SRC.."Interface/"

	D_PHYSX			= DDEPS.."NVIDIA.PhysX.SDK/"
    D_NXSTREAM		= D_PHYSX.."Tools/NxuStream2/"
	D_NXCOMMON		= D_PHYSX.."SDKs/Common/"
	D_NXCHARACTER	= D_PHYSX.."SDKs/NxCharacter/"
	D_TINYXML		= DDEPS.."TinyXML/"

	D_CORE_INCLUDES  =
	{
		DROOT.."SDK/Include/Core/Common";
		DROOT.."SDK/Include/Core/Manager";
		DROOT.."SDK/Include/Core/pWorld";
		DROOT.."SDK/Include/Core/pCloth";
		DROOT.."SDK/Include/Core/pJoint";
		DROOT.."SDK/Include/Core/pVehicle";
		DROOT.."SDK/Include/Core/pRigidBody";
		DROOT.."SDK/Include/Core/pFactory";
		DROOT.."SDK/Include/Core/pFluid";
		DROOT.."SDK/Include/Core/pSerializer";
		DROOT.."SDK/Include/Core/pCharacter";
		DROOT.."SDK/Include/Core";
		DROOT.."SDK/Include/xmlstream";

	}

	D_STD_INCLUDES =
	{
		"../Shared";
		"../../usr/include";
		"../../usr/include/virtools";
	}



	D_PHYSX_INCLUDES =
	{
		D_PHYSX.."SDKs/Foundation/include";
	    D_PHYSX.."SDKs/Physics/include";
	    D_PHYSX.."SDKs/Physics/include/cloth";
	    D_PHYSX.."SDKs/Physics/include/fluids";
	    D_PHYSX.."SDKs/Physics/include/softbody";
	    D_PHYSX.."SDKs/NxVehicle";
	    D_PHYSX.."SDKs/Cooking/include";
	    D_PHYSX.."SDKs/PhysXLoader/include";
	    D_PHYSX.."SDKs/Common";
		D_PHYSX.."SDKs/NxCharacter/include";
	    D_PHYSX.."Tools/NxuStream2";
	}

	F_VT_STD_INC  =
	{
		D_VT_BASE_INC.."**.h";
		DROOT.."SDK/Include/xmlstream/XLoader.h";
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
		D_BASE_SRC.."xAssertion.cpp";
		D_BASE_SRC.."xAssertCustomization.cpp";
		D_BASE_SRC.."ConStream.cpp";
	}

	F_EXTRA_BB_SRC =
	{
		D_BASE_VT_SRC.."Behaviors/generic/GetSubBBId.cpp";
		D_BASE_VT_SRC.."Behaviors/joystick/hasFFe.cpp";
		D_BASE_VT_SRC.."Behaviors/joystick/JSetXYForce.cpp";

	}

	F_SHARED_SRC =
	{
		"../Shared/*.cpp";
	}





	DEF_STD_DIRECTIVES  	=
	{
		"WIN32";"_WINDOWS";
	}

	DEF_PHYSX_DIRECTIVES	=
	{
		"NOMINMAX";"_USRDLL";
	}


	--/************************************

	--~ 	Virtools SDK Paths. All files have to be inside the module path !


	DEV35DIR=DDEPS.."vt/Dev35/"
	--DEV40DIR="x:/sdk/dev4/SDK/"

	DEV40DIR=DDEPS.."vt/Dev40/"
	DEV41DIR=DDEPS.."vt/Dev41/"
	DEV5DIR=DDEPS.."vt/Dev5/"



	--~ 	Virtools Real Paths. This paths will be used as deployment targets due the post build command

	DEV_40_BIN="x:\\sdk\\dev4"
	DEV_41_BIN="x:\\sdk\\dev41"
	DEV_35_BIN="x:\\sdk\\dev35"
	DEV_5_BIN="x:\\sdk\\dev5"


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
















