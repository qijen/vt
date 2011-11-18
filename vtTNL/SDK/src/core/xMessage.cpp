#include "xMessage.h"
#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"




xMessage::~xMessage()
{
	xDistributedPropertyArrayIterator begin = getParameters().begin();
	xDistributedPropertyArrayIterator end = getParameters().end();

	while (begin!=end)
	{
		xDistributedProperty *prop = *begin;
		if (prop)
		{

			xDistributedPropertyInfo *info = prop->m_PropertyInfo;
			if (info)
			{
				delete info;
				prop->m_PropertyInfo=NULL;
			}

			getParameters().erase(begin);
			delete prop;
			begin = getParameters().begin();
			int size = getParameters().size();
			if (size)
				continue;
			else
				break;

		}
		begin++;
	}

}