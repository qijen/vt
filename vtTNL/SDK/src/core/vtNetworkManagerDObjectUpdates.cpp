#include "precomp.h"
#include "vtPrecomp.h"
#include "vtNetAll.h"
#include "vtNetworkManager.h"
#include <xDOStructs.h>
#include "xMathTools.h"


#include "xDistributedPoint3F.h"
#include "xDistributedPoint4F.h"
#include "xDistributedPoint2F.h"
#include "xDistributedPoint1F.h"

void
vtNetworkManager::updateLocalRotation(xDistributedObject* distObject,CK3dEntity *target,xDistributedProperty*prop)
{


	if (!distObject || !target || !prop)return;

	xDistributedQuatF*propValue = static_cast<xDistributedQuatF*>(prop);
	if (!propValue)return;

	xDistributed3DObject *dobj3D = static_cast<xDistributed3DObject*>(distObject);
	if (!dobj3D)return;

	switch(prop->getPropertyInfo()->mNativeType)
	{
	case E_DC_3D_NP_WORLD_ROTATION:
		{
			QuatF currentRot  = propValue->mLastServerValue;
			VxQuaternion t(currentRot.x,currentRot.y,currentRot.z,currentRot.w);
			target->SetQuaternion(&t,NULL,false,TRUE);

			/*
			Point4F currentPos = propValue->mLastServerValue;
			Point4F velocity = propValue->mLastServerDifference;

			VxVector realPos;
			target->GetPosition(&realPos);
			Point3F realPos2(realPos.x,realPos.y,realPos.z);


			float oneWayTime =  (distObject->GetNetInterface()->GetConnectionTime() / 1000.0f);
			oneWayTime +=oneWayTime * prop->getPredictionSettings()->getPredictionFactor();
			Point3F predictedPos  = currentPos + velocity * oneWayTime;
			Point3F realPosDiff  =  - predictedPos - realPos2 ;

			VxVector posNew(predictedPos.x,predictedPos.y,predictedPos.z);
			target->SetPosition(&posNew);*/

			/*
			VxVector v = posNew - realPos;
			float current_d  = Magnitude( v );
			if( current_d < 0.001f )
			{
			current_d = 0.001f;
			v.Set(0,0,0.001f);
			}

			float attenuation = 15.0f;
			// Get the distance wanted
			float wanted_d = 1.0f;
			wanted_d = current_d - wanted_d;
			float tmp;
			if(tmp = current_d * (attenuation+1.0f) )
			{
			float f = wanted_d / tmp;
			v *= f;
			}
			*/
			//target->Translate(&v);

			break;
		}
	}
}

void
vtNetworkManager::updateLocalPosition(xDistributedObject* distObject,CK3dEntity *target,xDistributedProperty*prop)
{


	if (!distObject || !target || !prop)return;

	xDistributedPoint3F *propValue = static_cast<xDistributedPoint3F*>(prop);
	if (!propValue)return;

	xDistributed3DObject *dobj3D = static_cast<xDistributed3DObject*>(distObject);
	if (!dobj3D)return;
	
	switch(prop->getPropertyInfo()->mNativeType)
	{
		case E_DC_3D_NP_WORLD_POSITION:
		{
			Point3F currentPos = propValue->mLastServerValue;
			Point3F velocity = propValue->mLastServerDifference;

			VxVector realPos;
			target->GetPosition(&realPos);
			Point3F realPos2(realPos.x,realPos.y,realPos.z);


			float oneWayTime =  (distObject->getNetInterface()->GetConnectionTime() / 1000.0f);
			oneWayTime +=oneWayTime * prop->getPredictionSettings()->getPredictionFactor();
			Point3F predictedPos  = currentPos + velocity * oneWayTime;
			Point3F realPosDiff  =  - predictedPos - realPos2 ;
			
			VxVector posNew(predictedPos.x,predictedPos.y,predictedPos.z);
			target->SetPosition(&posNew);

			/*
			VxVector v = posNew - realPos;
			float current_d  = Magnitude( v );
			if( current_d < 0.001f )
			{
				current_d = 0.001f;
				v.Set(0,0,0.001f);
			}

			float attenuation = 15.0f;
			// Get the distance wanted
			float wanted_d = 1.0f;
			wanted_d = current_d - wanted_d;
			float tmp;
			if(tmp = current_d * (attenuation+1.0f) )
			{
				float f = wanted_d / tmp;
				v *= f;
			}
			*/
			//target->Translate(&v);

			break;
		}
	}
}








