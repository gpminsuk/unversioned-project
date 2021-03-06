#pragma once
#include "BThing.h"

class CRenekton: public BThing
{	
DECLARE_CLASS(CRenekton,);
public:
	CRenekton();
	~CRenekton();

	char m_Name[256];				//이름

	float m_Height;					//키
	float m_Radius;					//반지름
	float m_Weight;					//무게
	float m_WalkSpeed;				//걷는 속도
	float m_RunSpeed;					//뛰는 속도	

	BITFIELD bIsMovable :1;				//움직일 수 있는가
	BITFIELD bIsPhysical :1;				//물리가 적용이 되는가?
	BITFIELD bIsPassable :1;				//다른 물체가 이 물체를 통과할 수 있는가

	void SetCharacterPosition(TVector3 pos);	//위치를 pos로 세팅
	virtual bool Tick(unsigned long dTime);		//업데이트	
	virtual void PhysicsTick(unsigned long dTime);
	virtual void UpdateTransform();

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);
	virtual void InputChar();
};
