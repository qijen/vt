#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "IParameter.h"


XString PhysicManager::_getConfigPath()
{

	XString result;


	CKPathManager *pm = (CKPathManager*)this->m_Context->GetPathManager();
	
	//pm->OpenSubFile()

	XString configFile("DonglePaths.ini");
	XString section("Paths");

	char Ini[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char szPath[MAX_PATH];

	GetModuleFileName(NULL,szPath,_MAX_PATH);
	_splitpath(szPath, drive, dir, NULL, NULL );
	sprintf(Ini,"%s%s%s",drive,dir,configFile);


	int     errorLine;
	XString errorText;
	VxConfiguration config;
	VxConfigurationSection *tsection = NULL;
	VxConfigurationEntry *entry = NULL;

	if (!config.BuildFromFile(Ini, errorLine, errorText))
	{
		MessageBox(NULL,"Cannot open DonglePath.Ini in Virtools directory",0,MB_OK|MB_ICONERROR);
		return XString("none");
	}

	if ((tsection = config.GetSubSection((char*)section.Str(), FALSE)) != NULL)
	{

		ConstEntryIt it  = tsection->BeginChildEntry();
		VxConfigurationEntry *sEntry  = NULL;


		char newPath[MAX_PATH];
		while (sEntry=tsection->GetNextChildEntry(it))
		{
			if (sEntry!=NULL)
			{
				const char * value = sEntry->GetValue();
				XString path(value);

				FILE *file  = fopen(path.CStr(),"r");
				if (file)
				{
					fclose(file);
					return path;
				}
			}
		}
		MessageBox(NULL,"Couldnt find any valid license file in the DonglePaths.Ini",0,MB_OK|MB_ICONERROR);
	}
	return result;
}


void PhysicManager::bindVariables()
{

	CKVariableManager* vm = m_Context->GetVariableManager();
	if( !vm )
		return;

	pSDKParameters& p = getSDKParameters();  
		
	vm->Bind( 
		"Physic/Skin Width", &p.SkinWidth, 0.025f, VxVar::COMPOSITIONBOUND,
		"Default value for pShape::skinWidth.");

	vm->Bind( 
		"Physic/Default Sleep Linear Velocity Squared", &p.DefaultSleepLinVelSquared, 0.15f*0.15f, VxVar::COMPOSITIONBOUND,
		"The default linear velocity, squared, below which objects start going to sleep. Note: Only makes sense when the pSDKP_BF_ENERGY_SLEEP_TEST is not set.");

	vm->Bind( 
		"Physic/Default Sleep Angular Velocity Squared", &p.DefaultSleepAngVel_squared, 0.14f * 0.14f, VxVar::COMPOSITIONBOUND,
		"The default angular velocity, squared, below which objects start going to sleep. Note: Only makes sense when the pSDKP_BF_ENERGY_SLEEP_TEST is not set.");

	vm->Bind( 
		"Physic/Bounce Threshold", &p.BounceThreshold , -2.0f  , VxVar::COMPOSITIONBOUND,
		"A contact with a relative velocity below this will not bounce.");

	vm->Bind( 
		"Physic/Dynamic Friction Scaling", &p.DynFrictScaling,1.0f , VxVar::COMPOSITIONBOUND,
		"This lets the user scale the magnitude of the dynamic friction applied to all objects. ");

	
	vm->Bind( 
		"Physic/Static Friction Scaling", &p.StaFrictionScaling,1.0f,VxVar::COMPOSITIONBOUND,
		"This lets the user scale the magnitude of the static friction applied to all objects.");



	vm->Bind( 
		"Physic/Maximum Angular Velocity", &p.MaxAngularVelocity , 7.0f, VxVar::COMPOSITIONBOUND,
		"See the comment for pRigidBody::setMaxAngularVelocity() for details.");


	vm->Bind( 
		"Physic/Continuous Collision Detection", &p.ContinuousCD ,0.0f, VxVar::COMPOSITIONBOUND,
		"Enable/disable continuous collision detection (0.0f to disable).");

	vm->Bind( 
		"Physic/Adaptive Force", &p.AdaptiveForce ,1.0f , VxVar::COMPOSITIONBOUND,
		"Used to enable adaptive forces to accelerate convergence of the solver. ");


	vm->Bind( 
		"Physic/Collision Veto Jointed", &p.CollVetoJointed , 1.0f , VxVar::COMPOSITIONBOUND,
		"Controls default filtering for jointed bodies. True means collision is disabled.");

	vm->Bind( 
		"Physic/Trigger Trigger Callback", &p.TriggerTriggerCallback, 1.0f , VxVar::COMPOSITIONBOUND,
		"Controls whether two touching triggers generate a callback or not.");



	vm->Bind( 
		"Physic/CCD Epsilon", &p.CCDEpsilon,0.01f, VxVar::COMPOSITIONBOUND,
		"Distance epsilon for the CCD algorithm.");

	vm->Bind( 
		"Physic/Solver Convergence Threshold", &p.SolverConvergenceThreshold, 0.0f , VxVar::COMPOSITIONBOUND,
		"Used to accelerate solver.");



	vm->Bind( 
		"Physic/BBox Noise Level", &p.BBoxNoiseLevel, 0.001f, VxVar::COMPOSITIONBOUND,
		"Used to accelerate HW Broad Phase. ");

	vm->Bind( 
		"Physic/Implicit Sweep Cache Size", &p.ImplicitSweepCacheSize, 5.0f , VxVar::COMPOSITIONBOUND,
		"Used to set the sweep cache size. ");

	vm->Bind( 
		"Physic/Default Sleep Energy", &p.DefaultSleepEnergy, 0.005f, VxVar::COMPOSITIONBOUND,
		"The default sleep energy threshold. Objects with an energy below this threshold are allowed to go to sleep. Note: Only used when the pSDKP_BF_ENERGY_SLEEP_TEST flag is set.");

	vm->Bind( 
		"Physic/Constant Fluid Max Packets", &p.ConstantFluidMaxPackets, 925, VxVar::COMPOSITIONBOUND,
		" Constant for the maximum number of packets per fluid. Used to compute the fluid packet buffer size in NxFluidPacketData.");

	vm->Bind( 
		"Physic/Constant Fluid Maximum Particles Per Step", &p.ConstantFluidMaxParticlesPerStep, 4096, VxVar::COMPOSITIONBOUND,
		"Constant for the maximum number of new fluid particles per frame.");

	vm->Bind( 
		"Physic/Improved Spring Solver", &p.ImprovedSpringSolver,1.0f , VxVar::COMPOSITIONBOUND,
		"Enable/disable improved spring solver for joints and wheel shapes.");

	vm->Bind( 
		"Physic/Disable Physics", &p.disablePhysics,false, VxVar::COMPOSITIONBOUND,
		"Enable/disable physic calculation");

	//////////////////////////////////////////////////////////////////////////
	pRemoteDebuggerSettings &dbgSetup = getRemoteDebuggerSettings();


	vm->Bind( 
		"Physic Debugger/Host", &dbgSetup.mHost,XString("localhost"), VxVar::COMPOSITIONBOUND,
		"Specifies the host running a debugger");

	vm->Bind( 
		"Physic Debugger/Port", &dbgSetup.port,5425, VxVar::COMPOSITIONBOUND,
		"Specifies the port of the remote debugger");

	vm->Bind( 
		"Physic Debugger/Enabled", &dbgSetup.enabled,0, VxVar::COMPOSITIONBOUND,
		"Enables/Disables the remote debugger");

	vm->Bind( 
		"Physic Console Logger/Errors", &_LogErrors,0, VxVar::COMPOSITIONBOUND,
		"Log Errors");

	vm->Bind( 
		"Physic Console Logger/Infos", &_LogInfo,0, VxVar::COMPOSITIONBOUND,
		"Log Infos");

	vm->Bind( 
		"Physic Console Logger/Trace", &_LogTrace,0, VxVar::COMPOSITIONBOUND,
		"Log Trace");

	vm->Bind( 
		"Physic Console Logger/Warnings", &_LogWarnings,0, VxVar::COMPOSITIONBOUND,
		"Log Warnings");

	vm->Bind( 
		"Physic Console Logger/Console", &_LogWarnings,0, VxVar::COMPOSITIONBOUND,
		"Console");
		

}

void PhysicManager::unBindVariables()
{


	CKVariableManager* vm = m_Context->GetVariableManager();
	if( !vm )
		return;

	pSDKParameters& p = getSDKParameters();  


	vm->UnBind( "Physic Console Logger/Errors" );
	vm->UnBind( "Physic Console Logger/Infos" );
	vm->UnBind( "Physic Console Logger/Trace" );
	vm->UnBind( "Physic Console Logger/Warnings" );
	vm->UnBind( "Physic Console Logger/Console" );



	vm->UnBind("Physic/Skin Width");
	vm->UnBind("Physic/Default Sleep Linear Velocity Squared");
	vm->UnBind( "Physic/Default Sleep Angular Velocity Squared ");
	vm->UnBind( "Physic/Bounce Threshold");
	vm->UnBind( "Physic/Dynamic Friction Scaling");
	vm->UnBind( "Physic/Static Friction Scaling");
	vm->UnBind( "Physic/Maximum Angular Velocity");
	vm->UnBind( "Physic/Continuous Collision Detection");
	vm->UnBind( "Physic/Adaptive Force ");
	vm->UnBind( "Physic/Collision Veto Jointed ");
	vm->UnBind( "Physic/Trigger Trigger Callback");
	vm->UnBind( "Physic/CCD Epsilon");
	vm->UnBind( "Physic/Solver Convergence Threshold");
	vm->UnBind( "Physic/BBox Noise Level");
	vm->UnBind( "Physic/Implicit Sweep Cache Size");
	vm->UnBind( "Physic/Default Sleep Energy");
	vm->UnBind( "Physic/Constant Fluid Max Packets");
	vm->UnBind( "Physic/Constant Fluid Maximum Particles Per Step");
	vm->UnBind( "Physic/Improved Spring Solver" );
	vm->UnBind( "Physic/Disable Physics");


		
	vm->UnBind( "Physic Debugger/Host" );
	vm->UnBind( "Physic Debugger/Port" );
	vm->UnBind( "Physic Debugger/Enabled" );

}



#define	PMANAGER_CHUNKID		1005
#define	PMANAGER_SAVE_VERSION		3
#define	PMANAGER_FLAGS		0
#define	PMANAGER_USER_ENUM_IDENTIFIER		11005

int PhysicManager::_migrateOldCustomStructures(CKScene *scnene)
{

	bool bIsLoading = ctx()->IsInLoad();
	//CKObject* o  = ctx->GetObject(ol[CKCID_MESH]);
	int count = ctx()->GetObjectsCountByClassID(CKCID_3DOBJECT);
	if (!count)
		return 0;

		
//	CK_ID* ol = ctx->GetObjectsListByClassID(i);
		
	XString errMessage;
	int nbOfObjects = 0 ;
	CKAttributeManager* attman = m_Context->GetAttributeManager();
	const XObjectPointerArray& Array = attman->GetAttributeListPtr(GetPAttribute());
	nbOfObjects = Array.Size();
	
	if (nbOfObjects==0)
		return 0;

	

	if (!scnene)
		return 0;

	int attOld = GetPAttribute();
	int attNew = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR);
	
	int nbRecovered=0;

	for (CKObject** it = Array.Begin(); it != Array.End(); ++it)
	{	
		int cCount = attman->GetAttributeListPtr(GetPAttribute()).Size();
		if (!cCount)
			break;
		
		CK3dEntity*target = static_cast<CK3dEntity*>(*it);
		if (!target)//shit happens
			break;

		CKParameterOut * pOld =target->GetAttributeParameter(attOld);
		if (!pOld)//shit happens
			break;

		pObjectDescr * oDescr = pFactory::Instance()->createPObjectDescrFromParameter(pOld);
		if (!oDescr)//shit happens
			continue;
		

		//----------------------------------------------------------------
		//
		// fix up : 
		//

		//hierarchy 
		bool hierarchy =false;
		if( (oDescr->flags & BF_Hierarchy) || oDescr->hirarchy )
			hierarchy = true;

		if (hierarchy)
			oDescr->flags << BF_Hierarchy;


		oDescr->massOffsetLinear = oDescr->massOffset;
		oDescr->pivotOffsetLinear = oDescr->shapeOffset;

		//----------------------------------------------------------------
		//
		// attach new attribute parameter
		//
		target->SetAttribute(attNew);
		CKParameterOut *newPar = target->GetAttributeParameter(attNew);
		if (newPar)
		{
			IParameter::Instance()->copyTo(newPar,oDescr);
		}

		//----------------------------------------------------------------
		//
		// clean old attribute
		//
		target->RemoveAttribute(attOld);
		//----------------------------------------------------------------
		//
		// set IC
		//
		
		
		//----- Restore the IC
		if(target->IsInScene(scnene)) 
		{
			CKStateChunk *chunk = scnene->GetObjectInitialValue(target);				
			if(chunk) 
			{
				CKStateChunk *chunk = CKSaveObjectState(target);
				scnene->SetObjectInitialValue(target,chunk);
			}
		}

		it = Array.Begin();

		SAFE_DELETE(oDescr);

		nbRecovered++;

		//----------------------------------------------------------------
		//
		// cleanup
		//

	}
	if (attman->GetAttributeListPtr(GetPAttribute()).Size() >0)
	{
		_migrateOldCustomStructures(GetContext()->GetCurrentScene());
	}
	
	/*
	errMessage.Format("nub of old objects recovered: %d",nbRecovered);
	xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errMessage.Str());

	*/
	return nbOfObjects;


}

