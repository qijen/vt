#include "CKAll.h"
#include "vtNetworkManager.h"
#include <vtTools.h>
#include "xDistTools.h"
#include "xDistTypesAll.h"
#include "xMathTools.h"
#include "xDistributed3DObject.h"
#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"

#include "xNetInterface.h"
#include "vtConnection.h"

#include "xDistributedBaseClass.h"
#include "xDistributed3DObjectClass.h"
#include "IDistributedClasses.h"
#include "xDOStructs.h"

#include "IDistributedObjectsInterface.h"

#include "xDistributedPoint4F.h"
#include "xDistributedPoint3F.h"
#include "xDistributedPoint2F.h"
#include "xDistributedPoint1F.h"
#include "xDistributedString.h"
#include "xDistributedInteger.h"

CKObjectDeclaration	*FillBehaviorDODestroyedDecl();
CKERROR CreateDODestroyedProto(CKBehaviorPrototype **);
int DODestroyed(const CKBehaviorContext& behcontext);
CKERROR DODestroyedCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1




CKObjectDeclaration	*FillBehaviorDODestroyedDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("DODestroyed");	
	od->SetDescription("Notifies about a deleted Object");
	
	od->SetCategory("TNL/Distributed Objects");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x440a6b7a,0x2efb0cee));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Günter Baumgart");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateDODestroyedProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateDODestroyedProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("DODestroyed");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareInput("Stop");
	proto->DeclareInput("Next");

	proto->DeclareOutput("Exit On");
	proto->DeclareOutput("Exit Off");
	proto->DeclareOutput("Object");

	proto->DeclareOutput("Error");

	
	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	
	proto->DeclareOutParameter("Object", CKPGUID_BEOBJECT, "-1");
	proto->DeclareOutParameter("Object ID", CKPGUID_INT, "-1");
	proto->DeclareOutParameter("Error", CKPGUID_INT, "-1");

	

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(DODestroyed);
	proto->SetBehaviorCallbackFct(DODestroyedCB);

	*pproto = proto;
	return CK_OK;

}


int DODestroyed(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

	
	int connectionID=-1;
	beh->GetInputParameterValue(0,&connectionID);
	

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	
	if (beh->IsInputActive(1))
	{
		beh->ActivateInput(1,FALSE);
		beh->ActivateOutput(1);
		return 0;

	}

	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin || !cin->isValid() )
	{
		return CKBR_ACTIVATENEXTFRAME;
	}

	TNL::Vector<xDistDeleteInfo*>&clientTable = cin->getDistDeleteTable();

	for (int i  = 0  ; i < clientTable.size() ; i++  )
	{
		xDistDeleteInfo*cInfo = clientTable[i];
		if (cInfo->deleteState  == E_DO_DS_DELETED  )
		{
			int sID = cInfo->serverID;
			beh->SetOutputParameterValue(1,&sID);
			CKBeObject * obj = (CKBeObject*)ctx->GetObject(cInfo->entityID);
			if (obj)
			{
				beh->SetOutputParameterObject(0,obj);
			}
			beh->ActivateOutput(2);
			clientTable.erase(i);
			return CKBR_ACTIVATENEXTFRAME;
		}
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	return CKBR_ACTIVATENEXTFRAME;
}

CKERROR DODestroyedCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKBeObject *beo = (CKBeObject *)beh->GetTarget();
	if (!beo) {
		return CKBR_OWNERERROR;
	}
  
	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{
	
	}

	return CKBR_OK;
}