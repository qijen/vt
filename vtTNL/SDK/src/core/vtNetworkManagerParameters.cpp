
#include "CKAll.h"
#include "vtNetworkManager.h"

#include "xNetTypes.h"
#include "tnl.h"
#include "tnlPlatform.h"

#include "xNetInterface.h"
#include "tnlGhostConnection.h"
#include "vtConnection.h"

#include "xDistributed3DObject.h"
#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"


#include "IDistributedObjectsInterface.h"

#include "xDistributedBaseClass.h"
#include "xDistributed3DObjectClass.h"
#include "IDistributedClasses.h"

#include "xPredictionSetting.h"
#include "vtGuids.h"
#include "xNetConstants.h"
#include "vtTools.h"


void ParamOpBGetErrorString(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpBGetErrorString(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	int  error;
	p1->GetValue(&error);
	if (error < 15)
	{
		res->SetStringValue(sErrorStrings[error]);
		
	}
}

void 
vtNetworkManager::RegisterParameters()
{



	CKParameterManager *pm = m_Context->GetParameterManager();
	CKAttributeManager* attman = m_Context->GetAttributeManager();


	pm->RegisterNewEnum(VTE_NETWORK_ERROR,"Network Error","Ok=0,\
			Intern=1,\
			No connection=2,\
			Not joined to a session=3,\
			Session needs password=4,\
			Session is locked=5,\
			Session already exists=6,\
			Session is full=7,\
			You must be session master=8,\
			Invalid parameter=9,\
			There is not such user=10,\
			Distributed class already exists=11,\
			Couldn't connect to any server=12,\
			You already joined to a session=13,\
			No such session=14");

	pm->RegisterOperationType(PARAM_OP_TYPE_GET_ERROR_TEXT, "GetString");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_GET_ERROR_TEXT,CKPGUID_STRING,VTE_NETWORK_ERROR,CKPGUID_NONE,ParamOpBGetErrorString);
}




















