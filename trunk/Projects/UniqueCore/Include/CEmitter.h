#pragma once
#include "BThing.h"

class CEmitter : public BThing
{
public:
	CEmitter();
	~CEmitter();

	virtual bool Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();

	TArray<struct TParticleInstance*> Particles;
	unsigned int ParticleIndex;
	unsigned int CurrentParticleNum;
};
