#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "tinyxml.h"


pVehicle *pFactory::createVehicle(CK3dEntity *body,pVehicleDesc descr)
{

	pVehicle *result =NULL;

	if (!body)
	{
		return result;
	}

	pRigidBody *rBody = GetPMan()->getBody(body);

	if (!rBody)
	{
		return result;
	}

	if (rBody->getVehicle())
	{
		return rBody->getVehicle();
	}
	
	descr.body = rBody;
	result  = new pVehicle(descr,body);
	result->_vehicleMotor  = NULL;
	result->_vehicleGears = NULL;
	result->setProcessOptions(descr.processFlags);

	result->setActor(rBody->getActor());
	rBody->setVehicle(result);
	result->setMaxSteering(descr.steeringMaxAngle);
	result->initWheels(0);
	result->initEngine(0);

	
	//result->control(0, true, 0, true, false);
	//result->control(0, true, 0, true, false);
	return result;
    
}



pVehicleGears* pFactory::createVehicleGears(pVehicleGearDesc descr)
{

	if (!descr.isValid())
		return NULL;
	pVehicleGears *gears = new pVehicleGears();
	NxI32 nbForwardGears = gears->_nbForwardGears = descr.nbForwardGears;
	
	memcpy(gears->_forwardGearRatios, descr.forwardGearRatios, sizeof(NxF32) * nbForwardGears);
	memcpy(gears->_forwardGears, descr.forwardGears, sizeof(pLinearInterpolation) * nbForwardGears);
	
	gears->_curGear = 1;

	//gears->_backwardGear = gearDesc.backwardGear;
	gears->_backwardGearRatio = descr.backwardGearRatio;

	return gears;

}

pVehicleMotor* pFactory::createVehicleMotor(pVehicleMotorDesc descr)
{

	if (!descr.isValid())
		return NULL;
	pVehicleMotor* motor = new pVehicleMotor();
	motor->_torqueCurve = descr.torqueCurve;
	NxReal maxTorque = 0;
	NxI32 maxTorquePos = -1;
	for (NxU32 i = 0; i < motor->_torqueCurve.getSize(); i++) {
		NxReal v = motor->_torqueCurve.getValueAtIndex(i);
		if (v > maxTorque) {
			maxTorque = v;
			maxTorquePos = i;
		}
	}
	motor->_maxTorque			= maxTorque;
	motor->_maxTorquePos		= (float)maxTorquePos;
	motor->_maxRpmToGearUp		= descr.maxRpmToGearUp;
	motor->_minRpmToGearDown	= descr.minRpmToGearDown;
	motor->_maxRpm				= descr.maxRpm;
	motor->_minRpm				= descr.minRpm;

	motor->_rpm = 0.0f;

	return motor;

}

XString pFactory::_getVehicleWheelAsEnumeration(const TiXmlDocument * doc)
{

	if (!doc)
	{
		return XString("");
	}

	XString result("None=0");
	int counter = 1;

	

	/************************************************************************/
	/* */
	/************************************************************************/
	if ( doc)
	{
		const TiXmlElement *root = getFirstDocElement(doc->RootElement());
		for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( (child->Type() == TiXmlNode::ELEMENT))
			{
				XString name = child->Value();
				if (!strcmp(child->Value(), "wheel" ) )
				{

					const TiXmlElement *sube = (const TiXmlElement*)child;

					const char* vSName  = NULL;
					vSName = sube->Attribute("name");
					if (vSName && strlen(vSName))
					{
						if (result.Length())
						{
							result << ",";
						}
						result << vSName;
						result << "=" << counter;
						counter ++;
					}
				}
			}
		}
	}

	return result;
}

XString pFactory::_getVehicleSettingsAsEnumeration(const TiXmlDocument * doc)
{

	if (!doc)
	{
		return XString("");
	}

	XString result("None=0");
	int counter = 1;


	/************************************************************************/
	/* */
	/************************************************************************/
	if ( doc)
	{
		const TiXmlElement *root = getFirstDocElement(doc->RootElement());
		for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( (child->Type() == TiXmlNode::ELEMENT))
			{
				XString name = child->Value();
				if (!strcmp(child->Value(), "vehicle" ) )
				{

					const TiXmlElement *sube = (const TiXmlElement*)child;

					const char* vSName  = NULL;
					vSName = sube->Attribute("name");
					if (vSName && strlen(vSName))
					{
						if (result.Length())
						{
							result << ",";
						}
						result << vSName;
						result << "=" << counter;
						counter ++;
					}
				}
			}
		}
	}

	return result;

}






