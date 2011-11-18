#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "IParameter.h"
#include <xDebugTools.h>

bool pPivotSettings::isValid()
{
	return true;

}

bool pObjectDescr::setToDefault()
{

	hullType =HT_Sphere;
	density = 1.0f;
	massOffset = VxVector();
	shapeOffset  = VxVector();
	flags  = (BodyFlags)0;
	skinWidth =0.0f;
	newDensity = 0.0f;
	totalMass = 0.0f;
	collisionGroup = 0;
	hirarchy = 0;
	subEntID = -1;
	transformationFlags = 0;
	worlReference = 0;


	internalXmlID = externalXmlID = xmlImportFlags = 0;

	massOffsetReference = 0;

	pivotOffsetReference = 0;


	ccdMotionThresold = 0.0;
	ccdFlags = 0 ;
	ccdMeshReference =NULL;
	ccdScale = 1.0f;

	linearDamping  = angularDamping = 0.0f;

	linearSleepVelocity = angularSleepVelocity = sleepingEnergyThresold = 0.0f;

	solverIterations = dominanceGroups = compartmentID  = 0;

	version  = OD_DECR_V0;
	mask = (pObjectDescrMask)0;

	groupsMask.bits0=0;
	groupsMask.bits1=0;
	groupsMask.bits2=0;
	groupsMask.bits3=0;


	wheel.setToDefault();
	optimization.setToDefault();
	material.setToDefault();
	collision.setToDefault();
	pivot.setToDefault();
	//mass.setToDefault();
	ccd.setToDefault();


	return true;
}

bool pCollisionSettings::setToDefault()
{
	collisionGroup = 0;
	collisionGroup = 0;
	skinWidth = 0.025f;
	return true;
}

bool pPivotSettings::setToDefault()
{
	pivotReference = 0;
	return true;
}
bool pCCDSettings::setToDefault()
{
	motionThresold = 0.0;
	flags = 0 ;
	meshReference =0;
	scale = 1.0f;

	return true;
}

bool pOptimization::setToDefault()
{
	transformationFlags = (BodyLockFlags)0;

	linDamping  = angDamping = 0.0f;
	solverIterations = 4 ;
	dominanceGroup=0;
	compartmentGroup=0;
	sleepEnergyThreshold =0.0f;
	linSleepVelocity = angSleepVelocity = 0.0;

	return true;

}
bool pOptimization::isValid()
{

	bool result = true;
	
	iAssertWR( X_IS_BETWEEN(solverIterations,1,255) ,solverIterations=4,result );
	iAssertWR( !(linDamping < 0) ,"",result );
	iAssertWR( !(angDamping< 0) ,"",result );
	iAssertWR( !(transformationFlags < 0) ,transformationFlags=((BodyLockFlags)0),result );
	iAssertWR( !(linSleepVelocity < 0) ,"",result );
	iAssertWR( !(angSleepVelocity < 0) ,"",result );


	return true;

}
bool pMaterial::isValid()
{
	bool result = true;

	
	iAssertWR( !(dynamicFriction < 0.0f) ,"",result );
	iAssertWR( !(staticFriction < 0.0f) ,"",result );
	iAssertWR( !(restitution < 0.0f || restitution > 1.0f) ,"",result );

	if (flags & 1)
	{
		float sMagnitude = dirOfAnisotropy.SquareMagnitude();
		iAssertWR( sMagnitude > 0.98f || sMagnitude < 1.03f ,"",result );
		iAssertWR( !(dynamicFrictionV < 0.0f) ,"",result );
		iAssertWR( !(staticFrictionV < 0.0f) ,"",result );
	}
	
	iAssertWR( frictionCombineMode <= 4 ,"",result );
	iAssertWR( restitutionCombineMode <= 4 ,"",result );


	return result;

}

bool pConvexCylinderSettings::setToDefault()
{
	radius.setToDefault();
	height.setToDefault();
	approximation = 4;

	downAxis.Set(0,-1,0);
	rightAxis.Set(1,0,0);
	forwardAxis.Set(0,0,1);

	downAxisRef = 0 ;
	rightAxisRef = 0 ;
	forwardAxisRef = 0 ;
	convexFlags = CF_ComputeConvex;


	return true;

}
bool pConvexCylinderSettings::isValid()
{

	bool result=true;
	iAssertWR(radius.isValid(),"",result);
	iAssertWR(height.isValid(),"",result);
	iAssertWR(approximation >= 4 && approximation > 0,"",result);
	iAssertWR( XAbs(forwardAxis.SquareMagnitude()) > 0.1f || forwardAxisRef,"",result);
	iAssertWR( XAbs(downAxis.SquareMagnitude()) > 0.1f || downAxisRef,"",result);
	iAssertWR( XAbs(rightAxis.SquareMagnitude()) > 0.1f || rightAxisRef,"",result);

	return result;
}

bool pAxisReferencedLength::isValid()
{
	return  ( referenceAxis<3	&& referenceAxis >=0) && (value > 0.0f || reference ) ;
}

bool pAxisReferencedLength::evaluate(CKBeObject *referenceObject)
{
	if (!reference && referenceObject)
	{
		this->reference = referenceObject;
	}
	
	if (reference)
	{
		VxVector size;
		
		if (reference->GetClassID()  == CKCID_3DOBJECT )
		{
			CK3dEntity *ent   = (CK3dEntity*)reference;
			if (ent && ent->GetCurrentMesh() )
				size = ent->GetCurrentMesh()->GetLocalBox().GetSize();
		}
		else if(reference->GetClassID()  == CKCID_MESH)
		{
			CKMesh *mesh = (CKMesh*)reference;
			if (mesh)
			{
				size = mesh->GetLocalBox().GetSize();
			}
		}
		value  = size[referenceAxis];
		return XAbs(size.SquareMagnitude()) >0.0f;
	}
	return false;
}
bool pAxisReferencedLength::setToDefault()
{
	value=0.0f;
	referenceAxis = 0;
	reference = NULL;

	return  true;
}

bool pCapsuleSettingsEx::setToDefault()
{
	radius.setToDefault();
	height.setToDefault();
	return true;
}
bool pCapsuleSettingsEx::isValid()
{
	return radius.isValid() && height.isValid();
}


