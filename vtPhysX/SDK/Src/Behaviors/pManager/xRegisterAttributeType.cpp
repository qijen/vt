#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorRegisterAttributeTypeDecl();
CKERROR CreateRegisterAttributeTypeProto(CKBehaviorPrototype **pproto);
int RegisterAttributeType(const CKBehaviorContext& behcontext);
CKERROR RegisterAttributeTypeCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_Name,
	bbI_Category,
	bbI_DefValue,
	bbI_PType,
	bbI_Class,
	bbI_User,
	bbI_Save,



};




//************************************
// Method:    FillBehaviorRegisterAttributeTypeDecl
// FullName:  FillBehaviorRegisterAttributeTypeDecl
// Access:    public
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorRegisterAttributeTypeDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("RegisterAttribute");
	od->SetCategory("Physic/Manager");
	od->SetDescription("Registers a new attribute type.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x63e567c4,0x65583276));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateRegisterAttributeTypeProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreateRegisterAttributeTypeProto
// FullName:  CreateRegisterAttributeTypeProto
// Access:    public
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreateRegisterAttributeTypeProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("RegisterAttribute");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->DeclareOutput("Error");

	proto->DeclareInParameter("Name",CKPGUID_STRING);
	proto->DeclareInParameter("Category",CKPGUID_STRING);
	proto->DeclareInParameter("Default Value",CKPGUID_STRING);
	proto->DeclareInParameter("Parameter Type",CKPGUID_PARAMETERTYPE);
	proto->DeclareInParameter("Compatible Class",CKPGUID_CLASSID);

	proto->DeclareInParameter("User",CKPGUID_BOOL,"TRUE");
	proto->DeclareInParameter("Save",CKPGUID_BOOL,"TRUE");


	proto->SetBehaviorCallbackFct( RegisterAttributeTypeCB );


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	
	proto->SetFunction(RegisterAttributeType);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    RegisterAttributeType
// FullName:  RegisterAttributeType
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int RegisterAttributeType(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();

	CKAttributeManager* attman = ctx->GetAttributeManager();
	CKParameterManager *pMan  = ctx->GetParameterManager();

	using namespace vtTools::BehaviorTools;

	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);

		
		CKSTRING name = GetInputParameterValue<CKSTRING>(beh,bbI_Name);
		CKSTRING category = GetInputParameterValue<CKSTRING>(beh,bbI_Category);
		CKSTRING defValue = GetInputParameterValue<CKSTRING>(beh,bbI_DefValue);
		int pType = GetInputParameterValue<int>(beh,bbI_PType);
		CK_CLASSID classId = GetInputParameterValue<CK_CLASSID>(beh,bbI_Class);

		int attFlags = 0 ;
		int user = GetInputParameterValue<int>(beh,bbI_User);
		int save = GetInputParameterValue<int>(beh,bbI_Save);

		if(user)
			attFlags|=CK_ATTRIBUT_USER;

		if(save)
			attFlags|=CK_ATTRIBUT_TOSAVE;

		attFlags|=CK_ATTRIBUT_CAN_DELETE;

		CKAttributeType aIType = attman->GetAttributeTypeByName(name);
		if (aIType!=-1)
		{
			beh->ActivateOutput(1);
		}

		int att =  attman->RegisterNewAttributeType(name,pMan->ParameterTypeToGuid(pType),classId,(CK_ATTRIBUT_FLAGS)attFlags);

		if (strlen(category))
		{
			attman->AddCategory(category);
			attman->SetAttributeCategory(att,category);
		}
		if (strlen(defValue))
		{
			attman->SetAttributeDefaultValue(att,defValue);
		}

	}
	pm->populateAttributeFunctions();
	pm->_RegisterAttributeCallbacks();

	beh->ActivateOutput(0);
	return 0;
}

//************************************
// Method:    RegisterAttributeTypeCB
// FullName:  RegisterAttributeTypeCB
// Access:    public
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR RegisterAttributeTypeCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	/*
	switch(behcontext.CallbackMessage) {

	case CKM_BEHAVIORCREATE:
	case CKM_BEHAVIORLOAD:
		{
			BB_LOAD_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
		case CKM_BEHAVIORDETACH:
		{
			BB_DESTROY_PIMAP;
			break;
		}

		case CKM_BEHAVIORATTACH:
		{
			BB_INIT_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
		case CKM_BEHAVIORSETTINGSEDITED:
		{
			BB_REMAP_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
	}
	*/
	return CKBR_OK;

}

