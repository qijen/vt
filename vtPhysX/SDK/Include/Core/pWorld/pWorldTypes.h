#ifndef __PWORLDTYPES_H__
#define __PWORLDTYPES_H__


//################################################################
//
// Help structures and types
//

//----------------------------------------------------------------
//
//! \brief Meta info to track a broken joint from a NX-SDK callback to the next frame.
//
struct pBrokenJointEntry
{
	pJoint*joint;
	float impulse;
	CK_ID mAEnt;
	CK_ID mBEnt;
	bool isTriggered;
	int jType;

	pBrokenJointEntry()
	{
		impulse = 0 ;
		mAEnt = NULL;
		mBEnt = NULL;
		jType -1;
		isTriggered = false;
	}
};

typedef XArray<pBrokenJointEntry*>JointFeedbackListType;


/** \addtogroup World
@{
*/

//----------------------------------------------------------------
//
//! \brief Dominance setup for the scene
//
struct pDominanceSetupItem
{
	struct pDominanceConstraint
	{
		float dominanceA;
		float dominanceB;

		pDominanceConstraint() : 
		dominanceA(1.0f), 
			dominanceB(0.0f)
		{

		}
	};

	pDominanceConstraint constraint;

	int dominanceGroup0;
	int dominanceGroup1;

	pDominanceSetupItem()  : 
	dominanceGroup0(0),
		dominanceGroup1(0)
	{
	}
};

/** @} */

//################################################################
//
// User callbacks
//
#endif 
