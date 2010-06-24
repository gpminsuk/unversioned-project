#pragma once
#include "BThing.h"

class CTerrain : public BThing
{
public:
	CTerrain();
	virtual ~CTerrain(void);

	virtual void Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();
};