CKERROR PhysicManager::PostLoad()
{
	CKScene *scene = GetContext()->GetCurrentScene();
	_migrateOldCustomStructures(scene);

	//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Post Load");
	return true;
}


void PhysicManager::setPSDKParameters(pSDKParameters&param)
{

	if (!getPhysicsSDK())
		return ;

	getPhysicsSDK()->setParameter((NxParameter)pSDKP_SkinWidth,param.SkinWidth);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_DefaultSleepLinVelSquared,param.DefaultSleepLinVelSquared);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_DefaultSleepAngVelSquared,param.DefaultSleepAngVel_squared);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_BounceThreshold,param.BounceThreshold);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_DynFrictScaling,param.DynFrictScaling);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_StaFrictionScaling,param.StaFrictionScaling);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_MaxAngularVelocity,param.MaxAngularVelocity);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_ContinuousCD,param.ContinuousCD);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_AdaptiveForce,param.AdaptiveForce);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_CollVetoJointed,param.CollVetoJointed);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_TriggerTriggerCallback,param.TriggerTriggerCallback);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_CCDEpsilon,param.CCDEpsilon);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_SolverConvergenceThreshold,param.SolverConvergenceThreshold);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_BBoxNoiseLevel,param.BBoxNoiseLevel);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_ImplicitSweepCacheSize,param.ImplicitSweepCacheSize);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_DefaultSleepEnergy,param.DefaultSleepEnergy);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_ConstantFluidMaxPackets,param.ConstantFluidMaxPackets);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_ConstantFluidMaxParticlesPerStep,param.ConstantFluidMaxParticlesPerStep);
	getPhysicsSDK()->setParameter((NxParameter)pSDKP_ImprovedSpringSolver,param.ImprovedSpringSolver);
	




}

