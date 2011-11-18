#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPJCreateDistanceJointDecl();
CKERROR CreatePJCreateDistanceJointProto(CKBehaviorPrototype **pproto);
int PJCreateDistanceJoint(const CKBehaviorContext& behcontext);
CKERROR PJCreateDistanceJointCB(const CKBehaviorContext& behcontext);

enum bbInputs
{

	bI_ObjectB,
	bI_Anchor0,
	bI_Anchor1,
	bI_Collision,
	bbI_Min,
	bbI_Max,
	bbI_Spring,
};

//************************************
// Method:    FillBehaviorPJCreateDistanceJointDecl
// FullName:  FillBehaviorPJCreateDistanceJointDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPJCreateDistanceJointDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJDistance");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Sets a distance joint between two bodies.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x52ff6b14,0x474e4938));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePJCreateDistanceJointProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreatePJCreateDistanceJointProto
// FullName:  CreatePJCreateDistanceJointProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePJCreateDistanceJointProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJDistance");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PJDistance  

	<br>
	PJDistance is categorized in \ref Joints
	<br>
	<br>See <A HREF="PJDistance.cmo">PJDistance.cmo</A>.

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Creates or modifies a distance joint.
	<br>
	<br>
	DOFs removed: 1<br>
	DOFs remaining: 5<br>
	<br>
	\image html distanceJoint.png


	The distance joint tries to maintain a certain minimum and/or maximum distance between two points attached to a pair of actors. It can be set to springy in order to behave like a rubber band.

	An example for a distance joint is a pendulum swinging on a string, or in the case of a springy distance joint, a rubber band between two objects.


	<h3>Technical Information</h3>

	\image html PJDistance.png

	<SPAN CLASS="in">In :</SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out :</SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Body B :</SPAN>The second body. Leave blank to create a joint constraint with the world. 
	<BR>
	<BR>
	<SPAN CLASS="pin">Local Anchor 0 :</SPAN>A point in bodies a local space. See pJointDistance::setLocalAnchor0().
	<BR>
	<BR>
	<SPAN CLASS="pin">Local Anchor 1 :</SPAN>A point in bodies a local space. See pJointDistance::setLocalAnchor1().
	<BR>
	<BR>
	<SPAN CLASS="pin">Collision :</SPAN>Enable Collision. See pJointDistance::enableCollision().
	<BR>
	<BR>

	<SPAN CLASS="pin">Minimum Distance :</SPAN>The minimum rest length of the rope or rod between the two anchor points.The value must be non-zero! See pJointDistance::setMinDistance().
	<BR>
	<BR>

	<SPAN CLASS="pin">Maximum Distance :</SPAN>The maximum rest length of the rope or rod between the two anchor points.	The value must be non-zero! See pJointDistance::setMaxDistance().
	<BR>
	<BR>
	
	<SPAN CLASS="pin">Joint Spring: </SPAN>The spring to make it springy. The spring.targetValue field is not used. See pJointDistance::setSpring().
	<BR>
	<BR>
	

	
	<SPAN CLASS="setting">Minimum Distance : </SPAN>Enables parameter input for minimum distance.
	<BR>
	<SPAN CLASS="setting">Maximum Distance : </SPAN>Enables parameter input for maximum distance.
	<BR>
	<SPAN CLASS="setting">Spring : </SPAN>Enables parameter input for the spring.
	<BR>

	<BR>
	<h3>Warning</h3>
	The body must be dynamic.
	<br>
	<br>

	<h3>VSL : Creation </h3><br>
	<SPAN CLASS="NiceCode">
	\include PJDistance.vsl
	</SPAN>
	<br>

	<h3>VSL : Modifiy  </h3><br>
	<SPAN CLASS="NiceCode">
	\include PJDistanceModify.vsl
	</SPAN>
	<br>
	<br>
	<br>

	Is utilizing #pRigidBody #pWorld #PhysicManager #pFactory::createDistanceJoint().<br>

	*/
	proto->DeclareInput("In0");
	proto->DeclareOutput("Out0");
	proto->SetBehaviorCallbackFct( PJCreateDistanceJointCB );
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	
	proto->DeclareInParameter("Local Anchor 0",CKPGUID_VECTOR,"0.0f");
	proto->DeclareInParameter("Local Anchor 1",CKPGUID_VECTOR,"0.0f");
	proto->DeclareInParameter("Collision",CKPGUID_BOOL,"TRUE");
	
	proto->DeclareInParameter("Minimum Distance",CKPGUID_FLOAT,"0.0f");
	proto->DeclareInParameter("Maximum Distance",CKPGUID_FLOAT,"0.0f");
	proto->DeclareInParameter("Joint Spring",VTS_JOINT_SPRING,"2.1,3.0,4.0");

	proto->DeclareSetting("Minimum Distance",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Maximum Distance",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Spring",CKPGUID_BOOL,"TRUE");


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(PJCreateDistanceJoint);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PJCreateDistanceJoint
// FullName:  PJCreateDistanceJoint
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PJCreateDistanceJoint(const CKBehaviorContext& behcontext)
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
		
		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,JT_Distance))
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



		if(bodyA && bodyB)
		{


			
			
			//anchor : 
			VxVector anchor0  = GetInputParameterValue<VxVector>(beh,bI_Anchor0);
			VxVector anchor1  = GetInputParameterValue<VxVector>(beh,bI_Anchor1);

			int coll = GetInputParameterValue<int>(beh,bI_Collision);


			float min= 0.0f;
			float max = 0.0f;

			
			DWORD minDistance;
			beh->GetLocalParameterValue(0,&minDistance);
			if (minDistance)
			{
				min = GetInputParameterValue<float>(beh,bbI_Min);
			}

			DWORD maxDistance;
			beh->GetLocalParameterValue(1,&maxDistance);
			if (maxDistance)
			{
				max = GetInputParameterValue<float>(beh,bbI_Max);
			}

			
			pSpring spring;
			DWORD hasspring;
			beh->GetLocalParameterValue(2,&hasspring);
			if (hasspring)
			{
				CKParameterIn *par = beh->GetInputParameter(bbI_Spring);
				if (par)
				{
					CKParameter *rPar = par->GetRealSource();
					if (rPar)
					{
						spring  = pFactory::Instance()->createSpringFromParameter(rPar);
					}
				}
			}else
			{
				spring.spring = 0.0f;
				spring.damper = 0.0f;
				spring.targetValue= 0.0f;
			}
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
		
			pJointDistance *joint = static_cast<pJointDistance*>(worldA->getJoint(target,targetB,JT_Distance));
		
			if (!joint)
			{
				joint   = static_cast<pJointDistance*>(pFactory::Instance()->createDistanceJoint(target,targetB,anchor0,anchor1,min,max,spring));
			}
			if (joint)
			{

				joint->setMinDistance(min);
				joint->setMaxDistance(max);
				joint->setLocalAnchor0(anchor0);
				joint->setLocalAnchor1(anchor1);
				joint->setSpring(spring);
				joint->enableCollision(coll);

			}
		}
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    PJCreateDistanceJointCB
// FullName:  PJCreateDistanceJointCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PJCreateDistanceJointCB(const CKBehaviorContext& behcontext)
{
	
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
			
			DWORD minDistance;
			beh->GetLocalParameterValue(0,&minDistance);
			beh->EnableInputParameter(bbI_Min,minDistance);
			
			DWORD maxDistance;
			beh->GetLocalParameterValue(1,&maxDistance);
			beh->EnableInputParameter(bbI_Max,maxDistance);

			DWORD spring;
			beh->GetLocalParameterValue(2,&spring);
			beh->EnableInputParameter(bbI_Spring,spring);

		
	
			

	
			

		break;
	}
	}
	return CKBR_OK;
}



