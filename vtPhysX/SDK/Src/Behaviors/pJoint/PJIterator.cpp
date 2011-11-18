#include <StdAfx.h>
#include "pCommon.h"

#include "pWorldCallbacks.h"



CKObjectDeclaration	*FillBehaviorPJIteratorDecl();
CKERROR CreatePJIteratorProto(CKBehaviorPrototype **pproto);
int PJIterator(const CKBehaviorContext& behcontext);
CKERROR PJIteratorCB(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorPJIteratorDecl()
{

	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJIterator");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Iterates through bodies joints");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x115941e7,0x15861d73));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePJIteratorProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePJIteratorProto
// FullName:  CreatePJIteratorProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
enum bInput
{
};
enum bbO
{

	bbO_BodyB,
	bbO_Type,

};
enum bbOT
{
	bbOT_Finish,
	bbOT_Next,
};
CKERROR CreatePJIteratorProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJIterators");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PJIterator


	PJIterators is categorized in \ref Collision

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Performs a ray cast test. Outputs hit informations.<br>
	See <A HREF="PWRayCasts.cmo">PWRayCasts.cmo</A> for example.

	<h3>Technical Information</h3>

	\image html PJIterators.png

	<SPAN CLASS="in">In: </SPAN>Triggers the process.
	<BR>
	<SPAN CLASS="in">Next: </SPAN>Next Hit.
	<BR>
	
	
	<SPAN CLASS="out">Yes: </SPAN>Hit occured.
	<BR>
	<SPAN CLASS="out">No: </SPAN>No hits.
	<BR>
	<SPAN CLASS="out">Finish: </SPAN>Last hit.
	<BR>
	<SPAN CLASS="out">Next: </SPAN>Loop out.
	<BR>





	<SPAN CLASS="pin">Target: </SPAN>World Reference. pDefaultWorld!
	<BR>
	<SPAN CLASS="pin">Ray Origin: </SPAN>Start of the ray.
	<BR>
	<SPAN CLASS="pin">Ray Origin Reference: </SPAN>Reference object to determine the start of the ray. Ray Origin becomes transformed if an object is given.
	<BR>
	<SPAN CLASS="pin">Ray Direction: </SPAN>Direction of the ray.
	<BR>
	<SPAN CLASS="pin">Ray Direction Reference: </SPAN>Reference object to determine the direction of the ray. Ray Direction becomes transformed if an object is given. Up axis will be used then.
	<BR>
	<SPAN CLASS="Length">Length: </SPAN>Lenght of the ray.
	<BR>
	<SPAN CLASS="pin">Shapes Types: </SPAN>Adds static and/or dynamic shapes to the test.
	<BR>

	<SPAN CLASS="pin">Groups: </SPAN>Includes specific groups to the test. 
	<BR>
	<SPAN CLASS="pin">Groups Mask: </SPAN>Alternative mask used to filter shapes. See #pRigidBody::setGroupsMask
	<BR>

	<SPAN CLASS="pout">Touched Body: </SPAN>The touched body.
	<BR>
	<SPAN CLASS="pout">Impact Position: </SPAN>Hit point in world space.
	<BR>
	<SPAN CLASS="pout">Face Normal: </SPAN>Normal of the hit.
	<BR>
	<SPAN CLASS="pout">Face Index: </SPAN>
	<BR>
	<SPAN CLASS="pout">Distance: </SPAN>Distance between ray start and hit.
	<BR>
	<SPAN CLASS="pout">UV: </SPAN> not used yet !
	<BR>
	<SPAN CLASS="pout">Material Index: </SPAN> Index of the internal physic material.
	<BR>
	
	
	
	<br>
	<br>

	Is utilizing #pWorld::raycastAllShapes() <br>
	
	*/
	
	proto->DeclareInput("In");
	proto->DeclareInput("Next");
	proto->DeclareOutput("Finish");
	proto->DeclareOutput("LoopOut");

	proto->DeclareLocalParameter("result", CKPGUID_POINTER);
	proto->DeclareLocalParameter("index", CKPGUID_INT);

	proto->DeclareOutParameter("Body B",CKPGUID_3DENTITY);
	proto->DeclareOutParameter("Joint Type",VTE_JOINT_TYPE);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetBehaviorCallbackFct( PJIteratorCB );
	proto->SetFunction(PJIterator);
	*pproto = proto;
	return CK_OK;
}