CKStateChunk*  PhysicManager::SaveData(CKFile* SavedFile)
{

	if (!getNbObjects())
	{
		return NULL;
	}


	CKStateChunk *chunk = CreateCKStateChunk(PMANAGER_CHUNKID, SavedFile);
	if (!chunk)
		return NULL;
	chunk->StartWrite();
	chunk->WriteIdentifier(PMANAGER_CHUNKID);
	chunk->WriteInt(PMANAGER_SAVE_VERSION);
	chunk->WriteInt(PMANAGER_FLAGS);


	CKVariableManager* vm = m_Context->GetVariableManager();
	if( !vm )
		return NULL;


	pSDKParameters& p = getSDKParameters();  


	CKVariableManager::Variable  *var = vm->GetVariable("Physic/Skin Width");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.SkinWidth);
	}

	var = vm->GetVariable("Physic/Default Sleep Linear Velocity Squared");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.DefaultSleepLinVelSquared);
	}

	var = vm->GetVariable("Physic/Default Sleep Angular Velocity Squared");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.DefaultSleepAngVel_squared);
	}



	var = vm->GetVariable("Physic/Bounce Threshold");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.BounceThreshold);
	}



	var = vm->GetVariable("Physic/Dynamic Friction Scaling");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.DynFrictScaling);
	}



	var = vm->GetVariable("Physic/Static Friction Scaling");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.StaFrictionScaling);
	}

	var = vm->GetVariable("Physic/Maximum Angular Velocity");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.MaxAngularVelocity);
	}

	var = vm->GetVariable("Physic/Continuous Collision Detection");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.ContinuousCD);
	}

	var = vm->GetVariable("Physic/Adaptive Force");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.AdaptiveForce);
	}

	var = vm->GetVariable("Physic/Collision Veto Jointed");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.CollVetoJointed);
	}

	var = vm->GetVariable("Physic/Trigger Trigger Callback");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.TriggerTriggerCallback);
	}

	var = vm->GetVariable("Physic/CCD Epsilon");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.CCDEpsilon);
	}

	var = vm->GetVariable("Physic/Solver Convergence Threshold");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.SolverConvergenceThreshold);
	}

	var = vm->GetVariable("Physic/BBox Noise Level");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.BBoxNoiseLevel);
	}

	var = vm->GetVariable("Physic/Implicit Sweep Cache Size");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.ImplicitSweepCacheSize);
	}


	var = vm->GetVariable("Physic/Default Sleep Energy");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.DefaultSleepEnergy);
	}




	var = vm->GetVariable("Physic/Constant Fluid Max Packets");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.ConstantFluidMaxPackets);
	}



	var = vm->GetVariable("Physic/Constant Fluid Maximum Particles Per Step");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.ConstantFluidMaxParticlesPerStep);
	}


	

	var = vm->GetVariable("Physic/Improved Spring Solver");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteFloat(p.ImprovedSpringSolver);
	}

	//////////////////////////////////////////////////////////////////////////

	pRemoteDebuggerSettings &dbgSetup = getRemoteDebuggerSettings();

	var = vm->GetVariable("Physic Debugger/Host");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteString(dbgSetup.mHost.CStr());
	}

	var = vm->GetVariable("Physic Debugger/Port");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteInt(dbgSetup.port);
	}
	
	var = vm->GetVariable("Physic Debugger/Enabled");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteInt(dbgSetup.enabled);
	}


	
	var = vm->GetVariable("Physic Console Logger/Errors");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteInt(_LogErrors);
	}

	var = vm->GetVariable("Physic Console Logger/Infos");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteInt(_LogInfo);
	}

	var = vm->GetVariable("Physic Console Logger/Trace");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteInt(_LogTrace);
	}

	var = vm->GetVariable("Physic Console Logger/Warnings");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteInt(_LogWarnings);
	}

	
	_saveUserEnumeration(chunk,SavedFile);

	var = vm->GetVariable("Physic Console Logger/Console");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteInt(_LogToConsole);
	}


	//**new

	var = vm->GetVariable("Physic/Disable Physics");
	if (var)
	{
		chunk->WriteInt(var->IsCompositionDepending());
		chunk->WriteInt(p.disablePhysics);
	}
    
	chunk->CloseChunk();
	return chunk;
}

