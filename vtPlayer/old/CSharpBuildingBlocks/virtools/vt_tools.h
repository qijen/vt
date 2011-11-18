/*
*****************************************************************
*        Copyright � ITI Scotland 2006      
*----------------------------------------------------------------
* Module     : $File: //depot/ITITM005/Code/GBLCommon/Managers/Include/GBLTools.h $       
*
* Programmer : $Author: sylvain.le.breton $
* Date       : $DateTime: 2006/06/12 18:38:19 $ 
*
*----------------------------------------------------------------
*
* Module Summary : Generic Tools for GBL.
*			Parts :	+ structures 
						+ typedefs
						+ Enums
						+ tools for CKParameter
						+ tools for CKBehavior

*----------------------------------------------------------------
* $Revision: #7 $ 
* $Change: 23589 $
*****************************************************************
*/
#ifndef __VT_TOOLS_H
	#define __VT_TOOLS_H

#include "CKAll.h"

namespace vtTools
{
	/*************************************************************************/
	/* common enumerations																						   */
	/*  							    																						   */
	namespace Enums
	{
		/*
			Represents a virtools super type.
			See GetVirtoolsSuperType.
		*/
		enum SuperType
		{
			vtSTRING = 1,
			vtFLOAT = 2,
			vtINTEGER = 3,
			vtVECTOR = 4,
			vtVECTOR2D = 5,
			vtCOLOUR = 6,
			vtMATRIX = 7,
			vtQUATERNION = 8,
			vtRECTANGLE		= 9,
			vtBOX		= 10,
			vtBOOL		= 11,
			vtENUMERATION = 12,
			vtFLAGS = 13,
			vtFile = 14,
			vtOBJECT = 16,
			vtUNKNOWN = 17
		};
        
		/*	Author: gunther.baumgart
			Represents the c++ equivalent for TGBLWidgetType
			pm->RegisterNewEnum(EVTWIDGETTYPE,"EVTWidgetType",
			"StaticText=1,
			TextEdit=2,
			CheckButton=4,
			PushButton=8,
			ListBox=16,
			ComboBox=32,
			FileSelector=64,
			ColorSelector=128" );
		*/

		enum EVTWidgetType
		{
			EVT_WIDGET_STATIC_TEXT		= 1,
			EVT_WIDGET_EDIT_TEXT			=2,
			EVT_WIDGET_CHECK_BUTTON  =3,
			EVT_WIDGET_LIST_BOX			=5,
			EVT_WIDGET_COMBO_BOX		=6,
			EVT_WIDGET_FILE_SELECTION		=7,
			EVT_WIDGET_COLOR_SELECTION	=8
		};

		/*

		*/

	}/* end  namespace typedefs */

	/*************************************************************************/
	/* common Structs																			   							*/
	/*  							    																							*/
	namespace Structs
	{


		//////////////////////////////////////////////////////////////////////////
		/*
			ParameterInfo is used to describe a virtools parameter type. 
			Example : Matrix	   = horizontalItems = 4,
											  verticalItems = 4
											  memberType =vtFLOAT,
											  superType= vtMATRIX
		*/
		struct ParameterInfo
		{
			int horizontalItems;
			int verticalItems;

			Enums::SuperType superType;
			Enums::SuperType memberType;

		};
		//////////////////////////////////////////////////////////////////////////
		/*
			SGBL_WidgetInfo describes an parameter for using GBLWidgets
			GBL-Widgets. 
		*/
		struct WidgetInfo
		{
			struct WidgetInfoItem
			{	
				Enums::SuperType baseType;
				Enums::EVTWidgetType widgetType;		//with which widget 
				XString value;
				XString label;

			};
			XArray<WidgetInfoItem*>items;
			Structs::ParameterInfo parameterInfo;
		};
	}/* end  namespace Structs */

	/************************************************************************/
	/* parameter tools																							*/
	/*  																														*/
	namespace ParameterTools	
	{

	
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
		int IsNumeric(const char* Str,Enums::SuperType superType);

		/*
		*******************************************************************
		* Function: GetParameterMemberInfo()
		*
		* Description :	GetParameterMemberInfo returns the type super type of the used structure members.
								Example : vtVECTOR returns vtFLOAT 				
		* Parameters :	CKContext* context, r : the virtools context.
								CKParameterType parameterType, r : the type to test.
		* Returns : CO_EVT_VirtoolsSuperType
		*
		*******************************************************************
		*/
		Enums::SuperType GetParameterMemberInfo(CKContext* context,CKParameterType parameterType);

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
		Structs::ParameterInfo GetParameterInfo(CKContext* context,CKParameterType parameterType);

