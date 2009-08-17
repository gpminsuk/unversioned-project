#pragma once
#include "BThing.h"

class CCharacter : public BThing
{
public:

	CCharacter();
	~CCharacter();	

	char			m_Name[256];				//이름

	float			m_Height;					//키
	float			m_Radius;					//반지름
	float			m_Weight;					//무게
	float			m_WalkSpeed;				//걷는 속도
	float			m_RunSpeed;					//뛰는 속도	

	BITFIELD		bIsMovable:1;				//움직일 수 있는가
	BITFIELD		bIsPhysical:1;				//물리가 적용이 되는가?
	BITFIELD		bIsPassable:1;				//다른 물체가 이 물체를 통과할 수 있는가


	void SetCharacterPosition(TVector3 pos);	//위치를 pos로 세팅
	virtual void Tick(unsigned long dTime);		//업데이트	
};
