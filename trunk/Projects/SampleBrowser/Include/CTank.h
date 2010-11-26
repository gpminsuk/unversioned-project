#pragma once

#include "BThing.h"

class CTank : public BThing
{
public:
	CTank();
	~CTank();

	bool IsInTurn;

	virtual void Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();
};