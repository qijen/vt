#include <WTypes.h>
#include "vt_tools.h"

/*
*******************************************************************
* Function: IsNumeric()
*
* Description :		Check each character of the string
If a character at a certain position is not a digit,
then the string is not a valid natural number

* Parameters :	char*Str,r : the string to test
* Returns : int
*
*******************************************************************
*/
int 
vtTools::ParameterTools::
IsNumeric(const char* Str,vtTools::Enums::SuperType superType)
{
	bool dotAlreadyFound = false;


	//allow empty strings : 
	if(strlen(Str) == 0 )
	{
		return true;
	}

	using namespace vtTools::Enums;

	for(unsigned int i = 0; i < strlen(Str); i++)
		{
		if(  Str[i] < '0' || Str[i] > '9' )
			{

			if ( ( Str[i] == '.' || Str[i] == ',') )
			{
				if (superType == vtINTEGER )
				{
					return false;
				}
				
				if (dotAlreadyFound)
				{
					return false;
				}

				dotAlreadyFound = true;
				continue;
				}

				if (!(i == 0 &&  Str[i]== '-'))
				{
					return false;
				}
			}
		}
	return true;
}

/*
*******************************************************************
* Function: CreateWidgetInfo()
*
* Description :	CreateWidgetInfo returns an handy info about an parameter type an a
						given string value of this parameter.
                        
* Parameters :	CKContext* context, r : the virtools context.
						CKParameterType parameterType, r : the type to test.
						XString stringValue,r : the string value of the given type.

* Returns : WidgetInfo
*******************************************************************
*/
vtTools::Structs::WidgetInfo* 
vtTools::ParameterTools::
CreateWidgetInfo(CKContext* context,
				 CKParameterType parameterType,
				 XString stringValue)
{
	using namespace vtTools::Structs;
	using namespace vtTools::Enums;
	using namespace vtTools::ParameterTools;


	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	CKGUID guid =pm->ParameterTypeToGuid(parameterType);

	WidgetInfo* result = new WidgetInfo();

	result->parameterInfo  =  GetParameterInfo(context,parameterType);

	CKParameterLocal *localCopy = context->CreateCKParameterLocal("buffer",guid,TRUE);

	localCopy->SetStringValue(stringValue.Str());


	switch(result->parameterInfo.superType) {

		//////////////////////////////////////////////////////////////////////////	color  //////////////////////////////////////////////////////////////////////////
		case vtCOLOUR:
			{
				WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();
				VxColor realValue;
				localCopy->GetValue(&realValue);

				DWORD32 dColor = realValue.GetRGBA();

				int r = ColorGetRed(dColor);
				int g = ColorGetGreen(dColor);
				int b = ColorGetBlue(dColor);
				int a = ColorGetAlpha(dColor);

				item->value << r << "," << g << "," << b << "," << a;

				item->widgetType = EVT_WIDGET_COLOR_SELECTION;
				result->items.PushBack(item);
				break;
			}
		//////////////////////////////////////////////////////////////////////////	integer //////////////////////////////////////////////////////////////////////////
		case vtFLOAT:
			{
				WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();
				float realValue;
				localCopy->GetValue(&realValue);
				item->value << realValue;
				item->widgetType = EVT_WIDGET_EDIT_TEXT;
				result->items.PushBack(item);
				break;
			}
		//////////////////////////////////////////////////////////////////////////	integer //////////////////////////////////////////////////////////////////////////
		case vtINTEGER:
			{
				WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();
				int realValue;
				localCopy->GetValue(&realValue);
				item->value << realValue;
				item->widgetType = EVT_WIDGET_EDIT_TEXT;
				result->items.PushBack(item);
				break;
			}
		//////////////////////////////////////////////////////////////////////////	bool //////////////////////////////////////////////////////////////////////////
		case vtBOOL:
			{
				WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();
				CKBOOL realValue;
				localCopy->GetValue(&realValue);
				item->value << realValue;
				item->widgetType = EVT_WIDGET_CHECK_BUTTON;
				result->items.PushBack(item);
				break;
			}

		//////////////////////////////////////////////////////////////////////////	string or file //////////////////////////////////////////////////////////////////////////
		case vtFile :
			{
				WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();
				item->value = stringValue;
				item->widgetType = EVT_WIDGET_FILE_SELECTION;
				result->items.PushBack(item);
				break;
			}
		case vtSTRING:
			{
				WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();
				item->value = stringValue;
				item->widgetType = EVT_WIDGET_EDIT_TEXT;
				result->items.PushBack(item);
				break;
			}
		//////////////////////////////////////////////////////////////////////////	box	//////////////////////////////////////////////////////////////////////////
		case vtBOX:
		{
			for (int i = 0 ; i < 6 ; i ++ )
				{
				WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();

				VxBbox realValue;
				localCopy->GetValue(&realValue);
				item->widgetType = EVT_WIDGET_EDIT_TEXT;
				switch(i) 
					{
					case 0:
						item->value << realValue.v[i];
						item->label = "min x: ";
						break;
					case 1:
						item->value << realValue.v[i];
						item->label = "min y: ";
						break;
					case 2:
						item->value << realValue.v[i];
						item->label = "min z: ";
						break;
					case 3:
						item->value << realValue.v[i];
						item->label = "max x: ";
						break;
					case 4:
						item->value << realValue.v[i];
						item->label = "max y: ";
						break;
					case 5:
						item->value << realValue.v[i];
						item->label = "max z: ";
						break;
					}
				result->items.PushBack(item);
				}
			break;
			}
		//////////////////////////////////////////////////////////////////////////	rectangle	//////////////////////////////////////////////////////////////////////////
		case vtRECTANGLE:
			{
				for (int i = 0 ; i < 4 ; i ++ )
				{
					WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();

					VxRect realValue;
					localCopy->GetValue(&realValue);
					item->widgetType = EVT_WIDGET_EDIT_TEXT;
					switch(i) 
						{
							case 0:
								item->value << realValue.left;
								item->label = "Left: ";
								break;
							case 1:
								item->value << realValue.top;
								item->label = "Top: ";
								break;
							case 2:
								item->value << realValue.right;
								item->label = "Right: ";
								break;
							case 3:
								item->value << realValue.bottom;
								item->label = "Bottom: ";
								break;
						}
					result->items.PushBack(item);
				}
				break;
			}
		//////////////////////////////////////////////////////////////////////////	vector 2	//////////////////////////////////////////////////////////////////////////
		case vtVECTOR2D:
			{
				for (int i = 0 ; i < 2 ; i ++ )
				{

					WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();
					Vx2DVector VectorValue;
					localCopy->GetValue(&VectorValue);
					item->value << VectorValue[i];
					item->widgetType = EVT_WIDGET_EDIT_TEXT;
					switch(i) 
					{
						case 0:
							item->label = "x: ";
							break;
						case 1:
							item->label = "y: ";
					}
					result->items.PushBack(item);
				}
				break;
			}
		//////////////////////////////////////////////////////////////////////////	vector 3	//////////////////////////////////////////////////////////////////////////
		case vtVECTOR:
			{
				for (int i = 0 ; i < 3 ; i ++ )
				{

					WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();
					VxVector VectorValue;
					localCopy->GetValue(&VectorValue);
					item->value << VectorValue[i];
					item->widgetType = EVT_WIDGET_EDIT_TEXT;

					switch(i) 
					{
						case 0:
							item->label = "x";
							break;
						case 1:
							item->label = "y";
							break;
						case 2:
							item->label = "z";
							break;
						}
					result->items.PushBack(item);
					}
				break;
			}
		//////////////////////////////////////////////////////////////////////////	vector4	//////////////////////////////////////////////////////////////////////////
		case vtQUATERNION:
			{

				for (int i = 0 ; i < 4 ; i ++ )
				{

					WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();
					VxQuaternion realValue;
					localCopy->GetValue(&realValue);
					item->value << realValue[i];
					item->widgetType = EVT_WIDGET_EDIT_TEXT;
					switch(i) 
					{
						case 0:
							item->label = "x: ";
							break;
						case 1:
							item->label = "y: ";
							break;
						case 2:
							item->label = "z: ";
							break;
						case 3:
							item->label = "w: ";
							break;
					}
					result->items.PushBack(item);
				}
				break;
			}
		//////////////////////////////////////////////////////////////////////////	enumeration //////////////////////////////////////////////////////////////////////////
		case vtENUMERATION:
			{

				CKEnumStruct *enumStruct = pm->GetEnumDescByType(parameterType);
				if ( enumStruct )
				{
					for (int i = 0 ; i < enumStruct->GetNumEnums() ; i ++ )
					{
						WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();
						item->value = enumStruct->GetEnumDescription(i);
						item->widgetType = EVT_WIDGET_COMBO_BOX;
						result->items.PushBack(item);
					}
				}
			break;
			}
		//////////////////////////////////////////////////////////////////////////	flags	 //////////////////////////////////////////////////////////////////////////
		case vtFLAGS:
			{
				CKFlagsStruct*flagStruct = pm->GetFlagsDescByType(parameterType);
				if ( flagStruct )
				{
					for (int i = 0 ; i < flagStruct->GetNumFlags() ; i ++ )
					{
						WidgetInfo::WidgetInfoItem *item = new WidgetInfo::WidgetInfoItem();
						item->value = flagStruct->GetFlagDescription(i);
						item->widgetType = EVT_WIDGET_CHECK_BUTTON;
						result->items.PushBack(item);
					}
				}
				break;
			}
	}
	return result;
}
/*
*******************************************************************
* Function: GetParameterMemberInfo()
*
* Description :	GetParameterMemberInfo returns the type super type of the used structure members.
						Example : vtVECTOR returns vtFLOAT 				
*
* Parameters :	CKContext* context, r : the virtools context.
						CKParameterType parameterType, r : the type to test.
* Returns : SuperType
*
*******************************************************************
*/
vtTools::Enums::SuperType
vtTools::ParameterTools::GetParameterMemberInfo(CKContext* context,CKParameterType parameterType)
{
 	assert ( context );
	using namespace vtTools::Enums;
	using namespace vtTools::ParameterTools;
    	
	SuperType superType = GetVirtoolsSuperType(context,context->GetParameterManager()->ParameterTypeToGuid(parameterType));

	switch(superType) 
	{
		case vtENUMERATION:
			return vtSTRING;
		
		case vtBOOL:
		case vtFLAGS:
			return vtBOOL;
		case vtCOLOUR:
			return vtCOLOUR;
		case vtFile:
			return vtFile;
		case vtINTEGER:
			return vtINTEGER;
		case vtFLOAT:
			return vtFLOAT;
		case vtSTRING:
			return vtSTRING;
		case vtVECTOR2D:
		case vtRECTANGLE:
		case vtVECTOR: 
		case vtBOX:
		case vtQUATERNION:
		case vtMATRIX:
			return vtFLOAT;
		default:
			return vtSTRING;
	}
}

