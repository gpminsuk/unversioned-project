#pragma once
#include "BThing.h"

class CRift: public BThing
{
public:
	CRift(int num);
	~CRift();

	virtual bool Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);
	virtual void InputChar();
};
