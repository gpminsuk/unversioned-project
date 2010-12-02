#pragma once

#include "BThing.h"

class CTank : public BThing
{
private:
	TVector3 m_vecRotationCylinder;
	float m_fRadian;
	float m_fSize;

public:
	CTank();
	CTank(TVector3 _rot, float _radian, float _size);
	~CTank();
	
	bool SetSize(float i_fSize);
	bool SetRadian(float i_fRadian);
	bool SetRotationCylinder(TVector3 rot);

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