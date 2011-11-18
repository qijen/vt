#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "pWorldSettings.h"

#include <assert.h>

std::vector<pDominanceSetupItem*>&_getValidDominanceGroups(std::vector<pDominanceSetupItem*>&_inputGroups,pDominanceSetupItem *testItem)
{
	std::vector<pDominanceSetupItem*> result;

	if (!_inputGroups.size())
		return result;


	/*

	g1A  + g1A 
	g1A != g1B !

	g2A + g2B

	1 2 3


	1 : 1 - 2
	2 : 1 - 3

	*/

	for (int i  = 0 ; i < _inputGroups.size() ; i++)
	{
		pDominanceSetupItem* cItem  =_inputGroups[i];

		if ( cItem == testItem )
		{
			result.push_back(cItem);
			continue;
		}

		if ( cItem->dominanceGroup0 != testItem->dominanceGroup0 )
		{

		}

		
	}

}


void pWorld::_checkForDominanceConstraints()
{

#ifndef _DEBUG
	assert(getReference());
	assert(getScene());
#endif

	int att = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_DOMINANCE_SCENE_SETTINGS);
	
	if (!getReference()->HasAttribute(att)) 
		return;

	CKParameterOut *domPar = getReference()->GetAttributeParameter(att);
	if(!domPar)
		return;


	using namespace vtTools::ParameterTools;
	using namespace vtTools::AttributeTools;


	CKParameterOut *d1 = GetParameterFromStruct(domPar,PS_WDS_ITEM1);
	CKParameterOut *d2 = GetParameterFromStruct(domPar,PS_WDS_ITEM2);
	CKParameterOut *d3 = GetParameterFromStruct(domPar,PS_WDS_ITEM3);
	CKParameterOut *d4 = GetParameterFromStruct(domPar,PS_WDS_ITEM4);
	CKParameterOut *d5 = GetParameterFromStruct(domPar,PS_WDS_ITEM5);

	pDominanceSetupItem item1;
	pDominanceSetupItem item2;
	pDominanceSetupItem item3;
	pDominanceSetupItem item4;
	pDominanceSetupItem item5;



	int error = pFactory::Instance()->copyTo(d1,item1);

	error = pFactory::Instance()->copyTo(d2,item2);
	error = pFactory::Instance()->copyTo(d3,item3);
	error = pFactory::Instance()->copyTo(d4,item4);
	error = pFactory::Instance()->copyTo(d5,item5);

	std::vector<pDominanceSetupItem*> allDominanceItems;
	
	//################################################################
	//
	// gather all in an array
	//



	int a = item1.dominanceGroup0;
	int a1 = item1.dominanceGroup1;
	
	if (item1.dominanceGroup0 !=0 && item1.dominanceGroup1 !=0 && (item1.dominanceGroup0 != item1.dominanceGroup1 ) )
		allDominanceItems.push_back(&item1);
	
	if (item2.dominanceGroup0 !=0 && item2.dominanceGroup1 !=0 && (item2.dominanceGroup0 != item1.dominanceGroup1 ) )
		allDominanceItems.push_back(&item2);
	
	if (item3.dominanceGroup0 !=0 && item3.dominanceGroup1 !=0 && (item3.dominanceGroup0 != item3.dominanceGroup1 ) )
		allDominanceItems.push_back(&item3);

	if (item4.dominanceGroup0 !=0 && item4.dominanceGroup1 !=0 && (item4.dominanceGroup0 != item4.dominanceGroup1 ) )
		allDominanceItems.push_back(&item4);

	if (item5.dominanceGroup0 !=0 && item5.dominanceGroup1 !=0 && (item5.dominanceGroup0 != item5.dominanceGroup1 ) )
		allDominanceItems.push_back(&item5);


	//std::vector<pDominanceSetupItem*>_validDominanceItems1 = _getValidDominanceGroups(allDominanceItems,&item1);
	//int s = _validDominanceItems1.size();

	

	for (int i  = 0 ; i < allDominanceItems.size() ; i++)
	{
		pDominanceSetupItem* cItem  = allDominanceItems[i];
		NxConstraintDominance cD1( cItem->constraint.dominanceA , cItem->constraint.dominanceB );
		
		getScene()->setDominanceGroupPair(cItem->dominanceGroup0, cItem->dominanceGroup1,cD1);	




	}

	


}



namespace vtAgeia
{


pWorldSettings::pWorldSettings() : 
		m_Gravity(VxVector(0.0f,-9.81f,0.0f)) , 
		m_SkinWith(0.1)
{

}


}