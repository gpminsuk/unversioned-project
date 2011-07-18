#pragma once

#include "UWorld.h"

class USampleWorld : public UWorld
{
	DECLARE_CLASS(USampleWorld,);
public:
	USampleWorld();
	~USampleWorld();

	class CBox* Box;						// ��
	class CCharacter* Character;
	class CDirectionalLight* DirectionalLight;
	
	virtual bool InitializeWorld();
	virtual bool DestroyWorld();
	virtual bool Tick(DWORD dTime);	
};