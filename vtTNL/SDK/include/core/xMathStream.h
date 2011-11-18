#ifndef __XMATH_STREAM_H__
#define __XMATH_STREAM_H__

#ifndef _XNET_TYPES_H_
	#include "xNetTypes.h"
#endif

#ifndef _XPOINT_H_
	#include "xPoint.h"
#endif

#ifndef _XQUAT_H_
	#include "xQuat.h"
#endif
namespace xMath
{

	namespace stream
	{

		inline bool mathRead(TNL::BitStream& stream, Point2F* p)
		{
			bool success = stream.read(&p->x);
			success     &= stream.read(&p->y);
			return success;
		}

		inline bool mathRead(TNL::BitStream& stream, Point3F* p)
		{
			bool success = stream.read(&p->x);
			success     &= stream.read(&p->y);
			success     &= stream.read(&p->z);
			return success;
		}

		inline bool mathRead(TNL::BitStream& stream, Point4F* p)
		{
			bool success = stream.read(&p->x);
			success     &= stream.read(&p->y);
			success     &= stream.read(&p->z);
			success     &= stream.read(&p->w);
			return success;
		}

		inline bool mathRead(TNL::BitStream& stream, Point3D* p)
		{
			bool success = stream.read(&p->x);
			success     &= stream.read(&p->y);
			success     &= stream.read(&p->z);
			return success;
		}

		/*
		inline bool mathRead(TNL::BitStream& stream, PlaneF* p)
		{
			bool success = stream.read(&p->x);
			success     &= stream.read(&p->y);
			success     &= stream.read(&p->z);
			success     &= stream.read(&p->d);
			return success;
		}

		inline bool mathRead(TNL::BitStream& stream, Box3F* b)
		{
			bool success = mathRead(stream, &b->min);
			success     &= mathRead(stream, &b->max);
			return success;
		}

		inline bool mathRead(TNL::BitStream& stream, SphereF* s)
		{
			bool success = mathRead(stream, &s->center);
			success     &= stream.read(&s->radius);
			return success;
		}

		/*inline bool mathRead(TNL::BitStream& stream, RectI* r)
		{
			bool success = mathRead(stream, &r->point);
			success     &= mathRead(stream, &r->extent);
			return success;
		}

		inline bool mathRead(TNL::BitStream& stream, RectF* r)
		{
			bool success = mathRead(stream, &r->point);
			success     &= mathRead(stream, &r->extent);
			return success;
		}

		inline bool mathRead(TNL::BitStream& stream, MatrixF* m)
		{
			bool success = true;
			F32* pm    = *m;
			for (U32 i = 0; i < 16; i++)
				success &= stream.read(&pm[i]);
			return success;
		}

		*/
		
		inline bool mathRead(TNL::BitStream& stream, QuatF* q)
		{
			bool success = stream.read(&q->x);
			success     &= stream.read(&q->y);
			success     &= stream.read(&q->z);
			success     &= stream.read(&q->w);
			return success;
		}


		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		inline bool mathWrite(TNL::BitStream& stream, const Point2I& p)
		{
			bool success = stream.write(p.x);
			success     &= stream.write(p.y);
			return success;
		}

		inline bool mathWrite(TNL::BitStream& stream, const Point3I& p)
		{
			bool success = stream.write(p.x);
			success     &= stream.write(p.y);
			success     &= stream.write(p.z);
			return success;
		}

		inline bool mathWrite(TNL::BitStream& stream, const Point2F& p)
		{
			bool success = stream.write(p.x);
			success     &= stream.write(p.y);
			return success;
		}

		inline bool mathWrite(TNL::BitStream& stream, const Point3F& p)
		{
			bool success = stream.write(p.x);
			success     &= stream.write(p.y);
			success     &= stream.write(p.z);
			return success;
		}

		inline bool mathWrite(TNL::BitStream& stream, const Point4F& p)
		{
			bool success = stream.write(p.x);
			success     &= stream.write(p.y);
			success     &= stream.write(p.z);
			success     &= stream.write(p.w);
			return success;
		}

		inline bool mathWrite(TNL::BitStream& stream, const Point3D& p)
		{
			bool success = stream.write(p.x);
			success     &= stream.write(p.y);
			success     &= stream.write(p.z);
			return success;
		}

		/*
		inline bool mathWrite(TNL::BitStream& stream, const PlaneF& p)
		{
			bool success = stream.write(p.x);
			success     &= stream.write(p.y);
			success     &= stream.write(p.z);
			success     &= stream.write(p.d);
			return success;
		}

		inline bool mathWrite(TNL::BitStream& stream, const Box3F& b)
		{
			bool success = mathWrite(stream, b.min);
			success     &= mathWrite(stream, b.max);
			return success;
		}

		inline bool mathWrite(TNL::BitStream& stream, const SphereF& s)
		{
			bool success = mathWrite(stream, s.center);
			success     &= stream.write(s.radius);
			return success;
		}

		inline bool mathWrite(TNL::BitStream& stream, const MatrixF& m)
		{
			bool success    = true;
			const F32* pm = m;
			for (U32 i = 0; i < 16; i++)
				success &= stream.write(pm[i]);
			return success;
		}

		*/
		inline bool mathWrite(TNL::BitStream& stream, const QuatF& q)
		{
			bool success = stream.write(q.x);
			success     &= stream.write(q.y);
			success     &= stream.write(q.z);
			success     &= stream.write(q.w);
			return success;
		}
	}
}
#endif