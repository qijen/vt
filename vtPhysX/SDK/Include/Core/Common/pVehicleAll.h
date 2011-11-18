#ifndef __P_VEHICLE_ALL_H__
	#define  __P_VEHICLE_ALL_H__

#include <stdlib.h>
#include <limits.h>

//using namespace std::numeric_limits;

#define XFLT_EPSILON_MIN 0.00001f
#define XFLT_EPSILON_MAX FLT_MAX

#define xCheckFloat(n) ((fabs(n) > XFLT_EPSILON_MIN  && n < XFLT_EPSILON_MAX ) ? n : 0.0f)


typedef enum pVehicleProcessoptions
{
	pVPO_Lat_Damping=(1<<0),
	pVPO_Long_Damping=(1<<1),
	pVPO_SA_Damping=(1<<2),
	pVPO_SA_Delay=(1<<3),
	pVPO_SV_Tansa=(1<<4),
	pVPO_SA_DownSettle=(1<<5),
	pVPO_CheckLowSpeed=(1<<6),
	pVPO_Wheel_LockAdjust=(1<<7),
	pVPO_Wheel_UsePHYSX_Load=(1<<8),
	pVPO_Wheel_UsePHYSX_CONTACT_DATA=(1<<9),
	pVPO_Wheel_DoGregor=(1<<10),
	pVPO_Wheel_DampVerticalVelocityReversal=(1<<11),
	pVPO_Wheel_IntegrateImplicitVertical=(1<12),
	pVPO_Wheel_DiffDirect=(1<<13),
	pVPO_Engine_UseHardRevlimit=(1<<14),
	pVPO_Forces_No_Lateral=(1<<15),

};

#include "pEngine.h"
#include "pVehicleTypes.h"
#include "pGearbox.h"
#include "pDifferential.h"
#include "pSteer.h"




#endif