struct _pJoint
{
	pJoint *_j;
	CK3dEntity *_b;

};
typedef std::vector<_pJoint*>_pJointList;


//************************************
// Method:    PJIterator
// FullName:  PJIterator
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PJIterator(const CKBehaviorContext& behcontext)
{

	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	using namespace vtTools::ParameterTools;

	//////////////////////////////////////////////////////////////////////////
	//the object : 
	CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
	if( !target ) return CKBR_OWNERERROR;
	//////////////////////////////////////////////////////////////////////////
	// the world :  
	
	pWorld *world=GetPMan()->getWorldByBody(target); 
	if (!world)
		bbErrorME("No valid world object found");


	pRigidBody *body = NULL;

	body = GetPMan()->getBody(target);
	if (!body)	bbErrorME("Object not physicalized");

	/************************************************************************/
	/* we come in by reset                                                                     */
	/************************************************************************/

	if( beh->IsInputActive(0) )
	{

		beh->ActivateInput(0,FALSE);


		_pJointList *jarray = NULL;
		beh->GetLocalParameterValue(0,&jarray);
		if (jarray)
		{
			jarray->clear();
		}else
		{
			jarray =  new _pJointList();
		}
		
		beh->SetLocalParameterValue(0,&jarray);

		int hitIndex = 0;
		beh->SetLocalParameterValue(1,&hitIndex);

		//////////////////////////////////////////////////////////////////////////
		// get joint list
		int nbJoints = body->getNbJoints();
		if (nbJoints)
		{
			NxU32	jointCount = world->getScene()->getNbJoints();
			if (jointCount)
			{
				NxArray< NxJoint * > joints;

				world->getScene()->resetJointIterator();

				for	(NxU32 i = 0;	i	<	jointCount;	++i)
				{
					NxJoint	*j = world->getScene()->getNextJoint();
					pJoint *mJoint = static_cast<pJoint*>( j->userData );

					if ( mJoint )
					{
						if (mJoint->GetVTEntA() == target || mJoint->GetVTEntB() == target )
						{

							_pJoint *j = new _pJoint();
							j->_j = mJoint;
							

							if (mJoint->GetVTEntA() == target )
							{
								j->_b = mJoint->GetVTEntB();
							}

							if (mJoint->GetVTEntB() == target )
							{
								j->_b = mJoint->GetVTEntA();
							}
							
							jarray->push_back(j);
						}
					}
				}
			}
		}
		if (nbJoints)
		{
			beh->ActivateOutput(bbOT_Next);
			beh->ActivateInput(1,TRUE);

		}else{
			beh->ActivateOutput(bbOT_Finish);
		}
	}

	if( beh->IsInputActive(1) )
	{

		beh->ActivateInput(1,FALSE);
		_pJointList *carray = NULL;
		beh->GetLocalParameterValue(0,&carray);

		//////////////////////////////////////////////////////////////////////////
		if (carray)
		{
			if (carray->size())
			{
				_pJoint *hit = carray->at(carray->size()-1);
				if (hit)
				{
					beh->SetOutputParameterObject(bbO_BodyB,hit->_b);
					SetOutputParameterValue<int>(beh,bbO_Type,hit->_j->getType());
					carray->pop_back();
					if (carray->size())
					{
						beh->ActivateOutput(bbOT_Next);
					}else
					{
						beh->ActivateOutput(bbOT_Finish);
					}
				}
			}
		}

	}

	return 0;
}

//************************************
// Method:    PJIteratorCB
// FullName:  PJIteratorCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PJIteratorCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{

			
		
		}
		break;
	}
	return CKBR_OK;

}