/*
*******************************************************************
* Function: GetParameterInfo()
*
* Description :	GetParameterInfo returns an info about a virtools type. 
						Example : Vector4D returns :
										result.horizontalItems = 4 ;
										result.verticalItems = 1 ;
										result.memberType =  vtFLOAT
										result.superType = vtQUATERNION; 

* Parameters :	CKContext* context, r : the virtools context.
						CKParameterType parameterType, r : the type to test.
* Returns : ParameterInfo
*
*******************************************************************
*/
vtTools::Structs::ParameterInfo
vtTools::ParameterTools::GetParameterInfo(CKContext* context,CKParameterType parameterType)
{

	assert(context);

	using namespace vtTools::Structs;
	using namespace vtTools::Enums;
	using namespace vtTools::ParameterTools;

    CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	ParameterInfo result;
   
	result.memberType =   vtTools::ParameterTools::GetParameterMemberInfo(context,parameterType);
	result.superType =     vtTools::ParameterTools::GetVirtoolsSuperType(context,pm->ParameterTypeToGuid(parameterType));


	switch(result.superType) 
	{

		case vtFLAGS:
		{    
			CKFlagsStruct *flagStruct = pm->GetFlagsDescByType(parameterType);
			if (flagStruct)
			{ 
				result.horizontalItems = 1 ;
				result.verticalItems = flagStruct->GetNumFlags() ;

			}
			break;
		}
		case vtENUMERATION:
		{    
			CKEnumStruct* enumStruct = pm->GetEnumDescByType(parameterType);
			if (enumStruct)
			{
					result.horizontalItems = 1 ; 
					result.verticalItems = enumStruct->GetNumEnums();
					break;
			}
		}
		case vtBOOL:
		case vtINTEGER:
		case vtFLOAT:
		case vtFile:
		case vtSTRING:
			result.horizontalItems		= 1;
			result.verticalItems			= 1;
			break;
		case vtVECTOR2D:
			result.horizontalItems = 2;
			result.verticalItems = 1;
			break;
		case vtRECTANGLE:
			result.horizontalItems = 2;
			result.verticalItems	    = 2;
			break;
		case vtVECTOR: 
			result.horizontalItems = 3;
			result.verticalItems	    = 1;
			break;
		case vtBOX:
			result.horizontalItems = 3;
			result.verticalItems = 2;
			break;
		case vtQUATERNION:
		case vtCOLOUR:
			result.horizontalItems = 4;
			result.verticalItems	    = 1;
			break;
		case vtMATRIX:
			result.horizontalItems = 4;
			result.verticalItems	    = 4;
			break;
		default:
			{
								result.horizontalItems = 0;
								result.verticalItems	    = 0;
			break;
			}
	}
	return result;
}

