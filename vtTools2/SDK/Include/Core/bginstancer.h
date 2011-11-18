/********************************************************************
	created:	2006/22/06
	created:	22:06:2006   12:26
	filename: 	x:\junctions\ProjectRoot\current\vt_plugins\vt_toolkit\Behaviors\Generic\BGInstancer.h
	file path:	x:\junctions\ProjectRoot\current\vt_plugins\vt_toolkit\Behaviors\Generic
	file base:	BGInstancer
	file ext:	h
	author:		mc007
	
	purpose:	instancing of b-graphs per file
*********************************************************************/

#include "stdafx.h"

#define	BGWRAPPER_GUID					CKGUID(0x35fb3204,0x6b59721c)

// Parameters for BGWrapper
enum EBGWRAPPERPARAM
	{
	// local
	EBGWRAPPERPARAM_PARAMETER_SCRIPT = 0,
	EBGWRAPPERPARAM_PARAMETER_NAME = 1,
	EBGWRAPPERPARAM_LOCAL_PARAMETER_COUNT,
	};

class BGWrapper
{

	public:
		static CKObjectDeclaration*	 FillBehaviour( void );	
		static CKERROR				 CreatePrototype( CKBehaviorPrototype** behaviorPrototypePtr );
		static int					 BehaviourFunction( const CKBehaviorContext& behaviorContext );

	private:
		static CKERROR		BGWrapperCB(const CKBehaviorContext& behContext);
		
		static BOOL			HasIO(CKBehavior* pBeh);
		static BOOL			DeleteIO(CKBehavior* pBeh);
		static BOOL			CreateIO(CKBehavior* pBeh, CKBehavior* pScript);
		static BOOL			CheckIO(CKBehavior* pBeh, CKBehavior* pScript);

		static CKBehavior*	BGLoader(CKSTRING fileName,const CKBehaviorContext& behContext);

		static void ActivateNextFrameSubBB(CKBehavior* scriptObject,BOOL &bActivateNextFrame);
		static void DesactivateSubBB(CKBehavior* scriptObject);
		static void OwnerSubBB(CKBehavior* scriptObject,CKBeObject*owner);
		static void	SetNewBG(CKBehavior *behaviour,CKBehavior *newBehavior);
		static void	DestroyCurrentBG(CKLevel* level,CKBehavior *behaviour,CKBehavior *scriptObject);



};


