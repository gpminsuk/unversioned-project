#pragma once

#define generic GENERIC
#include "UWorld.h"
#undef GENERIC

class UMaderWorld : public UWorld
{
	DECLARE_CLASS(UMaderWorld,);
public:
	UMaderWorld();
	~UMaderWorld();

	class CBox* Box;						// ¹æ
	class CCharacter* Character;
	class CDirectionalLight* DirectionalLight;
	
	virtual bool InitializeWorld();
	virtual bool DestroyWorld();
	virtual bool Tick(DWORD dTime);	
};