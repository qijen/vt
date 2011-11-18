#include <StdAfx.h>
#include "pCommon.h"

CKObjectDeclaration	*FillBehaviorJD6SetDriveDecl();
CKERROR CreateJD6SetDriveProto(CKBehaviorPrototype **pproto);
int JD6SetDrive(const CKBehaviorContext& behcontext);
CKERROR JD6SetDriveCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_BodyA=0,
	bbI_BodyB,
	bbI_Anchor,
	bbI_AnchorRef,
	bbI_Axis,
	bbI_AxisRef
};

//************************************
// Method:    FillBehaviorJD6SetDriveDecl
// FullName:  FillBehaviorJD6SetDriveDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorJD6SetDriveDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJD6SetDrive");	
	od->SetCategory("Physic/D6");
	od->SetDescription("Modifies the D6 drive.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x2db16c45,0x695938e4));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateJD6SetDriveProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreateJD6SetDriveProto
// FullName:  CreateJD6SetDriveProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreateJD6SetDriveProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJD6SetDrive");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PJD6SetDrive

	<br>
	PJD6SetDrive is categorized in \ref D6BB
	<br>
	<br>See <A HREF="PJD6.cmo">PJD6.cmo</A>.

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies a linear or an angular drive.
	<br>
	<br>
	<h3>Technical Information</h3>

	\image html PJD6SetDrive.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Body B: </SPAN>The second body to identfy the joint.
	<BR>
	<BR>

	<SPAN CLASS="pin">Target Drive: </SPAN>The target drive. See #D6DriveAxis.
	<BR>
	<SPAN CLASS="pin">Drive: </SPAN> Holds drive settings. See #pJD6Drive. 
	<BR>
	
	See \ref D6AngularDrivesGuide "Angular Drives"<br>
	
	See \ref D6LinearDrivesGuide "Linear Drives"<br>


	
*/
	
	proto->SetBehaviorCallbackFct( JD6SetDriveCB );
	
	
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	proto->DeclareInParameter("Target Drive",VTE_JOINT_DRIVE_AXIS,0);
	proto->DeclareInParameter("Drive",VTS_JOINT_DRIVE);


	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(JD6SetDrive);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    JD6SetDrive
// FullName:  JD6SetDrive
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int JD6SetDrive(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	
	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);

		//////////////////////////////////////////////////////////////////////////
		//the object A: 
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		CK3dEntity *targetB = (CK3dEntity *) beh->GetInputParameterObject(0);
		
		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,JT_D6))
		{

			return CK_OK;
		
		}
		
		
		
		// the world :  
		pWorld *worldA=GetPMan()->getWorldByBody(target); 
		pWorld *worldB=GetPMan()->getWorldByBody(targetB); 
		if (!worldA)
		{
			worldA = worldB;
		}
		
		if (!worldA)
		{
			beh->ActivateOutput(0);
			return 0;
		}
		
		// the physic object A : 
		pRigidBody*bodyA= worldA->getBody(target);
		pRigidBody*bodyB= worldA->getBody(targetB);

		if(bodyA || bodyB)
		{

			pJointD6 *joint =static_cast<pJointD6*>(worldA->getJoint(target,targetB,JT_D6));
			D6DriveAxis targetDrive = GetInputParameterValue<D6DriveAxis>(beh,1); 
			pJD6Drive drive;


			CKParameter* pout = beh->GetInputParameter(2)->GetRealSource();
			CK_ID* ids = (CK_ID*)pout->GetReadDataPtr();
			float damping,spring,forceLimit;
			int driveMode;
			pout = (CKParameterOut*)ctx->GetObject(ids[0]);
			pout->GetValue(&damping);
			pout = (CKParameterOut*)ctx->GetObject(ids[1]);
			pout->GetValue(&spring);
			pout = (CKParameterOut*)ctx->GetObject(ids[2]);
			pout->GetValue(&forceLimit);
			pout = (CKParameterOut*)ctx->GetObject(ids[3]);
			pout->GetValue(&driveMode);


			drive.damping = damping;
			drive.spring = spring;
			drive.driveType = driveMode;
			drive.forceLimit = forceLimit;

			if (joint)
			{
				switch(targetDrive)
				{

					case D6DA_Twist:
						joint->setTwistDrive(drive);
					break;
					case D6DA_Swing:
						joint->setSwingDrive(drive);
					break;
					case D6DA_Slerp:
						joint->setSlerpDrive(drive);
					break;
					case D6DA_X:
						joint->setXDrive(drive);
					break;
					case D6DA_Y:
						joint->setYDrive(drive);
					break;
					case D6DA_Z:
						joint->setZDrive(drive);
					break;
				}
			}
		}
		
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    JD6SetDriveCB
// FullName:  JD6SetDriveCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR JD6SetDriveCB(const CKBehaviorContext& behcontext)
{
	return CKBR_OK;
}

