#pragma once

#include "UWorld.h"

class USampleWorld : public UWorldTemplate
{
	DECLARE_CLASS(USampleWorld,);
public:
	USampleWorld();
	~USampleWorld();

	virtual void Create(UWorld* pWorld);

	class CBox* Box;						// ��
	class CCharacter* Character;
	class CDirectionalLight* DirectionalLight;
};