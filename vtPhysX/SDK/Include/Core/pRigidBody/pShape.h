#ifndef __PSHAPE_H_
#define  __PSHAPE_H_


#include "vtOdeEnums.h"
#include "vtOdeTypes.h"
#include "pPrereqs.h"

namespace vtAgeia
{

	class pShape
	{
		public :

		pShape()
		{

		}
		virtual ~pShape(){}
		dGeomID m_OdeGeomID;
		
		dGeomID OdeGeomID() const { return m_OdeGeomID; }
		void OdeGeomID(dGeomID val) { m_OdeGeomID = val; }

		void                                SetOffsetPosition (VxVector position);
		void                                setOffsetQuaternion(VxQuaternion quad);

		void                               setOffsetWorldPosition(VxVector position);		
		void                               setOffsetWorldQuaternion(VxQuaternion orientation) ;

	};


}


#endif