#pragma once

#include "BThing.h"

class CMissile : public BThing
{
public:
	CMissile(TVector3 _m_Location);
	~CMissile();

	bool IsInTurn;
	bool boom;

	virtual void Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);
	void Init(float fPower,float fAngle,TVector3& vecStartPosition,float fDirection);

	BThing* Owner;

	TVector3 m_vecStartPos;
	float m_fDirection;
	float m_fPastTime;

	float m_dy;
	float m_dz;
};