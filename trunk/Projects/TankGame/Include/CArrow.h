#pragma once

#include "BThing.h"


class CArrow : public BThing
{
public:
	CArrow();
	~CArrow();

	TVector3 m_vecRotationCylinder;
	float m_fRadian;
	float m_fSize;
	TQuaternion m_qRot;
	TQuaternion m_initRot;
	enum
	{
		MAX = 2,
	};

	virtual bool Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);

	bool SetQuaternion(TQuaternion _rot);
	bool SetQuaternion(TVector3 _vecRotationCylinder, float _radian);

	bool SetSize(float i_fSize);
	bool SetRadian(float i_fRadian);
	bool SetRotationCylinder(TVector3 rot);

	float m_fAccAngle;

	//float m_fAngle;
};