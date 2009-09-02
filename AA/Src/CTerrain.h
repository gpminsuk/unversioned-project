#pragma once
#include "BThing.h"

class CTerrain : public BThing
{
public:
	CTerrain(class CCamera* Camera);
	virtual ~CTerrain(void);

	CCamera* m_pCamera;

	virtual void Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();
};
