#pragma once

#include "BThing.h"
#include "CMissile.h"
#include "CArrow.h"
#include "MyWorld.h"

class CTank : public BThing
{
private:
	TVector3 m_vecRotationCylinder;
	float m_fRadian;
	float m_fSize;
	

public:
	CTank();
	CTank(UMyWorld *world,TVector3 _rot, float _radian, float _size, int i);
	~CTank();
	
	bool SetQuaternion(TQuaternion _rot);
	bool SetQuaternion(TVector3 _vec, float _radian);
	bool SetSize(float i_fSize);
	bool SetRadian(float i_fRadian);
	bool SetRotationCylinder(TVector3 rot);


	UMyWorld *MyWorld;


	bool IsInTurn;
	long TurnTimeLeft;
	float m_nGage;
	int m_NetWorkID;
	float m_FireAngle;

	CTank* Opponent;
    CMissile* m_Missile;
	CArrow* m_Arrow;
	TQuaternion m_qRot;

	virtual void Tick(unsigned long dTime);
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);

	void Fire(float _m_nGage,float _m_FireAngle);
	void StartTurn();
	void EndTurn();
	void SetOpponent(CTank* InOpponent);

	void Init();
	void Forword();
	void Backword();
	void ArrowUp();
	void ArrowDown();


	float m_fDirection;
};