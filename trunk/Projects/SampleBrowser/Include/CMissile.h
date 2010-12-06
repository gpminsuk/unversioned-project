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
	void Init(float fPower,float fAngle,TVector3& vecStartPosition,float fDirection);

	BThing* Owner;

	float m_fAngle;
	float m_fPower;

	float m_fPastTime;
	TVector3 m_vecStartPos;
	float m_fDirection;
};