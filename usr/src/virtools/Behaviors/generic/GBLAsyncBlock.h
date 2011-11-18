#pragma once

class ExeInThread
{
    public:
    static CKObjectDeclaration * FillBehaviour( void );	
    static int CallBack( const CKBehaviorContext& behaviorContext );
    static int BehaviourFunction( const CKBehaviorContext& behaviorContext );
    static CKERROR CreatePrototype( CKBehaviorPrototype** behaviorPrototypePtr );

	enum ThreadStatus {
		Idle = 0, Requested = 2, Active = 3};


	friend unsigned int BlockingThreadFunction(void *arg);

	typedef struct ThreadInfo
	{
		CKBehavior*			targetBeh;
		int					targetInputToActivate;
	} AsyncThreadInfo;
};
