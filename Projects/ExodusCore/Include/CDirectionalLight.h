#pragma once

#include "BLight.h"

class CDirectionalLight: public BLight
{
public:
	CDirectionalLight(void);
	virtual ~CDirectionalLight(void);

	virtual bool Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();
};
