#ifndef __P_SERIALIZER_H__
#define __P_SERIALIZER_H__


#include "vtPhysXBase.h"

/** \addtogroup Serialization
@{
*/

/**
\brief Class to import and export NxStream files. Those files can be created by 3D related content editors
such as Maya and 3D-SMax. Also, you can dump the entire Virtools scene and load it in the
supplied PhysX Viewer.
*/
class MODULE_API pSerializer
{
public:
	pSerializer();
	~pSerializer();

	static pSerializer*Instance();
	NXU::NxuPhysicsCollection *getCollection(const char *pFilename,int type);
	bool overrideBody(pRigidBody *body,int flags);
	int loadCollection(const char*fileName,int flags);
	int saveCollection(const char*filename);
	void parseFile(const char*filename,int flags);

protected:
	NXU::NxuPhysicsCollection *mCollection;

private:
};

/** @} */

#endif