#include "stdafx.h"
#include "UWorld.h"
#include "CTankManager.h"
#include "CTank.h"
#include "CMissile.h"
#include "CArrow.h"

CTankManager::CTankManager() : m_nTimer(0) {}

CTankManager::CTankManager(UMyWorld *world) : m_nTimer(0) 
{
	Myworld=world;
}

CTankManager::~CTankManager()
{
	delete m_pTank[0];
	delete m_pTank[1];
}
CTank* CTankManager::GetTank(int _idx)
{
	return m_pTank[_idx];
}
bool CTankManager::SetStartTank(int _idx)
{
	m_pTank[_idx]->StartTurn();
	return true;
}
bool CTankManager::SetTimer(int i_nTimer)
{
	m_nTimer = i_nTimer;
	return true;
}

bool CTankManager::InitializeTank(TVector3 i_vecTank1, TVector3 i_vecTank2)
{
	m_pTank[0] = new CTank(Myworld,TVector3(1.0f, 0.0f, 0.0f), 1.5707963f, 0.04f, 0);
	
	m_pTank[1] = new CTank(Myworld,TVector3(1.0f, 0.0f, 0.0f), 1.5707963f, 0.04f, 0);
	
	m_pTank[0]->SetQuaternion(TVector3(0.0f, 1.0f, 0.0f), 1.5707963f*2);


	//각각의 위치 셋팅, 미사일은 발사시 위치잡고 AddThing()함.
	m_pTank[0]->m_Location = i_vecTank1;
	m_pTank[0]->UpdateTransform();

	m_pTank[1]->m_Location = i_vecTank2;
	m_pTank[1]->UpdateTransform();

	//서로 상대방에 대한 설정 turn을 위하여..
	m_pTank[0]->SetOpponent(m_pTank[1]);
	m_pTank[1]->SetOpponent(m_pTank[0]);
	return true;
}