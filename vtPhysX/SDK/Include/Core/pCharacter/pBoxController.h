#ifndef __P_BOX_CONTROLLER_H__
#define __P_BOX_CONTROLLER_H__

#include "pTypes.h"

class BoxController;

class pBoxController 
{
public:
									
	pBoxController(const NxControllerDesc& desc, NxScene* scene);
	
									virtual							~pBoxController();

	void					move(const VxVector& disp, int activeGroups, float minDist, int& collisionFlags, float sharpness, const pGroupsMask* groupsMask);

	//bool					setPosition(const VxVector& position)	{ return setPos(position);				}
	pRigidBody*				getBody()							const;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// NxpBoxController
	const VxVector&			getExtents()						const;
	bool					setExtents(const VxVector& extents);
	
	void					setStepOffset(const float offset);

	VxVector	getPosition()const;
	VxVector&	getFilteredPosition();
	bool					getWorldBox(VxBbox& box) const;
	void					setCollision(bool enabled);
	//virtual	void					setInteraction(NxCCTInteractionFlag flag)	{ Controller::setInteraction(flag);		}
	//virtual	NxCCTInteractionFlag	getInteraction()					const	{ return Controller::getInteraction();	}

	//vi//rtual	void					reportSceneChanged();
	//virtual	void*					getUserData()						const	{ return userData;						}
	
private:

	BoxController *mBoxController;


};

#endif
//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright (c) 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND
