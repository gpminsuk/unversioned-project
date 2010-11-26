#pragma once

#include "BThing.h"

class CTank : public BThing
{
public:
	CTank();
	~CTank();

	bool IsInTurn;
	long TurnTimeLeft;

	CTank* Opponent;

	virtual void Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);

	void Fire();
	void StartTurn();
	void EndTurn();
	void SetOpponent(CTank* InOpponent);
};