/*
*******************************************************************
* Function: GetWidgetType()
*
* Description :	GetWidgetType returns the type of the GBLWidget which should used for a 
						specific virtools super type. 
						Example : vtBOOL = CheckBox
						vtENUMERATION = ComboBox

* Parameters :	SuperType superType, r : the given virtools super type
* Returns : EWidgetType
*
*******************************************************************
*/


vtTools::Enums::EVTWidgetType
vtTools::ParameterTools::GetWidgetType(vtTools::Enums::SuperType superType)
{
	using namespace vtTools::Enums;

	switch(superType) 
	{
		case vtENUMERATION:
			return EVT_WIDGET_COMBO_BOX;
		
		case vtFLAGS:
		case vtBOOL:
			return EVT_WIDGET_CHECK_BUTTON; 

		case vtINTEGER:
		case vtFLOAT:
		case vtSTRING:
		case vtVECTOR2D:
		case vtRECTANGLE:
		case vtVECTOR: 
		case vtBOX:
		case vtQUATERNION:
		case vtMATRIX:
			return EVT_WIDGET_EDIT_TEXT;
		
		case vtCOLOUR:
			return EVT_WIDGET_COLOR_SELECTION;
		case vtFile:
			return EVT_WIDGET_FILE_SELECTION;


		default:
				return EVT_WIDGET_EDIT_TEXT;
	}
}

