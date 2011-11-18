#include "CKAll.h"

CKObjectDeclaration	*FillBehaviorAddAttributesDecl();
CKERROR CreateAddAttributesProto(CKBehaviorPrototype **);
int AddAttributes(const CKBehaviorContext& behcontext);

CKERROR SetAttributeCallBack(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorAddAttributesDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Add Attribute by Substring");	
	
	od->SetCategory("Logics/Attribute");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x290f0795,0x16743e10));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateAddAttributesProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	
	return od;
}


CKERROR CreateAddAttributesProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Add Attribute by Substring");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	
	proto->DeclareInParameter("Attribute",CKPGUID_ATTRIBUTE);
	proto->DeclareInParameter("Key Substring", CKPGUID_STRING, "PM_");
	proto->DeclareInParameter("Class", CKPGUID_CLASSID);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(AddAttributes);

	proto->SetBehaviorCallbackFct( SetAttributeCallBack );
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)(CKBEHAVIOR_INTERNALLYCREATEDINPUTPARAMS));
	
	*pproto = proto;
	return CK_OK;
}


int AddAttributes(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx = behcontext.Context;

	char string[512];
	beh->GetInputParameterValue(1,&string);

	int Attribute=-1;
	beh->GetInputParameterValue(0,&Attribute);
	
	CKLevel* level = ctx->GetCurrentLevel();
	CK_CLASSID C_ID;
	beh->GetInputParameterValue(2,&C_ID);

	XObjectPointerArray myarray=level->ComputeObjectList(C_ID);
	for (CKObject** it = myarray.Begin(); it != myarray.End(); ++it) {
		CKObject *obj = *it;
		if (obj) {
			if (obj->GetName()) {
				if (strstr(obj->GetName(),string)) {
					CK3dEntity *ent = (CK3dEntity *)*it;
					ent->SetAttribute(Attribute);
					CKParameterOut* pout = ent->GetAttributeParameter(Attribute);
					if(pout) { 					
						CKParameter* real;
						CKParameterIn* tpin = (CKParameterIn*)beh->GetInputParameter(3);
						if(tpin && (real=tpin->GetRealSource())) pout->CopyValue(real);
					}
				}
			}
		}
	}
	return CKBR_OK;
}

CKERROR SetAttributeCallBack(const CKBehaviorContext& behcontext)
{
 	CKBehavior* beh = behcontext.Behavior;
  
	switch( behcontext.CallbackMessage ){
		case CKM_BEHAVIOREDITED:
		{
			CKAttributeManager* Manager = behcontext.AttributeManager;
				
			int Attribute=-1;
			beh->GetInputParameterValue(0,&Attribute);
			int ParamType = Manager->GetAttributeParameterType(Attribute); 
				
			if(ParamType != -1) { // we test if there is a parameter attached
				beh->EnableInputParameter(2,TRUE);
				CKParameterIn* pin = beh->GetInputParameter(3);
				if (pin) {
					if (pin->GetType()!=ParamType){
						pin->SetType( ParamType );
					}
				} else beh->CreateInputParameter("Attribute Value",ParamType);
			} else beh->EnableInputParameter(3,FALSE);
		}

	}
	beh->ActivateOutput(0);
	
  	return CKBR_OK; 
}

