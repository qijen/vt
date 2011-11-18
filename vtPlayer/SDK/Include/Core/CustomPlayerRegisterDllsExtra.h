#if !defined(CUSTOMPLAYERSTATICDLLSEXTA_H)
#define CUSTOMPLAYERSTATICDLLSEXTRA_H


#if defined(CUSTOM_PLAYER_STATIC)


#ifdef WIN32
#define CDECL_CALL __cdecl
#else
#define CDECL_CALL
#endif




/***************************************************/
/**** BEHAVIORS ***********************************/

CKPluginInfo* CDECL_CALL CKGet_TOOLS_PluginInfo(int index);
void CDECL_CALL Register_TOOLS_BehaviorDeclarations(XObjectDeclarationArray *reg);

CKPluginInfo* CDECL_CALL CKGet_WIDGETS_PluginInfo(int index);
void CDECL_CALL Register_WIDGETS_BehaviorDeclarations(XObjectDeclarationArray *reg);

CKPluginInfo* CDECL_CALL CKGet_VTPHYSX_PluginInfo(int index);
void CDECL_CALL Register_VTPHYSX_BehaviorDeclarations(XObjectDeclarationArray *reg);




//--------------------- Implementation -------------------------------------//
//---------------------		  of       -------------------------------------//
//--------------------- registration functions -----------------------------//


/****************************************************************************
BEHAVIORS
*******************************************************************************/
inline void RegisterToolsBehaviors(CKPluginManager& iPluginManager)
{
	//--- 3D Tranfo
	iPluginManager.RegisterPluginInfo(0,CKGet_TOOLS_PluginInfo(0),Register_TOOLS_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_TOOLS_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Tools",2);
}

inline void RegisterWidgetsBehaviors(CKPluginManager& iPluginManager)
{
	//--- 3D Tranfo
	iPluginManager.RegisterPluginInfo(0,CKGet_WIDGETS_PluginInfo(0),Register_WIDGETS_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_WIDGETS_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Widgets",1);
}

inline void RegisterPhysXBehaviors(CKPluginManager& iPluginManager)
{
	//--- 3D Tranfo
	iPluginManager.RegisterPluginInfo(0,CKGet_VTPHYSX_PluginInfo(0),Register_VTPHYSX_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_VTPHYSX_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("vtPhysX",2);
}
inline void CustomPlayerRegisterBehaviorsExtra(CKPluginManager& iPluginManager)
{


#ifdef vtToolkit
	RegisterToolsBehaviors(iPluginManager);
#endif

#ifdef vtWidgets
	RegisterWidgetsBehaviors(iPluginManager);
#endif

#ifdef vtPhysX
	RegisterPhysXBehaviors(iPluginManager);
#endif
	

}



#endif // CUSTOM_PLAYER_STATIC


#endif // CUSTOMPLAYERSTATICDLLS_H