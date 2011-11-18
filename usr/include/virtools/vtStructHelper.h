#ifndef __VT_STRUCT_HELPER_H__
#define __VT_STRUCT_HELPER_H__



namespace vtTools
{

	namespace ParameterTools
	{

		struct StructurMember
		{
			CKGUID guid;		XString name;	XString defaultValue;
			CKObject *par;
			StructurMember()
			{
				guid = CKGUID(0,0);		name = "";		defaultValue = ""; 
			}

			StructurMember(CKGUID _guid,XString _name,XString _defaultValue) : 
			guid(_guid) , name(_name) , defaultValue(_defaultValue)
			{
			}

		};

		class CustomStructure
		{

		public : 
			CustomStructure(){}
			CustomStructure(XString name,CKGUID guid,StructurMember members[],int size) : mGuid(guid) , mName(name)
			{

				for (int i = 0 ; i < size ; i ++)
				{

					StructurMember *p=new StructurMember(members[i].guid,members[i].name,members[i].defaultValue);
					getArray().push_back(p);
				}

			} 
			std::vector<StructurMember*>pars;
			std::vector<StructurMember*>&getArray()
			{
				return pars;
			}

			CKGUID mGuid;
			XString mName;
		};

		class StructHelper
		{

		public :

			static XArray<CKGUID>getMemberGuids(CustomStructure _inStructure)
			{
				XArray<CKGUID>result;

				int size = _inStructure.getArray().size();
				for (int i = 0 ; i < size ; i ++)
				{
					StructurMember *m=_inStructure.getArray().at(i);
					result.PushBack(m->guid);
				}
				return result;
			}
			static XString getLabelNames(CustomStructure _inStructure)
			{
				XString result;

				int size = _inStructure.getArray().size();
				for (int i = 0 ; i < size ; i ++)
				{
					StructurMember *m=_inStructure.getArray().at(i);
					result << m->name.CStr();

					if (i != size -1)
					{
						result << ",";
					}
				}
				return result;
			}

			static XString getDefaultValue(CustomStructure _inStructure)
			{
				XString result;

				int size = _inStructure.getArray().size();
				for (int i = 0 ; i < size ; i ++)
				{
					StructurMember *m=_inStructure.getArray().at(i);
					result << m->defaultValue.CStr();
					if (i != size -1)
					{
						result << ";";
					}
				}
				return result;
			}
		};

	}
}

#define STRUCT_ATTRIBUTE(G,N,D) vtTools::ParameterTools::StructurMember(G,N,D)
#define DECLARE_STRUCT(T,N,G,A,S) CustomStructure cs##T(N,G,A,S)

#define STRUCT_SIZE(SOURCE_MAP)	(sizeof(SOURCE_MAP) / sizeof(SOURCE_MAP[0]))
#define STRUCT_MEMBER_NAMES(SRC) vtTools::ParameterTools::StructHelper::getLabelNames(cs##SRC)
#define STRUCT_MEMBER_GUIDS(SRC) vtTools::ParameterTools::StructHelper::getMemberGuids(cs##SRC)
#define STRUCT_MEMBER_DEFAULTS(SRC) vtTools::ParameterTools::StructHelper::getDefaultValue(cs##SRC)


#define REGISTER_CUSTOM_STRUCT(NAME,ENUM_TYPE,GUID,MEMBER_ARRAY,HIDDEN) DECLARE_STRUCT(ENUM_TYPE,NAME,GUID,MEMBER_ARRAY,STRUCT_SIZE(MEMBER_ARRAY)); \
	XArray<CKGUID> ListGuid##ENUM_TYPE = STRUCT_MEMBER_GUIDS(ENUM_TYPE);\
	pm->RegisterNewStructure(GUID,NAME,STRUCT_MEMBER_NAMES(ENUM_TYPE).Str(),ListGuid##ENUM_TYPE);\
	CKParameterTypeDesc* param_type##ENUM_TYPE=pm->GetParameterTypeDescription(GUID);\
	if (param_type##ENUM_TYPE && HIDDEN) param_type##ENUM_TYPE->dwFlags|=CKPARAMETERTYPE_HIDDEN


#define REGISTER_STRUCT_AS_ATTRIBUTE(NAME,ENUM_TYPE,CATEGORY,GUID,CLASS,MEMBER_ARRAY,USE_DEFAULTS)  int att##ENUM_TYPE = attman->RegisterNewAttributeType(NAME,GUID,CLASS);\
	attman->SetAttributeCategory(att##ENUM_TYPE,CATEGORY);\
	if(USE_DEFAULTS)\
	attman->SetAttributeDefaultValue(att##ENUM_TYPE,STRUCT_MEMBER_DEFAULTS(ENUM_TYPE).Str());



#endif