CKERROR PhysicManager::LoadData(CKStateChunk *chunk,CKFile* LoadedFile)
{


	assert(LoadedFile != 0);
	if (!chunk)
		return CKERR_INVALIDPARAMETER;
	

	chunk->StartRead();

	if (chunk->SeekIdentifier(PMANAGER_CHUNKID))
	{

		// Check the version
		int	version = chunk->ReadInt();

		// Check the flags
		int flags = chunk->ReadInt();

		CKVariableManager* vm = m_Context->GetVariableManager();
		if( !vm )
			return NULL;

		
		pSDKParameters& p = getSDKParameters();  


		//////////////////////////////////////////////////////////////////////////
		int isInCMO = chunk->ReadInt();
		float v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.SkinWidth  = v;
		}
		
		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.DefaultSleepLinVelSquared  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.DefaultSleepAngVel_squared  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.BounceThreshold  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.DynFrictScaling  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.StaFrictionScaling  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.MaxAngularVelocity  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.ContinuousCD  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.AdaptiveForce  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.CollVetoJointed  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.TriggerTriggerCallback  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.CCDEpsilon  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.SolverConvergenceThreshold  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.BBoxNoiseLevel  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.ImplicitSweepCacheSize  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.DefaultSleepEnergy  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.ConstantFluidMaxPackets  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.ConstantFluidMaxParticlesPerStep  = v;
		}


		//////////////////////////////////////////////////////////////////////////
		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.ImprovedSpringSolver  = v;
		}

		
		//////////////////////////////////////////////////////////////////////////
		pRemoteDebuggerSettings &dbgSetup = getRemoteDebuggerSettings();
		
		isInCMO = chunk->ReadInt();
		XString host = chunk->ReadString();
		if (isInCMO)
		{
			dbgSetup.mHost = host;
		}


		isInCMO = chunk->ReadInt();
		int port = chunk->ReadInt();

		if (isInCMO)
		{
			dbgSetup.port= port;
		}

		isInCMO = chunk->ReadInt();
		int enabled = chunk->ReadInt();

		if (isInCMO)
		{
			dbgSetup.enabled= enabled;
		}



		//////////////////////////////////////////////////////////////////////////
		
		isInCMO = chunk->ReadInt();
		int val = chunk->ReadInt();
		if (isInCMO==1 && val >=-1000 )
		{
			_LogErrors=val;
		}
		if (isInCMO==0)
		{
			_LogErrors=1;
		}

		isInCMO = chunk->ReadInt();
		val = chunk->ReadInt();
		if (isInCMO==1 && val >=-1000 )
		{
			_LogInfo=val;
		}

		isInCMO = chunk->ReadInt();
		val = chunk->ReadInt();
		if (isInCMO==1&& val >=-1000)
		{ 
			_LogTrace=val;
		}
		
		isInCMO = chunk->ReadInt();
		val = chunk->ReadInt();
		if (isInCMO==1&& val >=-1000 )
		{
			_LogWarnings=val;
		}

		_loadUserEnumeration(chunk,LoadedFile);

		isInCMO = chunk->ReadInt();
		val = chunk->ReadInt();
		if (isInCMO==1&& val >=-1000 )
		{
			_LogToConsole=val;
		}

		isInCMO = chunk->ReadInt();
		v = chunk->ReadFloat();

		if (isInCMO)
		{
			p.disablePhysics  = v;
		}

		chunk->CloseChunk();

		//disable physics
		

	}
	return CK_OK;
}