		/*
		*******************************************************************
		* Function: CreateWidgetInfo()
		*
		* Description :	CreateWidgetInfo returns an handy info about an parameter type and a
								given string value of this parameter.

		* Parameters :	CKContext* context, r : the virtools context.
								CKParameterType parameterType, r : the type to test.
								XString stringValue,r : the string value of the given type.

		* Returns : WidgetInfo
		*******************************************************************
		*/
		Structs::WidgetInfo* CreateWidgetInfo(CKContext* context,CKParameterType parameterType,XString stringValue);
		/*
		*******************************************************************
		* Function: GetWidgetType()
		*
		* Description :	GetWidgetType returns the type of the GBLWidget which should used for a 
								specific virtools super type. 
								Example : vtBOOL = CheckBox
												vtENUMERATION = ComboBox
		*
		* Parameters :	CO_EVT_VirtoolsSuperType superType, r : the given virtools super type
		* Returns : EWidgetType
		*
		*******************************************************************
		*/
		Enums::EVTWidgetType GetWidgetType(Enums::SuperType superType);
		/*
		 *******************************************************************
		 * Function: TypeCheckedParameterCopy()
		 *
		 * Description : Copies the source parameter to a destination parameter. On a type mismatch 
		 *						it tries to copy as string. This also works for nested virtools structures. See 
								CO_EVT_VirtoolsSuperType for supported types.
		 *
		 * Parameters :	CKParameter*dest,rw : the destination parameter
									CKParameter*src,r : the source parameter
		 * Returns : bool = true if succesful.
		 * todo:: return copied diff
		 *
		 *******************************************************************
		*/
		int TypeCheckedParameterCopy( CKParameter*dest, CKParameter*src);

		/*
		*******************************************************************
		* Function: GetParameterAsString()
		 *
		 * Description : Returns the string value a CKParameter.
		 *
		 * Parameters :	CKParameter*src ,r the given parameter
		 
		 * Returns : CKSTRING 
		 *
		 *******************************************************************
		 */
		 CKSTRING GetParameterAsString( CKParameter*src);

		/*
		*******************************************************************
		* Function: CompareStringRep()
		*
		* Description : This function compares the string representation of the two CKParameters.
		*
		* Parameters :	CKParameter* valueA,r : the first parameter 
								CKParameter* valueB,r : the second parameter 
		* Returns : int (like strcmp)
		*
		*******************************************************************
		*/
		int CompareStringRep(CKParameter*valueA, CKParameter*valueB);
		/*
		*******************************************************************
		* Function:GetVirtoolsSuperType()
		*
		* Description : GetVirtoolsSuperType returns the base type of a virtools parameter. 
								This is very useful to determine whether a parameter can be used for 
								network or a database.
		*
		* Parameters : CKContext *ctx, r : the virtools context
								 CKGUID guid, r : the guid of the parameter
		*
		* Returns : CO_EVT_VirtoolsSuperType, 
		*
		*******************************************************************
		*/ 
		Enums::SuperType GetVirtoolsSuperType(CKContext *ctx,const CKGUID guid);
		/*
		*******************************************************************
		* Function: TypeIsSerializable()
		* Description : Returns true if the given type can be stored as string. 
		+ It also checks custom virtools structures recursive.
		+ it checks only
		*
		* Parameters :		CKContext *ctx,	r : the virtools context
		CKParameterType type,	r : the type
		* Returns : bool 
		*
		*******************************************************************
		*/ 
		bool TypeIsSerializable(CKContext *ctx,CKParameterType type);
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
		bool IsValidStruct(CKContext *ctx,CKGUID type);
		/*
		*******************************************************************
		* Function:SetParameterStructureValue()
		*
		* Description : 	Sets a specific value in a custom structure.
		*
		* Parameters : CKParameter *par,				r : the virtools virtools parameter
								const unsigned short StructurIndex,	r : the index within the custom structure
								T value, r : the value.
								bool update,r : If this function is part of a virtools parameter operation, this 
								should set to false, otherwise it is an infinity loop.
		* Returns : is a void 
		*
		*******************************************************************
		*/ 
		template<class T>void SetParameterStructureValue(
			CKParameter *par,
			const unsigned short StructurIndex,
			T value,
			bool update = TRUE
			)
		{
			assert(par && StructurIndex>=0 );
			CK_ID* paramids = (CK_ID*)par->GetReadDataPtr(update);
			CKParameterOut *pout = static_cast<CKParameterOut*>(par->GetCKContext()->GetObject(paramids[StructurIndex]));
			pout->SetValue(&value);
		}