/*
*******************************************************************
* Function:GetVirtoolsSuperType()
*
* Description : GetVirtoolsSuperType returns the base type of virtools parameter. 
						This is very useful if want to determine whether a parameter can be used for 
						network of database.
*
* Parameters : CKContext *ctx, r : the virtools context
						 CKGUID guid, r : the guid of the parameter
*
* Returns : SuperType, 
*
*******************************************************************
*/ 



vtTools::Enums::SuperType 
vtTools::ParameterTools::
	GetVirtoolsSuperType(CKContext *ctx,
	const CKGUID guid)
{
	assert(ctx);

	using namespace vtTools::Enums;

	CKParameterTypeDesc *tdescr = ctx->GetParameterManager()->GetParameterTypeDescription(guid);

	CKParameterManager *pm = static_cast<CKParameterManager *>(ctx->GetParameterManager());

	if ( (pm->GetEnumDescByType(pm->ParameterGuidToType(guid))) )
	{
		return vtENUMERATION;
	}

	// parameter is a flag 
	if ( (pm->GetFlagsDescByType(pm->ParameterGuidToType(guid)))  )
	{
		return vtFLAGS;
	}

	// is a bool :
	if( (ctx->GetParameterManager()->ParameterGuidToType(guid)) ==  ctx->GetParameterManager()->ParameterGuidToType(CKPGUID_BOOL))
		return vtBOOL;

	/*// is a file :
	if( (ctx->GetParameterManager()->ParameterGuidToType(guid)) ==  ctx->GetParameterManager()->ParameterGuidToType(GBLCO_FILE_TYPE))
		return vtFile;*/

	// is a string :
	if(ctx->GetParameterManager()->IsDerivedFrom(guid,CKPGUID_STRING))
		return vtSTRING;

	// is a float :
	if(ctx->GetParameterManager()->IsDerivedFrom(guid,CKPGUID_FLOAT))
		return vtFLOAT;

	// is a integer :
	if(ctx->GetParameterManager()->IsDerivedFrom(guid,CKPGUID_INT))
		return vtINTEGER;

	// is a VxVector :
	if(ctx->GetParameterManager()->IsDerivedFrom(guid,CKPGUID_VECTOR))
		return vtVECTOR;

	// is a Vx2DVector :
	if(ctx->GetParameterManager()->IsDerivedFrom(guid,CKPGUID_2DVECTOR))
		return vtVECTOR2D;

	// is a VxColor :
	if(ctx->GetParameterManager()->IsDerivedFrom(guid,CKPGUID_COLOR))
		return vtCOLOUR;

	// is a VxMatrix :
	if(ctx->GetParameterManager()->IsDerivedFrom(guid,CKPGUID_MATRIX))
		return vtMATRIX;

	// is a VxQuaternion :
	if(ctx->GetParameterManager()->IsDerivedFrom(guid,CKPGUID_QUATERNION))
		return vtQUATERNION;
	
	// is a Rectangle :
	if(ctx->GetParameterManager()->IsDerivedFrom(guid,CKPGUID_RECT))
		return vtRECTANGLE;

	// is a Box :
	if(ctx->GetParameterManager()->IsDerivedFrom(guid,CKPGUID_BOX))
		return vtBOX;


	// is a CKObject :
	if(ctx->GetParameterManager()->IsDerivedFrom(guid,CKPGUID_OBJECT))
		return vtOBJECT;

	return vtUNKNOWN;
}

