#pragma once
#include "BThing.h"

class CCharacter : public BThing
{
public:

	CCharacter();
	~CCharacter();	

	char			m_Name[256];				//�̸�

	float			m_Height;					//Ű
	float			m_Radius;					//������
	float			m_Weight;					//����
	float			m_WalkSpeed;				//�ȴ� �ӵ�
	float			m_RunSpeed;					//�ٴ� �ӵ�	

	BITFIELD		bIsMovable:1;				//������ �� �ִ°�
	BITFIELD		bIsPhysical:1;				//������ ������ �Ǵ°�?
	BITFIELD		bIsPassable:1;				//�ٸ� ��ü�� �� ��ü�� ����� �� �ִ°�


	void SetCharacterPosition(TVector3 pos);	//��ġ�� pos�� ����
	virtual void Tick(unsigned long dTime);		//������Ʈ	
};
