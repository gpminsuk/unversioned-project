#pragma once
#include "BThing.h"

class CTerrain : public BThing
{
public:
	CTerrain();
	virtual ~CTerrain(void);

	TVector3 m_vecRotationCylinder;
	float m_fRadian;
	float m_fSize;
	TQuaternion m_qRot;


	virtual void Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();
};
