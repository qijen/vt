#ifndef __P_MISC_H__
#define __P_MISC_H__

#include "vtPhysXBase.h"


namespace vtAgeia
{

	VxVector BoxGetZero(CK3dEntity* ent);
	void SetEulerDirection(CK3dEntity* ent,VxVector direction);
	
	XString getEnumDescription(CKParameterManager* pm,CKGUID parGuide,int parameterSubIndex);
	XString getEnumDescription(CKParameterManager* pm,CKGUID parGuide);

	int getEnumIndex(CKParameterManager* pm,CKGUID parGuide,XString enumValue);
	int getHullTypeFromShape(NxShape *shape);
	CKGUID getEnumGuid(XString name);

	int getNbOfPhysicObjects(CK3dEntity *parentObject,int flags=0);
	bool calculateOffsets(CK3dEntity*source,CK3dEntity*target,VxQuaternion &quat,VxVector& pos);

	bool isChildOf(CK3dEntity*parent,CK3dEntity*test);
	CK3dEntity* findSimilarInSourceObject(CK3dEntity *parentOriginal,CK3dEntity* partentCopied,CK3dEntity *copiedObject,CK3dEntity*prev=NULL);

	CK3dEntity *getEntityFromShape(NxShape* shape);


}
#endif