		/*
		*******************************************************************
		* Function:SetParameterStructureValue() 
		*
		* Description : This is a template function specialization for CKSTRING.
		*						Sets a specific value in a custom structure.
		*	
		* Parameters : 	CKParameter *par,				r : the virtools virtools parameter
								const unsigned short StructurIndex,	r : the index within the custom structure
								T value, r : the value.
								bool update,r : If this function is part of a virtools parameter operation, this 
								should set to false, otherwise it is an infinity loop.
		* Returns : is a void 
		*
		*******************************************************************
		*/ 
		template<>__inline void SetParameterStructureValue<CKSTRING>
			(
			CKParameter *par,
			const unsigned short StructurIndex,
			CKSTRING value,
			bool update
			)
		{
			assert(par && StructurIndex>=0 );
			static_cast<CKParameterOut*>(par->GetCKContext()->GetObject( static_cast<CK_ID*>(par->GetReadDataPtr(update))[StructurIndex]))->SetStringValue(value);
		}

		/*
		*******************************************************************
		* Function:GetValueFromParameterStruct()
		*
		* Description : returns a object from a custom structure.  
		*
		* Parameters : 	CKParameter *par,				r : the virtools virtools parameter
								const unsigned short StructurIndex,	r : the index within the custom structures
								bool update,r : If this function is part of a virtools parameter operation, this 
								should set to false, otherwise it is an infinity loop.
		* Returns : T. 
		*
		*******************************************************************
		*/ 
		template<class T>__inline T GetValueFromParameterStruct(
			CKParameter *par,
			const unsigned short StructurIndex,	
			bool update)
		{
			T value;
			assert(par && value);
			CK_ID* paramids = static_cast<CK_ID*>(par->GetReadDataPtr(update));	
			CKParameterOut* pout = static_cast<CKParameterOut*>(par->GetCKContext()->GetObject(paramids[StructurIndex]));
				pout->GetValue(&value); 
			return value;
		}
		/*
		*******************************************************************
		* Function:GetValueFromParameterStruct()
		*
		* Description :	This is a template function specialization for CKSTRING
		*						Gets a specific value from custom structure.
		*
		* Parameters : 	CKParameter *par,				r : the virtools virtools parameter
								const unsigned short StructurIndex,	r : the index within the custom structures
								bool update,r : If this function is part of a virtools parameter operation, this 
								should set to false, otherwise it is an infinity loop.

		* Returns : CKSTRING. 
		*
		*******************************************************************
		*/ 
		template<>__inline CKSTRING GetValueFromParameterStruct<CKSTRING>(
			CKParameter *par,
			const unsigned short StructurIndex,	
			bool update)
		{
			assert( par && StructurIndex >=0 );
			CK_ID* paramids = static_cast<CK_ID*>(par->GetReadDataPtr(update));	
			CKParameterOut* pout = static_cast<CKParameterOut*>(par->GetCKContext()->GetObject(paramids[StructurIndex]));
			CKSTRING stringValue = static_cast<CKSTRING>(pout->GetReadDataPtr(update));
			return stringValue;

		}
	} /*end namespace virtools parameter tools */


	/************************************************************************/
	/* building block tools																							*/
	/*  																														*/
	namespace BehaviorTools	
	{

		/*
		*******************************************************************
		* Function:GetInputParameterValue()
		*
		* Description : Returns an input parameter value.
		*
		* Parameters : CKBehavior *beh,	r : the behavior context
								const unsigned short pos,	r : the parameter input index
		* Returns : T
		*
		*******************************************************************
		*/ 
		template<class T>__inline T GetInputParameterValue(
			CKBehavior *beh,
			const int pos)
		{
			assert(beh);
			T value ;
			beh->GetInputParameterValue(pos,&value);
			return value;
		}

		/*
		*******************************************************************
		* Function:GetInputParameterValue()
		*
		* Description : This is a template function specialization for CKSTRING.
							  Returns an input parameter value.
		*
		* Parameters : CKBehavior *beh,	r : the behavior context
								const unsigned short pos,	r : the parameter input index
		* Returns : CKSTRING
		*
		*******************************************************************
		*/ 
		template<>__inline CKSTRING GetInputParameterValue<CKSTRING>(
			CKBehavior *beh,
			const int pos)
		{
			assert(beh);
			return static_cast<CKSTRING>(beh->GetInputParameterReadDataPtr(pos));
		}

