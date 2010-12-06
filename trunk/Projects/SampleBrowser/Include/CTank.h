#pragma once

#include "BThing.h"
#include "CMissile.h"

class CTank : public BThing
{
private:
	TVector3 m_vecRotationCylinder;
	float m_fRadian;
	float m_fSize;
	TQuaternion m_qRot;

public:
	CTank();
	CTank(TVector3 _rot, float _radian, float _size, int i);
	~CTank();
	
	bool SetQuaternion(TQuaternion _rot);
	bool SetQuaternion(TVector3 _vec, float _radian);
	bool SetSize(float i_fSize);
	bool SetRadian(float i_fRadian);
	bool SetRotationCylinder(TVector3 rot);

	bool IsInTurn;
	long TurnTimeLeft;
	int m_nGage;
	float m_fAngle;

	CTank* Opponent;

	virtual void Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);

	void Fire();
	void StartTurn();
	void EndTurn();
	void SetOpponent(CTank* InOpponent);

	std::vector<CMissile*> vecMissile;

	float m_fDirection;
};