/*
*******************************************************************
* Function: TypeIsSerializable()
* Description : Returns true if the given type can be stored as string. 
						+ It also checks custom virtools structures recursive.
						+ hidden Structs or parameter types are skipped !
*
* Parameters :		CKContext *ctx,	r : the virtools context
							CKParameterType type,	r : the type
* Returns : bool 
*
*******************************************************************
*/ 
bool
vtTools::ParameterTools::
TypeIsSerializable(
	CKContext *ctx,
	CKParameterType type)
{

	assert(ctx);

	CKParameterTypeDesc *tdescr = ctx->GetParameterManager()->GetParameterTypeDescription(type);

	bool result = false;
	if(tdescr)
	{
		using namespace vtTools::Enums;
		using namespace vtTools::ParameterTools;

		// return  isNotHidden AND isNotCKBeObject AND isNotUnknown OR  isValidStruct  :  //
		return		 
			(
				! (tdescr->dwFlags & CKPARAMETERTYPE_HIDDEN) &&						// internally type  
				(GetVirtoolsSuperType(ctx,tdescr->Guid) != vtOBJECT ) &&					// objects not allowed ! 
				(GetVirtoolsSuperType(ctx,tdescr->Guid) != vtUNKNOWN )					// also possible !!
			)	
			|| 
			(		
				(
					( (tdescr->dwFlags & CKPARAMETERTYPE_STRUCT) == 0x00000010 )  ? // parameter is a custom struct
					IsValidStruct(ctx,tdescr->Guid)  :  false //check the struct
				)
				
			); 
	}
	return result;
}
/*
	*******************************************************************
	* Function: IsValidStruct()
	*
	* Description : Like TypeIsSerializable, even checks a custom virtools structure, recursive.
	*
	* Parameters :		CKContext *ctx,r : the virtools context
								CKGUID type,r : the type to check

	* Returns : bool
	*
	*******************************************************************
*/ 
bool 
vtTools::ParameterTools::
IsValidStruct(CKContext *ctx,CKGUID type)
{

	assert(ctx);

	CKParameterManager* pm	= ctx->GetParameterManager();
	CKStructStruct *data		= pm->GetStructDescByType(pm->ParameterGuidToType(type));

	bool result = true;

	if (data) 
		for (int i=0;i<data->NbData;++i)
			if(vtTools::ParameterTools::TypeIsSerializable(ctx,pm->ParameterGuidToType(data->GetSubParamGuid(i))))
				continue;
			else
				result = false;
	return result;
}
/*
*******************************************************************
* Function: TypeCheckedParameterCopy()
*
* Description : Copies the source parameter to a destination parameter. On a type mismatch 
*						it tries to copy as string. This also works for nested virtools structures. See 
SuperType for supported types.
*
* Parameters :	CKParameter*dest,rw : the destination parameter
CKParameter*src,r : the source parameter
* Returns : bool = true if succesful.
* todo:: return checked diff
*
*******************************************************************
*/
int 
vtTools::ParameterTools::
TypeCheckedParameterCopy( CKParameter*dest, CKParameter*src)
{
	using vtTools::ParameterTools::TypeIsSerializable;
    
	/* return (  (TypeIsOk AND CopyIsOk == CK_OK )  OR (TypeIsSerializable && CopyByStringIsOk== CK_OK ) )    */
	return 
		(	
			src &&	// useful
			//ok, compare the type :
			dest->GetGUID() == src->GetGUID()  &&
			//type match, store new value :
			( dest->CopyValue(src)  == CK_OK )
		) 
		||						/* canceled ? try a string : */
		(	
			TypeIsSerializable(src->GetCKContext(),src->GetType()) &&
			( dest->SetStringValue( static_cast<CKSTRING>(src->GetReadDataPtr())) ==CK_OK ) 
		);;
}
/*
*******************************************************************
* Function: CompareStringRep()
*
* Description : This function compares the string representation of the CKParameters
*
* Parameters :	CKParameter* valueA,r : the first parameter 
	CKParameter* valueB,r : the second parameter 
* Returns : int (like strcmp)
*
*******************************************************************
*/
int
vtTools::ParameterTools::
CompareStringRep(CKParameter*valueA, CKParameter*valueB)
{

	assert(valueA && valueB);

	VxScratch sbufferA(valueA->GetDataSize());
	CKSTRING bufferA = (CKSTRING)sbufferA.Mem();
	valueA->GetStringValue(bufferA);

	VxScratch sbufferB(valueB->GetDataSize());
	CKSTRING bufferB = (CKSTRING)sbufferB.Mem();
	valueB->GetStringValue(bufferB);
	return strcmp( bufferA,bufferB ); 
}
/*
*******************************************************************
* Function: GetParameterAsString()
*
* Description : Returns the string value of a CKParameter.
*
* Parameters :	CKParameter*src ,r the given parameter

* Returns : CKSTRING 
*
*******************************************************************
*/
CKSTRING
vtTools::ParameterTools::GetParameterAsString( CKParameter*src)
{
	assert(src);
    VxScratch scratch(src->GetDataSize());
	CKSTRING out2 = (CKSTRING)scratch.Mem();
	src->GetStringValue(out2);
	return out2;

}