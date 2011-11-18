#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPMaterialIteratorDecl();
CKERROR CreatePMaterialIteratorProto(CKBehaviorPrototype **pproto);
int PMaterialIterator(const CKBehaviorContext& behcontext);
CKERROR PMaterialIteratorCB(const CKBehaviorContext& behcontext);

using namespace vtTools::BehaviorTools;
using namespace vtTools::ParameterTools;


enum bInputs
{
	I_XML,
	I_DFRICTION,
	I_SFRICTION,
	I_RES,
	I_DFRICTIONV,
	I_SFRICTIONV,
	I_ANIS,
	I_FCMODE,
	I_RCMODE,
	I_FLAGS,
};

enum bOutputs
{
	O_INDEX,
	O_XML,
	O_NAME,
	O_DFRICTION,
	O_SFRICTION,
	O_RES,
	O_DFRICTIONV,
	O_SFRICTIONV,
	O_ANIS,
	O_FCMODE,
	O_RCMODE,
	O_FLAGS,
	O_MATERIAL,
};

CKERROR PMaterialIteratorCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;
	CKContext *ctx = beh->GetCKContext();

	switch(behcontext.CallbackMessage) 
	{

		case CKM_BEHAVIORLOAD:
		case CKM_BEHAVIORATTACH:
		{

			break;

		}

		case CKM_BEHAVIORSETTINGSEDITED:
		{


		
			
		}
		
	}
	return CKBR_OK;

}

//************************************
// Method:    FillBehaviorPMaterialIteratorDecl
// FullName:  FillBehaviorPMaterialIteratorDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration*FillBehaviorPMaterialIteratorDecl()
{

	CKObjectDeclaration *od = CreateCKObjectDeclaration("PMaterialIterator");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Attaches and/or modifies the physic material of a rigid body or its sub shape");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x121d1c0f,0x65a62e73));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePMaterialIteratorProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePMaterialIteratorProto
// FullName:  CreatePMaterialIteratorProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePMaterialIteratorProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PMaterialIterator");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");

	proto->DeclareInput("Next");
	proto->DeclareOutput("Finish");
	proto->DeclareOutput("Loop");

	proto->DeclareLocalParameter("current result", CKPGUID_POINTER, "0");

	proto->DeclareOutParameter("Index",CKPGUID_INT);
	proto->DeclareOutParameter("XML Link",VTE_XML_MATERIAL_TYPE);
	proto->DeclareOutParameter("Name",CKPGUID_STRING);
	proto->DeclareOutParameter("Dynamic Friction",CKPGUID_FLOAT);
	proto->DeclareOutParameter("Static Friction",CKPGUID_FLOAT);
	proto->DeclareOutParameter("Restitution",CKPGUID_FLOAT);
	proto->DeclareOutParameter("Dynamic Friction V",CKPGUID_FLOAT);
	proto->DeclareOutParameter("Static Friction V",CKPGUID_FLOAT);
	proto->DeclareOutParameter("Direction of Anisotropy ",CKPGUID_VECTOR);
	proto->DeclareOutParameter("Friction Combine Mode",VTE_MATERIAL_COMBINE_MODE);
	proto->DeclareOutParameter("Restitution Combine Mode",VTE_MATERIAL_COMBINE_MODE);
	proto->DeclareOutParameter("Flags",VTF_MATERIAL_FLAGS);
	proto->DeclareOutParameter("Result",VTS_MATERIAL);

	proto->SetBehaviorCallbackFct( PMaterialIteratorCB );
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PMaterialIterator);
	*pproto = proto;
	return CK_OK;
}


//************************************
// Method:    PMaterialIterator
// FullName:  PMaterialIterator
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************

typedef std::vector<NxMaterial*>LMaterials;


int PMaterialIterator(const CKBehaviorContext& behcontext)
{
	
	CKBehavior* beh = behcontext.Behavior;

	CKContext* ctx = behcontext.Context;

	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	if (beh->IsInputActive(0))
	{
		beh->ActivateInput(0,FALSE);

		//////////////////////////////////////////////////////////////////////////
		//we reset our session counter
		int sessionIndex=-1;
		beh->SetOutputParameterValue(0,&sessionIndex);


		LMaterials*sResults = NULL;
		beh->GetLocalParameterValue(0,&sResults);
		if (!sResults)
		{
			sResults = new LMaterials();
		}else
			sResults->clear();


		NxScene * scene = GetPMan()->getDefaultWorld()->getScene();
		for(int i = 0 ; i < GetPMan()->getDefaultWorld()->getScene()->getNbMaterials() ; i ++)
		{
		
			NxMaterial *currentMaterial = scene->getMaterialFromIndex(i);

			sResults->push_back(currentMaterial);
		}

		beh->SetLocalParameterValue(0,&sResults);



		if (sResults->size())
		{
			beh->ActivateInput(1);
		}else
		{
			beh->ActivateOutput(0);
			return 0;
		}
	}




	if( beh->IsInputActive(1) )
	{
		beh->ActivateInput(1,FALSE);

		int currentIndex=0;	CKParameterOut *pout = beh->GetOutputParameter(0);		pout->GetValue(&currentIndex);
		currentIndex++;



		LMaterials *sResults = NULL;	beh->GetLocalParameterValue(0,&sResults);
		if (!sResults)		{			beh->ActivateOutput(0);			return 0;		}

		if (currentIndex>=sResults->size())
		{
			sResults->clear();
			beh->ActivateOutput(0);
			return 0;
		}

		NxMaterial * material =  sResults->at(currentIndex);
		if (material!=NULL)
		{

			int sIndex = currentIndex+1;
			beh->SetOutputParameterValue(0,&sIndex);

			

					
			//SetOutputParameterValue<int>(beh,O_XML,material->xmlLinkID);
			SetOutputParameterValue<float>(beh,O_DFRICTION,material->getDynamicFriction());
			SetOutputParameterValue<float>(beh,O_SFRICTION,material->getStaticFriction());

			SetOutputParameterValue<float>(beh,O_RES,material->getRestitution());

			SetOutputParameterValue<float>(beh,O_DFRICTIONV,material->getDynamicFrictionV());
			SetOutputParameterValue<float>(beh,O_SFRICTIONV,material->getStaticFrictionV());

			SetOutputParameterValue<VxVector>(beh,O_ANIS,getFrom(material->getDirOfAnisotropy()));
			SetOutputParameterValue<int>(beh,O_FCMODE,material->getFrictionCombineMode());
			SetOutputParameterValue<int>(beh,O_RCMODE,material->getFrictionCombineMode());
			SetOutputParameterValue<int>(beh,O_FLAGS,material->getFlags());
		}

		if(material->userData )
		{
			pMaterial *bMaterial  = static_cast<pMaterial*>(material->userData);

			if (bMaterial && bMaterial->xmlLinkID )
			{
			
				int xid = bMaterial->xmlLinkID;
				XString nodeName = vtAgeia::getEnumDescription(GetPMan()->GetContext()->GetParameterManager(),VTE_XML_MATERIAL_TYPE,xid);
				CKParameterOut *nameStr = beh->GetOutputParameter(O_NAME);
				nameStr->SetStringValue(nodeName.Str());
			}
		}
		//pFactory::Instance()->copyTo(beh->GetOutputParameter(O_MATERIAL),material);


		beh->ActivateOutput(0);
	}
	return 0;
}
