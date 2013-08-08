#pragma once

#include "BThing.h"

class CBox: public BThing
{
public:
	CBox(EGeometrySideType _BoxSideType = SideType_Outside);
	~CBox();

	virtual bool Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();
};