		/*
		*******************************************************************
		* Function:GetInputParameterValue()
		*
		* Description :	This is a template function specialization for CKBeObject*.
								Returns a input parameter value from a  building block.
		*
		* Parameters :		CKBehavior *beh,	r : the behavior context
									const unsigned short pos,	r : the parameter input index
		* Returns : CKBeObject.
		*
		*******************************************************************
		*/ 
		template<>__inline CKObject* GetInputParameterValue<CKObject*>(
			CKBehavior *beh,
			const int pos)
		{
			assert(beh);
			return beh->GetInputParameterObject(pos);
		}

		//todo :  parametric log / debug forwarding!!!   - > g.baumgart !
		
		

	}/* end  namespace behavior tools*/


	/**************************************************************************/
	/* attribute tools																												*/
	/*  																																*/
	/*																																	*/
	namespace AttributeTools
	{
	
		/*
		******************************************************************
		* Function:GetObjectFromAttribute()
		*
		* Description :	This is a template function specialization for CKBeObject*.
							Returns a input parameter value from a  building block.
		*
		* Parameters :		CKBehavior *beh,	r : the behavior context
								const unsigned short pos,	r : the parameter input index
		* Returns : CKBeObject.
		*
		*******************************************************************
		*/ 
		template<class T>__inline T* GetObjectFromAttribute(CKContext *ctx,CKObject *_e,const int attributeID,const int StructurIndex)
		{
			T *value;
			CKParameterOut* pout = _e->GetAttributeParameter(attributeID);
			CK_ID* paramids = (CK_ID*)pout->GetReadDataPtr();
			pout = (CKParameterOut*)ctx->GetObject(paramids[StructurIndex]);
			value  = (T*)ctx->GetObject(*(CK_ID*)pout->GetReadDataPtr());
			return value  ? value : NULL;
		}
		/*
		******************************************************************
		* Function:GetObjectFromAttribute()
		*
		* Description :	This is a template function specialization for CKBeObject*.
							Returns a input parameter value from a  building block.
		*
		* Parameters :			CKBehavior *beh,	r : the behavior context
									const unsigned short pos,	r : the parameter input index
		* Returns : CKBeObject.
		*
		*******************************************************************
		*/ 
		template<class T>__inline void SetAttributeValue(CKBeObject*_e,const int attributeID,const int StructurIndex,T*value)
		{
			CKParameterOut* pout = _e->GetAttributeParameter(attributeID);
			if(pout)
			{
				CK_ID* paramids = (CK_ID*)pout->GetReadDataPtr();
				pout = (CKParameterOut*)_e->GetCKContext()->GetObject(paramids[StructurIndex]);
				if (pout)
				{
					pout->SetValue(value);
				}
			}
		}
		
		_inline void SetAttributeValue(CKBeObject*_e,const int attributeID,const int StructurIndex,CKObject*value)
		{
			CKParameterOut* pout = _e->GetAttributeParameter(attributeID);
			if(pout)
			{
				CK_ID* paramids = (CK_ID*)pout->GetReadDataPtr();
				pout = (CKParameterOut*)_e->GetCKContext()->GetObject(paramids[StructurIndex]);
				if (pout)
				{
					pout->SetValue(value);
				}
			}
		}
		/*
		******************************************************************
		* Function:GetObjectFromAttribute()
		*
		* Description :	This is a template function specialization for CKBeObject*.
							Returns a input parameter value from a  building block.
		*
		* Parameters :		CKBehavior *beh,	r : the behavior context
								const unsigned short pos,	r : the parameter input index
		* Returns : CKBeObject.
		*
		*******************************************************************
		*/ 
		template<class T>__inline T GetValueFromAttribute(CKBeObject *_e,int attributeID,int StructurIndex)
		{
			T value;
			CKParameterOut* pout = _e->GetAttributeParameter(attributeID);
			if (pout)
			{
				CK_ID* paramids = static_cast<CK_ID*>(pout->GetReadDataPtr());
				pout = static_cast<CKParameterOut*>(_e->GetCKContext()->GetObject(paramids[StructurIndex]));
				if (pout)
				{
					pout->GetValue(&value);
				}
			}
			return value;
		}

		/*
		******************************************************************
		* Function:GetObjectFromAttribute()
		*
		* Description :	This is a template function specialization for CKBeObject*.
		Returns a input parameter value from a  building block.
		*
		* Parameters :		CKBehavior *beh,	r : the behavior context
		const unsigned short pos,	r : the parameter input index
		* Returns : CKBeObject.
		*
		*******************************************************************
		*/ 
		template<class T>__inline T GetValueFromAttribute(CKBeObject *_e,int attributeID)
		{
			T value;
			CKParameterOut* pout = _e->GetAttributeParameter(attributeID);
			if (pout)
			{
				pout->GetValue(&value);
			}
			return value;
		}
		
	}/* end  namespace attribute tools*/
}

#endif
