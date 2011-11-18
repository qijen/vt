#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "NXU_helper.h"  // NxuStream helper functions.
#include "NXU_PhysicsInstantiator.h"

#include "UserAllocator.h"
#include "ErrorStream.h"
#include "Utilities.h"

static ErrorStream gErrorStream;
static pSerializer *gSerializer = NULL;

void pSerializer::parseFile(const char*filename,int flags)
{

	if(!GetPMan()->getPhysicsSDK())
		return;


	if (!loadCollection(filename,1))
	{
		return ;
	}


	NXU::instantiateCollection(mCollection, *GetPMan()->getPhysicsSDK(), 0, 0, 0);
	NXU::NxuPhysicsInstantiator Instantiator(mCollection);
	Instantiator.instantiate(*GetPMan()->getPhysicsSDK());
	int sCount = mCollection->mScenes.size();

	for (NxU32 i=0; i<mCollection->mScenes.size(); i++)
	{
		NXU::NxSceneDesc *sd = mCollection->mScenes[i];

		for (NxU32 j=0; j<sd->mActors.size(); j++)
		{

			NXU::NxActorDesc *ad = sd->mActors[j];
			const char*name   = ad->name;
			XString nameStr(name);

			CK3dEntity *ent  = (CK3dEntity*)ctx()->GetObjectByNameAndClass(nameStr.Str(),CKCID_3DOBJECT);
			if (ent)
			{
				pRigidBody *body    =  GetPMan()->getBody(nameStr.CStr());
				if (body)
					body->readFrom(ad,0);
				else
					body = pFactory::Instance()->createBody(ent,NULL,ad,1);

			}

			if ( ad->mHasBody ) // only for dynamic actors
			{
				for (NxU32 k=0; k<ad->mShapes.size(); k++)
				{
					NXU::NxShapeDesc *shape = ad->mShapes[k];
					NxVec3 locPos = shape->localPose.t;
					NxQuat localQuad = shape->localPose.M;
				}
			}
		}
	}

}



class MyUserNotify: public NXU_userNotify, public NXU_errorReport
{
public:
	virtual void NXU_errorMessage(bool isError, const char *str)
	{
		if (isError)
		{
			printf("NxuStream ERROR: %s\r\n", str);
		}
		else
		{
			printf("NxuStream WARNING: %s\r\n", str);
		}
	}

	virtual void	NXU_notifyScene(NxU32 sno,	NxScene	*scene,	const	char *userProperties)
	{
	
	};

	

};
MyUserNotify gUserNotify;





bool pSerializer::overrideBody(pRigidBody *body,int flags)
{

	if (!mCollection)
	{
		return false;
	}

	NXU::instantiateCollection(mCollection, *GetPMan()->getPhysicsSDK(), 0, 0, 0);
	NXU::NxuPhysicsInstantiator Instantiator(mCollection);
	Instantiator.instantiate(*GetPMan()->getPhysicsSDK());

	int sCount = mCollection->mScenes.size();
	for (NxU32 i=0; i<mCollection->mScenes.size(); i++)
	{

		NXU::NxSceneDesc *sd = mCollection->mScenes[i];

		for (NxU32 j=0; j<sd->mActors.size(); j++)
		{

			NXU::NxActorDesc *ad = sd->mActors[j];
			const char*name   = ad->name;
			XString nameStr(name);


			if ( ad->mHasBody ) // only for dynamic actors
			{
				for (NxU32 k=0; k<ad->mShapes.size(); k++)
				{
					NXU::NxShapeDesc *shape = ad->mShapes[k];
					NxVec3 locPos = shape->localPose.t;
					NxQuat localQuad = shape->localPose.M;
				}
			}
		}
	}

	return true;
}


int pSerializer::loadCollection(const char*fileName,int flags)
{
	if (mCollection)
	{
		releaseCollection(mCollection);
		mCollection = NULL;
	}
	mCollection  = getCollection(fileName,flags);
	if (mCollection)
	{
		return 1;
	}else
	{
		return 0;
	}
}


pSerializer::pSerializer()
{
	gSerializer = this;
	mCollection = NULL;

}

//************************************
// Method:    Instance
// FullName:  vtAgeia::pSerializer::Instance
// Access:    public static 
// Returns:   pSerializer*
// Qualifier:
//************************************
pSerializer*pSerializer::Instance()
{


	if (!gSerializer)
	{
		gSerializer  = new pSerializer();
	}

	return gSerializer;
}

NXU::NxuPhysicsCollection* pSerializer::getCollection(const char *pFilename,int type)
{

	NXU::NxuPhysicsCollection* c = NULL;
	c = NXU::loadCollection(pFilename,(NXU::NXU_FileType)type);
	if (!c)
	{
		return NULL;
	}
	return c;
}


int pSerializer::saveCollection(const char*filename)
{
	
	char SaveFilename[512];
	//GetTempFilePath(SaveFilename);
	strcat(SaveFilename, filename);

	NXU::setUseClothActiveState(false);
	NXU::setUseSoftBodyActiveState(false);

	NXU::setErrorReport(&gUserNotify);
	NXU::setEndianMode(isProcessorBigEndian());

	NXU::NxuPhysicsCollection *c = NXU::extractCollectionScene(GetPMan()->getDefaultWorld()->getScene());
	if (c)
	{
		char scratch[512];
		XString fName(filename);
		//fName << "\0";
		sprintf(scratch, "%s.xml", SaveFilename);
		printf("Saving NxuStream XML file to '%s'\r\n", scratch);
		NXU::saveCollection(c, fName.CStr(), NXU::FT_BINARY, false, false);
		NXU::releaseCollection(c);

	}

	releaseCollection(c);
	return 0;
}