void PhysicManager::_saveUserEnumeration(CKStateChunk *chunk,CKFile* SavedFile)
{
	//////////////////////////////////////////////////////////////////////////
	//write a identifier, just to ensure due the load we are at right seek point !
	
	chunk->WriteInt(PMANAGER_USER_ENUM_IDENTIFIER);

	///////////////////////////////////////////////////////////////////////////
	//write out the the user enumeration for pWDominanceGroups
	XString enumDescription   = getEnumDescription(m_Context->GetParameterManager(),VTE_PHYSIC_DOMINANCE_GROUP);
	XString enumDescriptionCollGroup   = getEnumDescription(m_Context->GetParameterManager(),VTE_PHYSIC_BODY_COLL_GROUP);

	XString errString;
	errString.Format("Writing dominance enum :%s",enumDescription.Str());

	//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errString.CStr());
	
	chunk->WriteString(enumDescription.CStr());
	chunk->WriteString(enumDescriptionCollGroup.CStr());

}

void PhysicManager::_loadUserEnumeration(CKStateChunk *chunk,CKFile* LoadedFile)
{

	int enumIdentfier  = chunk->ReadInt();
	
	if (enumIdentfier!=PMANAGER_USER_ENUM_IDENTIFIER) return;

	CKParameterManager *pm = m_Context->GetParameterManager();
	//////////////////////////////////////////////////////////////////////////
	//we read our dominance group enumeration back :
	
	XString dominanceGroupEnumerationString;
	int strEnumDescSizeDG  = chunk->ReadString(dominanceGroupEnumerationString);

	XString collisionGroupEnumerationString;
	int strEnumDescSizeCG  = chunk->ReadString(collisionGroupEnumerationString);

	XString errString;
	errString.Format("Loading dominance enum :%s",dominanceGroupEnumerationString.Str());
	//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,dominanceGroupEnumerationString.CStr());

	CKParameterType pType = pm->ParameterGuidToType(VTE_PHYSIC_DOMINANCE_GROUP);
	if (pType!=-1 && dominanceGroupEnumerationString.Length())
	{
		pm->ChangeEnumDeclaration(VTE_PHYSIC_DOMINANCE_GROUP,dominanceGroupEnumerationString.Str());
	}

	//----------------------------------------------------------------
	//
	// collision group 
	//
	pType = pm->ParameterGuidToType(VTE_PHYSIC_BODY_COLL_GROUP);
	if (pType!=-1 && collisionGroupEnumerationString.Length())
	{
		XString enumDescriptionCollGroup   = getEnumDescription(m_Context->GetParameterManager(),VTE_PHYSIC_BODY_COLL_GROUP);
		
		pm->ChangeEnumDeclaration(VTE_PHYSIC_BODY_COLL_GROUP,collisionGroupEnumerationString.Str());

		//----------------------------------------------------------------
		//
		// 
		//
		CKEnumStruct *eStruct = pm->GetEnumDescByType(pType);
		if (eStruct)
		{
			int a = eStruct->GetNumEnums();
			if (a)
			{
				XString  enumString  = eStruct->GetEnumDescription(0);
				if (!enumString.Length())
				{
					pm->ChangeEnumDeclaration(VTE_PHYSIC_BODY_COLL_GROUP,enumDescriptionCollGroup.Str());
				}
			}
			int ab = eStruct->GetNumEnums();
			
		}

	}


}
CKERROR PhysicManager::PostSave()
{

	return CK_OK;
}