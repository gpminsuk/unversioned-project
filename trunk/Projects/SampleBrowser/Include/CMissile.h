#pragma once

#include "BThing.h"

class CMissile : public BThing
{
public:
	CMissile();
	~CMissile();

	bool IsInTurn;
	long TurnTimeLeft;

	virtual void Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);
};